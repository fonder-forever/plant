#include<stdio.h>
#include<graphics.h>
#include<stdlib.h>
#include"tools.h"

#define WIDTH 900
#define HEGHT 600

enum {WAN_DOU_SHE_SHOU,XAING_RI_KIU,SHI_REN_HUA,ZHI_WU_COUNT};//���ж������ʱ�����ķ���
	
IMAGE imgBg;//��ʾ����ͼƬ
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
	
	//system("pause");			//���ֹͣ����˼
	return 0;
}
void gameInit()
{
	//����ͼƬ
	//���ַ����޸�Ϊ�����ֽ��ַ�����
	loadimage(&imgBg, "res/bg.jpg");
	loadimage(&imgBar,"res/bar5.png");
	//��ʼ������
	char name[64];
	for (int i = 0; i < ZHI_WU_COUNT; i++)
	{
		sprintf_s(name,sizeof(name),"res/Cards/card_%d.png",i+1);	//sprintf�������ǽ��ַ�����ӡ��name����ȥ����sprintf_sҪ��Ȼ�ᱨ��
		loadimage(&imgCards[i], name);
	}
	//��������
	initgraph(WIDTH, HEGHT,1);			//������ֵ��һ����
}
void updateWindow()
{
	BeginBatchDraw();	//��ʼ����										/*������������Ҫ�����ã�Ҫ��Ȼ�ͻ����һֱ����*/
	putimage(0,0,&imgBg);	//(0,0)�����Ͻǲο�ϵ
	//putimage(250,0,&imgBar);
	putimagePNG(250, 0, &imgBar);	//ȥ��
	for (int i = 0; i < ZHI_WU_COUNT; i++)	// ֲ�￨����Ⱦ
	{
		putimage(338+i*65,6,&imgCards[i]);
	}
	EndBatchDraw();		//��������
}
void userClick()		//�û�����
{
	ExMessage msg;
	if(peekmessage(&msg))	//�ж���û����Ϣ
	{
		if (msg.message == WM_LBUTTONDOWN)			//�ж��Ҽ�����
		{
			if (msg.x > 338 && msg.x < 338 + 65 * ZHI_WU_COUNT && msg.y < 96)
			{
				int num = (msg.x - 338) / 65;
				printf("%d\n",num);
			}
		}
		else if (msg.message == WM_MOUSEMOVE)		//�ж�����ƶ�
		{

		}
		else if (msg.message == WM_LBUTTONUP)		//�Ҽ�̧��
		{

		}
	}
}

