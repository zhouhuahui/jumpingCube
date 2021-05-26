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
	
	Point cubePoints[9]; // 由9个点组成的方块 
	int moveState; // 0:左上 1:右上 2: 左下 3: 右下
	int leftMove; // 剩余移动次数，初始化5次 
	int speed; // 默认初始为100 
	
	// 清除方块 
	void clearCube(){ 
		for(int i=0;i<9;i++){
			cubePoints[i].Clear();
		}
	}
	
	// 画出方块 
	void printCube(){
		for(int i=0;i<9;i++){
			cubePoints[i].Print();
		}
	}
	
	// 调整方块位置, 没有输出（屏幕上方块位置没有变） 
	void adjustCubePos(int dx, int dy){
		for(int i=0;i<9;i++){
			int x = cubePoints[i].GetX();
			int y = cubePoints[i].GetY();
			int x2 = x + dx;
			int y2 = y + dy;
			cubePoints[i].ChangePosition(x2, y2);
		}
	}
	
	// 正常移动：移动一个单位 
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
		
		//减少剩余移动次数
		if(moveState==0 || moveState==1){
			if(leftMove>0){
				leftMove--;
			}else{
				//更改移动状态
				moveState += 2;
			}
		}
	} 
};

// 整体的界面类 
class Interface{
public:
	// 
	vector<Point> barrierPoints; // 总体障碍物数组
	vector<Point> curIntQue; // 当前窗口队列
	int curIntPos; // 初始化为0， 表示窗口的y坐标
	int speed; // 默认初始化为50
	Cube *cube; 
	
	// 是否开始上移窗口 
	bool isBeginUp(){ 
		if(cube->cubePoints[4].GetY() <= 20){
		    return 1;
		}else{
		    return 0;
		}
	}
	
	// 方块是否碰到了障碍物或者碰到了墙壁 
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
	
	// 打印障碍物
	void printBarrier(){
		// 打印
		for(int i=0;i<curIntQue.size();i++){
			curIntQue[i].Print();
		}
		
	}
	
	// 改变当前窗口队列 
	void changeIntQue(){
		int preIntPos = curIntPos; 
		curIntPos++;
		// 第一步：pop
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
	
		
		// 第二部：添加 
		// 从preIntPos + 40 + 1开始找，一直到barrierPoints的终点或者curIntPos + 40
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
	
	// 改变屏幕的障碍物，使得窗口上移
	void upInt(){
		// 第一步：清除所有的curIntQue中的点 
		// 假设，障碍物点的y坐标是y2, 窗口的下边界是y, 则相对的y坐标是40- (y2 - y) - 1
		for(int i=0;i<curIntQue.size();i++){
			curIntQue[i].Clear();
		} 
		
		// 第二步：清除方块
		cube->clearCube();
		
		// 第三步：改变curIntQue
		changeIntQue();
		
		// 第三步：改变方块位置，下移一个位置
		cube->adjustCubePos(0,1);
		
		// 第四步：方块和当前障碍物队列同时print
		cube->printCube();
		printBarrier();
	} 
};


int main() {
	SetWindowSize(26,40);
	
	// 初始化cube
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
	cube.speed = 50;//初始化
	
	// 初始化界面类
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
	jiemian.speed = 50; // 移动15次
	
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
	int flag=0; //判断游戏是否开始
	int x=13,y=35;
	int jishu; 
	while(true){
		if(kbhit()){ // 判断是否有按键按下
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
	    // 判断是否碰到障碍物
	    if(jiemian.isTouchBarrier()){
	    	unsigned sleep(3000);
	    	for(int k=0;k<40*26;k++){
	    	cout<<"  ";
			}
	    	break;
		}
		
	    // 移动一次
		cube.normalMove();
		
		// 窗口上移
		if(jiemian.isBeginUp()){
			jiemian.upInt();
		}
	    
	}
	SetCursorPosition(0,20);
	cout<<"    ";
	cout<<"游戏结束"<<endl;
	cout<<"    ";
	cout<<"您最后的得分为：";
	cout<<jishu<<endl; 
	cout<<"    ";
	cout<<"输入任意键结束"; 
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
