// Solve4Rank.cpp: implementation of the Solve4Rank class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MagicCube.h"
#include "Solve3Rank.h"
#include "Solve4Rank.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Solve4Rank::Solve4Rank()
{

}

Solve4Rank::~Solve4Rank()
{

}
void Solve4Rank::initial4R(CMagicCubeDlg *dlgPointer)
{
	short i,j,k;
	for(i=0;i<6;i++)
		for(j=0;j<4;j++)
			for(k=0;k<4;k++)
				C[i][j][k]=dlgPointer->C[i][j][k];
}

void Solve4Rank::rotateCube(short axis,short lr,short k) //lr==0是逆时针，k的范围是1~4，aixs范围为0~2
{
	short i,j;
	short m=(axis+1)%3;
	short n=(m+1)%3;
	short bar[4],F[4][4];
	if(lr==0) //逆时针旋转
	{
		for(i=0;i<4;i++)
			bar[i]=C[n][4-k][i];
		for(i=0;i<4;i++)
			C[n][4-k][i]=C[m][4-1-i][4-k];
		for(i=0;i<4;i++)
			C[m][i][4-k]=C[n+3][4-k][i];
		for(i=0;i<4;i++)
			C[n+3][4-k][4-1-i]=C[m+3][i][4-k];
		for(i=0;i<4;i++)
			C[m+3][i][4-k]=bar[i];
		if(k==1)
		{
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
				F[4-1-j][i]=C[axis][i][j];
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
				C[axis][i][j]=F[i][j];
		}
		if(k==4)
		{
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
				F[4-1-j][i]=C[axis+3][i][j];
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
				C[axis+3][i][j]=F[i][j];
		}
	}
	else  //顺时针旋转
	{
		for(i=0;i<4;i++)
			bar[i]=C[n][4-k][i];
		for(i=0;i<4;i++)
			C[n][4-k][i]=C[m+3][i][4-k];
		for(i=0;i<4;i++)
			C[m+3][i][4-k]=C[n+3][4-k][4-1-i];
		for(i=0;i<4;i++)
			C[n+3][4-k][i]=C[m][i][4-k];
		for(i=0;i<4;i++)
			C[m][4-1-i][4-k]=bar[i];
		if(k==1)
		{
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
				F[i][j]=C[axis][4-1-j][i];
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
				C[axis][i][j]=F[i][j];
		}
		if(k==4)
		{
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
				F[i][j]=C[axis+3][4-1-j][i];
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
				C[axis+3][i][j]=F[i][j];
		}
	}
	/**********************将命令压入堆栈中**********************/
	orders4R.push(axis,lr,k);
}

///////////////////////////////////四阶还原///////////////////////////////////////////////
void Solve4Rank::solve4R()
{
	short color,i;
	/************拼中心块**********************/
	for(color=0;color<5;color++)
	{	
		//调整姿势
		switch(color)
		{
		case 1:
			rotateCube(0,0,2);
			rotateCube(0,0,3);
			break;
		case 2:
			while(C[4][1][2]!=0) 
			{
				rotateCube(2,0,2);
				rotateCube(2,0,3);	
			}
			rotateCube(1,0,2);
			rotateCube(1,0,3);
			break;
		case 3:
			while(C[0][1][2]!=0) 
			{
				rotateCube(2,0,2);
				rotateCube(2,0,3);	
			}
			rotateCube(1,0,2);
			rotateCube(1,0,3);	
			break;
		case 4:
			while(C[0][1][2]!=2) 
			{
				rotateCube(2,0,2);
				rotateCube(2,0,3);	
			}
			rotateCube(0,1,2);
			rotateCube(0,1,3);
		default:
			break;
		}

		//一点
		if(C[2][1][1]!=color)
		{
			if(C[2][1][2]==color||C[2][2][2]==color||C[2][2][1]==color)
			{
				while(C[2][1][1]!=color) rotateCube(2,0,1);
			}
			else
			{
				for(i=0;i<4;i++)
				{
					if(C[3][1][1]==color||C[3][1][2]==color||C[3][2][2]==color||C[3][2][1]==color)
					{
						while(C[3][1][1]!=color) rotateCube(0,0,4);
						rotateCube(1,1,2);
						rotateCube(0,0,4);
						rotateCube(1,0,2);
						rotateCube(2,0,1);
						break;
					}
					rotateCube(2,0,2);
					rotateCube(2,0,3);
				}
				if(i==4)
				{
					orders4R.pop(8);
					while(C[5][1][2]!=color) rotateCube(2,0,4);
					while(C[3][1][2]==C[3][1][1]&&C[3][2][2]==C[3][1][1]&&C[3][2][1]==C[3][1][1]) 
					{
						rotateCube(2,0,2);
						rotateCube(2,0,3);
					}
					rotateCube(1,0,2);
					rotateCube(0,0,4);
					rotateCube(1,1,2);

					rotateCube(0,0,4);

					rotateCube(1,1,2);
					rotateCube(0,0,4);
					rotateCube(1,0,2);
					rotateCube(2,0,1);
				}
			}
		}
		
		//两点
		if(C[2][1][2]!=color)
		{
			for(i=0;i<4;i++)
			{
				if(C[3][1][1]==color||C[3][1][2]==color||C[3][2][2]==color||C[3][2][1]==color)
				{
					while(C[3][1][1]!=color) rotateCube(0,0,4);
					rotateCube(1,1,2);
					rotateCube(0,0,4);
					rotateCube(1,0,2);
					rotateCube(2,0,1);
					break;
				}
				rotateCube(2,0,2);
				rotateCube(2,0,3);
			}
			if(i==4)
			{
				orders4R.pop(8);
				while(C[5][1][2]!=color) rotateCube(2,0,4);
				rotateCube(1,0,2);
				rotateCube(0,0,4);
				rotateCube(1,1,2);

				rotateCube(0,0,4);
				rotateCube(1,1,2);
				rotateCube(0,0,4);
				rotateCube(1,0,2);
				rotateCube(2,0,1);
			}
		}
		else rotateCube(2,0,1);

		//三点
		if(C[2][1][2]!=color)
		{
			for(i=0;i<4;i++)
			{
				if(C[3][1][1]==color||C[3][1][2]==color||C[3][2][2]==color||C[3][2][1]==color)
				{
					while(C[3][1][1]!=color) rotateCube(0,0,4);
					rotateCube(1,1,2);
					rotateCube(0,0,4);
					rotateCube(1,0,2);
					break;
				}
				rotateCube(2,0,2);
				rotateCube(2,0,3);
			}
			if(i==4)
			{
				orders4R.pop(8);
				while(C[5][1][2]!=color) rotateCube(2,0,4);
				rotateCube(1,0,2);
				rotateCube(0,0,4);
				rotateCube(1,1,2);

				rotateCube(0,0,4);
				rotateCube(1,1,2);
				rotateCube(0,0,4);
				rotateCube(1,0,2);
			}
		}

		//四点
		if(C[2][2][2]!=color)
		{
			for(i=0;i<4;i++)
			{
				if(C[3][1][1]==color||C[3][1][2]==color||C[3][2][2]==color||C[3][2][1]==color)
				{
					while(C[3][1][1]!=color) rotateCube(0,0,4);
					rotateCube(1,1,2);
					rotateCube(0,0,4);
					rotateCube(1,0,2);
					break;
				}
				rotateCube(2,0,2);
				rotateCube(2,0,3);
			}
			if(i==4)
			{
				orders4R.pop(8);
				while(C[5][1][2]!=color) rotateCube(2,0,4);
				rotateCube(1,0,2);
				rotateCube(0,0,4);
				rotateCube(1,1,2);

				rotateCube(0,0,4);

				rotateCube(1,1,2);
				rotateCube(0,0,4);
				rotateCube(1,0,2);
			}
		}
	}

	/***********************对好12条棱***************************/	
	short end=0,even=0,j=0;
	while(end!=4)
	{
		//将一块未对好的棱放在C[2][0][1]的位置
		for(i=0;i<4;i++)
		{
			if(C[2][0][1]!=C[2][0][2]||C[3][1][3]!=C[3][2][3]) break;
			rotateCube(2,0,1);
		}
		if(i==4)//顶面棱都已对好,考察侧棱
		{
			orders4R.pop(4);
			rotateCube(0,0,1);
			rotateCube(0,0,2);
			rotateCube(0,0,3);
			rotateCube(0,0,4);

			for(i=0;i<4;i++)
			{
				if(C[2][0][1]!=C[2][0][2]||C[3][1][3]!=C[3][2][3]) break;
				rotateCube(1,0,2);
				rotateCube(1,0,3);
				
			}
			if(i==4) //考察底面棱
			{
				orders4R.pop(8);
				rotateCube(0,0,1);
				rotateCube(0,0,2);
				rotateCube(0,0,3);
				rotateCube(0,0,4);

				for(end=0;end<4;end++)
				{
					if(C[2][0][1]!=C[2][0][2]||C[3][1][3]!=C[3][2][3]) break;
					rotateCube(2,0,1);					
				}
			}	
		}

		if(end!=4)
		{
x6:			for(i=0;i<4;i++)
			{
				if(C[2][0][1]==C[0][2][3]&&C[3][1][3]==C[2][3][2]) break;
				if(C[2][0][1]==C[2][3][1]&&C[0][1][3]==C[3][1][3]) 
				{
					rotateCube(2,1,1);
					rotateCube(1,1,4);
					rotateCube(2,1,1);
					rotateCube(0,1,4);
					break;
				}
				rotateCube(0,0,1);
			}
			if(i==4)
			{
				orders4R.pop(4);
				for(i=0;i<4;i++)
				{
					if(C[2][0][1]==C[0][3][1]&&C[3][1][3]==C[1][1][3]) goto x6;
					if(C[2][0][1]==C[1][2][3]&&C[3][1][3]==C[0][3][2]) goto x6;
					rotateCube(1,0,1);
				}
				if(i==4)
				{
					orders4R.pop(4);
					for(i=0;i<4;i++)
					{
						if(C[2][0][1]==C[0][0][2]&&C[3][1][3]==C[4][2][3]) goto x6;
						if(C[2][0][1]==C[4][1][3]&&C[3][1][3]==C[0][0][1]) goto x6;
						rotateCube(1,0,4);
					}
					if(i==4)
					{
						orders4R.pop(4);
						while(1)
						{
							if(C[2][0][1]==C[0][1][0]&&C[3][1][3]==C[5][3][1]) goto x6;
							if(C[2][0][1]==C[5][3][2]&&C[3][1][3]==C[0][2][0]) goto x6;
							rotateCube(2,0,4);
						}
					}
				}
			}
			//现在仅有对角线棱的情况，尝试调整使前右侧棱不同
			for(i=0;i<4;i++)
			{
				if(C[0][3][2]!=C[0][3][1]||C[1][2][3]!=C[1][1][3]) break;
				rotateCube(1,0,1);
			}
			if(i==4)
			{
				orders4R.pop(4);
				for(i=0;i<4;i++)
				{
					if(C[1][0][2]!=C[1][0][1]||C[5][2][3]!=C[5][1][3]) 
					{
						rotateCube(1,1,1);
						break;
					}
					rotateCube(2,0,4);
				}
				if(i==4)
				{
					orders4R.pop(4);
					for(even=0;even<4;even++)
					{
						if(C[4][0][2]!=C[4][0][1]||C[5][2][0]!=C[5][1][0]) 
						{
							rotateCube(2,0,4);
							rotateCube(2,0,4);
							rotateCube(1,1,1);
							break;
						}
						rotateCube(1,0,4);
					}
				}
			}

			if(even!=4)
			{
				rotateCube(1,1,1);
				rotateCube(1,1,2);
				rotateCube(0,1,4);
				rotateCube(1,1,1);
				rotateCube(0,0,4);
				rotateCube(1,0,1);
				rotateCube(1,0,2);
			}
			else //最后一种特殊情况////////////////////////////////////////////////
			{
				orders4R.pop(4);

				rotateCube(0,1,1);

				rotateCube(1,1,1);
				rotateCube(1,1,2);
				rotateCube(0,1,4);
				rotateCube(1,1,1);
				rotateCube(0,0,4);
				rotateCube(1,0,1);
				rotateCube(1,0,2);
				
				rotateCube(1,1,1);
				rotateCube(1,1,1);

				rotateCube(0,0,1);

				rotateCube(0,0,4);

				rotateCube(2,0,4);
				rotateCube(2,0,4);

				rotateCube(0,1,1);

				rotateCube(1,1,1);
				rotateCube(1,1,2);
				rotateCube(0,1,4);
				rotateCube(1,1,1);
				rotateCube(0,0,4);
				rotateCube(1,0,1);
				rotateCube(1,0,2);

				even=0;
			}

		}
	}
	/***********************按三阶的方法还原两层***************************/
	Solve3Rank rank3;
	R4toR3(&rank3);
	rank3.solve3R_1();//解完3阶第二层就停止
	translateOrders(&rank3);
	//下一步解决第四层
	i=0;
	if(rank3.C[2][1][0]==2) i++;
	if(rank3.C[2][2][1]==2) i++;
	if(rank3.C[2][1][2]==2) i++;
	if(rank3.C[2][0][1]==2) i++;
	if(i==1||i==3)
	{
		while(C[2][3][1]!=2) rotateCube(2,0,1);

		rotateCube(1,1,1);
		rotateCube(1,1,1);
		rotateCube(1,1,2);
		rotateCube(1,1,2);

		rotateCube(0,0,4);
		rotateCube(0,0,4);

		rotateCube(2,1,1);
		rotateCube(2,1,1);

		rotateCube(1,0,3);
		rotateCube(1,0,4);

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

		rotateCube(1,1,3);
		rotateCube(1,1,4);

		rotateCube(0,0,4);
		rotateCube(0,0,4);
		
		rotateCube(1,1,1);
		rotateCube(1,1,1);
		rotateCube(1,1,2);
		rotateCube(1,1,2);
	}
	R4toR3(&rank3);
	rank3.solve3R_2(); //拼完顶面十字并对好顶角
	translateOrders(&rank3);

	i=0;
	if(rank3.C[0][2][2]==rank3.C[0][1][2]) i++;
	if(rank3.C[1][2][2]==rank3.C[1][2][1]) i++;
	if(rank3.C[3][2][2]==rank3.C[3][1][2]) i++;
	if(rank3.C[4][2][2]==rank3.C[4][2][1]) i++;
	if(i==0)
	{
		R4toR3(&rank3);
		rank3.rotate4();
		translateOrders(&rank3);
		i=0;
		if(rank3.C[0][2][2]==rank3.C[0][1][2]) i++;
		if(rank3.C[1][2][2]==rank3.C[1][2][1]) i++;
		if(rank3.C[3][2][2]==rank3.C[3][1][2]) i++;
		if(rank3.C[4][2][2]==rank3.C[4][2][1]) i++;
		if(i==0) 
		{
			R4toR3(&rank3);
			rank3.rotate4();
			translateOrders(&rank3);
			finalAdjust(&rank3);
		}
		if(i==2) finalAdjust(&rank3);
	} 
	else if(i==2) finalAdjust(&rank3);
	R4toR3(&rank3);
	rank3.solve3R_3();
	translateOrders(&rank3);
	/*************************四阶魔方还原大功告成！！***************************/	
} 

void Solve4Rank::finalAdjust(Solve3Rank *p3R)
{
	while(1) 
	{
		if(C[0][0][3]!=C[0][1][3]&&C[1][3][3]!=C[1][3][2]) break;
		if(C[4][3][3]!=C[4][3][2]&&C[1][3][3]!=C[1][3][2])
		{
			R4toR3(p3R);
			p3R->rotate4();
			translateOrders(p3R);
		}
		rotateCube(2,0,1);
		rotateCube(2,0,2);
		rotateCube(2,0,3);
		rotateCube(2,0,4);
	}
	rotateCube(1,1,2);
	rotateCube(1,1,2);

	rotateCube(2,1,1);
	rotateCube(2,1,1);
	
	rotateCube(1,1,2);
	rotateCube(1,1,2);

	rotateCube(2,1,1);
	rotateCube(2,1,1);
	rotateCube(2,1,2);
	rotateCube(2,1,2);

	rotateCube(1,1,2);
	rotateCube(1,1,2);

	rotateCube(2,1,2);
	rotateCube(2,1,2);
}


void Solve4Rank::printInitialState()//打印初始状态，便于验证
{
	FILE *fp;
	short i,j,k;
	fp=fopen("随机打乱的4阶魔方数组.txt","w");  //打开文件
    if(fp==NULL)
	{	
      //MessageBox("无法创建“随机打乱的4阶魔方数组.txt” !");
      exit(0);
	}
	for(i=0;i<6;i++)	
	{
		for(j=0;j<4;j++)
		{
			for(k=0;k<4;k++)
				fprintf(fp,"%d ",C[i][j][k]);
			fprintf(fp,"\n");
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}

void Solve4Rank::printOrders()//打印旋转指令
{
	FILE *fp;
	fp=fopen("4阶魔方还原指令.txt","w");  //打开文件
    if(fp==NULL)
	{
      //MessageBox("无法创建“4阶魔方还原指令.txt” !");
      exit(0);
	}
	Order *myOrder=orders4R.head;
	while(myOrder!=NULL)
	{
		fprintf(fp,"%d %d %d\n",myOrder->axis,myOrder->lr,myOrder->k);
		myOrder=myOrder->forward;
	}
	fclose(fp);
}

void Solve4Rank::R4toR3(Solve3Rank *p3R)  //复制数据到三阶对象
{
	short i;
	for(i=0;i<6;i++)
	{
		p3R->C[i][0][0]=C[i][0][0];
		p3R->C[i][0][1]=C[i][0][1];
		p3R->C[i][1][0]=C[i][1][0];
		p3R->C[i][1][1]=C[i][1][1];

		p3R->C[i][2][0]=C[i][3][0];
		p3R->C[i][0][2]=C[i][0][3];
		p3R->C[i][2][1]=C[i][3][1];
		p3R->C[i][2][2]=C[i][3][3];
		p3R->C[i][1][2]=C[i][1][3];
	}
}

void Solve4Rank::translateOrders(Solve3Rank *p3R)//把三阶堆栈中的指令读出，并转换为四阶指令， 压栈
{
	Order *myOrder=p3R->orders3R.head;
	while(myOrder!=NULL) 
	{
		if(myOrder->k==2) 
		{
			rotateCube(myOrder->axis,myOrder->lr,2);
			rotateCube(myOrder->axis,myOrder->lr,3);
		}
		else if(myOrder->k==3) 
		{
			rotateCube(myOrder->axis,myOrder->lr,4);
		}
		else
		{
			rotateCube(myOrder->axis,myOrder->lr,1);
		}
		myOrder=myOrder->forward;
	}
	p3R->orders3R.pop(0);//清空3阶的堆栈
}