// MagicCubeDlg.h : header file
//

#if !defined(AFX_MAGICCUBEDLG_H__54242F03_2BAA_43A9_A9F5_6CB518903EA1__INCLUDED_)
#define AFX_MAGICCUBEDLG_H__54242F03_2BAA_43A9_A9F5_6CB518903EA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PictureEx.h"
/////////////////////////////////////////////////////////////////////////////
// CMagicCubeDlg dialog

class CMagicCubeDlg : public CDialog
{
// Construction
public:
	float colorV[3]; //颜色向量
	CDC *m_pDC;//绘图DC
	short *bar; //临时储存一条边的颜色数据
	short **F;//临时储存一个面的颜色
	short ***C;//储存立方体颜色数据，动态决定其大小
	bool hasDrawn;//判断是否重绘
	short timerState;//判断时钟状态，0表示停止，1表示随机转动魔方，2表示还原魔方动画
	Order *orderPointer;
	short angle;
	void *toBeDelete;

	CMagicCubeDlg(CWnd* pParent = NULL);	// standard constructor
	void Init();
	bool bSetupPixelFormat();
	void DrawScene();
	void rotateCube(short aixs,short lr,short k); //旋转魔方的核心函数。axis表示坐标轴，lr表示旋转方向，k表示旋转哪一层
	void getColor(short num); //映射得到颜色数组
	double random(bool func);  //16807随机数产生器
	void setAnimate(Order *head); 
	void autoShow(short axis,short lr, short k, short angle);//负责渲染动画的每一帧
	void drawCell(short axis, short k);
	void lookAt();//设置视角
	void drawAixses();//绘制坐标轴

// Dialog Data
	//{{AFX_DATA(CMagicCubeDlg)
	enum { IDD = IDD_MAGICCUBE_DIALOG };
	CPictureEx	m_gif;
	CButton	m_automation;
	CButton	m_DrawAxis;
	int		m_N;
	int		m_K;
	int		m_lr;
	int		m_axis;
	int		m_phi;
	int		m_theta;
	int		m_slider;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMagicCubeDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	float P[3];//储存鼠标返回的位置
	bool mouseState;//指示鼠标的监视状态
	short r[3];//记录旋转命令

	void getPosition(CPoint point,float P[3]); 
	// Generated message map functions
	//{{AFX_MSG(CMagicCubeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnDraw();
	afx_msg void OnSetN();
	afx_msg void OnDrawAxis();
	afx_msg void OnRandomRotate();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSolve();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHelp();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAGICCUBEDLG_H__54242F03_2BAA_43A9_A9F5_6CB518903EA1__INCLUDED_)
