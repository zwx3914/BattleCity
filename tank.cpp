#include "tank.h"


using std::cout;
using std::endl;

/**
*变量
*/
int speed = 5;						//游戏速度
int level = 0;						//当前关卡
int Canvas[HIGHT][WIDTH] = {0};		//游戏画布数组
unsigned int interval[12] = {1, 1,1,1,1, 1,1,1,1, 1, 1, 1};	//间隔计数器
int bornPosition = 0;				//敌方坦克3个出生点
int enemynum = 20;					//敌军坦克数
int players = 1;					//玩家数

Tank p1;							//玩家1
Tank p2;							//玩家2
Tank enemys[4];						//最多4个敌方坦克



/**
*自定义函数声明
*/

void HideCursor(int n);  			//设置是否隐藏光标
void gotoxy(int x, int y);			//设置光标位置
void setColor(int color);			//设置文本颜色
void ASCIIArt();					//打印字符画
void printInfo();					//打印游戏信息
void selectBox(int x, int y);		//显示选中框
void cleanSelectBox(int x, int y);	//清除选中框
bool pass(int n);					//判断是否能通过
bool bornCheck(int position);		//检查当前出生点
void stop();						//暂停
void initialized();					//初始化

/**
*函数定义
*/
void HideCursor(int n)  			//设置是否隐藏光标
{   
	n %= 2;
	CONSOLE_CURSOR_INFO cursor_info={1,n};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info); 
}

void gotoxy(int x, int y)			//设置光标位置
{
	COORD r;
	r.X = x;
	r.Y = y;
	
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	
	SetConsoleCursorPosition(h, r);
}

void setColor(int color)			//设置文本颜色
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	
	SetConsoleTextAttribute(h, color);
}

void  ASCIIArt()					//打印字符画
{
	char str[] = "         ";
	cout << str << " =@@@@@@@`       =@@^     @@@@@@@@@@@O@@@@@@@@@@@ .@@*       =@@@@@@@^	" << endl;
	cout << str << " =@^   .@@^     =@@@@^        =@@         @@^     .@@*       =@^		" << endl;
	cout << str << " =@^   ,@@.    .@@`,@@.       =@@         @@^     .@@*       =@^		" << endl;
	cout << str << " =@@@@@@^      /@^  =@\\       =@@         @@^     .@@*       =@\\]]]]]	" << endl;
	cout << str << " =@^   .\\@\\   =@/    @@^      =@@         @@^     .@@*       =@^		" << endl;
	cout << str << " =@^    =@@* ,@@[[[[[[@@`     =@@         @@^     .@@*       =@^		" << endl;
	cout << str << " =@^   ./@/ .@@^      =@@.    =@@         @@^     .@@*       =@^		" << endl;
	cout << str << " =@@@@@@[.  /@/        \\@\\    =@@         @@^     .@@@@@@@@^ =@@@@@@@@" << endl;
	cout << str << "																		" << endl;
	cout << str << "                ]/@@@@] .@@@@@=@@@@@@@@@@@\\@\\      /@/	" << endl;
	cout << str << "             ,@@/     ,`  =@^      @@^     @@\\    =@@		" << endl;
	cout << str << "            =@@.          =@^      @@^      @@\\  =@@		" << endl;
	cout << str << "           .@@^           =@^      @@^      .@@^=@@			" << endl;
	cout << str << "           .@@^           =@^      @@^       .@@@@			" << endl;
	cout << str << "            @@\\           =@^      @@^        .@@.			" << endl;
	cout << str << "            .@@\\.         =@^      @@^        .@@.			" << endl; 
	cout << str << "              ,\\@@@@@@@`.@@@@@.    @@^        .@@.			" << endl;
}

void printInfo()					//打印游戏信息
{
	gotoxy(WIDTH * 2,3);
	setColor(0x0e);
	cout << "第 " << level+1 << " 关";

	gotoxy(WIDTH * 2,8);
	cout << "剩余敌军";
	gotoxy(WIDTH * 2,9);
	cout << "数量：" << setiosflags(std::ios::left) << std::setw(2) << enemynum;//向左对齐占两个字符宽度
	gotoxy(WIDTH * 2,11);
	cout << "当前游戏";
	gotoxy(WIDTH * 2,12);
	cout << "速度：" << setiosflags(std::ios::left) << std::setw(2) << speed;	
	gotoxy(WIDTH * 2,14);
	cout << "P1 HP：" << p1.GetHP();
	
	if (players == 2)
	{
		gotoxy(WIDTH * 2,16);
		cout << "P2 HP：" << p2.GetHP();
	}
	
	setColor(0x07);
}

void selectBox(int x, int y)		//显示选中框
{	
	gotoxy(x,y);
	cout << "------------";
	gotoxy(x,y+1);
	cout << "|";
	gotoxy(x+11,y+1);
	cout << "|";
	gotoxy(x,y+2);
	cout << "------------";
}

void cleanSelectBox(int x, int y)	//清除选中框
{
	gotoxy(x,y);
	cout << "            ";
	gotoxy(x,y+1);
	cout << " ";
	gotoxy(x+11,y+1);
	cout << " ";
	gotoxy(x,y+2);
	cout << "            ";
}

bool pass(int n)					//判断是否能通过
{
	switch(n)
	{
		case 0:
		case 2:
			return true;
			break;
		default:	
			return false;	
			break;
	}
}

bool bornCheck(int position)		//检查当前出生点
{
	int x = 2 + 18 * position;
	int y = 2;
	int count = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			count = Canvas[y+i-1][x+j-1];
		}
	}
	
	if (count == 0)
		return true;
	else
		return false;
}

void stop()							//暂停
{
	setColor(0x0e);
	gotoxy(WIDTH * 2,13);
	cout << "暂停";
	
	while (true)
	{
		if (GetAsyncKeyState(VK_RETURN)& 0x8000)
		{	
			gotoxy(WIDTH * 2,13);
			cout << "    ";
			setColor(0x07);
			break;
		}
	}
}

void initialized()					//初始化
{
	system("cls");
	srand(time(0));
	//初始化画布
	for(int i = 0; i < HIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			Canvas[i][j] = Map[level][i][j];
		}
	}
	
	//显示地图
	gotoxy(0,0);
	
	for (int i = 0; i < HIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			switch(Canvas[i][j])
			{
				case 0:		cout << "  ";	break;
				case 1:		setColor(0x04);	cout << "▓";	break;	//红砖▓
				case 2:		setColor(0x6a);	cout << "▓";	break;	//树林▓
				case 3:		cout << "  ";	break;
				case 4:		setColor(0x07);	cout << "■";	break;	//边界■
				case 5:		setColor(0x9f);	cout << "～";	break;	//水面～
				case 6:		setColor(0x7f);	cout << "■";	break;	//铁砖■
				//case 9:		cout << "㊣";	break;
				default:	cout << "  ";	break;
			}
			setColor(0x07);
		}
		cout << endl;
	}
	
	gotoxy(38,37);
	cout << "◣★◢";
	gotoxy(38,38);
	cout << "███";
	gotoxy(38,39);
	cout << "◢█◣";
	
	p1 = Tank(15, 38 , 20, 0);		//初始化玩家1
	if (players == 2)				//初始化玩家2
	{
		p2 = Tank(25, 38, 21, 0);
		p2.SetColor(0x0e);
	}
	
	enemynum = 20;
	enemys[0] = Tank();
	enemys[1] = Tank();
	enemys[2] = Tank();
	enemys[3] = Tank();
	
	
}

void startMenu()					//开始菜单
{
	HideCursor(0);  			//隐藏光标

	gotoxy(0,5);
	setColor(0x0e);
	ASCIIArt();
	

	gotoxy(40,30);
	cout << "单人游戏" << endl;
	gotoxy(40,32);
	cout << "双人游戏" << endl;
	gotoxy(40,34);
	cout << "帮助"	<< endl;
	gotoxy(40,36);
	cout << "退出游戏" << endl;

	gotoxy(2,43);
	cout << "v1.0";

	setColor(0x07);
	
	int choise = 29;
	char ch;
	selectBox(38, choise);
	while (true)
	{
		cleanSelectBox(38,choise);

		if (GetAsyncKeyState( VK_DOWN)& 0x8000)
			choise += 2;
		else if (GetAsyncKeyState( VK_UP )& 0x8000)
			choise -= 2;

		if (choise == 37)
		{
			choise = 29;
		}
		else if (choise == 27)
		{ 
			choise = 35;
		}
		selectBox(38, choise);
		if (GetAsyncKeyState(VK_RETURN)& 0x8000)
			break;
		Sleep(100);
	}

	switch(choise)
	{
		case 29: 
			players = 1;
			initialized();		
			break;

		case 31:
			players = 2;
			initialized();
			break;

		case 33:
			system("cls");
			gotoxy(WIDTH-18, HIGHT/2);
			cout << "方向键：玩家1【↑ ↓ ← →】玩家2【W S A D】";
			gotoxy(WIDTH-18, HIGHT/2+1);
			cout << "攻击键：玩家1【小键盘0】    玩家2【J】";
			gotoxy(WIDTH-18, HIGHT/2+2);
			cout << "确认/跳关/恢复游戏键：回车键";
			gotoxy(WIDTH-18, HIGHT/2+3);
			cout << "暂停键：空格键";
			gotoxy(WIDTH-18, HIGHT/2+4);
			cout << "游戏速度增减(1最快，20最慢)：+ -";
			gotoxy(WIDTH-18, HIGHT/2+5);
			cout << "退出键：ESC";
			gotoxy(70, 42);
			system("pause");
			system("cls");
			startMenu();
			break;

		case 35:
			exit(0);
		break;
	}
}

void show()							//显示画面
{
	printInfo();
	
	//p1.Clean();
	
	p1.GetBullet().Draw();
	p1.Draw();
	
	if (players == 2)
	{
		p2.GetBullet().Draw();
		p2.Draw();
	}
	
	for (int i = 0; i < 4; i++)
	{	
		enemys[i].GetBullet().Draw();
		enemys[i].Draw();
	}
	
	/*
	for (int i = 0; i < HIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (Canvas[i][j] == 2)
			{
				gotoxy(j * 2, i);
				setColor(0x6a);
				cout << "▓";
			}			
		}
	}
	setColor(0x07);
	*/
}

void updateWithInput()				//与用户有关的输入
{
	if (players == 2)
	{
		if (GetAsyncKeyState(87)& 0x8000)  
			p2.Move(UP);
		else if (GetAsyncKeyState(83)& 0x8000)  
			p2.Move(DOWN);
		else if (GetAsyncKeyState(65)& 0x8000)
			p2.Move(LEFT);
		else if (GetAsyncKeyState(68)& 0x8000)  
			p2.Move(RIGHT);
		else if (GetAsyncKeyState(74) & 0x8000)
		{
			p2.Fire();
		}
		/*
		char input;
		if (_kbhit())
		{
			//有按键接受
			fflush(stdin);
			input = tolower(_getch());
			if (strchr("wsadj", input))
			{
				if(input == 'w')
					p2.Move(UP);
				else if(input == 's')
					p2.Move(DOWN);
				else if(input == 'a')
					p2.Move(LEFT);
				else if(input == 'd')
					p2.Move(RIGHT);
				else if(input == 'j')
					p2.Fire();
			}

		}
		*/
	}
	
	int count = 0;
	if (GetAsyncKeyState(VK_UP)& 0x8000)  
		p1.Move(UP);
	else if (GetAsyncKeyState(VK_DOWN)& 0x8000)  
		p1.Move(DOWN);
	else if (GetAsyncKeyState(VK_LEFT)& 0x8000)
        p1.Move(LEFT);
	else if (GetAsyncKeyState(VK_RIGHT)& 0x8000)  
		p1.Move(RIGHT);
	else if (GetAsyncKeyState(VK_NUMPAD0) & 0x8000)
	{
		p1.Fire();
	}
	else if (GetAsyncKeyState( 0x1B )& 0x8000)  // Esc键
		exit(0);                               	//退出程序函数
	else if (GetAsyncKeyState(VK_RETURN)& 0x8000)  //空格
	{
		level++;
		level %= 8;
		initialized();
	}
	else if (GetAsyncKeyState(0x20) & 0x8000)
		stop();
	else if (count++%7==0)            //这里添加计数器是为了防止按键粘连不能达到微调效果
	{
		if (speed>1 && GetAsyncKeyState( 0x6B )& 0x8000)   // +键
		{
			speed--;
			
		}
		else if (speed<20 && GetAsyncKeyState( 0x6D )& 0x8000)  // - 键
		{
			speed++;
			
		}
	}
}

void updateWithoutInput()			//与用户无关的输入
{
	p1.GetBullet().Move();
	p1.GetBullet().Check();
	
	if (players == 2)
	{
		p2.GetBullet().Move();
		p2.GetBullet().Check();
	}
	
	for (int i = 0; i < 4; i++)
	{
		enemys[i].GetBullet().Move();
		enemys[i].GetBullet().Check();
	}
	
	//新建敌方坦克
	for (int i = 0; i < 4; i++)
	{
		if (enemynum > 0 && enemys[i].GetHP() <= 0 && interval[9]++%90 == 0)
		{
			for (int j = 0; j < 3; j++)
			{	
				bornPosition %= 3;
				if (bornCheck(bornPosition))
				{
					enemys[i] = Tank(bornPosition * 18 +2, 2, 10+i, rand() % 3 + 1);
					bornPosition++;
					enemynum--;
					break;
				}
				
			}
		}
	}
	
	//敌方坦克行动
	for(int i = 0; i < 4; i ++)
	{
		if(enemys[i].GetModel() == 2 && interval[i+1]++%2 == 0) //四个坦克中的快速坦克单独使用计数器1,2,3,4
			enemys[i].TankAI();
		if(enemys[i].GetModel() != 2 && interval[i+5]++%3 == 0) //四个坦克中的慢速坦克单独使用计数器5,6,7,8
			enemys[i].TankAI();
		
	}
}

void gameCheck()					//游戏状态检测
{
	if (p1.GetHP() <= 0 && p2.GetHP() <= 0)
	{
		gotoxy(WIDTH-4, HIGHT/2);
		cout << "游戏结束！";
		Sleep(3000);
		exit(0);
	}	
	
	if(enemynum <= 0 && enemys[0].GetHP() == 0 && enemys[1].GetHP() == 0 
		&& enemys[0].GetHP() == 0	&& enemys[3].GetHP() == 0)
	{
		level++;
		
		if(level > 7)
		{
			gotoxy(WIDTH-4, HIGHT/2);
			cout << "恭喜通关！";
			Sleep(3000);
			exit(0);
		}
		else
		{
			gotoxy(WIDTH-4, HIGHT/2);
			cout << "恭喜过关！";
			Sleep(2000);
			initialized();
		}
	}
}

/**
*子弹类实现
*/
Bullet::Bullet(int a, int b, DIRECTION c, int d, bool e)
{
	x = a;
	y = b;
	dire = c;
	vist = d;
	exist = e;
}

void Bullet::Reset(int a, int b, DIRECTION c, int d, bool e)
{
	x = a;
	y = b;
	dire = c;
	vist = d;
	exist = e;
}

void Bullet::Check()
{
	if (exist == true)
	{
		if(vist >= 20)
		{
			switch(Canvas[y][x])
			{
				case 0:
					break;
				case 1:
					switch(dire)
					{
						case UP:
						case DOWN:
							for (int i = -1; i < 2; i++)
							{
								if(Canvas[y][x+i] == 1)						
								{
									gotoxy((x+i)*2,y);
									cout << "  ";
									Canvas[y][x+i] = 0;
								}
							}
							break;

						case LEFT:
						case RIGHT:
							for (int i = -1; i < 2; i++)
							{
								if(Canvas[y+i][x] == 1)
								{
									gotoxy(x*2,y+i);
									cout << "  ";
									Canvas[y+i][x] = 0;
								}
							}
							break;
					}
					this->Clean();
					exist = false;
					break;
				case 2:
					break;
				case 5:
					break;
				case 9:
					this->Clean();
					exist = false;
					gotoxy(38,37);
					cout << "      ";
					gotoxy(38,38);
					cout << "◢◣  ";
					gotoxy(38,39);
					cout << "███";
					gotoxy(WIDTH-4, HIGHT/2);
					cout << "你家没了！";
					Sleep(3000);
					exit(0);
					break;
				case 10:
					enemys[0].SetHP(-1);
					if (enemys[0].GetHP() <= 0)
					{
						enemys[0].Clean();
					}
					this->Clean();
					exist = false;
					break;
				case 11:
					enemys[1].SetHP(-1);
					if (enemys[1].GetHP() <= 0)
					{
						enemys[1].Clean();
					}
					this->Clean();
					exist = false;
					break;
				case 12:
					enemys[2].SetHP(-1);
					if (enemys[2].GetHP() <= 0)
					{
						enemys[2].Clean();
					}
					this->Clean();
					exist = false;
					break;
				case 13:
					enemys[3].SetHP(-1);
					if (enemys[3].GetHP() <= 0)
					{
						enemys[3].Clean();
					}
					this->Clean();
					exist = false;
					break;
				case 20:
					this->Clean();
					exist = false;
					break;
				case 21:
					this->Clean();
					exist = false;
					break;
				default:			
					this->Clean();
					exist = false;
					break;
			}
		}
		else
		{
			switch(Canvas[y][x])
			{
				case 0:
					break;
				case 1:
					switch(dire)
					{
						case UP:
						case DOWN:
							for (int i = -1; i < 2; i++)
							{
								if(Canvas[y][x+i] == 1)						
								{
									gotoxy((x+i)*2,y);
									cout << "  ";
									Canvas[y][x+i] = 0;
								}
							}
							break;

						case LEFT:
						case RIGHT:
							for (int i = -1; i < 2; i++)
							{
								if(Canvas[y+i][x] == 1)
								{
									gotoxy(x*2,y+i);
									cout << "  ";
									Canvas[y+i][x] = 0;
								}
							}
							break;
					}
					this->Clean();
					exist = false;
					break;
				case 2:
					break;
				case 5:
					break;
				case 9:
					this->Clean();
					exist = false;
					gotoxy(38,37);
					cout << "      ";
					gotoxy(38,38);
					cout << "◢◣  ";
					gotoxy(38,39);
					cout << "███";
					gotoxy(WIDTH-4, HIGHT/2);
					cout << "你家没了！";
					Sleep(3000);
					exit(0);
					break;
				case 10:
				case 11:
				case 12:
				case 13:
					this->Clean();
					exist = false;
					break;
				case 20:
					p1.SetHP(-1);
					p1.Clean();
					if (p1.GetHP() > 0)
					{
						p1.SetPosition(15,38);
						p1.MarkPosition();
					}	
					this->Clean();
					exist = false;
					break;
				case 21:
					p2.SetHP(-1);
					p2.Clean();
					if (p2.GetHP() > 0)
					{
						p2.SetPosition(25,38);
						p2.MarkPosition();
					}	
					this->Clean();
					exist = false;
					break;
				default:			
					this->Clean();
					exist = false;
					break;
			}
		}
	}
}

void Bullet::Move()
{
	if (exist == true)
	{
		this->Clean();
		switch(dire)
		{
			case UP:	y--;	break;
			case DOWN:	y++;	break;
			case LEFT:	x--;	break;
			case RIGHT:	x++;	break;
		}
	}
}

void Bullet::Draw()
{
	if (exist == true)
	{
		gotoxy(x*2, y);
		
		switch(Canvas[y][x])
		{
			case 2:		setColor(0x6a);	cout << "▓";	break;	//树林▓
			case 5:		setColor(0x9f);	cout << "☉";	break;
			case 4:		break;
			case 6:		break;
			default:	cout << "☉";	break;
		}
		setColor(0x07);
	}
}

void Bullet::Clean()
{
	gotoxy(x*2, y);
	switch(Canvas[y][x])
	{	
		case 2:		setColor(0x6a);	cout << "▓";	break;	//树林▓
		case 5:		setColor(0x9f);	cout << "～";	break;	//水面～
		case 4:		break;
		case 6:		break;
		default:	setColor(0x07);	cout << "  ";	break;
	}
	setColor(0x07);
}


/**
*坦克类实现
*/
const char * Tank::style[4][3][4] = {
	{
		//我方坦克
		{"◢┃◣", "◢━◣", "◢┳◣", "◢┳◣"},
		{"┣●┫", "┣●┫", "━●┃", "┃●━"},
		{"◥━◤", "◥┃◤", "◥┻◤", "◥┻◤"}
	}, 
	{
		//普通坦克
		{"┏┃┓", "┏┳┓", "┏┳┓", "┏┳┓"},
		{"┣●┫", "┣●┫", "━●┫", "┣●━"},
		{"┗┻┛", "┗┃┛", "┗┻┛", "┗┻┛"}
	}, 
	{
		//快速坦克
		{"┏┃┓", "◢━◣", "┏┳◣", "◢┳┓"},
		{"┣●┫", "┣●┫", "━●┃", "┃●━"},
		{"◥━◤", "┗┃┛", "┗┻◤", "◥┻┛"}
	},
	{
		//超级坦克
		{"╔┃╗", "╔╦╗", "╔╦╗", "╔╦╗"},
		{"╠█╣", "╠█╣", "━█╣", "╠█━"},
		{"╚╩╝", "╚┃╝", "╚╩╝", "╚╩╝"}
	}
};

Tank::Tank()
{
	x = 2;
	y = 2;
	camp = 10;
	model = -1;
	
	
	switch(model)
	{
		case 0:	hp = 3;	color = 0x0b;	break;
		case 1:	hp = 1; color = 0x07;	break;
		case 2: hp = 1; color = 0x0c;	break;
		case 3:	hp = 3; color = 0x0a;	break;
		default:hp = 0; color = 0x07;	break;
	}
	
	if (camp >= 20)
		dire = UP;
	else
		dire = DOWN;
	
	bullet.Reset(x, y, dire, camp, false);
	this->MarkPosition();
}

Tank::Tank(int a, int b, int c, int d)
{
	x = a;
	y = b;
	camp = c;
	model = d % 4;

	
	switch(model)
	{
		case 0:	hp = 3;	color = 0xb;	break;
		case 1:	hp = 1; color = 0x07;	break;
		case 2: hp = 1; color = 0x0c;	break;
		case 3:	hp = 3; color = 0x0a;	break;
		default:hp = 0; color = 0x07;	break;
	}
	
	if (camp >= 20)
		dire = UP;
	else
		dire = DOWN;
	
	bullet.Reset(x, y, dire, camp, false);
	this->MarkPosition();
}

bool Tank::MoveCheck(DIRECTION d)		//检查前方能否前进
{
	switch(d)
	{
		case UP:
			if (pass(Canvas[y-2][x-1]) && pass(Canvas[y-2][x]) && pass(Canvas[y-2][x+1]))
				return true;
			else
				return false;

		case DOWN:
			if (pass(Canvas[y+2][x-1]) && pass(Canvas[y+2][x]) && pass(Canvas[y+2][x+1]))
				return true;
			else
				return false;

		case LEFT:
			if (pass(Canvas[y-1][x-2]) && pass(Canvas[y][x-2]) && pass(Canvas[y+1][x-2]))
				return true;
			else
				return false;

		case RIGHT:
			if (Canvas[y-1][x+2] == 0 && Canvas[y][x+2] == 0 && Canvas[y+1][x+2] == 0)
				return true;
			else
				return false;
		default:
			return false;
	}
}

void Tank::Move(DIRECTION d)			//移动
{
	if (hp > 0)
	{
		this->Clean();
		if (dire == d)
		{
			if(this->MoveCheck(dire))
			{
				switch(dire)
				{
					case UP:	y--;	break;
					case DOWN:	y++;	break;
					case LEFT:	x--;	break;
					case RIGHT:	x++;	break;		
				}	
			}
		}
		else
		{
			dire = DIRECTION(d % 4);
		}
		this->MarkPosition();
	}
}

void Tank::Draw()						//画坦克
{
	if (hp > 0)
	{
		if (model == 3)
		{
			switch(hp)
			{
				case 3:	color = 0x0a; break;
				case 2:	color = 0x0e; break;
				case 1:	color = 0x0c; break;
			}
		}
		setColor(color);
		for(int i = 0; i < 3; i++)
		{
			gotoxy((x-1)*2, y-1+i);
			cout << style[model][i][dire];
		}

		setColor(0x07);
	}
}

void Tank::Clean()						//擦除坦克
{
	for(int i = 0; i < 3; i++)
	{
		gotoxy((x-1)*2, y-1+i);
		cout << "      ";
	}
	this->CleanMark();
}

void Tank::Fire()						//开炮
{
	if (hp > 0)
	{
		if (bullet.GetExist() == false)
		{
			int a = x;
			int b = y;
			switch(dire)
			{
				case UP:	b -= 1;	break;
				case DOWN:	b += 1;	break;
				case LEFT:	a -= 1;	break;
				case RIGHT:	a += 1;	break;
			}
			bullet.Reset(a, b, dire, camp, true);
		}
	}
	
}

void Tank::MarkPosition()				//标记坦克所在位置的值
{
	if (hp > 0)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				Canvas[y+i-1][x+j-1] = camp;
			}
		}
	}
}

void Tank::CleanMark()					//清除标记
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Canvas[y+i-1][x+j-1] = 0;
		}
	}
}

void Tank::TankAI()						//坦克AI
{
	if (hp > 0)
	{
		DIRECTION dir;
		if ((rand() % 20) == 0)			//20份之一不动
		{
			return;
		}
		else
		{
			int a = (MoveCheck(UP)) ?  (y * 3) : y;
			int b = (MoveCheck(DOWN)) ?  (a + 40 * 3) : (a + 40);
			int c = (MoveCheck(LEFT)) ?  (b + x * 3) : (b + x);
			int d = (MoveCheck(RIGHT)) ?  (c + (40 - x) * 3) : (c + 40 - x);
			
			int num = (rand())% d;

			if (num > 0 && num <= a)
				dir = UP;
			else if(num > a && num <= b)
				dir = DOWN;
			else if(num > b && num <= c)
				dir = LEFT;
			else if(num > c && num <= d)
				dir = RIGHT;

			this->Move(dir);
			
			if (MoveCheck(dir))
			{
				if ((rand() % 3) == 0)
				{
					this->Move(dir);		
				}
			}
			/*
			else
			{
				if ((rand() % 3) == 0)
				{
					for (int i = 0; i < 4; i++)
					{
						if (MoveCheck((DIRECTION)i))
						{
							this->Move((DIRECTION)i);
							break;
						}
					}
				}
			}
			*/
		}
		
		if (this->bullet.GetExist() == false)
		{
			if(this->MoveCheck(dire) == false && (rand() % 3) == 0)
				this->Fire();
			else if(x >= 19 && x <= 22 && dire == DOWN)
			{	
				if((rand() % 10) == 0)
					this->Fire();
			}
			else if((y == 37 || x == 38) && (dire == LEFT || dir == RIGHT))
			{
				if ((rand() % 10) == 0)
					this->Fire();
			}
			else if ((rand() % 15) == 0)
				this->Fire();
				
		}

	}
}






