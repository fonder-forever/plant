#include<stdio.h>
#include<graphics.h>
#include<stdlib.h>
#include"tools.h"

#define WIDTH 900
#define HEGHT 600

enum {WAN_DOU_SHE_SHOU,XAING_RI_KIU,SHI_REN_HUA,ZHI_WU_COUNT};//当有多个对象时计数的方法
	
IMAGE imgBg;//表示背景图片
IMAGE imgBar;//
IMAGE imgCards[ZHI_WU_COUNT];

void gameInit();
void updateWindow();
void userClick();
//void tool();
int main()
{
	gameInit();

	while (1) 
	{
		updateWindow();
		userClick();
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
	//初始画卡牌
	char name[64];
	for (int i = 0; i < ZHI_WU_COUNT; i++)
	{
		sprintf_s(name,sizeof(name),"res/Cards/card_%d.png",i+1);	//sprintf的作用是将字符串打印到name里面去，用sprintf_s要不然会报错
		loadimage(&imgCards[i], name);
	}
	//创建窗口
	initgraph(WIDTH, HEGHT,1);			//第三个值填一调试
}
void updateWindow()
{
	BeginBatchDraw();	//开始缓冲										/*缓冲起到至关重要的作用，要不然就会产生一直闪屏*/
	putimage(0,0,&imgBg);	//(0,0)从左上角参考系
	//putimage(250,0,&imgBar);
	putimagePNG(250, 0, &imgBar);	//去角
	for (int i = 0; i < ZHI_WU_COUNT; i++)	// 植物卡牌渲染
	{
		putimage(338+i*65,6,&imgCards[i]);
	}
	EndBatchDraw();		//结束缓冲
}
void userClick()		//用户操作
{
	ExMessage msg;
	if(peekmessage(&msg))	//判断有没有消息
	{
		if (msg.message == WM_LBUTTONDOWN)			//判断右键按下
		{
			if (msg.x > 338 && msg.x < 338 + 65 * ZHI_WU_COUNT && msg.y < 96)
			{
				int num = (msg.x - 338) / 65;
				printf("%d\n",num);
			}
		}
		else if (msg.message == WM_MOUSEMOVE)		//判断鼠标移动
		{

		}
		else if (msg.message == WM_LBUTTONUP)		//右键抬起
		{

		}
	}
}

