#include <iostream>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <vector>
#include"point.h"
#include"tools.h"
#include<stdio.h>
#include<cstdlib>
using namespace std;

int dirx[9] = {-1,-1,-1, 0, 0, 0, 1, 1, 1};
int diry[9] = { 0, 1,-1, 1, 0,-1, 0, 1,-1};

class Cube{
public:
	
	Point cubePoints[9]; // ��9������ɵķ��� 
	int moveState; // 0:���� 1:���� 2: ���� 3: ����
	int leftMove; // ʣ���ƶ���������ʼ��5�� 
	int speed; // Ĭ�ϳ�ʼΪ100 
	
	// ������� 
	void clearCube(){ 
		for(int i=0;i<9;i++){
			cubePoints[i].Clear();
		}
	}
	
	// �������� 
	void printCube(){
		for(int i=0;i<9;i++){
			cubePoints[i].Print();
		}
	}
	
	// ��������λ��, û���������Ļ�Ϸ���λ��û�б䣩 
	void adjustCubePos(int dx, int dy){
		for(int i=0;i<9;i++){
			int x = cubePoints[i].GetX();
			int y = cubePoints[i].GetY();
			int x2 = x + dx;
			int y2 = y + dy;
			cubePoints[i].ChangePosition(x2, y2);
		}
	}
	
	// �����ƶ����ƶ�һ����λ 
	void normalMove(){
		int dx, dy;
		if(moveState == 0){
			dx = -1;
			dy = -1;
		}else if(moveState == 1){
			dx = 1;
			dy = -1;
		}else if(moveState == 2){
			dx = -1;
			dy = 1;
		}else{
			dx = 1;
			dy = 1;
		}
		Sleep(speed);
		clearCube();
		adjustCubePos(dx, dy);
		printCube();
		
		//����ʣ���ƶ�����
		if(moveState==0 || moveState==1){
			if(leftMove>0){
				leftMove--;
			}else{
				//�����ƶ�״̬
				moveState += 2;
			}
		}
	} 
};

// ����Ľ����� 
class Interface{
public:
	// 
	vector<Point> barrierPoints; // �����ϰ�������
	vector<Point> curIntQue; // ��ǰ���ڶ���
	int curIntPos; // ��ʼ��Ϊ0�� ��ʾ���ڵ�y����
	int speed; // Ĭ�ϳ�ʼ��Ϊ50
	Cube *cube; 
	
	// �Ƿ�ʼ���ƴ��� 
	bool isBeginUp(){ 
		if(cube->cubePoints[4].GetY() <= 20){
		    return 1;
		}else{
		    return 0;
		}
	}
	
	// �����Ƿ��������ϰ������������ǽ�� 
	bool isTouchBarrier(){
		int i;
		int curIntQue_size=curIntQue.size();
		for(int k=0;k<9;k++){
			Point p2 = cube->cubePoints[k];
			if(p2.GetX()<0 || p2.GetX()>=25 || p2.GetY()>=39){
				return true;
			}
		}
		for(i=0;i<curIntQue.size();i++){
			for(int k=0; k<9;k++){
				Point p1 = curIntQue[i];  
				Point p2 = cube->cubePoints[k];
                if(p1.GetX()==p2.GetX() && p1.GetY()==p2.GetY()){
                	return true;
				} 
            }
	    }
	    return false;
	}
	
	// ��ӡ�ϰ���
	void printBarrier(){
		// ��ӡ
		for(int i=0;i<curIntQue.size();i++){
			curIntQue[i].Print();
		}
		
	}
	
	// �ı䵱ǰ���ڶ��� 
	void changeIntQue(){
		int preIntPos = curIntPos; 
		curIntPos++;
		// ��һ����pop
		int i = 0;
		for(i=0;i<curIntQue.size();i++){
			if(39-curIntQue[i].GetY()+preIntPos >= curIntPos){
				break;
			}
		}
		
		for(int j=i;j<curIntQue.size();j++){
			int x = curIntQue[j].GetX();
			int y = curIntQue[j].GetY();
			curIntQue[j].ChangePosition(x, y+1);
		}
		
		i--;
		while(i>=0){
			curIntQue.erase(curIntQue.begin());
			i--;
		}
	
		
		// �ڶ�������� 
		// ��preIntPos + 40 + 1��ʼ�ң�һֱ��barrierPoints���յ����curIntPos + 40
		int beginY = preIntPos + 40 + 1;
		int endY = curIntPos + 40;
		for(int i=0;i<barrierPoints.size();i++){
			int y = barrierPoints[i].GetY();
			if(y>=beginY && y<=endY){
				Point p = barrierPoints[i];
				p.ChangePosition(p.GetX(), 40-(p.GetY()-curIntPos)-1);
				curIntQue.push_back(p);
			} 
		}
	}
	
	// �ı���Ļ���ϰ��ʹ�ô�������
	void upInt(){
		// ��һ����������е�curIntQue�еĵ� 
		// ���裬�ϰ�����y������y2, ���ڵ��±߽���y, ����Ե�y������40- (y2 - y) - 1
		for(int i=0;i<curIntQue.size();i++){
			curIntQue[i].Clear();
		} 
		
		// �ڶ������������
		cube->clearCube();
		
		// ���������ı�curIntQue
		changeIntQue();
		
		// ���������ı䷽��λ�ã�����һ��λ��
		cube->adjustCubePos(0,1);
		
		// ���Ĳ�������͵�ǰ�ϰ������ͬʱprint
		cube->printCube();
		printBarrier();
	} 
};


int main() {
	SetWindowSize(26,40);
	
	// ��ʼ��cube
	Cube cube;// [-1,0],[-1,1],[-1,-1],[0,1],[0,0],[0,-1],[1,0],[1,1],[1,-1]
	cube.cubePoints[0]=Point(12,35);
	cube.cubePoints[1]=Point(12,34);
	cube.cubePoints[2]=Point(12,36);
	cube.cubePoints[3]=Point(13,35);
	cube.cubePoints[4]=Point(13,34);
	cube.cubePoints[5]=Point(13,36);
	cube.cubePoints[6]=Point(14,35);
	cube.cubePoints[7]=Point(14,34);
	cube.cubePoints[8]=Point(14,36);
	cube.speed = 50;//��ʼ��
	
	// ��ʼ��������
	Interface jiemian; 
	jiemian.barrierPoints.push_back(Point(0,15)); // 1
	jiemian.barrierPoints.push_back(Point(1,15)); 
	jiemian.barrierPoints.push_back(Point(2,15));
	jiemian.barrierPoints.push_back(Point(3,15)); 
	jiemian.barrierPoints.push_back(Point(4,15)); 
	jiemian.barrierPoints.push_back(Point(5,15));
	
	jiemian.barrierPoints.push_back(Point(26,15)); // 2
	jiemian.barrierPoints.push_back(Point(25,15)); 
	jiemian.barrierPoints.push_back(Point(24,15));
	jiemian.barrierPoints.push_back(Point(23,15));
	jiemian.barrierPoints.push_back(Point(22,15)); 
	jiemian.barrierPoints.push_back(Point(21,15));
	
	jiemian.barrierPoints.push_back(Point(0,30)); // 3
	jiemian.barrierPoints.push_back(Point(1,30)); 
	jiemian.barrierPoints.push_back(Point(2,30));
	jiemian.barrierPoints.push_back(Point(3,30)); 
	jiemian.barrierPoints.push_back(Point(4,30)); 
	jiemian.barrierPoints.push_back(Point(5,30));
	
	jiemian.barrierPoints.push_back(Point(26,30)); //4
	jiemian.barrierPoints.push_back(Point(25,30)); 
	jiemian.barrierPoints.push_back(Point(24,30));
	jiemian.barrierPoints.push_back(Point(23,30)); 
	jiemian.barrierPoints.push_back(Point(22,30)); 
	jiemian.barrierPoints.push_back(Point(21,30));
	
	jiemian.cube = &cube;
	jiemian.curIntPos = 0;
	jiemian.speed = 50; // �ƶ�15��
	
	jiemian.curIntQue.push_back(Point(0,24));
	jiemian.curIntQue.push_back(Point(1,24));
	jiemian.curIntQue.push_back(Point(2,24));
	jiemian.curIntQue.push_back(Point(3,24));
	jiemian.curIntQue.push_back(Point(4,24));
	jiemian.curIntQue.push_back(Point(5,24));
	
	jiemian.curIntQue.push_back(Point(26,24));
	jiemian.curIntQue.push_back(Point(25,24));
	jiemian.curIntQue.push_back(Point(24,24));
	jiemian.curIntQue.push_back(Point(23,24));
	jiemian.curIntQue.push_back(Point(22,24));
	jiemian.curIntQue.push_back(Point(21,24));
	
	jiemian.curIntQue.push_back(Point(0,9));
	jiemian.curIntQue.push_back(Point(1,9));
	jiemian.curIntQue.push_back(Point(2,9));
	jiemian.curIntQue.push_back(Point(3,9));
	jiemian.curIntQue.push_back(Point(4,9));
	jiemian.curIntQue.push_back(Point(5,9));
	
	jiemian.curIntQue.push_back(Point(26,9));
	jiemian.curIntQue.push_back(Point(25,9));
	jiemian.curIntQue.push_back(Point(24,9));
	jiemian.curIntQue.push_back(Point(23,9));
	jiemian.curIntQue.push_back(Point(22,9));
	jiemian.curIntQue.push_back(Point(21,9));

	for(int i=45;i<999;i+=10){
		int o=rand()%(15-4+1)+4;
		for(int k=0;k<o;k++){
			jiemian.barrierPoints.push_back(Point(k,i));
		}
		for(int m=26;m>o+6;m--){
			jiemian.barrierPoints.push_back(Point(m,i));
		}
	}
	cube.printCube();
	jiemian.printBarrier();
	
	
	int ch;
	int flag=0; //�ж���Ϸ�Ƿ�ʼ
	int x=13,y=35;
	int jishu; 
	while(true){
		if(kbhit()){ // �ж��Ƿ��а�������
			ch=getch();
		 	if(ch==75){
				cube.moveState = 0;
	        	cube.leftMove = 10;
	        	flag = 1;
	        	jishu++;
	    	}
	    	if(ch==77){
				cube.moveState = 1;
	        	cube.leftMove = 10;
	        	flag = 1;
	        	jishu++;
	    	}
	    }
        if(flag==0){
	    	continue;
	    }
	    Point point;
	    // �ж��Ƿ������ϰ���
	    if(jiemian.isTouchBarrier()){
	    	unsigned sleep(3000);
	    	for(int k=0;k<40*26;k++){
	    	cout<<"  ";
			}
	    	break;
		}
		
	    // �ƶ�һ��
		cube.normalMove();
		
		// ��������
		if(jiemian.isBeginUp()){
			jiemian.upInt();
		}
	    
	}
	SetCursorPosition(0,20);
	cout<<"    ";
	cout<<"��Ϸ����"<<endl;
	cout<<"    ";
	cout<<"�����ĵ÷�Ϊ��";
	cout<<jishu<<endl; 
	cout<<"    ";
	cout<<"�������������"; 
	ch=0;
	unsigned sleep(3000);
	while(1){
		ch=getch();
		if (ch>0){
			break;
		}
    }
    return 0;
}
