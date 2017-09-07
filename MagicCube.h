// MagicCube.h : main header file for the MAGICCUBE application
//

#if !defined(AFX_MAGICCUBE_H__94C58E62_E8A3_4A98_BC3E_47173C6FBDA3__INCLUDED_)
#define AFX_MAGICCUBE_H__94C58E62_E8A3_4A98_BC3E_47173C6FBDA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMagicCubeApp:
// See MagicCube.cpp for the implementation of this class
//

class CMagicCubeApp : public CWinApp
{
public:
	CMagicCubeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMagicCubeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMagicCubeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAGICCUBE_H__94C58E62_E8A3_4A98_BC3E_47173C6FBDA3__INCLUDED_)
