// Solve5Rank1.cpp: implementation of the Solve5Rank class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MagicCube.h"
#include "Solve5Rank.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Solve5Rank::Solve5Rank()
{

}

Solve5Rank::~Solve5Rank()
{

}

void Solve5Rank::initial5R(CMagicCubeDlg *dlgPointer)//初始化操作
{
	short i,j,k;
	for(i=0;i<6;i++)
		for(j=0;j<5;j++)
			for(k=0;k<5;k++)
				C[i][j][k]=dlgPointer->C[i][j][k];
}

void Solve5Rank::rotateCube(short axis,short lr,short k)//5阶的旋转
{
	short i,j;
	short m=(axis+1)%3;
	short n=(m+1)%3;
	short bar[5],F[5][5];
	if(lr==0) //逆时针旋转
	{
		for(i=0;i<5;i++)
			bar[i]=C[n][5-k][i];
		for(i=0;i<5;i++)
			C[n][5-k][i]=C[m][5-1-i][5-k];
		for(i=0;i<5;i++)
			C[m][i][5-k]=C[n+3][5-k][i];
		for(i=0;i<5;i++)
			C[n+3][5-k][5-1-i]=C[m+3][i][5-k];
		for(i=0;i<5;i++)
			C[m+3][i][5-k]=bar[i];
		if(k==1)
		{
			for(i=0;i<5;i++)
				for(j=0;j<5;j++)
				F[5-1-j][i]=C[axis][i][j];
			for(i=0;i<5;i++)
				for(j=0;j<5;j++)
				C[axis][i][j]=F[i][j];
		}
		if(k==5)
		{
			for(i=0;i<5;i++)
				for(j=0;j<5;j++)
				F[5-1-j][i]=C[axis+3][i][j];
			for(i=0;i<5;i++)
				for(j=0;j<5;j++)
				C[axis+3][i][j]=F[i][j];
		}
	}
	else  //顺时针旋转
	{
		for(i=0;i<5;i++)
			bar[i]=C[n][5-k][i];
		for(i=0;i<5;i++)
			C[n][5-k][i]=C[m+3][i][5-k];
		for(i=0;i<5;i++)
			C[m+3][i][5-k]=C[n+3][5-k][5-1-i];
		for(i=0;i<5;i++)
			C[n+3][5-k][i]=C[m][i][5-k];
		for(i=0;i<5;i++)
			C[m][5-1-i][5-k]=bar[i];
		if(k==1)
		{
			for(i=0;i<5;i++)
				for(j=0;j<5;j++)
				F[i][j]=C[axis][5-1-j][i];
			for(i=0;i<5;i++)
				for(j=0;j<5;j++)
				C[axis][i][j]=F[i][j];
		}
		if(k==5)
		{
			for(i=0;i<5;i++)
				for(j=0;j<5;j++)
				F[i][j]=C[axis+3][5-1-j][i];
			for(i=0;i<5;i++)
				for(j=0;j<5;j++)
				C[axis+3][i][j]=F[i][j];
		}
	}
	/**********************将命令压入堆栈中**********************/
	orders5R.push(axis,lr,k);
}

void Solve5Rank::printInitialState()//打印初始状态，便于验证
{
	FILE *fp;
	short i,j,k;
	fp=fopen("随机打乱的5阶魔方数组.txt","w");  //打开文件
    if(fp==NULL)
	{	
      //MessageBox("无法创建“随机打乱的5阶魔方数组.txt” !");
      exit(0);
	}
	for(i=0;i<6;i++)	
	{
		for(j=0;j<5;j++)
		{
			for(k=0;k<5;k++)
				fprintf(fp,"%d ",C[i][j][k]);
			fprintf(fp,"\n");
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}
void Solve5Rank::printOrders() //打印旋转指令
{
	FILE *fp;
	fp=fopen("5阶魔方还原指令.txt","w");  //打开文件
    if(fp==NULL)
	{
      //MessageBox("无法创建“5阶魔方还原指令.txt” !");
      exit(0);
	}
	Order *myOrder=orders5R.head;
	while(myOrder!=NULL)
	{
		fprintf(fp,"%d %d %d\n",myOrder->axis,myOrder->lr,myOrder->k);
		myOrder=myOrder->forward;
	}
	fclose(fp);
}

void Solve5Rank::R5toR3()//复制数据到三阶对象
{
	short i;
	for(i=0;i<6;i++)
	{
		rank3.C[i][0][0]=C[i][0][0];
		rank3.C[i][0][1]=C[i][0][1];
		rank3.C[i][1][0]=C[i][1][0];
		rank3.C[i][1][1]=C[i][1][1];

		rank3.C[i][2][0]=C[i][4][0];
		rank3.C[i][0][2]=C[i][0][4];
		rank3.C[i][2][1]=C[i][4][1];
		rank3.C[i][2][2]=C[i][4][4];
		rank3.C[i][1][2]=C[i][1][4];
	}
}

void Solve5Rank::translateOrders()//把三阶堆栈中的指令读出，并转换为5阶指令， 压栈
{
	Order *myOrder=rank3.orders3R.head;
	while(myOrder!=NULL) 
	{
		if(myOrder->k==2) 
		{
			rotateCube(myOrder->axis,myOrder->lr,2);
			rotateCube(myOrder->axis,myOrder->lr,3);
			rotateCube(myOrder->axis,myOrder->lr,4);
		}
		else if(myOrder->k==3) 
		{
			rotateCube(myOrder->axis,myOrder->lr,5);
		}
		else
		{
			rotateCube(myOrder->axis,myOrder->lr,1);
		}
		myOrder=myOrder->forward;
	}
	rank3.orders3R.pop(0);//清空3阶的堆栈
}

void Solve5Rank::solve5R()//5阶魔方还原函数
{
	short i;
	
	for(color=0;color<5;color++)
	{
		//中心点放在上面
		if(C[2][2][2]!=color)
		{
			for(i=0;i<4;i++)
			{
				if(C[2][2][2]==color) break;
				rotateCube(0,0,2);
				rotateCube(0,0,3);
				rotateCube(0,0,4);
			}
			if(i==4)
			{
				orders5R.pop(12);
				while(C[2][2][2]!=color)
				{
					rotateCube(1,0,2);
					rotateCube(1,0,3);
					rotateCube(1,0,4);

				}
			}
		}
		
		//两点
		if(C[2][2][3]!=color)
		{
			if(C[2][1][2]==color||C[2][2][1]==color||C[2][3][2]==color)
			{
				while(C[2][2][3]!=color) rotateCube(2,0,1);		
			}
			else search1();				
		}
		rotateCube(2,0,1);//转到一边
	
		//3,4点
		if(C[2][1][1]!=color||C[2][2][1]!=color)
		{
			if(!(C[2][1][3]==color&&C[2][2][3]==color))
			{
				if(C[2][3][3]!=color) 	search2();
				search1();				
			}
			rotateCube(2,0,1);
		}

		//5,6点		
		if(!(C[2][1][3]==color&&C[2][2][3]==color))
		{
			if(C[2][3][3]!=color) 	search2();
			search1();				
		}
		rotateCube(2,0,1);

		//7点，8点
		if(!(C[2][1][3]==color&&C[2][2][3]==color))
		{
			if(C[2][3][3]!=color) search2();
			search1();
		}


		//9点
		if(C[2][3][3]!=color)
		{
			for(i=0;i<4;i++) //侧面搜索2
			{
				if(C[3][1][3]==color||C[3][3][3]==color||C[3][1][1]==color||C[3][3][1]==color)
				{
					while(C[3][1][3]!=color) rotateCube(0,0,5);	
					break;
				}
				rotateCube(2,0,2);
				rotateCube(2,0,3);
				rotateCube(2,0,4);					
			}
			if(i==4) //底面搜索2
			{
				orders5R.pop(12);
				while(C[5][1][3]!=color) rotateCube(2,0,5);
				while(C[3][2][2]<color)
				{
					rotateCube(2,0,2);
					rotateCube(2,0,3);
					rotateCube(2,0,4);
				}

				rotateCube(1,0,2);
				rotateCube(0,0,5);
				rotateCube(1,1,2);			
			}

			rotateCube(1,1,2);
			rotateCube(0,0,5);
			rotateCube(0,0,5);
			rotateCube(1,0,2);	

			rotateCube(0,0,5);	
			rotateCube(1,1,2);	
			rotateCube(0,0,5);	
			rotateCube(1,0,2);	
		}
	}
	
	/********************************拼12条棱*********************************/
	short end=0,even=0,j;
	while(end!=4)//for(j=0;j<30;j++)
	{
		//将一块未对好的棱放在C[2][0][1]的位置
		for(i=0;i<4;i++)
		{
			if(!(C[2][0][1]==C[2][0][2]&&C[3][1][4]==C[3][2][4]&&C[2][0][3]==C[2][0][2]&&C[3][3][4]==C[3][2][4])) break;
			rotateCube(2,0,1);
		}
		if(i==4)//顶面棱都已对好,考察侧棱
		{
			orders5R.pop(4);
			rotateCube(0,0,1);
			rotateCube(0,0,2);
			rotateCube(0,0,3);
			rotateCube(0,0,4);
			rotateCube(0,0,5);


			for(i=0;i<4;i++)
			{
				if(!(C[2][0][1]==C[2][0][2]&&C[3][1][4]==C[3][2][4]&&C[2][0][3]==C[2][0][2]&&C[3][3][4]==C[3][2][4])) break;
				rotateCube(1,0,2);
				rotateCube(1,0,3);
				rotateCube(1,0,4);
				
			}
			if(i==4) //考察底面棱
			{
				orders5R.pop(12);
				rotateCube(0,0,1);
				rotateCube(0,0,2);
				rotateCube(0,0,3);
				rotateCube(0,0,4);
				rotateCube(0,0,5);

				for(end=0;end<4;end++)
				{
					if(!(C[2][0][1]==C[2][0][2]&&C[3][1][4]==C[3][2][4]&&C[2][0][3]==C[2][0][2]&&C[3][3][4]==C[3][2][4])) break;
					rotateCube(2,0,1);					
				}
			}	
		}

		if(end!=4)
		{
x0:			if(C[2][0][2]==C[2][0][3]&&C[3][2][4]==C[3][3][4]) //已有两块对好，但要转换为下面的情况
			{
				rotateCube(2,0,1);
				rotateCube(1,1,5);
				rotateCube(2,1,1);
				rotateCube(0,1,5);
			}
			if(C[2][0][1]==C[2][0][2]&&C[3][1][4]==C[3][2][4]) //有两个棱块已对好,查找第3块
			{	
x1:				for(i=0;i<4;i++) //前面棱寻找
				{
					if(C[2][0][1]==C[0][3][4]&&C[3][1][4]==C[2][4][3]) break;
					if(C[2][0][1]==C[2][4][1]&&C[3][1][4]==C[0][1][4]) 
					{
						rotateCube(2,0,1);
						rotateCube(1,0,1);
						rotateCube(2,1,1);
						rotateCube(0,0,1);
						break;
					}
					rotateCube(0,0,1);
				}
				if(i==4)
				{
					orders5R.pop(4);
					for(i=0;i<4;i++)//到右侧棱上找
					{
						if(C[2][0][1]==C[0][4][1]&&C[3][1][4]==C[1][1][4]) goto x1;
						if(C[2][0][1]==C[1][3][4]&&C[3][1][4]==C[0][4][3]) goto x1;
						rotateCube(1,0,1);
					}
					if(i==4)
					{
						orders5R.pop(4);
						for(i=0;i<4;i++) //到左侧棱上找
						{
							if(C[2][0][1]==C[0][0][3]&&C[3][1][4]==C[4][3][4]) goto x1;
							if(C[2][0][1]==C[4][1][4]&&C[3][1][4]==C[0][0][1]) goto x1;
							rotateCube(1,0,5);
						}
						if(i==4)
						{
							orders5R.pop(4);
							while(1) //底面找
							{
								if(C[2][0][1]==C[0][1][0]&&C[3][1][4]==C[5][4][1]) goto x1;
								if(C[2][0][1]==C[5][4][3]&&C[3][1][4]==C[0][3][0]) goto x1;
								rotateCube(2,0,5);
							}
						}
					}
				}
				//判断前右棱是否对好
				if(search3()!=4)//至少有3组未对好的棱
				{
					if(C[0][4][3]==C[0][4][2]&&C[1][3][4]==C[1][2][4])//调整到不破坏成对的前右棱的状态
					{
						rotateCube(0,1,1);
						rotateCube(2,0,5);
						rotateCube(0,0,1);
						rotateCube(1,1,1);
					}
					rotateCube(1,1,1);
					rotateCube(1,1,2);
					rotateCube(0,1,5);
					rotateCube(1,1,1);
					rotateCube(0,0,5);
					rotateCube(1,0,1);
					rotateCube(1,0,2);
				}
				else//这时只有上面两条棱没对好，特殊情况2
				{
					rotateCube(1,1,4);
					rotateCube(1,1,5);

					rotateCube(2,1,1);
					rotateCube(2,1,1);

					rotateCube(1,1,4);
					rotateCube(1,1,5);

					rotateCube(2,1,1);
					rotateCube(2,1,1);

					rotateCube(0,1,1);
					rotateCube(0,1,1);

					rotateCube(1,1,4);
					rotateCube(1,1,5);

					rotateCube(0,1,1);
					rotateCube(0,1,1);

					rotateCube(1,1,1);
					rotateCube(1,1,2);

					rotateCube(2,1,1);
					rotateCube(2,1,1);

					rotateCube(1,0,1);
					rotateCube(1,0,2);

					rotateCube(2,1,1);
					rotateCube(2,1,1);

					rotateCube(1,0,4);
					rotateCube(1,0,4);
					rotateCube(1,0,5);
					rotateCube(1,0,5);
				}
			}
			else //三个棱块都没对好，那么先尝试拼好左边一对棱块
			{
				if(C[2][0][1]==C[3][2][4]&&C[3][1][4]==C[2][0][2]) 
				{
					if(search3()==4)//有两种特殊情况1，3,运用公式即可
					{
						if(C[2][4][1]==C[2][4][2]&&C[2][4][3]==C[2][4][2]&&C[0][1][4]==C[0][2][4]&&C[0][3][4]==C[0][2][4]) //特殊情况1
						{
							rotateCube(2,1,1);
							rotateCube(2,1,1);

							rotateCube(1,1,1);
							rotateCube(1,1,1);

							rotateCube(1,1,2);
							rotateCube(1,1,2);

							rotateCube(0,0,5);
							rotateCube(0,0,5);

							rotateCube(2,1,1);
							rotateCube(2,1,1);

							rotateCube(1,0,4);
							rotateCube(1,0,5);

							rotateCube(2,1,1);
							rotateCube(2,1,1);

							rotateCube(1,0,1);
							rotateCube(1,0,2);

							rotateCube(2,1,1);
							rotateCube(2,1,1);

							rotateCube(1,1,1);
							rotateCube(1,1,2);

							rotateCube(2,1,1);
							rotateCube(2,1,1);

							rotateCube(0,1,1);
							rotateCube(0,1,1);

							rotateCube(1,1,1);
							rotateCube(1,1,2);

							rotateCube(0,1,1);
							rotateCube(0,1,1);

							rotateCube(1,1,4);
							rotateCube(1,1,5);

							rotateCube(0,0,5);
							rotateCube(0,0,5);

							rotateCube(1,1,1);
							rotateCube(1,1,1);

							rotateCube(1,1,2);
							rotateCube(1,1,2);

						}
						else //特殊情况3
						{
							rotateCube(2,0,1);
							rotateCube(1,0,1);
							rotateCube(1,0,5);

							rotateCube(2,0,3);
							rotateCube(1,1,1);
							rotateCube(2,1,1);
							rotateCube(1,0,1);
							rotateCube(0,1,1);
							rotateCube(1,0,1);
							rotateCube(0,0,1);
							rotateCube(1,1,1);
							rotateCube(2,1,3);
						}
					}
					else//化成下面的情况
					{
						if(C[0][4][1]==C[0][4][2]&&C[1][1][4]==C[1][2][4])//调整到不破坏成对的前右棱的状态
						{
							rotateCube(0,1,1);
							rotateCube(2,0,5);
							rotateCube(0,0,1);
							rotateCube(1,1,1);
						}
						//执行基本对棱公式
						rotateCube(1,1,1);
						rotateCube(1,1,2);
						rotateCube(1,1,3);

						rotateCube(0,1,5);
						rotateCube(1,1,1);
						rotateCube(0,0,5);

						rotateCube(1,0,1);
						rotateCube(1,0,2);
						rotateCube(1,0,3);

						goto x0;
					}
				}
				else 
				{	
					//down:把中间棱转到指定位置
x2:					for(i=0;i<4;i++)//寻找前面棱
					{
						if(C[2][0][1]==C[2][4][2]&&C[3][1][4]==C[0][2][4])
						{
							rotateCube(2,0,1);
							rotateCube(1,0,1);
							rotateCube(2,1,1);
							rotateCube(0,0,1);
							break;
						}
						if(C[2][0][1]==C[0][2][4]&&C[3][1][4]==C[2][4][2]) break;
						rotateCube(0,0,1);
					}
					if(i==4)
					{
						orders5R.pop(4);
						for(i=0;i<4;i++) //寻找右侧棱
						{
							if(C[2][0][1]==C[1][2][4]&&C[3][1][4]==C[0][4][2]||C[2][0][1]==C[0][4][2]&&C[3][1][4]==C[1][2][4]) goto x2;
							rotateCube(1,0,1);
						}
						if(i==4)
						{
							orders5R.pop(4);
							for(i=0;i<4;i++) //寻找左侧棱
							{
								if(C[2][0][1]==C[4][2][4]&&C[3][1][4]==C[0][0][2]||C[2][0][1]==C[0][0][2]&&C[3][1][4]==C[4][2][4]) goto x2;
								rotateCube(1,0,5);
							}
							if(i==4)
							{
								orders5R.pop(4);
								for(i=0;i<4;i++) //寻找下面棱
								{
									if(C[2][0][1]==C[5][4][2]&&C[3][1][4]==C[0][2][0]||C[2][0][1]==C[0][2][0]&&C[3][1][4]==C[5][4][2]) goto x2;
									rotateCube(2,0,5);
								}
							}
						}
					}
					//up:把中间棱转到指定位置
					if(search3()==4) //特殊情况4
					{
						rotateCube(1,1,1);
						rotateCube(1,1,1);

						rotateCube(1,1,2);
						rotateCube(1,1,2);

						rotateCube(0,0,5);
						rotateCube(0,0,5);

						rotateCube(1,0,1);
						rotateCube(1,0,2);

						rotateCube(2,1,1);
						rotateCube(2,1,1);

						rotateCube(1,0,1);
						rotateCube(1,0,2);

						rotateCube(2,1,1);
						rotateCube(2,1,1);

						rotateCube(0,0,5);
						rotateCube(0,0,5);

						rotateCube(1,0,1);
						rotateCube(1,0,2);

						rotateCube(0,0,5);
						rotateCube(0,0,5);

						rotateCube(1,1,1);
						rotateCube(1,1,2);

						rotateCube(0,0,5);
						rotateCube(0,0,5);

						rotateCube(1,0,1);
						rotateCube(1,0,2);

						rotateCube(0,0,5);
						rotateCube(0,0,5);

						rotateCube(1,1,1);
						rotateCube(1,1,1);

						rotateCube(1,1,2);
						rotateCube(1,1,2);
					}
					else  //拼出一对棱块
					{
						if(C[0][4][1]==C[0][4][2]&&C[1][1][4]==C[1][2][4])//调整到不破坏成对的前右棱的状态
						{
							rotateCube(0,1,1);
							rotateCube(2,0,5);
							rotateCube(0,0,1);
							rotateCube(1,1,1);
						}
						rotateCube(1,1,1);
						rotateCube(1,1,2);
						rotateCube(1,1,3);

						rotateCube(0,1,5);
						rotateCube(1,1,1);
						rotateCube(0,0,5);

						rotateCube(1,0,1);
						rotateCube(1,0,2);
						rotateCube(1,0,3);
					}
				}
			}
		}
	} 
	
	/******************************按3阶还原*******************************/
	R5toR3();
	rank3.solve3R();
	translateOrders();
	/******************************3阶大功告成******************************/

}


void Solve5Rank::search1()
{
	short i;
	for(i=0;i<4;i++) //侧面搜索1
	{
		if(C[3][2][3]==color||C[3][1][2]==color||C[3][2][1]==color||C[3][3][2]==color)
		{
			while(C[3][2][3]!=color) rotateCube(0,0,5);
			rotateCube(1,1,2);
			rotateCube(0,1,5);
			rotateCube(1,0,2);	
			break;
		}
		rotateCube(2,0,2);
		rotateCube(2,0,3);
		rotateCube(2,0,4);					
	}
	if(i==4) //底面搜索1
	{
		orders5R.pop(12);
		for(i=0;i<4;i++)
		{
			if(C[5][2][3]==color) 
			{
				while(C[3][2][2]<color)
				{
					rotateCube(2,0,2);
					rotateCube(2,0,3);
					rotateCube(2,0,4);
				}
				rotateCube(1,0,2);
				rotateCube(0,0,5);
				rotateCube(1,1,2);

				rotateCube(1,1,2);
				rotateCube(0,1,5);
				rotateCube(1,0,2);

				break;
			}
			rotateCube(2,0,5);
		}
		if(i==4)
		{
			orders5R.pop(4);
			while(C[3][2][2]<color)
			{
				rotateCube(2,0,2);
				rotateCube(2,0,3);
				rotateCube(2,0,4);
			}

			rotateCube(1,1,2);
			rotateCube(0,1,5);
			rotateCube(1,0,2);

			rotateCube(0,1,5);
			rotateCube(1,1,2);
			rotateCube(0,0,5);
			rotateCube(1,0,2);

			rotateCube(0,0,5);
			rotateCube(0,0,5);

			rotateCube(1,1,2);
			rotateCube(0,1,5);
			rotateCube(1,0,2);
		}
	}
}

void Solve5Rank::search2()
{
	short i;
	for(i=0;i<4;i++) //侧面搜索2
	{
		if(C[3][1][3]==color||C[3][3][3]==color||C[3][1][1]==color||C[3][3][1]==color)
		{
			while(C[3][1][3]!=color) rotateCube(0,0,5);
			rotateCube(1,1,2);
			rotateCube(0,1,5);
			rotateCube(1,0,2);	
			break;
		}
		rotateCube(2,0,2);
		rotateCube(2,0,3);
		rotateCube(2,0,4);					
	}
	if(i==4) //底面搜索2
	{
		orders5R.pop(12);
		while(C[5][1][3]!=color) rotateCube(2,0,5);
		while(C[3][2][2]<color)
		{
			rotateCube(2,0,2);
			rotateCube(2,0,3);
			rotateCube(2,0,4);
		}

		rotateCube(1,0,2);
		rotateCube(0,0,5);
		rotateCube(1,1,2);

		rotateCube(1,1,2);
		rotateCube(0,1,5);
		rotateCube(1,0,2);				
	}
}

short Solve5Rank::search3()//搜索10条棱是否拼好，若没有，将未拼好的棱移到前右棱
{
	short i=0;
	for(i=0;i<4;i++) //搜寻右面棱
	{
		if(!(C[0][4][3]==C[0][4][2]&&C[1][3][4]==C[1][2][4]&&C[0][4][1]==C[0][4][2]&&C[1][1][4]==C[1][2][4])) break;
		rotateCube(1,0,1);
	}
	if(i==4)
	{
		orders5R.pop(4);
		for(i=0;i<4;i++)//搜寻下面棱
		{
			if(!(C[1][0][3]==C[1][0][2]&&C[1][0][1]==C[1][0][2]&&C[5][3][4]==C[5][2][4]&&C[5][1][4]==C[5][2][4]))
			{
				rotateCube(1,1,1);
				break;
			}
			rotateCube(2,0,5);
		}
		if(i==4)
		{
			orders5R.pop(4);
			for(i=0;i<4;i++)//搜寻左面棱
			{
				if(!(C[4][0][3]==C[4][0][2]&&C[4][0][1]==C[4][0][2]&&C[5][3][0]==C[5][2][0]&&C[5][1][0]==C[5][2][0]))
				{
					rotateCube(2,0,5);
					rotateCube(2,0,5);
					rotateCube(1,1,1);
					break;
				}
				rotateCube(1,0,5);
			}
		}
	}
	return i;
}