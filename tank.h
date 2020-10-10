#ifndef TANK_H_
#define TANK_H_

#include <windows.h>
#include <iostream>
#include <iomanip>
#include <conio.h>

#define WIDTH 41	//������
#define HIGHT 41	//������

enum DIRECTION{UP, DOWN, LEFT, RIGHT};	//����/ǰ������

extern int speed;						//��Ϸ�ٶ�
extern int Map[8][41][41];				//��ͼ����
extern int level;						//��ǰ�ؿ�
extern int Canvas[HIGHT][WIDTH];		//��Ϸ��������
extern unsigned int interval[12];				//���������



class Bullet
{
	private:
		int x;			//�ӵ�������
		int y;			//�ӵ�������
		DIRECTION dire;	//�ӵ�ǰ������
		int vist;		//˭������ӵ�
		bool exist;		//�ӵ��Ƿ����

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
		int x;				//������
		int y;				//������
		DIRECTION dire;		//����
		int model;			//ģ��
		int hp;				//����ֵ
		int camp;			//��Ӫ
		int color;			//��ɫ
		Bullet bullet;		//�ӵ�


	public:
		Tank();
		Tank(int a, int b, int c, int d);
		~Tank(){}

		bool MoveCheck(DIRECTION d);	//���ǰ���ܷ�ǰ��
		void Move(DIRECTION d);			//�ƶ�
		void Draw();					//��̹��
		void Clean();					//����̹��

		void Fire();					//����
		void MarkPosition();			//���̹������λ�õ�ֵ
		void CleanMark();				//������
		void TankAI();					//̹��AI
		
		inline Bullet & GetBullet() {return bullet;}	//�����ӵ�����
		inline int GetHP() {return hp;}
		inline void SetHP(int i) {hp += i;}
		inline int GetModel() {return model;}
		inline void SetPosition(int a, int b) {x = a; y = b;}
		inline void SetColor(int c) {color = c;}
	
};


void startMenu();					//��ʼ�˵�
void show();						//��ʾ����
void updateWithoutInput();			//���û��޹ص�����
void updateWithInput();				//���û��йص�����
void gameCheck();					//��Ϸ״̬���


#endif