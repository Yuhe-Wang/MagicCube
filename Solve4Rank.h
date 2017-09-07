// Solve4Rank.h: interface for the Solve4Rank class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLVE4RANK_H__AA471FF2_C76C_452E_8850_5F1966E62322__INCLUDED_)
#define AFX_SOLVE4RANK_H__AA471FF2_C76C_452E_8850_5F1966E62322__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "MagicCubeDlg.h"
#include "Solve3Rank.h"
#include "Orders.h"

class Solve4Rank  
{
public:
	Solve4Rank();
	virtual ~Solve4Rank();
	
	short C[6][4][4]; //���浱ǰ״̬������
	Orders orders4R; //����ָ��Ķ���
	
	void initial4R(CMagicCubeDlg *dlgPointer);//��ʼ������
	void solve4R();//�Ľ׻�ԭ����
	void rotateCube(short axis,short lr,short k);//3�׵���ת
	void printInitialState(); //��ӡ��ʼ״̬��������֤
	void printOrders(); //��ӡ��תָ��
	void R4toR3(Solve3Rank *p3R); //�������ݵ����׶���
	void translateOrders(Solve3Rank *p3R);//�����׶�ջ�е�ָ���������ת��Ϊ�Ľ�ָ� ѹջ
	void finalAdjust(Solve3Rank *p3R);

};

#endif // !defined(AFX_SOLVE4RANK_H__AA471FF2_C76C_452E_8850_5F1966E62322__INCLUDED_)
