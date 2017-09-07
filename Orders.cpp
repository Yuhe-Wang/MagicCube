// Orders.cpp: implementation of the Orders class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MagicCube.h"
#include "Orders.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Orders::Orders()
{
	head=NULL;
	current=head;
}

Orders::~Orders()
{
	while(current!=head)
	{
		current=current->backward;
		free(current->forward);
	}
	if(head!=NULL) free(head);
	current=head=NULL;
}

void Orders::push(short axis, short lr, short k)
{
	Order * temp;
	if(head==NULL)//说明堆栈为空
	{
		current=head=(Order *)malloc(sizeof(Order));  //创建第一个节点
		head->forward=NULL;
		head->axis=axis;
		head->lr=lr;
		head->k=k;

	}
	else 
	{
		current->forward=(Order *)malloc(sizeof(Order)); 
		temp=current;
		current=current->forward;
		current->backward=temp;
		current->forward=NULL;
		current->axis=axis;
		current->lr=lr;
		current->k=k;
	}
}

void Orders::pop(short n)
{
	short i;
	for(i=0;i<n;i++)
	{
		if(current!=head)
		{
			current=current->backward;
			free(current->forward);
			current->forward=NULL;
		}
		else 
		{
			free(head);
			current=head=NULL;
		}
	}
	if(n==0) //清空堆栈
	{
		while(current!=head)
		{
			current=current->backward;
			free(current->forward);
		}
		if(head!=NULL) free(head);
		current=head=NULL;
	}
}
