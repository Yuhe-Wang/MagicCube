// Orders.h: interface for the Orders class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORDERS_H__F7546DA1_7298_41DF_8CF4_7A4834CD6AE2__INCLUDED_)
#define AFX_ORDERS_H__F7546DA1_7298_41DF_8CF4_7A4834CD6AE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Orders  
{
public:
	Order *head, *current;

	Orders();
	virtual ~Orders();
	
	void push(short axis,short lr,short k);
	void pop(short n);

};

#endif // !defined(AFX_ORDERS_H__F7546DA1_7298_41DF_8CF4_7A4834CD6AE2__INCLUDED_)
