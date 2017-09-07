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

void Solve3Rank::rotateCube(short axis,short lr,short k) //lr==0����ʱ�룬k�ķ�Χ��1~3��aixs��ΧΪ0~2
{
	short i,j;
	short m=(axis+1)%3;
	short n=(m+1)%3;
	short bar[3];
	short F[3][3];
	if(lr==0) //��ʱ����ת
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
			else  //˳ʱ����ת
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
	/**********************������ѹ���ջ��**********************/
	orders3R.push(axis,lr,k);
}

void Solve3Rank::rotate1() //��ʽ5
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

void Solve3Rank::rotate2() //��ʽ6
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

void Solve3Rank::rotate3() //��ʽ7
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

void Solve3Rank::rotate4() //��ʽ8
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

void Solve3Rank::solve3R_1() //��ԭ3��ħ������1
{
	//�Ƚ�ħ��ת����������color=5
	short i=0;
	for(i=0;i<4;i++)
	{
		if(C[5][1][1]==5) break;
		rotateCube(1,0,2);
	}
	while(C[5][1][1]!=5) rotateCube(0,0,2);
	

	//�γɵ���ʮ��
	while(!(C[5][1][0]==5&&C[5][0][1]==5&&C[5][1][2]==5&&C[5][2][1]==5))
	{
		//�ȼ���м��
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
		if(i==4)	//i=4˵���м��û��color5�����������һ��
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

	//����1��2�����
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
	if(i==2)  //���������������
	{
		while(1)
		{
			if(C[4][0][1]==C[4][1][1]&&C[3][1][0]==C[3][1][1]) //�����滥��������
			{
				rotateCube(1,1,1);
				rotateCube(2,1,2);
				rotateCube(0,0,1);
				rotateCube(2,0,2);
				rotateCube(1,1,1);
				break;
			}
			if(C[0][1][0]==C[0][1][1]&&C[3][1][0]==C[3][1][1]) //���滥��������
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
			//û�жԺã���ת�������㣬������������������֮һ
			rotateCube(2,0,2);
			rotateCube(2,0,3);
		}
	}


	//ƴ�õ�һ���ĸ��ǿ� ***********���Բ���ѭ������if...else
	while(!(C[5][0][0]==5&&C[5][0][2]==5&&C[5][2][0]==5&&C[5][2][2]==5&&C[0][2][0]==C[0][1][1]&&C[0][0][0]==C[0][1][1]&&C[3][2][0]==C[3][1][1]&&C[3][0][0]==C[3][1][1]&&C[1][0][2]==C[1][1][1]&&C[1][0][0]==C[1][1][1]&&C[4][0][2]==C[4][1][1]&&C[4][0][0]==C[4][1][1]))
	{
	x1:	for(i=0;i<4;i++)//��������color=5
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
		if(i==4) //i=4˵�����㶥�����û��color=5�����춥��
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
					goto x1; //ת��Ϊx1�����
				}
				rotateCube(2,0,1);
			}
		}
		if(i==4) //i=4˵����һ�㶥������û��color=5������ײ�
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
			goto x1; //ת��Ϊx1�����	
		}
	}

	//ƴ����м��
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
		if(i==4)//i=4˵����Ҫ�������м�㣬Ҫ�����滻������
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

void Solve3Rank::solve3R_2() //��ԭ3��ħ������2
{
	//ƴ����ʮ��
	short i=0;
	if(C[2][1][0]==2) i++;
	if(C[2][2][1]==2) i++;
	if(C[2][1][2]==2) i++;
	if(C[2][0][1]==2) i++;
	if(i==2)
	{
		while(1)
		{
	x3:		if(C[2][1][0]==2&&C[2][1][2]==2) //һ����
			{
				rotateCube(0,1,1);
				rotateCube(1,1,1);
				rotateCube(2,1,1);
				rotateCube(1,0,1);
				rotateCube(2,0,1);
				rotateCube(0,0,1);
				break;
			}
			if(C[2][2][1]==2&&C[2][1][2]==2) //����
			{
				rotateCube(0,1,1);
				rotateCube(1,1,1);
				rotateCube(2,1,1);
				rotateCube(1,0,1);
				rotateCube(2,0,1);
				rotateCube(0,0,1);
				goto x3; //����һ�ֳ�����
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
		goto x3; //����i=2�����
	}

	
	//ʹ�����ĸ��ǿ����붥�����Ŀ���ɫ��ͬ���涼����
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

	//����ǿ��λ(ֻ�����ĸ��ǿ飬�������)
	x4:	i=0;
	if(C[0][0][2]==C[0][2][2]) i++;
	if(C[1][2][2]==C[1][2][0]) i++;
	if(C[3][0][2]==C[3][2][2]) i++;
	if(C[4][2][2]==C[4][2][0]) i++;//iͳ���ж��ٸ����Ѿ���λ
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


void Solve3Rank::solve3R() //��ԭ3��ħ������
{
	solve3R_1();
	solve3R_2();
	solve3R_3();

}
	

void Solve3Rank::solve3R_3() //��ԭ3��ħ������3
{	

	//���һ������������λ
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
		if(C[0][1][2]==C[1][1][1]) //��ʱ��
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
		else rotate4();//˳ʱ��
			
	}

	/***************3�״󹦸�ɣ�*****************/
}


void Solve3Rank::printInitialState()//��ӡ��ʼ״̬��������֤
{
	FILE *fp;
	short i,j,k;
	fp=fopen("������ҵ�3��ħ������.txt","w");  //���ļ�
    if(fp==NULL)
	{	
      //MessageBox("�޷�������������ҵ�3��ħ������.txt�� !");
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

void Solve3Rank::printOrders()//��ӡ��תָ��
{
	FILE *fp;
	fp=fopen("3��ħ����ԭָ��.txt","w");  //���ļ�
    if(fp==NULL)
	{
      //MessageBox("�޷�������3��ħ����ԭָ��.txt�� !");
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