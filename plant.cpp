#include<stdio.h>
#include<graphics.h>
#include<stdlib.h>
#include"tools.h"
#include<time.h>
#include<mmsystem.h>
#pragma comment (lib,"winmm.lib")

#define WIDTH 900
#define HEGHT 600

enum {WAN_DOU_SHE_SHOU,XAING_RI_KIU,SHI_REN_HUA,ZHI_WU_COUNT};//当有多个对象时计数的方法
	
IMAGE imgBg;//背景图片
IMAGE imgBar;//栏目
IMAGE imgCards[ZHI_WU_COUNT];//卡牌
IMAGE *imgZHIWU[ZHI_WU_COUNT][20];//为了灵活运用内存先用数组指针初始化，后面再分配内存
IMAGE imgSun[29];
IMAGE imgZm[22];


int curX, curY;			//当前鼠标坐标
int curZhiWu;          //当前植物
int timer = 0;
int sunshine = 150;

struct zhiwu{
	int type;			//植物类型
	int frameIndex;//当前帧
};
struct zhiwu map[3][9];
struct sunshine 
{
	int x;
	int y;
	int frameIndex;
	int destY;
	bool used;  //检测是否在使用
};
struct sunshine balls[10];		//预先准备十个阳光（池的概念）
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

bool fileExist(const char* name);//？
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

		
	
	//system("pause");			//最后停止的意思
	return 0;
}
void gameInit()
{
	//加载图片
	//把字符集修改为‘多字节字符集’
	loadimage(&imgBg, "res/bg.jpg");
	loadimage(&imgBar,"res/bar5.png");
	memset(imgZHIWU,0,sizeof(imgZHIWU));		//初始化指针数组，将一块内存连续设置为一个值时用这个函数
	memset(map,0,sizeof(map));
	//初始画卡牌
	char name[64];
	for (int i = 0; i < ZHI_WU_COUNT; i++)
	{
		sprintf_s(name,sizeof(name),"res/Cards/card_%d.png",i+1);	//sprintf的作用是将字符串打印到name里面去，用sprintf_s要不然会报错
		loadimage(&imgCards[i], name);
		//初始化
		for (int j = 0; j < 20; j++)
		{
			sprintf_s(name, sizeof(name), "res/zhiwu/%d/%d.png",i, j+1); //D:\code\c\plant\plant\plant\res\zhiwu\0\2.png
			if (fileExist(name))
			{
				imgZHIWU[i][j] = new IMAGE;      //分配内存？new IMAGE会返回一个指针，指向一个IMAGE结构体
				loadimage(imgZHIWU[i][j],name);
			}
			else
				break;
		}

		
	}
	memset(balls,0,sizeof(balls));//阳光初始化
	for (int i = 0; i < 29; i++)
	{
		sprintf_s(name, sizeof(name), "res/sunshine/%d.png", i + 1);
		loadimage(&imgSun[i], name);
	}
	srand(time(NULL));
	//初始化僵尸
	for (int i = 0; i < 22; i++)
	{
		sprintf_s(name, sizeof(name), "res/zm/%d.png", i + 1);
		loadimage(&imgZm[i], name);
	}
	curZhiWu = 0;
	//创建窗口
	initgraph(WIDTH, HEGHT,1);			//第三个值填一调试
	//设置字体
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 30;
	f.lfWeight = 15;
	strcpy(f.lfFaceName,"Segoe UI Black");
	f.lfQuality = ANTIALIASED_QUALITY;// 抗锯齿
	settextstyle(&f);
	setbkmode(TRANSPARENT);//设置背景
	setcolor(BLACK);



	//僵尸初始化
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
	BeginBatchDraw();	//开始缓冲										/*缓冲起到至关重要的作用，要不然就会产生一直闪屏*/
	putimage(0,0,&imgBg);	//(0,0)从左上角参考系
	//putimage(250,0,&imgBar);
	putimagePNG(250, 0, &imgBar);	//去角
	for (int i = 0; i < ZHI_WU_COUNT; i++)	
	{   
		// 植物卡牌渲染
		putimage(338+i*65,6,&imgCards[i]);
		//渲染拖动中的植物
		
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
	sprintf_s(score,sizeof(score),"%d",sunshine);		//转成字符串
	outtextxy(276,67,score);


	createZm();





	EndBatchDraw();		//结束缓冲
}
void userClick()		//用户操作
{
	ExMessage msg;		//当前鼠标状态
	if(peekmessage(&msg))	//判断有没有消息
	{
		static int status = 0;		//纪录是否按下
		if (msg.message == WM_LBUTTONDOWN)			//判断右键按下
		{
			if (msg.x > 338 && msg.x < 338 + 65 * ZHI_WU_COUNT && msg.y < 96)
			{
				int num = (msg.x - 338) / 65;
				printf("%d\n",num);
				status = 1;			//选中
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
		else if (msg.message == WM_MOUSEMOVE &&status == 1)		//判断鼠标移动
				{
					curX = msg.x;
					curY = msg.y;
				}
		
		else if (msg.message == WM_LBUTTONUP)		//右键抬起
		{
			if (msg.x > 256 && msg.x < 256 + 81 * 9 && msg.y > 179 && msg.y < 179 + 102 * 3)
			{
			int row = (msg.y - 179) / 102;
			int col = (msg.x - 256) / 81;
			if (map[row][col].type == 0)    //判断之前有没有植物在这里
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
bool fileExist(const char* name)		//用来判断文件是否存在
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
	//渲染sunshine
	updateSun();
		
}
void startUI()
{
	IMAGE imgBg,imgM1,imgM2;
	loadimage(&imgBg, "res/menu.png"); 
	loadimage(&imgM1, "res/menu1.png");
	loadimage(&imgM2, "res/menu2.png");
	int flag = 0;		//用于判断是否按下
	while (1)
	{
		BeginBatchDraw();
		putimage(0,0,&imgBg);
		putimagePNG(474,75,flag? &imgM2: &imgM1);		//三元运算符运用
		

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
	static int count = 0;		//用来减少阳光产生的频率
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
