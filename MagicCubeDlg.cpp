// MagicCubeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MagicCube.h"
#include "MagicCubeDlg.h"
#include "Solve3Rank.h"
#include "Solve4Rank.h"
#include "Solve5Rank.h"

//add down 
#include "gl\gl.h"
#include "gl\glu.h"
#include "gl\glut.h"
#include "math.h"
#define PI 3.1415926
#define R 200
#define Na 5//90度转动分Na步完成
#define Lw 300//定义openGL投影空间
//add up

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMagicCubeDlg dialog
int state=0;


CMagicCubeDlg::CMagicCubeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMagicCubeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMagicCubeDlg)
	m_N = 3;
	m_K = 2;
	m_lr = 1;
	m_axis = 0;
	m_phi = 30;
	m_theta = 70;
	m_slider = 50;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMagicCubeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMagicCubeDlg)
	DDX_Control(pDX, IDC_GIF, m_gif);
	DDX_Control(pDX, IDC_CHECK2, m_automation);
	DDX_Control(pDX, IDC_CHECK1, m_DrawAxis);
	DDX_Text(pDX, IDC_N, m_N);
	DDX_Text(pDX, IDC_K, m_K);
	DDX_Text(pDX, IDC_LR, m_lr);
	DDX_Text(pDX, IDC_AXIS, m_axis);
	DDX_Text(pDX, IDC_PHI, m_phi);
	DDX_Text(pDX, IDC_THETA, m_theta);
	DDX_Slider(pDX, IDC_SLIDER1, m_slider);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMagicCubeDlg, CDialog)
	//{{AFX_MSG_MAP(CMagicCubeDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(ID_DRAW, OnDraw)
	ON_BN_CLICKED(IDC_SET, OnSetN)
	ON_BN_CLICKED(IDC_CHECK1, OnDrawAxis)
	ON_BN_CLICKED(IDC_RANDOM_ROTATE, OnRandomRotate)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SOLVE, OnSolve)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_HELP, OnHelp)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMagicCubeDlg message handlers

BOOL CMagicCubeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	bar=NULL;
	F=NULL;
	C=NULL;
	angle=0;
	random(0);
	timerState=0;
	mouseState=0;
	m_automation.SetCheck(1); //初始时动画演示是选中状态
	hasDrawn=FALSE;
	Init(); // 初始化OpenGL

	if (m_gif.Load(MAKEINTRESOURCE(IDR_GIF),_T("Gif")))
	{
		m_gif.m_ObjectArea.cx=180;
		m_gif.m_ObjectArea.cy=240;
		m_gif.Draw();
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMagicCubeDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		
		CDialog::OnPaint();
	}
	if(hasDrawn) DrawScene();
	else OnSetN();
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMagicCubeDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CMagicCubeDlg::PreCreateWindow(CREATESTRUCT& cs) 
{
    cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;	
	return CDialog::PreCreateWindow(cs);
}

void CMagicCubeDlg::OnClose() //关闭时释放工作
{

	HGLRC hrc = ::wglGetCurrentContext();
	::wglMakeCurrent(NULL,  NULL);
	if (hrc) ::wglDeleteContext(hrc);
	CDialog::OnClose();	
}

void CMagicCubeDlg::Init()
{
	PIXELFORMATDESCRIPTOR pfd;
	int         n;
	HGLRC       hrc;

	CWnd* pWnd=GetDlgItem(IDC_PICTURE);//定位绘图区
    m_pDC=pWnd->GetDC();//获得绘图环境

	ASSERT(m_pDC != NULL);

	if (!bSetupPixelFormat())
		return;

	n = ::GetPixelFormat(m_pDC->GetSafeHdc());
	::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);

	hrc = wglCreateContext(m_pDC->GetSafeHdc());
	wglMakeCurrent(m_pDC->GetSafeHdc(), hrc);
	
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);  //开启深度测试

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(-Lw,Lw,-Lw,Lw,-Lw,Lw);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	lookAt();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

}

bool CMagicCubeDlg::bSetupPixelFormat()  //设置像素格式函数
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		  PFD_SUPPORT_OPENGL |          // support OpenGL
		  PFD_DOUBLEBUFFER,             // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		32,                             // 32-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	int pixelformat;

	if ( (pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0 )
	{
		MessageBox("ChoosePixelFormat failed");
		return FALSE;
	}

	if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		MessageBox("SetPixelFormat failed");
		return FALSE;
	}

	return TRUE;
}





void CMagicCubeDlg::OnDraw() 
{
	UpdateData(TRUE);//读取控件里的数据
	rotateCube(m_axis,m_lr,m_K);
	DrawScene();
}



void CMagicCubeDlg::rotateCube(short axis,short lr,short k) //lr==0是逆时针，k的范围是1~m_N，aixs范围为0~2
{
	short i,j;
	short m=(axis+1)%3;
	short n=(m+1)%3;
	if(lr==0) //逆时针旋转
	{
		for(i=0;i<m_N;i++)
			bar[i]=C[n][m_N-k][i];
		for(i=0;i<m_N;i++)
			C[n][m_N-k][i]=C[m][m_N-1-i][m_N-k];
		for(i=0;i<m_N;i++)
			C[m][i][m_N-k]=C[n+3][m_N-k][i];
		for(i=0;i<m_N;i++)
			C[n+3][m_N-k][m_N-1-i]=C[m+3][i][m_N-k];
		for(i=0;i<m_N;i++)
			C[m+3][i][m_N-k]=bar[i];
		if(k==1)
		{
			for(i=0;i<m_N;i++)
				for(j=0;j<m_N;j++)
				F[m_N-1-j][i]=C[axis][i][j];
			for(i=0;i<m_N;i++)
				for(j=0;j<m_N;j++)
				C[axis][i][j]=F[i][j];
		}
		if(k==m_N)
		{
			for(i=0;i<m_N;i++)
				for(j=0;j<m_N;j++)
				F[m_N-1-j][i]=C[axis+3][i][j];
			for(i=0;i<m_N;i++)
				for(j=0;j<m_N;j++)
				C[axis+3][i][j]=F[i][j];
		}
	}
	else  //顺时针旋转
	{
		for(i=0;i<m_N;i++)
			bar[i]=C[n][m_N-k][i];
		for(i=0;i<m_N;i++)
			C[n][m_N-k][i]=C[m+3][i][m_N-k];
		for(i=0;i<m_N;i++)
			C[m+3][i][m_N-k]=C[n+3][m_N-k][m_N-1-i];
		for(i=0;i<m_N;i++)
			C[n+3][m_N-k][i]=C[m][i][m_N-k];
		for(i=0;i<m_N;i++)
			C[m][m_N-1-i][m_N-k]=bar[i];
		if(k==1)
		{
			for(i=0;i<m_N;i++)
				for(j=0;j<m_N;j++)
				F[i][j]=C[axis][m_N-1-j][i];
			for(i=0;i<m_N;i++)
				for(j=0;j<m_N;j++)
				C[axis][i][j]=F[i][j];
		}
		if(k==m_N)
		{
			for(i=0;i<m_N;i++)
				for(j=0;j<m_N;j++)
				F[i][j]=C[axis+3][m_N-1-j][i];
			for(i=0;i<m_N;i++)
				for(j=0;j<m_N;j++)
				C[axis+3][i][j]=F[i][j];
		}
	}
}

void CMagicCubeDlg::OnSetN() 
{
	short i,j,k;
	/***********申请一个m_N维数组*****************/
	if(bar!=NULL) 
	{
		delete[] bar; //释放申请的内存
		bar=NULL;
	}
	if(F!=NULL)
	{
		for(i=0;i<m_N;i++)
			delete[] F[i];
		delete[] F;
		F=NULL;
	}
	if(C!=NULL)
	{
		for(i=0;i<6;i++)
			for(j=0;j<m_N;j++)
			delete[] C[i][j];
		for(i=0;i<6;i++)
			delete[] C[i];
		delete[] C;
		C=NULL;
	}
	UpdateData(TRUE);//读取控件里的数据
	bar=new short[m_N]; //申请一个m_N维数组
	/***********申请一个m_N*m_N数组*****************/
	F=new short*[m_N];
	for(i=0;i<m_N;i++)
		F[i]=new short[m_N];
	/***********申请一个6*m_N*m_N数组*****************/	
	C=new short**[6];
	for(i=0;i<6;i++)
		C[i]=new short*[m_N];
	for(i=0;i<6;i++)
		for(j=0;j<m_N;j++)
			C[i][j]=new short[m_N];
	/***********初始化魔方颜色*****************/
	for(i=0;i<6;i++)
		for(j=0;j<m_N;j++)
			for(k=0;k<m_N;k++)
				C[i][j][k]=i;
    DrawScene();
	hasDrawn=TRUE;
}

void CMagicCubeDlg::lookAt()
{
	glLoadIdentity();
	double ks1=sin(m_theta*PI/180.0),ks2=sin(m_phi*PI/180.0),kc1=cos(m_theta*PI/180.0),kc2=cos(m_phi*PI/180.0);
	gluLookAt(R*ks1*kc2,R*ks1*ks2,R*kc1,0.0f,0.0f,0.0f,-kc1*kc2,-kc1*ks2,ks1); //确定视角
}

void CMagicCubeDlg::DrawScene()
{
	short i,j,k;
    float delta=200.0/m_N;	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);//设置清屏颜色为黑色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色缓冲区和深度缓冲区
	glMatrixMode(GL_MODELVIEW); //切换到模型视图矩阵
	glPushMatrix(); //记载当前矩阵
	for(k=0;k<2;k++)//绘制第0面和第3面
	for(i=0;i<m_N;i++) 
		for(j=0;j<m_N;j++)
		{
			getColor(C[k*3][i][j]);
			glColor3fv(colorV);
			glBegin(GL_QUADS);
				glVertex3f(100-k*200,-100+i*delta,-100+j*delta);
				glVertex3f(100-k*200,-100+(i+1)*delta,-100+j*delta);
				glVertex3f(100-k*200,-100+(i+1)*delta,-100+(j+1)*delta);
				glVertex3f(100-k*200,-100+i*delta,-100+(j+1)*delta);
			glEnd();			
		}
		
	for(k=0;k<2;k++)//绘制第1面和第4面
	for(i=0;i<m_N;i++) 
		for(j=0;j<m_N;j++)
		{
			getColor(C[1+k*3][i][j]);
			glColor3fv(colorV);
			glBegin(GL_QUADS);
				glVertex3f(-100+j*delta,100-k*200,-100+i*delta);
				glVertex3f(-100+(j+1)*delta,100-k*200,-100+i*delta);
				glVertex3f(-100+(j+1)*delta,100-k*200,-100+(i+1)*delta);
				glVertex3f(-100+j*delta,100-k*200,-100+(i+1)*delta);
			glEnd();
		}
		
	for(k=0;k<2;k++)//绘制第2面和第5面
	for(i=0;i<m_N;i++) 
		for(j=0;j<m_N;j++)
		{
			getColor(C[2+k*3][i][j]);
			glColor3fv(colorV);
			glBegin(GL_QUADS);
			glVertex3f(-100+i*delta,-100+j*delta,100-k*200);
			glVertex3f(-100+(i+1)*delta,-100+j*delta,100-k*200);
			glVertex3f(-100+(i+1)*delta,-100+(j+1)*delta,100-k*200);
			glVertex3f(-100+i*delta,-100+(j+1)*delta,100-k*200);
			glEnd();
		}

	if(m_DrawAxis.GetCheck()) drawAixses();
	
	glLineWidth(3.0);
	for(k=0;k<2;k++)//绘制白色的网格
	{
		glColor3f(1.0,1.0,1.0); 
		glBegin(GL_LINES);
		for(i=1;i<m_N;i++)
		{
			//第0，3面网格
			glVertex3f(100-k*200,-100+i*delta,-100);
			glVertex3f(100-k*200,-100+i*delta,100);
			glVertex3f(100-k*200,-100,-100+i*delta);
			glVertex3f(100-k*200,100,-100+i*delta);
			//第1，4面网格
			glVertex3f(-100+i*delta,100-k*200,-100);
			glVertex3f(-100+i*delta,100-k*200,100);
			glVertex3f(-100,100-k*200,-100+i*delta);
			glVertex3f(100,100-k*200,-100+i*delta);
			//第2，5面网格
			glVertex3f(-100+i*delta,-100.0,100-k*200);
			glVertex3f(-100+i*delta,100.0,100-k*200);
			glVertex3f(-100,-100+i*delta,100-k*200);
			glVertex3f(100,-100+i*delta,100-k*200);
		}
		glEnd();
	}
	glLineWidth(1.0);
	glPopMatrix(); //弹出矩阵，即回到单位矩阵
	glFinish(); //强制绘图，无论缓冲区是否满了
	SwapBuffers(wglGetCurrentDC()); //交换视图，显示画面（含双缓存机制）
	
}

void CMagicCubeDlg::drawAixses()//绘制坐标轴
{
	glColor3f(1.0,1.0,1.0);//绘图颜色设为白色
	glBegin(GL_LINES);
		glVertex3f(-200.0f,0.0f,0.0f);
		glVertex3f(200.0f,0.0f,0.0f);
		glVertex3f(0.0f,-200.0f,0.0f);
		glVertex3f(0.0f,200.0f,0.0f);
		glVertex3f(0.0f,0.0f,-200.0f);
		glVertex3f(0.0f,0.0f,200.0f);
	glEnd();
	glBegin(GL_LINE_STRIP); //绘制x轴箭头
		glVertex3f(200-10,0.0f,4.0f);
		glVertex3f(200.0f,0.0f,0.0f);
		glVertex3f(200-10,0.0f,-4.0f);
	glEnd();
	glBegin(GL_LINE_STRIP); //绘制y轴箭头
		glVertex3f(0,200-10,4.0f);
		glVertex3f(0.0f,200.0f,0.0f);
		glVertex3f(0,200-10,-4.0f);
	glEnd();
	glBegin(GL_LINE_STRIP); //绘制z轴箭头
		glVertex3f(0,4.0f,200-10);
		glVertex3f(0,0.0f,200.0f);
		glVertex3f(0,-4.0f,200-10);
	glEnd();
}

BOOL CMagicCubeDlg::PreTranslateMessage(MSG* pMsg) 
{
	CString msg;
	switch(pMsg->wParam)
	{			
		case VK_DOWN://下	
			m_theta-=1;
			if(m_theta<0) m_theta=0;
			UpdateData(FALSE);//将数据写入控件
			lookAt();
			if(timerState!=2) DrawScene();
			return TRUE;//break;
		case VK_UP://上
			m_theta+=1;
			if(m_theta>180) m_theta=180;
			UpdateData(FALSE);//将数据写入控件
			lookAt();
			if(timerState!=2) DrawScene();
			return TRUE;//break;
		case VK_LEFT://左
			m_phi-=1;
			if(m_phi<0) m_phi+=360;
			UpdateData(FALSE);//将数据写入控件
			lookAt();
			if(timerState!=2) DrawScene();
			return TRUE;//break;
		case VK_RIGHT://右
			m_phi+=1;
			if(m_phi>360) m_phi-=360;
			UpdateData(FALSE);//将数据写入控件
			lookAt();
			if(timerState!=2) DrawScene();
			return TRUE;//break;
		case VK_ESCAPE:
			return TRUE;
		case VK_RETURN:
			return TRUE;
		default:
			break;
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

void CMagicCubeDlg::getColor(short num) //返回RGB颜色数组指针
{
	switch(num)
	{
	case 0:
		colorV[0]=1.0;
		colorV[1]=0.0;
		colorV[2]=0.0;
		break;
	case 1:
		colorV[0]=0.0;
		colorV[1]=1.0;
		colorV[2]=0.0;
		break;
	case 2:
		colorV[0]=0.0;
		colorV[1]=0.0;
		colorV[2]=1.0;
		break;
	case 3:
		colorV[0]=1.0;
		colorV[1]=1.0;
		colorV[2]=0.0;
		break;
	case 4:
		colorV[0]=1.0;
		colorV[1]=0.0;
		colorV[2]=1.0;
		break;
	case 5:
		colorV[0]=0.0;
		colorV[1]=1.0;
		colorV[2]=1.0;
		break;
	default:
		break;
	}
}

void CMagicCubeDlg::OnDrawAxis() 
{
	if(timerState!=2) DrawScene();
}

double CMagicCubeDlg::random(bool func)  //16807随机数产生器
{
	static int In;
    if(func==0)  //用系统时间初始化迭代数
	{
		CTime tm=CTime::GetCurrentTime();
        short y,m,d,h,n,s;
		y=tm.GetYear();
		m=tm.GetMonth();
		d=tm.GetDay();
		h=tm.GetHour();
        n=tm.GetMinute();
		s=tm.GetSecond();
		In=y+70*(m+12*(d+31*(h+23*(n+59*s))));
	}
    else
	{
     In=16807*(In%127773)-2836*(In/127773);
     if(In<0) In=In+2147483647;
     return (double)In/2147483647;
	}
}

void CMagicCubeDlg::OnRandomRotate() 
{
	if(timerState==0) 
	 {
		SetTimer(1,50,NULL);
		timerState=1;
		SetDlgItemText(IDC_RANDOM_ROTATE,"停止");
		GetDlgItem(IDC_SOLVE)->EnableWindow(FALSE);
	 }
	 else 
	 {
		KillTimer(1);
		timerState=0;
		SetDlgItemText(IDC_RANDOM_ROTATE,"随机旋转");
		GetDlgItem(IDC_SOLVE)->EnableWindow(TRUE);
	 }	
}

void CMagicCubeDlg::OnTimer(UINT nIDEvent) 
{	
	if(timerState==3)
	{
		angle+=90/Na;
		autoShow(r[0], r[1], r[2], angle);
		if(angle==90)
		{
			KillTimer(1);
			timerState=0;
			angle=0;
			rotateCube(r[0], r[1], r[2]);
			DrawScene();
		}	
	}
	if(timerState==2)
	{		
		if(orderPointer==NULL) 
		{
			KillTimer(1);
			timerState=0;
			SetDlgItemText(IDC_SOLVE,"解魔方");
			GetDlgItem(ID_DRAW)->EnableWindow(TRUE);
			GetDlgItem(IDC_RANDOM_ROTATE)->EnableWindow(TRUE);
			GetDlgItem(IDC_SET)->EnableWindow(TRUE);
			angle=0;
			delete toBeDelete;
			DrawScene();
			m_gif.Draw();
		}
		else 
		{
			angle+=90/Na;
			autoShow(orderPointer->axis,orderPointer->lr, orderPointer->k, angle);
			if(angle==90) 
			{
				angle=0;
				rotateCube(orderPointer->axis,orderPointer->lr,orderPointer->k);
				autoShow(orderPointer->axis,orderPointer->lr, orderPointer->k, angle);
				orderPointer=orderPointer->forward;		
			}
		}
	}
	if(timerState==1)
	{
		short axis=(short)(random(1)*3);
		short lr=(short)(random(1)*2);
		short k=(short)(random(1)*m_N)+1;
		rotateCube(axis,lr,k);
		DrawScene();
	}
	CDialog::OnTimer(nIDEvent);
}

void CMagicCubeDlg::OnSolve() //还原魔方命令
{
	short i,j,k;
	UpdateData(TRUE);//读取控件里的数据
	if(timerState==2)
	{
		KillTimer(1);
		timerState=0;
		SetDlgItemText(IDC_SOLVE,"解魔方");
		GetDlgItem(ID_DRAW)->EnableWindow(TRUE);
		GetDlgItem(IDC_RANDOM_ROTATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_SET)->EnableWindow(TRUE);
		angle=0;
		delete toBeDelete;
		m_gif.Draw();
	}
	else
	{
		if(m_N==3) 
		{
			Solve3Rank *p_rank3=new Solve3Rank();
			toBeDelete=p_rank3;
			p_rank3->initial3R(this);//把初始状态复制到rank3
			p_rank3->printInitialState(); //打印初始状态		
			p_rank3->solve3R();
			p_rank3->printOrders();
			if(!m_automation.GetCheck())
			{
				for(i=0;i<6;i++)	
					for(j=0;j<3;j++)
						for(k=0;k<3;k++)
							C[i][j][k]=p_rank3->C[i][j][k];
			}
			else setAnimate(p_rank3->orders3R.head);
		}
		if(m_N==4) 
		{
			Solve4Rank *p_rank4=new Solve4Rank();
			toBeDelete=p_rank4;
			p_rank4->initial4R(this);
			p_rank4->printInitialState(); //打印初始状态
			p_rank4->solve4R();
			p_rank4->printOrders();
			if(!m_automation.GetCheck())
			{
				for(i=0;i<6;i++)	
					for(j=0;j<4;j++)
						for(k=0;k<4;k++)
							C[i][j][k]=p_rank4->C[i][j][k];
			}
			else setAnimate(p_rank4->orders4R.head);

		}
		if(m_N==5) 
		{
			Solve5Rank *p_rank5=new Solve5Rank();
			toBeDelete=p_rank5;
			p_rank5->initial5R(this);
			p_rank5->printInitialState(); //打印初始状态
			p_rank5->solve5R();
			p_rank5->printOrders();
			if(!m_automation.GetCheck())
			{
				for(i=0;i<6;i++)	
					for(j=0;j<5;j++)
						for(k=0;k<5;k++)
							C[i][j][k]=p_rank5->C[i][j][k];
			}
			else setAnimate(p_rank5->orders5R.head);
		}
		if(m_N>5) MessageBox("抱歉，尚未完成高阶的程序，敬请期待！");
	}
	DrawScene();
}

void CMagicCubeDlg::setAnimate(Order *head)
{
	orderPointer=head;
	SetTimer(1,m_slider,NULL);
	timerState=2;
	SetDlgItemText(IDC_SOLVE,"终止动画");
	GetDlgItem(ID_DRAW)->EnableWindow(FALSE);
	GetDlgItem(IDC_RANDOM_ROTATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_SET)->EnableWindow(FALSE);
	m_gif.Stop();
}

void CMagicCubeDlg::autoShow(short axis,short lr, short k, short angle)
{
	short i;
    float delta=200.0/m_N;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//设置清屏颜色为黑色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色缓冲区和深度缓冲区
	glMatrixMode(GL_MODELVIEW); //切换到模型视图矩阵
	glPushMatrix(); //记载当前矩阵
	if(axis==0)
	{
		glRotatef(angle,1.0-2*lr,0.0,0.0);
	}
	else if(axis==1)
	{
		glRotatef(angle,0.0,1.0-2*lr,0.0);
	}
	else
	{
		glRotatef(angle,0.0,0.0,1.0-2*lr);
	}
	drawCell(axis,k);
	glPopMatrix();
	for(i=1;i<=m_N;i++)
	{
		if(i!=k) drawCell(axis,i);
	}
	if(m_DrawAxis.GetCheck()) drawAixses();
	glFinish(); //强制绘图，无论缓冲区是否满了
	SwapBuffers(wglGetCurrentDC()); //交换视图，显示画面（含双缓存机制）
}

void CMagicCubeDlg::drawCell(short axis, short k)//绘制沿axis轴第k层
{
	short i,j,m;
	float delta=200.0/m_N;
	if(axis==0)
	{
		for(m=0;m<2;m++)
		{
			for(i=0;i<m_N;i++)
			{
				getColor(C[2+m*3][m_N-k][i]);
				glColor3fv(colorV);
				glBegin(GL_QUADS);
				glVertex3f(100-(k-1)*delta,-100+i*delta,100-200*m);
				glVertex3f(100-(k-1)*delta,-100+(i+1)*delta,100-200*m);
				glVertex3f(100-k*delta,-100+(i+1)*delta,100-200*m);
				glVertex3f(100-k*delta,-100+i*delta,100-200*m);
				glEnd();
			}
		}
		for(m=0;m<2;m++)
		{
			for(i=0;i<m_N;i++)
			{
				getColor(C[1+m*3][i][m_N-k]);
				glColor3fv(colorV);
				glBegin(GL_QUADS);
				glVertex3f(100-(k-1)*delta,100-200*m,-100+i*delta);
				glVertex3f(100-(k-1)*delta,100-200*m,-100+(i+1)*delta);
				glVertex3f(100-k*delta,100-200*m,-100+(i+1)*delta);
				glVertex3f(100-k*delta,100-200*m,-100+i*delta);
				glEnd();
			}
		}
		if(k==1) //绘制每一层的前面
		{
			for(i=0;i<m_N;i++) 
			for(j=0;j<m_N;j++)
			{
				getColor(C[0][i][j]);
				glColor3fv(colorV);
				glBegin(GL_QUADS);
					glVertex3f(100,-100+i*delta,-100+j*delta);
					glVertex3f(100,-100+(i+1)*delta,-100+j*delta);
					glVertex3f(100,-100+(i+1)*delta,-100+(j+1)*delta);
					glVertex3f(100,-100+i*delta,-100+(j+1)*delta);
				glEnd();			
			}
		}
		else
		{
			glColor3f(1.0,1.0,1.0);
			glBegin(GL_QUADS);
				glVertex3f(100-(k-1)*delta,-100,-100);
				glVertex3f(100-(k-1)*delta,-100,100);
				glVertex3f(100-(k-1)*delta,100,100);
				glVertex3f(100-(k-1)*delta,100,-100);
			glEnd();
		}

		if(k==m_N)//绘制每一层的后面
		{
			for(i=0;i<m_N;i++) 
			for(j=0;j<m_N;j++)
			{
				getColor(C[3][i][j]);
				glColor3fv(colorV);
				glBegin(GL_QUADS);
					glVertex3f(-100,-100+i*delta,-100+j*delta);
					glVertex3f(-100,-100+(i+1)*delta,-100+j*delta);
					glVertex3f(-100,-100+(i+1)*delta,-100+(j+1)*delta);
					glVertex3f(-100,-100+i*delta,-100+(j+1)*delta);
				glEnd();			
			}
		}
		else
		{			
			glColor3f(1.0,1.0,1.0);
			glBegin(GL_QUADS);
				glVertex3f(100-k*delta,-100,-100);
				glVertex3f(100-k*delta,-100,100);
				glVertex3f(100-k*delta,100,100);
				glVertex3f(100-k*delta,100,-100);
			glEnd();
		}
	}

	if(axis==1)
	{
		for(m=0;m<2;m++)
		{
			for(i=0;i<m_N;i++)
			{
				getColor(C[2+m*3][i][m_N-k]);
				glColor3fv(colorV);
				glBegin(GL_QUADS);
				glVertex3f(-100+i*delta,100-(k-1)*delta,100-200*m);
				glVertex3f(-100+i*delta,100-k*delta,100-200*m);
				glVertex3f(-100+(i+1)*delta,100-k*delta,100-200*m);
				glVertex3f(-100+(i+1)*delta,100-(k-1)*delta,100-200*m);
				glEnd();
			}
		}
		for(m=0;m<2;m++)
		{
			for(i=0;i<m_N;i++)
			{
				getColor(C[m*3][m_N-k][i]);
				glColor3fv(colorV);
				glBegin(GL_QUADS);
				glVertex3f(100-200*m,100-(k-1)*delta,-100+i*delta);
				glVertex3f(100-200*m,100-k*delta,-100+i*delta);
				glVertex3f(100-200*m,100-k*delta,-100+(i+1)*delta);
				glVertex3f(100-200*m,100-(k-1)*delta,-100+(i+1)*delta);
				glEnd();
			}
		}
		if(k==1)//绘制每层右面
		{
			for(i=0;i<m_N;i++) 
			for(j=0;j<m_N;j++)
			{
				getColor(C[1][i][j]);
				glColor3fv(colorV);
				glBegin(GL_QUADS);
					glVertex3f(-100+j*delta,100,-100+i*delta);
					glVertex3f(-100+(j+1)*delta,100,-100+i*delta);
					glVertex3f(-100+(j+1)*delta,100,-100+(i+1)*delta);
					glVertex3f(-100+j*delta,100,-100+(i+1)*delta);
				glEnd();
			}
		}
		else
		{
			glColor3f(1.0,1.0,1.0);
			glBegin(GL_QUADS);
				glVertex3f(-100,100-(k-1)*delta,-100);
				glVertex3f(-100,100-(k-1)*delta,100);
				glVertex3f(100,100-(k-1)*delta,100);
				glVertex3f(100,100-(k-1)*delta,-100);
			glEnd();
		}
		if(k==m_N)//绘制每层左面
		{
			for(i=0;i<m_N;i++) 
				for(j=0;j<m_N;j++)
				{
					getColor(C[4][i][j]);
					glColor3fv(colorV);
					glBegin(GL_QUADS);
						glVertex3f(-100+j*delta,-100,-100+i*delta);
						glVertex3f(-100+(j+1)*delta,-100,-100+i*delta);
						glVertex3f(-100+(j+1)*delta,-100,-100+(i+1)*delta);
						glVertex3f(-100+j*delta,-100,-100+(i+1)*delta);
					glEnd();
				}
		}
		else
		{
			glColor3f(1.0,1.0,1.0);
			glBegin(GL_QUADS);
				glVertex3f(-100,100-k*delta,-100);
				glVertex3f(-100,100-k*delta,100);
				glVertex3f(100,100-k*delta,100);
				glVertex3f(100,100-k*delta,-100);
			glEnd();
		}
	}

	if(axis==2)
	{
		for(m=0;m<2;m++)
		{
			for(i=0;i<m_N;i++)
			{
				getColor(C[m*3][i][m_N-k]);
				glColor3fv(colorV);
				glBegin(GL_QUADS);
				glVertex3f(100-200*m,-100+i*delta,100-(k-1)*delta);
				glVertex3f(100-200*m,-100+(i+1)*delta,100-(k-1)*delta);
				glVertex3f(100-200*m,-100+(i+1)*delta,100-k*delta);
				glVertex3f(100-200*m,-100+i*delta,100-k*delta);
				glEnd();
			}
		}
		for(m=0;m<2;m++)
		{
			for(i=0;i<m_N;i++)
			{
				getColor(C[1+m*3][m_N-k][i]);
				glColor3fv(colorV);
				glBegin(GL_QUADS);
				glVertex3f(-100+i*delta,100-200*m,100-(k-1)*delta);
				glVertex3f(-100+(i+1)*delta,100-200*m,100-(k-1)*delta);
				glVertex3f(-100+(i+1)*delta,100-200*m,100-k*delta);
				glVertex3f(-100+i*delta,100-200*m,100-k*delta);
				glEnd();
			}
		}
		if(k==1)//绘制每层上面
		{
			for(i=0;i<m_N;i++) 
				for(j=0;j<m_N;j++)
				{
					getColor(C[2][i][j]);
					glColor3fv(colorV);
					glBegin(GL_QUADS);
					glVertex3f(-100+i*delta,-100+j*delta,100);
					glVertex3f(-100+(i+1)*delta,-100+j*delta,100);
					glVertex3f(-100+(i+1)*delta,-100+(j+1)*delta,100);
					glVertex3f(-100+i*delta,-100+(j+1)*delta,100);
					glEnd();
				}
		}
		else
		{
			glColor3f(1.0,1.0,1.0);
			glBegin(GL_QUADS);
				glVertex3f(-100,-100,100-(k-1)*delta);
				glVertex3f(-100,100,100-(k-1)*delta);
				glVertex3f(100,100,100-(k-1)*delta);
				glVertex3f(100,-100,100-(k-1)*delta);
			glEnd();
		}
		if(k==m_N) //绘制每层下面
		{
			for(i=0;i<m_N;i++) 
				for(j=0;j<m_N;j++)
				{
					getColor(C[5][i][j]);
					glColor3fv(colorV);
					glBegin(GL_QUADS);
					glVertex3f(-100+i*delta,-100+j*delta,-100);
					glVertex3f(-100+(i+1)*delta,-100+j*delta,-100);
					glVertex3f(-100+(i+1)*delta,-100+(j+1)*delta,-100);
					glVertex3f(-100+i*delta,-100+(j+1)*delta,-100);
					glEnd();
				}
		}
		else
		{
			glColor3f(1.0,1.0,1.0);
			glBegin(GL_QUADS);
				glVertex3f(-100,-100,100-k*delta);
				glVertex3f(-100,100,100-k*delta);
				glVertex3f(100,100,100-k*delta);
				glVertex3f(100,-100,100-k*delta);
			glEnd();
		}
	}

}

void CMagicCubeDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	UpdateData(TRUE);//读取控件里的数据
	if(timerState==2)	SetTimer(1,m_slider,NULL);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMagicCubeDlg::OnHelp() 
{
	FILE *fp;
	fp=fopen("help.txt","r"); 
	if(fp==NULL) MessageBox("丢失help.txt文件！");
	else
	{
		fclose(fp);
		ShellExecute(NULL,"open","help.txt",NULL,NULL,SW_SHOW);
	}
	
}

void CMagicCubeDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
    getPosition(point,P);
	if(P[0]!=-1) mouseState=1;	
	CDialog::OnLButtonDown(nFlags, point);
}

void CMagicCubeDlg::getPosition(CPoint point,float P[3]) 
{
	 CRect rc;
	 GetDlgItem(IDC_PICTURE)->GetWindowRect(&rc); //获得picturebox的矩形区
	 ScreenToClient(&rc); 
	 GLint    viewport[4]; 
	 GLdouble modelview[16]; 
	 GLdouble projection[16]; 
	 float  winX, winY, winZ; 
	 GLdouble posX, posY, posZ; 
	 glGetIntegerv(GL_VIEWPORT, viewport); 
	 glGetDoublev(GL_MODELVIEW_MATRIX, modelview); 
	 glGetDoublev(GL_PROJECTION_MATRIX, projection); 
	 winX =(float)point.x-rc.left; 
	 winY = rc.bottom - (float)point.y;
	 glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ); 
	 gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ); 
	 if(99<=posX&&posX<=101&&-100<posY&&posY<=100&&-100<posZ&&posZ<=100)
	 {
		 P[0]=0;
		 P[1]=posY;
		 P[2]=posZ;
	 }
	 else if(-101<=posX&&posX<=-99&&-100<posY&&posY<=100&&-100<posZ&&posZ<=100)
	 {
		 P[0]=3;
		 P[1]=posY;
		 P[2]=posZ;
	 }
	 else if(99<=posY&&posY<=101&&-100<posX&&posX<=100&&-100<posZ&&posZ<=100)
	 {
		 P[0]=1;
		 P[1]=posZ;
		 P[2]=posX;
	 }
	 else if(-101<=posY&&posY<=-99&&-100<posX&&posX<=100&&-100<posZ&&posZ<=100)
	 {
		 P[0]=4;
		 P[1]=posZ;
		 P[2]=posX;
	 }
	 else if(99<=posZ&&posZ<=101&&-100<posX&&posX<=100&&-100<posY&&posY<=100)
	 {
		 P[0]=2;
		 P[1]=posX;
		 P[2]=posY;
	 }
	 else if(-101<=posZ&&posZ<=-99&&-100<posX&&posX<=100&&-100<posY&&posY<=100)
	 {
		 P[0]=5;
		 P[1]=posX;
		 P[2]=posY;
	 }
	 else P[0]=-1;
}

void CMagicCubeDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	static short times=0;
	if(mouseState)
	{
		if(times==5)
		{
			float Q[3],V[2];
			short N[2];
			getPosition(point,Q);
			if(Q[0]==P[0])
			{
				V[0]=Q[1]-P[1];
				V[1]=Q[2]-P[2];
				N[0]=(short)((100-P[1])*m_N/200.0)+1;
				N[1]=(short)((100-P[2])*m_N/200.0)+1;
				if(P[0]==0)
				{
					if(fabs(V[1])>fabs(V[0]))
					{
						r[0]=1;
						r[2]=N[0];
						if(V[1]>0) r[1]=1;
						else r[1]=0;
					}
					else
					{
						r[0]=2;
						r[2]=N[1];
						if(V[0]>0) r[1]=0;
						else r[1]=1;
					}
				}
				if(P[0]==1)
				{
					if(fabs(V[1])>fabs(V[0]))
					{
						r[0]=2;
						r[2]=N[0];
						if(V[1]>0) r[1]=1;
						else r[1]=0;
					}
					else
					{
						r[0]=0;
						r[2]=N[1];
						if(V[0]>0) r[1]=0;
						else r[1]=1;
					}
				}
				if(P[0]==2)
				{
					if(fabs(V[1])>fabs(V[0]))
					{
						r[0]=0;
						r[2]=N[0];
						if(V[1]>0) r[1]=1;
						else r[1]=0;
					}
					else
					{
						r[0]=1;
						r[2]=N[1];
						if(V[0]>0) r[1]=0;
						else r[1]=1;
					}
				}
				if(P[0]==3)
				{
					if(fabs(V[1])>fabs(V[0]))
					{
						r[0]=1;
						r[2]=N[0];
						if(V[1]>0) r[1]=0;
						else r[1]=1;
					}
					else
					{
						r[0]=2;
						r[2]=N[1];
						if(V[0]>0) r[1]=1;
						else r[1]=0;
					}
				}
				if(P[0]==4)
				{
					if(fabs(V[1])>fabs(V[0]))
					{
						r[0]=2;
						r[2]=N[0];
						if(V[1]>0) r[1]=0;
						else r[1]=1;
					}
					else
					{
						r[0]=0;
						r[2]=N[1];
						if(V[0]>0) r[1]=1;
						else r[1]=0;
					}
				}
				if(P[0]==5)
				{
					if(fabs(V[1])>fabs(V[0]))
					{
						r[0]=0;
						r[2]=N[0];
						if(V[1]>0) r[1]=0;
						else r[1]=1;
					}
					else
					{
						r[0]=1;
						r[2]=N[1];
						if(V[0]>0) r[1]=1;
						else r[1]=0;
					}
				}
			}
			timerState=3;
			SetTimer(1,m_slider,NULL);
			times=0;
			mouseState=0;		
		}
		else times++;
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CMagicCubeDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	mouseState=0;	
	CDialog::OnLButtonUp(nFlags, point);
}
