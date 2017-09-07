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
	short C[6][5][5]; //储存当前状态的数组
	Orders orders5R; //储存指令的对象
	Solve3Rank rank3;//调用3阶函数
	short color;
	
	void initial5R(CMagicCubeDlg *dlgPointer);//初始化操作
	void solve5R();//5阶还原函数
	void rotateCube(short axis,short lr,short k);//5阶的旋转
	void printInitialState(); //打印初始状态，便于验证
	void printOrders(); //打印旋转指令
	void R5toR3(); //复制数据到三阶对象
	void translateOrders();//把三阶堆栈中的指令读出，并转换为5阶指令， 压栈
	void search1();
	void search2();
	short search3();
	

};

#endif // !defined(AFX_SOLVE5RANK1_H__CE40FC1E_E45B_4230_B30E_D9EF62A95C38__INCLUDED_)
