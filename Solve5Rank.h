// Solve5Rank1.h: interface for the Solve5Rank class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLVE5RANK1_H__CE40FC1E_E45B_4230_B30E_D9EF62A95C38__INCLUDED_)
#define AFX_SOLVE5RANK1_H__CE40FC1E_E45B_4230_B30E_D9EF62A95C38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Orders.h"
#include "Solve3Rank.h"


class Solve5Rank  
{
public:
	Solve5Rank();
	virtual ~Solve5Rank();
	short C[6][5][5]; //���浱ǰ״̬������
	Orders orders5R; //����ָ��Ķ���
	Solve3Rank rank3;//����3�׺���
	short color;
	
	void initial5R(CMagicCubeDlg *dlgPointer);//��ʼ������
	void solve5R();//5�׻�ԭ����
	void rotateCube(short axis,short lr,short k);//5�׵���ת
	void printInitialState(); //��ӡ��ʼ״̬��������֤
	void printOrders(); //��ӡ��תָ��
	void R5toR3(); //�������ݵ����׶���
	void translateOrders();//�����׶�ջ�е�ָ���������ת��Ϊ5��ָ� ѹջ
	void search1();
	void search2();
	short search3();
	

};

#endif // !defined(AFX_SOLVE5RANK1_H__CE40FC1E_E45B_4230_B30E_D9EF62A95C38__INCLUDED_)
