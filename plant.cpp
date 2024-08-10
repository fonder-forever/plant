#include<stdio.h>
#include<graphics.h>
#include<stdlib.h>
#include"tools.h"

#define WIDTH 900
#define HEGHT 600

enum {WAN_DOU_SHE_SHOU,XAING_RI_KIU,SHI_REN_HUA,ZHI_WU_COUNT};//���ж������ʱ�����ķ���
	
IMAGE imgBg;//����ͼƬ
IMAGE imgBar;//��Ŀ
IMAGE imgCards[ZHI_WU_COUNT];//����
IMAGE *imgZHIWU[ZHI_WU_COUNT][20];//Ϊ����������ڴ���������ָ���ʼ���������ٷ����ڴ�
IMAGE imgSun[29];


int curX, curY;			//��ǰ�������
int curZhiWu;          //��ǰֲ��

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
	int used;  //����Ƿ���ʹ��
};
struct sunshine balls[10];		//Ԥ��׼��ʮ�����⣨�صĸ��

void gameInit();
void updateWindow();
void userClick();
void updateGame();
void startUI();
void createSun();
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
	memset(balls,0,sizeof(balls));
	for (int i = 0; i < 29; i++)
	{
		sprintf_s(name, sizeof(name), "res/sunshine/%d.png", i + 1);
		loadimage(&imgSun[i], name);
	}
	
	curZhiWu = 0;
	//��������
	initgraph(WIDTH, HEGHT,1);			//������ֵ��һ����

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
			if (map[row][col].type == 0) 
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
	int ballMax = sizeof(balls) / sizeof(balls[0]);
	for (int i = 0; i < ballMax && balls[i].used; i++)
	{

	}
}
