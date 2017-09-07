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
	float colorV[3]; //��ɫ����
	CDC *m_pDC;//��ͼDC
	short *bar; //��ʱ����һ���ߵ���ɫ����
	short **F;//��ʱ����һ�������ɫ
	short ***C;//������������ɫ���ݣ���̬�������С
	bool hasDrawn;//�ж��Ƿ��ػ�
	short timerState;//�ж�ʱ��״̬��0��ʾֹͣ��1��ʾ���ת��ħ����2��ʾ��ԭħ������
	Order *orderPointer;
	short angle;
	void *toBeDelete;

	CMagicCubeDlg(CWnd* pParent = NULL);	// standard constructor
	void Init();
	bool bSetupPixelFormat();
	void DrawScene();
	void rotateCube(short aixs,short lr,short k); //��תħ���ĺ��ĺ�����axis��ʾ�����ᣬlr��ʾ��ת����k��ʾ��ת��һ��
	void getColor(short num); //ӳ��õ���ɫ����
	double random(bool func);  //16807�����������
	void setAnimate(Order *head); 
	void autoShow(short axis,short lr, short k, short angle);//������Ⱦ������ÿһ֡
	void drawCell(short axis, short k);
	void lookAt();//�����ӽ�
	void drawAixses();//����������

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
	float P[3];//������귵�ص�λ��
	bool mouseState;//ָʾ���ļ���״̬
	short r[3];//��¼��ת����

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
