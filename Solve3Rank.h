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
	short C[6][3][3]; //储存当前状态的数组
	Orders orders3R; //储存指令的对象

	Solve3Rank();
	virtual ~Solve3Rank();

	void initial3R(CMagicCubeDlg *dlgPointer);//初始化操作
	void rotateCube(short axis,short lr,short k);//3阶的旋转 
	void rotate1();
	void rotate2();
	void rotate3();
	void rotate4(); //这4个都是solve3R的辅助函数
	void solve3R_1();
	void solve3R_2();
	void solve3R_3();//解3阶魔方分步命令
	void solve3R(); //解3阶魔方函数
	void printInitialState(); //打印初始状态，便于验证
	void printOrders(); //打印旋转指令

};

#endif // !defined(AFX_SOLVE3RANK_H__D2F70EEE_FB34_44D6_99BA_8A81DC63A515__INCLUDED_)
