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

void Solve5Rank::initial5R(CMagicCubeDlg *dlgPointer)//��ʼ������
{
	short i,j,k;
	for(i=0;i<6;i++)
		for(j=0;j<5;j++)
			for(k=0;k<5;k++)
				C[i][j][k]=dlgPointer->C[i][j][k];
}

void Solve5Rank::rotateCube(short axis,short lr,short k)//5�׵���ת
{
	short i,j;
	short m=(axis+1)%3;
	short n=(m+1)%3;
	short bar[5],F[5][5];
	if(lr==0) //��ʱ����ת
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
	else  //˳ʱ����ת
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
	/**********************������ѹ���ջ��**********************/
	orders5R.push(axis,lr,k);
}

void Solve5Rank::printInitialState()//��ӡ��ʼ״̬��������֤
{
	FILE *fp;
	short i,j,k;
	fp=fopen("������ҵ�5��ħ������.txt","w");  //���ļ�
    if(fp==NULL)
	{	
      //MessageBox("�޷�������������ҵ�5��ħ������.txt�� !");
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
void Solve5Rank::printOrders() //��ӡ��תָ��
{
	FILE *fp;
	fp=fopen("5��ħ����ԭָ��.txt","w");  //���ļ�
    if(fp==NULL)
	{
      //MessageBox("�޷�������5��ħ����ԭָ��.txt�� !");
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

void Solve5Rank::R5toR3()//�������ݵ����׶���
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

void Solve5Rank::translateOrders()//�����׶�ջ�е�ָ���������ת��Ϊ5��ָ� ѹջ
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
	rank3.orders3R.pop(0);//���3�׵Ķ�ջ
}

void Solve5Rank::solve5R()//5��ħ����ԭ����
{
	short i;
	
	for(color=0;color<5;color++)
	{
		//���ĵ��������
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
		
		//����
		if(C[2][2][3]!=color)
		{
			if(C[2][1][2]==color||C[2][2][1]==color||C[2][3][2]==color)
			{
				while(C[2][2][3]!=color) rotateCube(2,0,1);		
			}
			else search1();				
		}
		rotateCube(2,0,1);//ת��һ��
	
		//3,4��
		if(C[2][1][1]!=color||C[2][2][1]!=color)
		{
			if(!(C[2][1][3]==color&&C[2][2][3]==color))
			{
				if(C[2][3][3]!=color) 	search2();
				search1();				
			}
			rotateCube(2,0,1);
		}

		//5,6��		
		if(!(C[2][1][3]==color&&C[2][2][3]==color))
		{
			if(C[2][3][3]!=color) 	search2();
			search1();				
		}
		rotateCube(2,0,1);

		//7�㣬8��
		if(!(C[2][1][3]==color&&C[2][2][3]==color))
		{
			if(C[2][3][3]!=color) search2();
			search1();
		}


		//9��
		if(C[2][3][3]!=color)
		{
			for(i=0;i<4;i++) //��������2
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
			if(i==4) //��������2
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
	
	/********************************ƴ12����*********************************/
	short end=0,even=0,j;
	while(end!=4)//for(j=0;j<30;j++)
	{
		//��һ��δ�Ժõ������C[2][0][1]��λ��
		for(i=0;i<4;i++)
		{
			if(!(C[2][0][1]==C[2][0][2]&&C[3][1][4]==C[3][2][4]&&C[2][0][3]==C[2][0][2]&&C[3][3][4]==C[3][2][4])) break;
			rotateCube(2,0,1);
		}
		if(i==4)//�����ⶼ�ѶԺ�,�������
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
			if(i==4) //���������
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
x0:			if(C[2][0][2]==C[2][0][3]&&C[3][2][4]==C[3][3][4]) //��������Ժã���Ҫת��Ϊ��������
			{
				rotateCube(2,0,1);
				rotateCube(1,1,5);
				rotateCube(2,1,1);
				rotateCube(0,1,5);
			}
			if(C[2][0][1]==C[2][0][2]&&C[3][1][4]==C[3][2][4]) //����������ѶԺ�,���ҵ�3��
			{	
x1:				for(i=0;i<4;i++) //ǰ����Ѱ��
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
					for(i=0;i<4;i++)//���Ҳ�������
					{
						if(C[2][0][1]==C[0][4][1]&&C[3][1][4]==C[1][1][4]) goto x1;
						if(C[2][0][1]==C[1][3][4]&&C[3][1][4]==C[0][4][3]) goto x1;
						rotateCube(1,0,1);
					}
					if(i==4)
					{
						orders5R.pop(4);
						for(i=0;i<4;i++) //�����������
						{
							if(C[2][0][1]==C[0][0][3]&&C[3][1][4]==C[4][3][4]) goto x1;
							if(C[2][0][1]==C[4][1][4]&&C[3][1][4]==C[0][0][1]) goto x1;
							rotateCube(1,0,5);
						}
						if(i==4)
						{
							orders5R.pop(4);
							while(1) //������
							{
								if(C[2][0][1]==C[0][1][0]&&C[3][1][4]==C[5][4][1]) goto x1;
								if(C[2][0][1]==C[5][4][3]&&C[3][1][4]==C[0][3][0]) goto x1;
								rotateCube(2,0,5);
							}
						}
					}
				}
				//�ж�ǰ�����Ƿ�Ժ�
				if(search3()!=4)//������3��δ�Ժõ���
				{
					if(C[0][4][3]==C[0][4][2]&&C[1][3][4]==C[1][2][4])//���������ƻ��ɶԵ�ǰ�����״̬
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
				else//��ʱֻ������������û�Ժã��������2
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
			else //������鶼û�Ժã���ô�ȳ���ƴ�����һ�����
			{
				if(C[2][0][1]==C[3][2][4]&&C[3][1][4]==C[2][0][2]) 
				{
					if(search3()==4)//�������������1��3,���ù�ʽ����
					{
						if(C[2][4][1]==C[2][4][2]&&C[2][4][3]==C[2][4][2]&&C[0][1][4]==C[0][2][4]&&C[0][3][4]==C[0][2][4]) //�������1
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
						else //�������3
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
					else//������������
					{
						if(C[0][4][1]==C[0][4][2]&&C[1][1][4]==C[1][2][4])//���������ƻ��ɶԵ�ǰ�����״̬
						{
							rotateCube(0,1,1);
							rotateCube(2,0,5);
							rotateCube(0,0,1);
							rotateCube(1,1,1);
						}
						//ִ�л������⹫ʽ
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
					//down:���м���ת��ָ��λ��
x2:					for(i=0;i<4;i++)//Ѱ��ǰ����
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
						for(i=0;i<4;i++) //Ѱ���Ҳ���
						{
							if(C[2][0][1]==C[1][2][4]&&C[3][1][4]==C[0][4][2]||C[2][0][1]==C[0][4][2]&&C[3][1][4]==C[1][2][4]) goto x2;
							rotateCube(1,0,1);
						}
						if(i==4)
						{
							orders5R.pop(4);
							for(i=0;i<4;i++) //Ѱ�������
							{
								if(C[2][0][1]==C[4][2][4]&&C[3][1][4]==C[0][0][2]||C[2][0][1]==C[0][0][2]&&C[3][1][4]==C[4][2][4]) goto x2;
								rotateCube(1,0,5);
							}
							if(i==4)
							{
								orders5R.pop(4);
								for(i=0;i<4;i++) //Ѱ��������
								{
									if(C[2][0][1]==C[5][4][2]&&C[3][1][4]==C[0][2][0]||C[2][0][1]==C[0][2][0]&&C[3][1][4]==C[5][4][2]) goto x2;
									rotateCube(2,0,5);
								}
							}
						}
					}
					//up:���м���ת��ָ��λ��
					if(search3()==4) //�������4
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
					else  //ƴ��һ�����
					{
						if(C[0][4][1]==C[0][4][2]&&C[1][1][4]==C[1][2][4])//���������ƻ��ɶԵ�ǰ�����״̬
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
	
	/******************************��3�׻�ԭ*******************************/
	R5toR3();
	rank3.solve3R();
	translateOrders();
	/******************************3�״󹦸��******************************/

}


void Solve5Rank::search1()
{
	short i;
	for(i=0;i<4;i++) //��������1
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
	if(i==4) //��������1
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
	for(i=0;i<4;i++) //��������2
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
	if(i==4) //��������2
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

short Solve5Rank::search3()//����10�����Ƿ�ƴ�ã���û�У���δƴ�õ����Ƶ�ǰ����
{
	short i=0;
	for(i=0;i<4;i++) //��Ѱ������
	{
		if(!(C[0][4][3]==C[0][4][2]&&C[1][3][4]==C[1][2][4]&&C[0][4][1]==C[0][4][2]&&C[1][1][4]==C[1][2][4])) break;
		rotateCube(1,0,1);
	}
	if(i==4)
	{
		orders5R.pop(4);
		for(i=0;i<4;i++)//��Ѱ������
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
			for(i=0;i<4;i++)//��Ѱ������
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