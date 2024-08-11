#include<stdio.h>
#include<graphics.h>
#include<stdlib.h>
#include"tools.h"
#include<time.h>
#include<mmsystem.h>
#pragma comment (lib,"winmm.lib")

#define WIDTH 900
#define HEGHT 600

enum {WAN_DOU_SHE_SHOU,XAING_RI_KIU,SHI_REN_HUA,ZHI_WU_COUNT};//���ж������ʱ�����ķ���
	
IMAGE imgBg;//����ͼƬ
IMAGE imgBar;//��Ŀ
IMAGE imgCards[ZHI_WU_COUNT];//����
IMAGE *imgZHIWU[ZHI_WU_COUNT][20];//Ϊ����������ڴ���������ָ���ʼ���������ٷ����ڴ�
IMAGE imgSun[29];
IMAGE imgZm[22];


int curX, curY;			//��ǰ�������
int curZhiWu;          //��ǰֲ��
int timer = 0;
int sunshine = 150;

struct zhiwu{
	int type;			//ֲ������
	int frameIndex;//��ǰ֡
};
struct zhiwu map[3][9];
struct sunshine 
{
	int x;
	int y;
	int frameIndex;
	int destY;
	bool used;  //����Ƿ���ʹ��
};
struct sunshine balls[10];		//Ԥ��׼��ʮ�����⣨�صĸ��
struct zm {
	int x;
	int y;
	int frameIndex;
	bool used;
	int speed;

};
struct zm zms[10];

void gameInit();
void updateWindow();
void userClick();
void updateGame();
void startUI();
void createSun();
void updateSun();
void createZm();

bool fileExist(const char* name);//��
//void tool();
int main()
{
	gameInit();
	startUI();

	int timer = 0;
	bool flag = true;
	while (1) 
	{
		userClick();
		timer += getDelay();
		if (timer > 50)
		{
			flag = true;
			timer = 0;
		}
		if (flag)
		{
			flag = false;
			updateWindow();
			updateGame();
		}
	}

		
	
	//system("pause");			//���ֹͣ����˼
	return 0;
}
void gameInit()
{
	//����ͼƬ
	//���ַ����޸�Ϊ�����ֽ��ַ�����
	loadimage(&imgBg, "res/bg.jpg");
	loadimage(&imgBar,"res/bar5.png");
	memset(imgZHIWU,0,sizeof(imgZHIWU));		//��ʼ��ָ�����飬��һ���ڴ���������Ϊһ��ֵʱ���������
	memset(map,0,sizeof(map));
	//��ʼ������
	char name[64];
	for (int i = 0; i < ZHI_WU_COUNT; i++)
	{
		sprintf_s(name,sizeof(name),"res/Cards/card_%d.png",i+1);	//sprintf�������ǽ��ַ�����ӡ��name����ȥ����sprintf_sҪ��Ȼ�ᱨ��
		loadimage(&imgCards[i], name);
		//��ʼ��
		for (int j = 0; j < 20; j++)
		{
			sprintf_s(name, sizeof(name), "res/zhiwu/%d/%d.png",i, j+1); //D:\code\c\plant\plant\plant\res\zhiwu\0\2.png
			if (fileExist(name))
			{
				imgZHIWU[i][j] = new IMAGE;      //�����ڴ棿new IMAGE�᷵��һ��ָ�룬ָ��һ��IMAGE�ṹ��
				loadimage(imgZHIWU[i][j],name);
			}
			else
				break;
		}

		
	}
	memset(balls,0,sizeof(balls));//�����ʼ��
	for (int i = 0; i < 29; i++)
	{
		sprintf_s(name, sizeof(name), "res/sunshine/%d.png", i + 1);
		loadimage(&imgSun[i], name);
	}
	srand(time(NULL));
	//��ʼ����ʬ
	for (int i = 0; i < 22; i++)
	{
		sprintf_s(name, sizeof(name), "res/zm/%d.png", i + 1);
		loadimage(&imgZm[i], name);
	}
	curZhiWu = 0;
	//��������
	initgraph(WIDTH, HEGHT,1);			//������ֵ��һ����
	//��������
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 30;
	f.lfWeight = 15;
	strcpy(f.lfFaceName,"Segoe UI Black");
	f.lfQuality = ANTIALIASED_QUALITY;// �����
	settextstyle(&f);
	setbkmode(TRANSPARENT);//���ñ���
	setcolor(BLACK);



	//��ʬ��ʼ��
	memset(zms,0,sizeof(zms));
	for (int i = 0; i < 22; i++)
	{
		sprintf_s(name,sizeof(name),"res/zm/%d.png",i+1);
		loadimage(&imgZm[i],name);
	}

}
//

void updateWindow()
{
	BeginBatchDraw();	//��ʼ����										/*������������Ҫ�����ã�Ҫ��Ȼ�ͻ����һֱ����*/
	putimage(0,0,&imgBg);	//(0,0)�����Ͻǲο�ϵ
	//putimage(250,0,&imgBar);
	putimagePNG(250, 0, &imgBar);	//ȥ��
	for (int i = 0; i < ZHI_WU_COUNT; i++)	
	{   
		// ֲ�￨����Ⱦ
		putimage(338+i*65,6,&imgCards[i]);
		//��Ⱦ�϶��е�ֲ��
		
	}
	if (curZhiWu) 
	{
		IMAGE* img = imgZHIWU[curZhiWu - 1][0];
		putimagePNG(curX-img->getwidth()/2,curY-img->getheight()/2,img);
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (map[i][j].type)
			{
				int x = 256 + j * 81;
				int y = 179 + i * 102;

				putimagePNG(x,y,imgZHIWU[map[i][j].type - 1][map[i][j].frameIndex]);
			}
		}
	}
	int ballMax = sizeof(balls) / sizeof(balls[0]);
	for (int i = 0; i < ballMax; i++)
	{
		if (balls[i].used)
			putimagePNG(balls[i].x, balls[i].y, &imgSun[balls[i].frameIndex]);
	}
	char score[8];
	sprintf_s(score,sizeof(score),"%d",sunshine);		//ת���ַ���
	outtextxy(276,67,score);


	createZm();





	EndBatchDraw();		//��������
}
void userClick()		//�û�����
{
	ExMessage msg;		//��ǰ���״̬
	if(peekmessage(&msg))	//�ж���û����Ϣ
	{
		static int status = 0;		//��¼�Ƿ���
		if (msg.message == WM_LBUTTONDOWN)			//�ж��Ҽ�����
		{
			if (msg.x > 338 && msg.x < 338 + 65 * ZHI_WU_COUNT && msg.y < 96)
			{
				int num = (msg.x - 338) / 65;
				printf("%d\n",num);
				status = 1;			//ѡ��
				curZhiWu = num + 1;
				
			}
			else if (1)
			{
				int i;
				for (i = 0; i < 10; i++) {
					if (balls[i].used &&
						msg.x > balls[i].x && msg.x < balls[i].x + 80 &&
						msg.y > balls[i].y && msg.y < balls[i].y + 80)
					{
						balls[i].used = false;
						sunshine += 50;
						mciSendString("play res/sunshine.mp3", 0,0,0);
					}
				}
				
			}
			
		}
		else if (msg.message == WM_MOUSEMOVE &&status == 1)		//�ж�����ƶ�
				{
					curX = msg.x;
					curY = msg.y;
				}
		
		else if (msg.message == WM_LBUTTONUP)		//�Ҽ�̧��
		{
			if (msg.x > 256 && msg.x < 256 + 81 * 9 && msg.y > 179 && msg.y < 179 + 102 * 3)
			{
			int row = (msg.y - 179) / 102;
			int col = (msg.x - 256) / 81;
			if (map[row][col].type == 0)    //�ж�֮ǰ��û��ֲ��������
			{
				map[row][col].type = curZhiWu;
				map[row][col].frameIndex = 0;
			}
			
			}
			

			curZhiWu = 0;
			status = 0;
			curX = 0;
			curY = 0;
		}
	}
}
bool fileExist(const char* name)		//�����ж��ļ��Ƿ����
{
	FILE* fp = fopen(name,"r");
	if (fp == NULL)
		return false;
	else 
	{
		fclose(fp);
		return true;
	}
		
}
void updateGame()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (map[i][j].type)
			{
				map[i][j].frameIndex++;
				int zhiWuType = map[i][j].type - 1;
				int index = map[i][j].frameIndex;
				if (imgZHIWU[zhiWuType][index] == NULL)
				{
					map[i][j].frameIndex=0;
				}
			
			}
		}
	}
	
	createSun();
	//��Ⱦsunshine
	updateSun();
		
}
void startUI()
{
	IMAGE imgBg,imgM1,imgM2;
	loadimage(&imgBg, "res/menu.png"); 
	loadimage(&imgM1, "res/menu1.png");
	loadimage(&imgM2, "res/menu2.png");
	int flag = 0;		//�����ж��Ƿ���
	while (1)
	{
		BeginBatchDraw();
		putimage(0,0,&imgBg);
		putimagePNG(474,75,flag? &imgM2: &imgM1);		//��Ԫ���������
		

		ExMessage msg;
		if (peekmessage(&msg))
		{
			//printf("%d,%d\n",msg.x,msg.y);
			if (msg.message == WM_LBUTTONDOWN &&msg.x > 474 && msg.x < 803 && msg.y >75 && msg.y < 222)
			{
				flag = 1;
			}
			else if(msg.message == WM_LBUTTONUP&&flag)
				return;
		}
		
		EndBatchDraw();
	}

}
void createSun()
{
	static int count = 0;		//�����������������Ƶ��
	static int fre = 200;
	count++;
	if (count > fre )
	{
	fre = 100 + rand() % 200;
	int ballMax = sizeof(balls) / sizeof(balls[0]);
	int i;
	for (i = 0; i < ballMax && balls[i].used; i++);
	if (i >= ballMax)return;
		balls[i].used = true;
		balls[i].destY =200 + rand()%4*90;//
		balls[i].x = 260 + rand()%(900-260);	//260-900
		balls[i].y = 60;//
		balls[i].frameIndex = 0;
		count = 0;
	}
	
	
}
void updateSun()
{
	int ballMax = sizeof(balls) / sizeof(balls[0]);
	for (int i = 0; i < ballMax; i++)
	{
		if (balls[i].used)
		{
			balls[i].frameIndex = (balls[i].frameIndex + 1) % 29;					//balls[i].frameIndex++;
																					//balls[i].frameIndex %= 29;
			if(timer == 0)
				balls[i].y += 2;
				if (balls[i].y == balls[i].destY) {
						timer++;
						if (timer > 100)
						{
							balls[i].used = false;
							timer = 0;
						}	
			
				}
				
		}
	}
	
}
void createZm()
{
	int i;
	for (i = 0; i < 10 ; i++)
	{
		if (zms[i].used)
		{

		}
	}

}
