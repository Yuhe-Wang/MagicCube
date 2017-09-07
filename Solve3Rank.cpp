// Solve3Rank.cpp: implementation of the Solve3Rank class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MagicCube.h"
#include "MagicCubeDlg.h"
#include "Solve3Rank.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Solve3Rank::Solve3Rank()
{

}

Solve3Rank::~Solve3Rank()
{

}

void Solve3Rank::initial3R(CMagicCubeDlg *dlgPointer)
{
	short i,j,k;
	for(i=0;i<6;i++)
		for(j=0;j<3;j++)
			for(k=0;k<3;k++)
				C[i][j][k]=dlgPointer->C[i][j][k];
}

void Solve3Rank::rotateCube(short axis,short lr,short k) //lr==0是逆时针，k的范围是1~3，aixs范围为0~2
{
	short i,j;
	short m=(axis+1)%3;
	short n=(m+1)%3;
	short bar[3];
	short F[3][3];
	if(lr==0) //逆时针旋转
			{
				for(i=0;i<3;i++)
					bar[i]=C[n][3-k][i];
				for(i=0;i<3;i++)
					C[n][3-k][i]=C[m][3-1-i][3-k];
				for(i=0;i<3;i++)
					C[m][i][3-k]=C[n+3][3-k][i];
				for(i=0;i<3;i++)
					C[n+3][3-k][3-1-i]=C[m+3][i][3-k];
				for(i=0;i<3;i++)
					C[m+3][i][3-k]=bar[i];
				if(k==1)
				{
					for(i=0;i<3;i++)
						for(j=0;j<3;j++)
						F[3-1-j][i]=C[axis][i][j];
					for(i=0;i<3;i++)
						for(j=0;j<3;j++)
						C[axis][i][j]=F[i][j];
				}
				if(k==3)
				{
					for(i=0;i<3;i++)
						for(j=0;j<3;j++)
						F[3-1-j][i]=C[axis+3][i][j];
					for(i=0;i<3;i++)
						for(j=0;j<3;j++)
						C[axis+3][i][j]=F[i][j];
				}
			}
			else  //顺时针旋转
			{
				for(i=0;i<3;i++)
					bar[i]=C[n][3-k][i];
				for(i=0;i<3;i++)
					C[n][3-k][i]=C[m+3][i][3-k];
				for(i=0;i<3;i++)
					C[m+3][i][3-k]=C[n+3][3-k][3-1-i];
				for(i=0;i<3;i++)
					C[n+3][3-k][i]=C[m][i][3-k];
				for(i=0;i<3;i++)
					C[m][3-1-i][3-k]=bar[i];
				if(k==1)
				{
					for(i=0;i<3;i++)
						for(j=0;j<3;j++)
						F[i][j]=C[axis][3-1-j][i];
					for(i=0;i<3;i++)
						for(j=0;j<3;j++)
						C[axis][i][j]=F[i][j];
				}
				if(k==3)
				{
					for(i=0;i<3;i++)
						for(j=0;j<3;j++)
						F[i][j]=C[axis+3][3-1-j][i];
					for(i=0;i<3;i++)
						for(j=0;j<3;j++)
						C[axis+3][i][j]=F[i][j];
				}
			}
	/**********************将命令压入堆栈中**********************/
	orders3R.push(axis,lr,k);
}

void Solve3Rank::rotate1() //公式5
{
	rotateCube(1,1,1);
	rotateCube(2,1,1);
	rotateCube(1,0,1);
	rotateCube(2,1,1);

	rotateCube(1,1,1);
	rotateCube(2,0,1);
	rotateCube(2,0,1);
	rotateCube(1,0,1);
}

void Solve3Rank::rotate2() //公式6
{
	rotateCube(1,1,1);
	rotateCube(2,0,1);
	rotateCube(2,0,1);
	rotateCube(1,0,1);

	rotateCube(2,0,1);
	rotateCube(1,1,1);
	rotateCube(2,0,1);
	rotateCube(1,0,1);
}

void Solve3Rank::rotate3() //公式7
{
	rotateCube(1,0,3);
	rotateCube(0,0,1);
	rotateCube(1,0,3);
	rotateCube(0,1,3);
	rotateCube(0,1,3);

	rotateCube(1,1,3);
	rotateCube(0,1,1);
	rotateCube(1,0,3);
	rotateCube(0,1,3);
	rotateCube(0,1,3);

	rotateCube(1,1,3);
	rotateCube(1,1,3);
	rotateCube(2,1,1);
}

void Solve3Rank::rotate4() //公式8
{
	rotateCube(1,1,1);
	rotateCube(1,1,1);
	rotateCube(2,1,1);
	rotateCube(1,1,1);

	rotateCube(2,1,1);
	rotateCube(1,0,1);
	rotateCube(2,0,1);
	rotateCube(1,0,1);

	rotateCube(2,0,1);
	rotateCube(1,0,1);
	rotateCube(2,1,1);
	rotateCube(1,0,1);
}

void Solve3Rank::solve3R_1() //还原3阶魔方命令1
{
	//先将魔方转到底面中心color=5
	short i=0;
	for(i=0;i<4;i++)
	{
		if(C[5][1][1]==5) break;
		rotateCube(1,0,2);
	}
	while(C[5][1][1]!=5) rotateCube(0,0,2);
	

	//形成底面十字
	while(!(C[5][1][0]==5&&C[5][0][1]==5&&C[5][1][2]==5&&C[5][2][1]==5))
	{
		//先检查中间层
	x0:	while(C[5][1][2]==5) rotateCube(2,0,3);
		for(i=0;i<4;i++)
		{
			if(C[0][2][1]==5) 
			{
				rotateCube(1,0,1);
				break;
			}
			if(C[3][2][1]==5) 
			{
				rotateCube(1,1,1);
				break;
			}
			rotateCube(2,0,2);
		}
		if(i==4)	//i=4说明中间层没有color5，检查最上面一层
		{
			orders3R.pop(4);
			for(i=0;i<4;i++) 
			{
				if(C[2][1][2]==5||C[1][2][1]==5) break;
				rotateCube(2,0,1);
			}
			if(i!=4) {rotateCube(1,0,1);goto x0;}
			else 
			{
				orders3R.pop(4);
				while(C[1][0][1]!=5) rotateCube(2,0,3);
				rotateCube(1,1,1);
				goto x0;
			}
		}
	}

	//对齐1，2层侧面
	while(1)
	{
		i=0;
		if(C[0][1][0]==C[0][1][1]) i++;
		if(C[3][1][0]==C[3][1][1]) i++;
		if(C[1][0][1]==C[1][1][1]) i++;
		if(C[4][0][1]==C[4][1][1]) i++;
		if(i>1) break;
		else rotateCube(2,0,2);
	}
	if(i==2)  //仅有两个面对齐了
	{
		while(1)
		{
			if(C[4][0][1]==C[4][1][1]&&C[3][1][0]==C[3][1][1]) //相邻面互换的情形
			{
				rotateCube(1,1,1);
				rotateCube(2,1,2);
				rotateCube(0,0,1);
				rotateCube(2,0,2);
				rotateCube(1,1,1);
				break;
			}
			if(C[0][1][0]==C[0][1][1]&&C[3][1][0]==C[3][1][1]) //对面互换的情形
			{
				rotateCube(1,1,1);
				rotateCube(2,1,2);
				rotateCube(2,1,2);
				rotateCube(1,1,3);
				rotateCube(2,0,2);
				rotateCube(2,0,2);
				rotateCube(1,1,1);
				break;
			}
			//没有对好，旋转下面两层，调整到以上两种情形之一
			rotateCube(2,0,2);
			rotateCube(2,0,3);
		}
	}


	//拼好第一层四个角块 ***********可以不用循环改用if...else
	while(!(C[5][0][0]==5&&C[5][0][2]==5&&C[5][2][0]==5&&C[5][2][2]==5&&C[0][2][0]==C[0][1][1]&&C[0][0][0]==C[0][1][1]&&C[3][2][0]==C[3][1][1]&&C[3][0][0]==C[3][1][1]&&C[1][0][2]==C[1][1][1]&&C[1][0][0]==C[1][1][1]&&C[4][0][2]==C[4][1][1]&&C[4][0][0]==C[4][1][1]))
	{
	x1:	for(i=0;i<4;i++)//顶层侧边有color=5
		{
			if(C[1][2][2]==5)
			{
				while(1)
				{
					if(C[0][2][2]==C[0][1][1]&&C[2][2][2]==C[1][1][1]) break;
					rotateCube(2,0,2);
					rotateCube(2,0,3);
				}
				rotateCube(2,0,1);
				rotateCube(0,0,1);
				rotateCube(2,1,1);
				rotateCube(0,1,1);
				break;
			}
			if(C[0][2][2]==5) 
			{
				while(1)
				{
					if(C[1][2][2]==C[1][1][1]&&C[2][2][2]==C[0][1][1]) break;
					rotateCube(2,0,2);
					rotateCube(2,0,3);
				}	
				rotateCube(0,0,1);
				rotateCube(2,0,1);
				rotateCube(0,1,1);
				break;
			}
			rotateCube(2,0,1); 
		}
		if(i==4) //i=4说明顶层顶块侧面没有color=5，考察顶面
		{
			orders3R.pop(4);
			for(i=0;i<4;i++)
			{
				if(C[2][2][2]==5)
				{
					while(1)
					{
						if(C[0][2][2]==C[1][1][1]&&C[1][2][2]==C[0][1][1]) break;
						rotateCube(2,0,2);
						rotateCube(2,0,3);
					}
					rotateCube(0,0,1);
					rotateCube(2,0,1);
					rotateCube(2,0,1);
					rotateCube(0,1,1);
					goto x1; //转化为x1的情况
				}
				rotateCube(2,0,1);
			}
		}
		if(i==4) //i=4说明第一层顶块上面没有color=5，考察底层
		{
			orders3R.pop(4);
			while(C[0][2][0]==C[0][1][1]&&C[1][0][2]==C[1][1][1]&&C[5][2][2]==C[5][1][1]) {rotateCube(2,0,3);rotateCube(2,0,2);}
			if(C[1][0][2]==5)
			{
				rotateCube(1,1,1);
				rotateCube(2,1,1);
				rotateCube(1,0,1);
			}
			else
			{
				rotateCube(1,1,1);
				rotateCube(2,0,1);
				rotateCube(1,0,1);
			}
			goto x1; //转化为x1的情况	
		}
	}

	//拼完第中间层
	while(!(C[0][2][1]==C[0][1][1]&&C[0][0][1]==C[0][1][1]&&C[3][2][1]==C[3][1][1]&&C[3][0][1]==C[3][1][1]&&C[1][1][2]==C[1][1][1]&&C[1][1][0]==C[1][1][1]&&C[4][1][2]==C[4][1][1]&&C[4][1][0]==C[4][1][1]))
	{
	x2:	for(i=0;i<4;i++)
		{
			if(C[1][2][1]!=2&&C[2][1][2]!=2)
			{
				while(C[1][2][1]!=C[1][1][1])
				{
					rotateCube(2,0,2);
					rotateCube(2,0,3);
				}
				if(C[2][1][2]==C[3][1][1])
				{
					rotateCube(1,1,1);
					rotateCube(2,1,1);
					rotateCube(1,1,1);
					rotateCube(2,1,1);
					rotateCube(1,1,1);
					rotateCube(2,0,1);
					rotateCube(1,0,1);
					rotateCube(2,0,1);
					rotateCube(1,0,1);
				}
				else
				{
					rotateCube(1,0,1);
					rotateCube(2,0,1);
					rotateCube(1,0,1);
					rotateCube(2,0,1);
					rotateCube(1,0,1);
					rotateCube(2,1,1);
					rotateCube(1,1,1);
					rotateCube(2,1,1);
					rotateCube(1,1,1);
				}
				break;
			}
			rotateCube(2,0,1);
		}
		if(i==4)//i=4说明所要的棱在中间层，要把它替换到上面
		{
			orders3R.pop(4);
			while(C[1][1][0]==C[1][1][1]&&C[3][2][1]==C[3][1][1])
			{
				rotateCube(2,0,2);
				rotateCube(2,0,3);
			}
			rotateCube(1,1,1);
			rotateCube(2,1,1);
			rotateCube(1,1,1);
			rotateCube(2,1,1);
			rotateCube(1,1,1);
			rotateCube(2,0,1);
			rotateCube(1,0,1);
			rotateCube(2,0,1);
			rotateCube(1,0,1);
			goto x2;
		}
	}
}

void Solve3Rank::solve3R_2() //还原3阶魔方命令2
{
	//拼顶面十字
	short i=0;
	if(C[2][1][0]==2) i++;
	if(C[2][2][1]==2) i++;
	if(C[2][1][2]==2) i++;
	if(C[2][0][1]==2) i++;
	if(i==2)
	{
		while(1)
		{
	x3:		if(C[2][1][0]==2&&C[2][1][2]==2) //一字形
			{
				rotateCube(0,1,1);
				rotateCube(1,1,1);
				rotateCube(2,1,1);
				rotateCube(1,0,1);
				rotateCube(2,0,1);
				rotateCube(0,0,1);
				break;
			}
			if(C[2][2][1]==2&&C[2][1][2]==2) //相邻
			{
				rotateCube(0,1,1);
				rotateCube(1,1,1);
				rotateCube(2,1,1);
				rotateCube(1,0,1);
				rotateCube(2,0,1);
				rotateCube(0,0,1);
				goto x3; //化成一字长条形
			}
			rotateCube(2,0,1);
		}
	}
	if(i==0)
	{
		rotateCube(0,1,1);
		rotateCube(1,1,1);
		rotateCube(2,1,1);
		rotateCube(1,0,1);
		rotateCube(2,0,1);
		rotateCube(0,0,1);
		i=2;
		goto x3; //化成i=2的情况
	}

	
	//使顶层四个角块中与顶层中心块颜色形同的面都向上
	i=0;
	if(C[2][2][2]==2) i++;
	if(C[2][0][2]==2) i++;
	if(C[2][0][0]==2) i++;
	if(C[2][2][0]==2) i++;
	if(i==0)
	{
		while(1)
		{
			if(C[1][2][2]==2&&C[1][2][0]==2&&C[4][2][2]==2&&C[4][2][0]==2)
			{
				rotate1();
				rotate1();
				break;
			}
			if(C[4][2][2]==2&&C[4][2][0]==2&&C[0][2][2]==2&&C[3][2][2]==2)
			{
				rotate1();
				rotateCube(2,0,1);
				rotate1();
				break;
			}
			rotateCube(2,0,1);
		}
	}
	if(i==2)
	{
		while(1)
		{
			if(C[0][2][2]==2&&C[0][0][2]==2)
			{
				rotate1();
				rotateCube(2,1,1);
				rotate2();
				break;
			}
			if(C[0][0][2]==2&&C[3][0][2]==2)
			{
				rotate1();
				rotateCube(2,0,1);
				rotate2();
				break;
			}
			if(C[0][0][2]==2&&C[1][2][0]==2)
			{
				rotate1();
				rotateCube(2,1,1);
				rotateCube(2,1,1);
				rotate2();
				break;
			}
			rotateCube(2,0,1);
		}
	}
	if(i==1)
	{
		while(1)
		{
			if(C[0][2][2]==2&&C[1][2][0]==2&&C[3][0][2]==2)
			{
				rotate1();
				break;
			}
			if(C[0][0][2]==2&&C[1][2][2]==2&&C[4][2][0]==2)
			{
				rotate2();
				break;
			}
			rotateCube(2,0,1);
		}
		
	}

	//顶面角块归位(只考虑四个角块，不管棱块)
	x4:	i=0;
	if(C[0][0][2]==C[0][2][2]) i++;
	if(C[1][2][2]==C[1][2][0]) i++;
	if(C[3][0][2]==C[3][2][2]) i++;
	if(C[4][2][2]==C[4][2][0]) i++;//i统计有多少个面已经归位
	if(i==0)
	{
		rotate3();
		goto x4;
	}
	if(i==1||i==2)
	{
		while(C[3][0][2]!=C[3][2][2]) rotateCube(2,0,1);
		while(C[3][1][1]!=C[3][2][2]) 
		{
			rotateCube(2,0,2);
			rotateCube(2,0,3);
		}
		rotate3();
	}
	
}


void Solve3Rank::solve3R() //还原3阶魔方命令
{
	solve3R_1();
	solve3R_2();
	solve3R_3();

}
	

void Solve3Rank::solve3R_3() //还原3阶魔方命令3
{	

	//最后一步：顶面棱块归位
	short i;
	x5:	i=0;
	if(C[0][2][2]==C[0][1][2]) i++;
	if(C[1][2][2]==C[1][2][1]) i++;
	if(C[3][2][2]==C[3][1][2]) i++;
	if(C[4][2][2]==C[4][2][1]) i++;
	if(i==0)
	{
		rotate4();
		goto x5;
	}
	if(i==1)
	{
		while(C[3][1][2]!=C[3][2][2]) rotateCube(2,0,1);
		while(C[3][1][2]!=C[3][1][1])
		{
			rotateCube(2,0,2);
			rotateCube(2,0,3);
		}
		if(C[0][1][2]==C[1][1][1]) //逆时针
		{
			rotateCube(1,1,1);
			rotateCube(2,0,1);
			rotateCube(1,1,1);
			rotateCube(2,1,1);

			rotateCube(1,1,1);
			rotateCube(2,1,1);
			rotateCube(1,1,1);
			rotateCube(2,0,1);

			rotateCube(1,0,1);
			rotateCube(2,0,1);
			rotateCube(1,1,1);
			rotateCube(1,1,1);
		}
		else rotate4();//顺时针
			
	}

	/***************3阶大功告成！*****************/
}


void Solve3Rank::printInitialState()//打印初始状态，便于验证
{
	FILE *fp;
	short i,j,k;
	fp=fopen("随机打乱的3阶魔方数组.txt","w");  //打开文件
    if(fp==NULL)
	{	
      //MessageBox("无法创建“随机打乱的3阶魔方数组.txt” !");
      exit(0);
	}
	for(i=0;i<6;i++)	
	{
		for(j=0;j<3;j++)
		{
			for(k=0;k<3;k++)
				fprintf(fp,"%d ",C[i][j][k]);
			fprintf(fp,"\n");
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}

void Solve3Rank::printOrders()//打印旋转指令
{
	FILE *fp;
	fp=fopen("3阶魔方还原指令.txt","w");  //打开文件
    if(fp==NULL)
	{
      //MessageBox("无法创建“3阶魔方还原指令.txt” !");
      exit(0);
	}
	Order *myOrder=orders3R.head;
	while(myOrder!=NULL)
	{
		fprintf(fp,"%d %d %d\n",myOrder->axis,myOrder->lr,myOrder->k);
		myOrder=myOrder->forward;
	}
	fclose(fp);
}