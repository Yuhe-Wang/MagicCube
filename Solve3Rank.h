// Solve3Rank.h: interface for the Solve3Rank class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLVE3RANK_H__D2F70EEE_FB34_44D6_99BA_8A81DC63A515__INCLUDED_)
#define AFX_SOLVE3RANK_H__D2F70EEE_FB34_44D6_99BA_8A81DC63A515__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "Orders.h"
#include "MagicCube.h"
#include "MagicCubeDlg.h"

class Solve3Rank  
{
public:
	short C[6][3][3]; //���浱ǰ״̬������
	Orders orders3R; //����ָ��Ķ���

	Solve3Rank();
	virtual ~Solve3Rank();

	void initial3R(CMagicCubeDlg *dlgPointer);//��ʼ������
	void rotateCube(short axis,short lr,short k);//3�׵���ת 
	void rotate1();
	void rotate2();
	void rotate3();
	void rotate4(); //��4������solve3R�ĸ�������
	void solve3R_1();
	void solve3R_2();
	void solve3R_3();//��3��ħ���ֲ�����
	void solve3R(); //��3��ħ������
	void printInitialState(); //��ӡ��ʼ״̬��������֤
	void printOrders(); //��ӡ��תָ��

};

#endif // !defined(AFX_SOLVE3RANK_H__D2F70EEE_FB34_44D6_99BA_8A81DC63A515__INCLUDED_)
