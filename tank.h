#ifndef TANK_H_
#define TANK_H_

#include <windows.h>
#include <iostream>
#include <iomanip>
#include <conio.h>

#define WIDTH 41	//画布宽
#define HIGHT 41	//画布高

enum DIRECTION{UP, DOWN, LEFT, RIGHT};	//方向/前进方向

extern int speed;						//游戏速度
extern int Map[8][41][41];				//地图数组
extern int level;						//当前关卡
extern int Canvas[HIGHT][WIDTH];		//游戏画布数组
extern unsigned int interval[12];				//间隔计数器



class Bullet
{
	private:
		int x;			//子弹横坐标
		int y;			//子弹纵坐标
		DIRECTION dire;	//子弹前进方向
		int vist;		//谁发射的子弹
		bool exist;		//子弹是否存在

	public:
		Bullet() : x(-1), y(-1), dire(UP), vist(20), exist(false) {}
		Bullet(int a, int b, DIRECTION c, int d, bool e);
		~Bullet(){};

		void Reset(int a, int b, DIRECTION c, int d, bool e);
		void Check();
		void Move();
		void Draw();
		void Clean();
		
		inline bool GetExist() {return exist;}

};

class Tank
{
	private:
		static const char * style[4][3][4];
		int x;				//横坐标
		int y;				//纵坐标
		DIRECTION dire;		//方向
		int model;			//模型
		int hp;				//生命值
		int camp;			//阵营
		int color;			//颜色
		Bullet bullet;		//子弹


	public:
		Tank();
		Tank(int a, int b, int c, int d);
		~Tank(){}

		bool MoveCheck(DIRECTION d);	//检查前方能否前进
		void Move(DIRECTION d);			//移动
		void Draw();					//画坦克
		void Clean();					//擦除坦克

		void Fire();					//开炮
		void MarkPosition();			//标记坦克所在位置的值
		void CleanMark();				//清除标记
		void TankAI();					//坦克AI
		
		inline Bullet & GetBullet() {return bullet;}	//返回子弹引用
		inline int GetHP() {return hp;}
		inline void SetHP(int i) {hp += i;}
		inline int GetModel() {return model;}
		inline void SetPosition(int a, int b) {x = a; y = b;}
		inline void SetColor(int c) {color = c;}
	
};


void startMenu();					//开始菜单
void show();						//显示画面
void updateWithoutInput();			//与用户无关的输入
void updateWithInput();				//与用户有关的输入
void gameCheck();					//游戏状态检测


#endif