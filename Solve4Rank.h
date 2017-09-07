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
	
	short C[6][4][4]; //储存当前状态的数组
	Orders orders4R; //储存指令的对象
	
	void initial4R(CMagicCubeDlg *dlgPointer);//初始化操作
	void solve4R();//四阶还原函数
	void rotateCube(short axis,short lr,short k);//3阶的旋转
	void printInitialState(); //打印初始状态，便于验证
	void printOrders(); //打印旋转指令
	void R4toR3(Solve3Rank *p3R); //复制数据到三阶对象
	void translateOrders(Solve3Rank *p3R);//把三阶堆栈中的指令读出，并转换为四阶指令， 压栈
	void finalAdjust(Solve3Rank *p3R);

};

#endif // !defined(AFX_SOLVE4RANK_H__AA471FF2_C76C_452E_8850_5F1966E62322__INCLUDED_)
