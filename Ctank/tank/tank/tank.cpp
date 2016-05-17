// tank.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "tank.h"
extern TANK Tank;
extern TANK Tank1;
DWORD dwRet;
HANDLE hout=GetStdHandle(STD_OUTPUT_HANDLE);
COORD pos={30,10};
wchar_t szStr[10][40]={ L"欢  迎  来  到  坦  克  联  盟！",
	L"|1.单人游戏",L"|2.双人游戏",L"|3.系统设置",L"按1开始单人游戏 按2开始双人游戏 按3进入系统设置 ESC退出任意界面",
	L"程序员在努力加班中...",L"一秒后开始游戏...",L"您的输入有误",L"游戏结束",L"W（上）A（左）S（下）D（右）移动  J:发射子弹"
};

void TankCursor(int x,int y);//坦克的中心坐标
void GameMenu();//游戏菜单
char GetKey();//获取按键
void GraphicsInterface();//图形界面
void DrawMap();//画地图

void Tank1Collisionetection();//Tank1移动及碰撞检测
void DrawStyle1();//画出坦克1样式
void ClrDrawStyle1();//清除坦克1样式


void DrawStyle();//画出坦克样式
void ClrDrawStyle();//清除坦克样式
void BulletMove();//移动子弹
void TankCollisionetection();//Tank移动及碰撞检测
void FireBullet(TANK Tank);//定义开火

//坦克或子弹的中心坐标
void TankCursor(int x,int y)
{
	SetConsoleTitle(L"坦克大战");
	CONSOLE_CURSOR_INFO Cui={1,FALSE};
	HANDLE hout;
	COORD coord;
	coord.X=2*x;
	coord.Y=y;
	hout=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout,coord);//光标位置
	SetConsoleCursorInfo(hout,&Cui);//取消光标
}
//菜单
void GameMenu()
{
GameInterface:
	CONSOLE_CURSOR_INFO Cui={1,FALSE};
	COORD pos={32,10};
	SetConsoleCursorInfo(hout,&Cui);
	SMALL_RECT srctWindow={0,0,80,40};
	SetConsoleWindowInfo(hout,true,&srctWindow);//设置控制窗口大小
	COORD BufferSize={81,41};
	SetConsoleScreenBufferSize(hout,BufferSize);//设置控制台 缓冲区大小
	for(int i=0;i<10;i++)//主菜单界面
	{
		switch (i)
		{
		case 0:
			pos.X-=10;
			WriteConsoleOutputCharacter(hout,szStr[i],30,pos,&dwRet);break;//在指定位置打印相应字符串
		case 1:
		case 2:
		case 3:
			pos.X=30;
			pos.Y+=5;
			pos.Y++,
			WriteConsoleOutputCharacter(hout,szStr[i],30,pos,&dwRet);break;
		case 4:
			pos.X-=20;
			pos.Y+=5;
			WriteConsoleOutputCharacter(hout,szStr[i],40,pos,&dwRet);break;
		case 9:
			pos.X+=7;
			pos.Y+=3;
			WriteConsoleOutputCharacter(hout,szStr[i],35,pos,&dwRet);break;
		}
	}
	char ch;
	while(1)
	{
		if( !(ch = GetKey())){
			continue;
		}		
		if(ch==27)
		{
			exit(0);
		}
		switch (ch)
		{
		case '1':
			pos.X=30;
			pos.Y=15;
			system("cls");
			WriteConsoleOutputCharacter(hout,szStr[6],35,pos,&dwRet);
			Sleep(1000);
			goto GameStart;
		case '2':
		case '3':
			pos.X=30;
			pos.Y=15;
			system("cls");
			WriteConsoleOutputCharacter(hout,szStr[5],35,pos,&dwRet);//在指定位置打印相应字符串
			Sleep(1000);
			system("cls");
			goto GameInterface;//返回开始界面
		default:
			pos.X=30;
			pos.Y=15;
			system("cls");
			WriteConsoleOutputCharacter(hout,szStr[7],35,pos,&dwRet);
			Sleep(1000);
			system("cls");
			goto GameInterface;
			break;
		}
	}
GameStart:
	system("cls");
}
//获取按键
char GetKey()
{
	char ch;
	if(_kbhit())
	{
		ch=_getch();

	}
	else
	{
		return 0;
	}
	return ch;
}
//绘制坦克
void DrawStyle()
{
	for(unsigned int i=0,y=Tank.TankY-1;y<=Tank.TankY+1;i++,y++)
	{
		for(unsigned int j=0,x=Tank.TankX-1;x<=Tank.TankX+1;j++,x++)
		{
			if(g_PlayerTankStyle[Tank.Orientation][i][j]==1)
			{
				TankCursor( x, y);
					printf("■");
			}
			else
			{
				TankCursor( x, y);
				printf("  ");
			}
		}
	}
}
//绘制坦克1
void DrawStyle1()
{
	for(unsigned int i=0,y=Tank1.TankY-1;y<=Tank1.TankY+1;i++,y++)
	{
		for(unsigned int j=0,x=Tank1.TankX-1;x<=Tank1.TankX+1;j++,x++)
		{
			if(g_PlayerTankStyle[Tank1.Orientation][i][j]==1)
			{
				TankCursor( x, y);
					printf("■");
			}
			else
			{
				TankCursor( x, y);
				printf("  ");
			}
		}
	}
}
//清除坦克
void ClrDrawStyle()
{
	for(unsigned int i=0,y=Tank.TankY-1;y<=Tank.TankY+1;i++,y++)
	{
		for(unsigned int j=0,x=Tank.TankX-1;x<=Tank.TankX+1;j++,x++)
		{
			TankCursor( x, y);
			printf("  ");
		}
	}
}
//清除坦克1
void ClrDrawStyle1()
{
	for(unsigned int i=0,y=Tank1.TankY-1;y<=Tank1.TankY+1;i++,y++)
	{
		for(unsigned int j=0,x=Tank1.TankX-1;x<=Tank1.TankX+1;j++,x++)
		{
			TankCursor( x, y);
			printf("  ");
		}
	}
}
//画地图
void DrawMap()
{
	for(int i=0;i<40;i++)
	{
		for(int j=0;j<40;j++)
		{
			if(g_nMapData[i][j]==WALL_B)
			{
				printf("■");
			}
			else if(g_nMapData[i][j]==WALL_A)
			{
				printf("▓");
			}
			else if(g_nMapData[i][j]==WALL_F)
			{
				printf("◆");
			}
			else
			{
				printf("  ");
			}
		}
	}
}
//坦克（子弹、移动碰撞）引擎
void BulletMove()
{	
	int i;
	for ( i = 0; i < 160; i++)
	{
		if ( g_TankBullet[i] != 0)
		{
			if(	g_TankBullet[i]->TankY!=Tank.TankY||g_TankBullet[i]->TankX!=Tank.TankX)	//判断当前子弹位置是否与坦克位置重合
			{
				if(g_nMapData[g_TankBullet[i]->TankY][g_TankBullet[i]->TankX]==WALL_B)//不重合判断是否打到铁墙  打到清除子弹
				{
					g_TankBullet[i]=0;//清除子弹
					break;
				}
				else if((g_TankBullet[i]->Orientation==0||g_TankBullet[i]->Orientation==1)&&
					g_nMapData[g_TankBullet[i]->TankY][g_TankBullet[i]->TankX]==WALL_A)//判断是否为土砖 Y清零  空格覆盖
				{
					g_nMapData[g_TankBullet[i]->TankY][g_TankBullet[i]->TankX]=0;//打掉前方横排三格土墙
					g_nMapData[g_TankBullet[i]->TankY][g_TankBullet[i]->TankX-1]=0;
					g_nMapData[g_TankBullet[i]->TankY][g_TankBullet[i]->TankX+1]=0;
					TankCursor(g_TankBullet[i]->TankX, g_TankBullet[i]->TankY);
					printf(" ");
					TankCursor(g_TankBullet[i]->TankX-1, g_TankBullet[i]->TankY);
					printf(" ");
					TankCursor(g_TankBullet[i]->TankX+1, g_TankBullet[i]->TankY);
					printf(" ");
					g_TankBullet[i]=0;
					break;
				}
				else if((g_TankBullet[i]->Orientation==2||g_TankBullet[i]->Orientation==3)&&
					g_nMapData[g_TankBullet[i]->TankY][g_TankBullet[i]->TankX]==WALL_A)
				{
					g_nMapData[g_TankBullet[i]->TankY][g_TankBullet[i]->TankX]=0;//打掉前方竖排三格土墙
					g_nMapData[g_TankBullet[i]->TankY-1][g_TankBullet[i]->TankX]=0;
					g_nMapData[g_TankBullet[i]->TankY+1][g_TankBullet[i]->TankX]=0;
					TankCursor(g_TankBullet[i]->TankX, g_TankBullet[i]->TankY);
					printf(" ");
					TankCursor(g_TankBullet[i]->TankX, g_TankBullet[i]->TankY-1);
					printf(" ");
					TankCursor(g_TankBullet[i]->TankX, g_TankBullet[i]->TankY+1);
					printf(" ");
					g_TankBullet[i]=0;
					break;
				}
				else if(g_nMapData[g_TankBullet[i]->TankY][g_TankBullet[i]->TankX]==WALL_F)
				{
					system("cls");
					pos.X=35;
					WriteConsoleOutputCharacter(hout,szStr[8],35,pos,&dwRet);
					Sleep(2000);
					exit(0);
				}
				else if(g_nMapData[g_TankBullet[i]->TankY][g_TankBullet[i]->TankX]==1)
				{
					system("cls");
					pos.X=35;
					WriteConsoleOutputCharacter(hout,szStr[8],35,pos,&dwRet);
					Sleep(3000);
					exit(0);
				}
				TankCursor(g_TankBullet[i]->TankX, g_TankBullet[i]->TankY);
				printf(" ");				
			}
			if((g_nMapData[g_TankBullet[i]->TankY-1][g_TankBullet[i]->TankX]==WALL_B)||//物体碰撞检查 铁墙不可销毁
				(g_nMapData[g_TankBullet[i]->TankY+1][g_TankBullet[i]->TankX]==WALL_B)||
				(g_nMapData[g_TankBullet[i]->TankY][g_TankBullet[i]->TankX+1]==WALL_B)||
				(g_nMapData[g_TankBullet[i]->TankY][g_TankBullet[i]->TankX-1]==WALL_B))
			{
				g_TankBullet[i]=0;
				break;
			}
			switch (g_TankBullet[i]->Orientation)
			{
			case UP:
				if(g_TankBullet[i]->TankY==Tank.TankY||g_TankBullet[i]->TankY==Tank1.TankY)//tank第一颗子弹打出位置和第一颗子弹贴墙打检测
				{
					g_TankBullet[i]->TankY=g_TankBullet[i]->TankY-2;
					if(g_nMapData[g_TankBullet[i]->TankY][g_TankBullet[i]->TankX]==WALL_B)//第一颗子弹贴墙打检测
					{
						break;
					}
					TankCursor(g_TankBullet[i]->TankX,g_TankBullet[i]->TankY);
					printf("⊕");
				}
				else if(g_TankBullet[i]->TankY-1==Tank1.TankY)//坦克1子弹中心点 
				{
					g_TankBullet[i]->TankY=g_TankBullet[i]->TankY-3;
					if(g_nMapData[g_TankBullet[i]->TankY][g_TankBullet[i]->TankX]==WALL_B)//tank1第一颗子弹贴墙打检测
					{
						break;
					}
					TankCursor(g_TankBullet[i]->TankX,g_TankBullet[i]->TankY);
					printf("⊕");

				}
				else
				{
					g_TankBullet[i]->TankY--;
					TankCursor(g_TankBullet[i]->TankX,g_TankBullet[i]->TankY);
					printf("⊕");
				}
				break;
			case DOWN:
				if(g_TankBullet[i]->TankY==Tank.TankY||g_TankBullet[i]->TankY==Tank1.TankY)
				{
				g_TankBullet[i]->TankY=g_TankBullet[i]->TankY+2;
				if(g_nMapData[g_TankBullet[i]->TankY][g_TankBullet[i]->TankX]==WALL_B)
				{
					break;
				}
				TankCursor(g_TankBullet[i]->TankX,g_TankBullet[i]->TankY);
				printf("⊕");
				}
				else if(g_TankBullet[i]->TankY+1==Tank1.TankY)
				{
					g_TankBullet[i]->TankY=g_TankBullet[i]->TankY+3;
					if(g_nMapData[g_TankBullet[i]->TankY][g_TankBullet[i]->TankX]==WALL_B)//tank1第一颗子弹贴墙打检测
					{
						break;
					}
					TankCursor(g_TankBullet[i]->TankX,g_TankBullet[i]->TankY);
					printf("⊕");
				}
				else
				{
				g_TankBullet[i]->TankY++;
				//g_TankBullet[i]->TankY+=3;
				TankCursor(g_TankBullet[i]->TankX,g_TankBullet[i]->TankY);
				printf("⊕");
				}
				break;
			case LEFT:
				if(g_TankBullet[i]->TankX==Tank.TankX||g_TankBullet[i]->TankX==Tank1.TankX)
				{
				g_TankBullet[i]->TankX=g_TankBullet[i]->TankX-2;
				if(g_nMapData[g_TankBullet[i]->TankY][g_TankBullet[i]->TankX]==WALL_B)
				{
					break;
				}
				TankCursor(g_TankBullet[i]->TankX,g_TankBullet[i]->TankY);
				printf("⊕");
				}
				else if(g_TankBullet[i]->TankX-1==Tank1.TankX)
				{
					g_TankBullet[i]->TankX=g_TankBullet[i]->TankX-3;
					if(g_nMapData[g_TankBullet[i]->TankY][g_TankBullet[i]->TankX]==WALL_B)//tank1第一颗子弹贴墙打检测
					{
						break;
					}
					TankCursor(g_TankBullet[i]->TankX,g_TankBullet[i]->TankY);
					printf("⊕");
				}
				else
				{
				g_TankBullet[i]->TankX--;
				TankCursor(g_TankBullet[i]->TankX,g_TankBullet[i]->TankY);
				printf("⊕");
				}
				break;
			case RIGHT:
				if(g_TankBullet[i]->TankX==Tank.TankX||g_TankBullet[i]->TankX==Tank1.TankX)
				{
					g_TankBullet[i]->TankX=g_TankBullet[i]->TankX+2;
					if(g_nMapData[g_TankBullet[i]->TankY][g_TankBullet[i]->TankX]==WALL_B)
					{
						break;
					}
					TankCursor(g_TankBullet[i]->TankX,g_TankBullet[i]->TankY);
					printf("⊕");
				}
				else if(g_TankBullet[i]->TankX+1==Tank1.TankX)
				{
					g_TankBullet[i]->TankX=g_TankBullet[i]->TankX+3;
					if(g_nMapData[g_TankBullet[i]->TankY][g_TankBullet[i]->TankX]==WALL_B)//tank1第一颗子弹贴墙打检测
					{
						break;
					}
					TankCursor(g_TankBullet[i]->TankX,g_TankBullet[i]->TankY);
					printf("⊕");
				}
				else
				{
					g_TankBullet[i]->TankX++;
					//g_TankBullet[i]->TankX+=3;
					TankCursor(g_TankBullet[i]->TankX,g_TankBullet[i]->TankY);
					printf("⊕");
				}
				break;
			}
		}
	}
}
//Tank1移动及碰撞检测
void Tank1Collisionetection(int caozuo)
{
	if(caozuo<4){
		Tank1.ChangeOrientation=caozuo;
	}
	if(caozuo>3&&caozuo<7){
		FireBullet(Tank1);
	}
	ClrDrawStyle1();
	Tank1.Orientation=Tank1.ChangeOrientation;
	switch(Tank1.ChangeOrientation)
	{
	case UP:		
		if(g_nMapData[Tank1.TankY-2][Tank1.TankX]==(WALL_B)||//坦克1移动
		g_nMapData[Tank1.TankY-2][Tank1.TankX]==WALL_A||
		g_nMapData[Tank1.TankY-2][Tank1.TankX+1]==WALL_A||
		g_nMapData[Tank1.TankY-2][Tank1.TankX-1]==WALL_A||
		g_nMapData[Tank1.TankY-2][Tank1.TankX]==WALL_F||
		g_nMapData[Tank1.TankY-2][Tank1.TankX+1]==WALL_F||
		g_nMapData[Tank1.TankY-2][Tank1.TankX-1]==WALL_F)
		{
			break;
		}				
		Tank1.TankY--;		
		break;
	case DOWN:
		if((g_nMapData[Tank1.TankY+2][Tank1.TankX]==WALL_B)||
			g_nMapData[Tank1.TankY+2][Tank1.TankX]==(WALL_A)||
			g_nMapData[Tank1.TankY+2][Tank1.TankX+1]==(WALL_A)||
			g_nMapData[Tank1.TankY+2][Tank1.TankX-1]==(WALL_A)||
			g_nMapData[Tank1.TankY+2][Tank1.TankX]==(WALL_F)||
			g_nMapData[Tank1.TankY+2][Tank1.TankX+1]==(WALL_F)||
			g_nMapData[Tank1.TankY+2][Tank1.TankX-1]==(WALL_F)
			)
		{
			break;
		}		
		Tank1.TankY++;		
		break;
	case LEFT:
		if((g_nMapData[Tank1.TankY][Tank1.TankX-2]==WALL_B)||
			g_nMapData[Tank1.TankY][Tank1.TankX-2]==(WALL_A)||
			g_nMapData[Tank1.TankY+1][Tank1.TankX-2]==(WALL_A)||
			g_nMapData[Tank1.TankY-1][Tank1.TankX-2]==(WALL_A)||
			g_nMapData[Tank1.TankY][Tank1.TankX-2]==(WALL_F)||
			g_nMapData[Tank1.TankY+1][Tank1.TankX-2]==(WALL_F)||
			g_nMapData[Tank1.TankY-1][Tank1.TankX-2]==(WALL_F)
			)
		{
			break;
		}		
		Tank1.TankX--;	
		break;
	case RIGHT:
		if((g_nMapData[Tank1.TankY][Tank1.TankX+2]==WALL_B)||
			g_nMapData[Tank1.TankY][Tank1.TankX+2]==(WALL_A)||
			g_nMapData[Tank1.TankY+1][Tank1.TankX+2]==(WALL_A)||
			g_nMapData[Tank1.TankY-1][Tank1.TankX+2]==(WALL_A)||
			g_nMapData[Tank1.TankY][Tank1.TankX+2]==(WALL_F)||
			g_nMapData[Tank1.TankY+1][Tank1.TankX+2]==(WALL_F)||
			g_nMapData[Tank1.TankY-1][Tank1.TankX+2]==(WALL_F)
			)					//碰撞实现
		{
			break;
		}
		Tank1.TankX++;	
		break;
	}
	DrawStyle1();
	Tank1.ID=1;
	//switch (Tank1.ChangeOrientation)
	//{
	//case UP:
	//	Tank1.TankY++;break;//tank1真实子弹位置
	//case DOWN:
	//	Tank1.TankY--;break;
	//case LEFT:
	//	Tank1.TankX++;break;
	//case RIGHT:
	//	Tank1.TankX--;break;
	//}
}
//Tank移动及碰撞检测
void TankCollisionetection()
{
	ClrDrawStyle();
	Tank.Orientation=Tank.ChangeOrientation;
	switch(Tank.ChangeOrientation)
	{
	case UP:
		if(g_nMapData[Tank.TankY-2][Tank.TankX]==(WALL_B)||
			g_nMapData[Tank.TankY-2][Tank.TankX]==WALL_A||
			g_nMapData[Tank.TankY-2][Tank.TankX+1]==WALL_A||
			g_nMapData[Tank.TankY-2][Tank.TankX-1]==WALL_A||
			g_nMapData[Tank.TankY-2][Tank.TankX]==WALL_F||
			g_nMapData[Tank.TankY-2][Tank.TankX+1]==WALL_F||
			g_nMapData[Tank.TankY-2][Tank.TankX-1]==WALL_F
			)
		{
			break;
		}
		Tank.TankY--;
		break;
	case DOWN:
		if((g_nMapData[Tank.TankY+2][Tank.TankX]==WALL_B)||
			g_nMapData[Tank.TankY+2][Tank.TankX]==(WALL_A)||
			g_nMapData[Tank.TankY+2][Tank.TankX+1]==(WALL_A)||
			g_nMapData[Tank.TankY+2][Tank.TankX-1]==(WALL_A)||
			g_nMapData[Tank.TankY+2][Tank.TankX]==(WALL_F)||
			g_nMapData[Tank.TankY+2][Tank.TankX+1]==(WALL_F)||
			g_nMapData[Tank.TankY+2][Tank.TankX-1]==(WALL_F)
			)
		{
			break;
		}
		Tank.TankY++;
		break;
	case LEFT:
		if((g_nMapData[Tank.TankY][Tank.TankX-2]==WALL_B)||
			g_nMapData[Tank.TankY][Tank.TankX-2]==(WALL_A)||
			g_nMapData[Tank.TankY+1][Tank.TankX-2]==(WALL_A)||
			g_nMapData[Tank.TankY-1][Tank.TankX-2]==(WALL_A)||
			g_nMapData[Tank.TankY][Tank.TankX-2]==(WALL_F)||
			g_nMapData[Tank.TankY+1][Tank.TankX-2]==(WALL_F)||
			g_nMapData[Tank.TankY-1][Tank.TankX-2]==(WALL_F)
			)
		{
			break;
		}
		Tank.TankX--;
		break;
	case RIGHT:
		if((g_nMapData[Tank.TankY][Tank.TankX+2]==WALL_B)||
			g_nMapData[Tank.TankY][Tank.TankX+2]==(WALL_A)||
			g_nMapData[Tank.TankY+1][Tank.TankX+2]==(WALL_A)||
			g_nMapData[Tank.TankY-1][Tank.TankX+2]==(WALL_A)||
			g_nMapData[Tank.TankY][Tank.TankX+2]==(WALL_F)||
			g_nMapData[Tank.TankY+1][Tank.TankX+2]==(WALL_F)||
			g_nMapData[Tank.TankY-1][Tank.TankX+2]==(WALL_F)
			)					//碰撞实现
		{
			break;
		}
		Tank.TankX++;
		break;
	}
	DrawStyle();
}
//定义开火
void FireBullet(TANK Tank)
{
	TANK *bullet = (TANK *)malloc(sizeof(TANK));
	bullet->TankX = Tank.TankX;
	bullet->TankY = Tank.TankY;
	bullet->Orientation = Tank.Orientation;
	for (int i = 0; i < 160; i++)
	{
		if ( g_TankBullet[i] == 0)
		{
			g_TankBullet[i] = bullet;
			break;
		}
	}
}
//图形界面
void GraphicsInterface()
{
		Tank.TankY=37;
		Tank.TankX=14;
		Tank1.TankY=2;
		Tank1.TankX=2;
		GameMenu();
		DrawMap();
		DrawStyle();
		DrawStyle1();
}
int _tmain(int argc, _TCHAR* argv[])
{
		char ch=GetKey();
		GraphicsInterface();//图形界面
		clock_t start = clock();
		clock_t finish ;
		clock_t start1= clock();
		clock_t finish1 ;
		srand((unsigned)time(NULL));//随机值
		while(1)
		{
			finish1=clock();
			if( (finish1 -start1 )>500)//控制敌方坦克方向及移动
			{
					start1  = finish1 ;				
					Tank1Collisionetection(rand()%15);
			}
			finish = clock();
			if( (finish -start )>50)//控制子弹频率
			{
					start  = finish ;
					BulletMove();
			}		
			ch=GetKey();
			if(ch==0)
			{
				continue;
			}
			if(ch==27)
			{
				break;
			}
			switch (ch)
			{
			case 'w':
			case 'W':
				Tank.ChangeOrientation=0;
				TankCollisionetection();
				break;
			case 's':
			case 'S':
				Tank.ChangeOrientation=1;
				TankCollisionetection();
				break;
			case 'a':
			case 'A':
				Tank.ChangeOrientation=2;
				TankCollisionetection();
				break;
			case 'd':
			case 'D':
				Tank.ChangeOrientation=3;
				TankCollisionetection();
				break;
			case 'j':
			case 'J':
				FireBullet(Tank);
				break;
			}	
		}
	
	return 0;
}
