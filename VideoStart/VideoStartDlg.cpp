
// VideoStartDlg.cpp: 实现文件
//

#include "pch.h"
#include "afxwin.h"
#include "framework.h"
#include "VideoStart.h"
#include "VideoStartDlg.h"
#include "afxdialogex.h"
#include "yuv2rgb.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BOOL CVideoStartDlg::m_bPause1 = FALSE;
BOOL CVideoStartDlg::m_bEnd1 = FALSE;
BOOL CVideoStartDlg::m_bPlay1 = FALSE;
BOOL CVideoStartDlg::m_bReverse1 = FALSE;
BOOL CVideoStartDlg::m_bLoop1 = FALSE;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVideoStartDlg 对话框



CVideoStartDlg::CVideoStartDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VIDEOSTART_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_fInit = FALSE;

	m_bWithinSlider = FALSE;
	m_pPreview = NULL;
	m_pSettingDlg = NULL;

	// just in case
	m_nWidth = 352;
	m_nHeight = 288;
	m_nFps = 25;
	m_nYuvFormat = FMT_YUV420;
	m_pbYuvData = NULL;
	m_pbBmpData = NULL;
	m_pbBmpData2 = NULL;
	m_bShowPreview = TRUE;
	m_fShowBlack = 1;
	m_nTotalFrame = m_nCurrentFrame = 0;
}

CVideoStartDlg::~CVideoStartDlg()
{
	if (m_pbYuvData != NULL)
	{
		delete[] m_pbYuvData;
		m_pbYuvData = NULL;
	}

	if (m_pbBmpData != NULL)
	{
		delete[] m_pbBmpData;
		m_pbBmpData = NULL;
	}

	if (m_pbBmpData2 != NULL)
	{
		delete[] m_pbBmpData2;
		m_pbBmpData2 = NULL;
	}

	if (m_pSettingDlg != NULL)
	{
		delete m_pSettingDlg;
		m_pSettingDlg = NULL;
	}

	if (m_pPreview != NULL)
	{
		delete m_pPreview;
		m_pPreview = NULL;
	}
}

void CVideoStartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_Slider1);
	DDX_Control(pDX, IDC_START_STOP, m_StartStop);
	DDX_Control(pDX, IDC_OPEN_FILE, m_OpenFile);
	DDX_Control(pDX, IDC_STOP, m_Stop);
	DDX_Control(pDX, IDC_PROS_CONS, m_pros_cons);
	DDX_Control(pDX, IDC_LOOP, m_Loop);
	DDX_Control(pDX, IDC_FIRST, m_First);
	DDX_Control(pDX, IDC_LAST, m_Last);
	DDX_Control(pDX, IDC_F_B, m_F_B);
	DDX_Control(pDX, IDC_F_F, m_F_F);
	DDX_Control(pDX, IDC_PREV, m_Prev);
	DDX_Control(pDX, IDC_NEXT, m_Next);
	DDX_Control(pDX, IDC_SETTING, m_Setting);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
	DDX_Control(pDX, IDC_PREVIEW_SCREEN, m_PreScreen);
}

BEGIN_MESSAGE_MAP(CVideoStartDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_SETTING, &CVideoStartDlg::OnBnClickedSetting)
	ON_STN_CLICKED(IDC_PICTURE, &CVideoStartDlg::OnStnClickedPicture)
	ON_BN_CLICKED(IDC_START_STOP, &CVideoStartDlg::OnBnClickedStartStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_FIRST, &CVideoStartDlg::OnBnClickedFirst)
	ON_BN_CLICKED(IDC_STOP, &CVideoStartDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_PROS_CONS, &CVideoStartDlg::OnBnClickedProsCons)
	ON_BN_CLICKED(IDC_LAST, &CVideoStartDlg::OnBnClickedLast)
	ON_BN_CLICKED(IDC_LOOP, &CVideoStartDlg::OnBnClickedLoop)
	ON_BN_CLICKED(IDC_F_B, &CVideoStartDlg::OnBnClickedFB)
	ON_BN_CLICKED(IDC_F_F, &CVideoStartDlg::OnBnClickedFF)
	ON_BN_CLICKED(IDC_PREV, &CVideoStartDlg::OnBnClickedPrev)
	ON_BN_CLICKED(IDC_NEXT, &CVideoStartDlg::OnBnClickedNext)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_OPEN_FILE, &CVideoStartDlg::OnBnClickedOpenFile)
	ON_WM_LBUTTONDOWN()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_PREVIEW_SCREEN, &CVideoStartDlg::OnBnClickedPreviewScreen)
	ON_MESSAGE(WM_SETTING_WND, &CVideoStartDlg::OnSettingWnd)
END_MESSAGE_MAP()


// CVideoStartDlg 消息处理程序

BOOL CVideoStartDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

		// 初始化 设置  对话框
	if (m_pSettingDlg == NULL)
	{
		m_pSettingDlg = new Setting();
		m_pSettingDlg->Create(IDD_SETTING, this);
		m_pSettingDlg->SetParentWnd(this);
		// 将子窗口的值传递给本窗口
		m_pSettingDlg->SetParametersToParentWnd(m_nWidth, m_nHeight, m_nFps, m_nYuvFormat);
	}

	if (m_pPreview == NULL)
	{
		m_pPreview = new CPreview();
		m_pPreview->Create(IDD_PREVIEW, this);
	}

	m_OpenFile.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_OPEN)));
	m_OpenFile.EnableWindow(TRUE);
	m_StartStop.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PLAY)));
	m_StartStop.EnableWindow(FALSE);	   
	m_Stop.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_STOP)));
	m_Stop.EnableWindow(FALSE);
	m_pros_cons.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BACK)));
	m_pros_cons.EnableWindow(FALSE);
	m_Loop.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_LOOP)));
	m_Loop.EnableWindow(FALSE);
	m_First.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_FIRST)));
	m_First.EnableWindow(FALSE);
	m_Last.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_LAST)));
	m_Last.EnableWindow(FALSE);
	m_F_B.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_F_B)));
	m_F_B.EnableWindow(FALSE);
	m_F_F.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_F_F)));
	m_F_F.EnableWindow(FALSE);
	m_Prev.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PREV)));
	m_Prev.EnableWindow(FALSE);
	m_Next.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_NEXT)));
	m_Next.EnableWindow(FALSE);
	m_PreScreen.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_NOT_PREVIEW)));
	m_PreScreen.EnableWindow(TRUE);
	m_Setting.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SETTING)));
	m_Setting.EnableWindow(TRUE);
	m_Slider1.EnableWindow(FALSE);

	// 控件提示语
	m_Cttc.Create(this);
	m_Cttc.AddTool(GetDlgItem(IDC_OPEN_FILE),	   L"打开文件");
	m_Cttc.AddTool(GetDlgItem(IDC_START_STOP),	   L"开始/暂停");
	m_Cttc.AddTool(GetDlgItem(IDC_STOP),		   L"停止");
	m_Cttc.AddTool(GetDlgItem(IDC_PROS_CONS),	   L"倒放");
	m_Cttc.AddTool(GetDlgItem(IDC_LOOP),		   L"循环");
	m_Cttc.AddTool(GetDlgItem(IDC_FIRST),		   L"跳转到第一帧");
	m_Cttc.AddTool(GetDlgItem(IDC_F_B),			   L"减速");
	m_Cttc.AddTool(GetDlgItem(IDC_F_F),			   L"加速");
	m_Cttc.AddTool(GetDlgItem(IDC_LAST),		   L"跳转到最后帧");
	m_Cttc.AddTool(GetDlgItem(IDC_PREV),		   L"步退");
	m_Cttc.AddTool(GetDlgItem(IDC_NEXT),		   L"步进");
	m_Cttc.AddTool(GetDlgItem(IDC_SETTING),		   L"设置");
	m_Cttc.AddTool(GetDlgItem(IDC_PREVIEW_SCREEN), L"鼠标悬停滑动条上是否显示预览画面");

	AfxGetMainWnd()->SetWindowText(L"照妖镜");

	CRect rect;
	GetClientRect(&rect);     //取客户区大小   

	m_Point.x = rect.right - rect.left;  //Point old,用于记录窗口原始坐标
	m_Point.y = rect.bottom - rect.top;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVideoStartDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVideoStartDlg::OnPaint()
{
	//picture控件背景色为黑色
	if (m_fShowBlack)
	{
		CRect rtTop;
		CStatic *pWnd = (CStatic*)GetDlgItem(IDC_PICTURE);
		CDC *cDc = pWnd->GetDC();
		pWnd->GetClientRect(&rtTop);
		cDc->FillSolidRect(rtTop.left, rtTop.top, rtTop.Width(), rtTop.Height(), RGB(0, 0, 0));
		Invalidate(FALSE);
	}

	ShowPicture((BYTE *)m_pbBmpData, m_nBmpSize);

	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CVideoStartDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 响应文件拖拽
void CVideoStartDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CDialogEx::OnDropFiles(hDropInfo);
	
	TCHAR szStr[MAX_PATH] = {};
	::DragQueryFile(hDropInfo, 0, szStr, MAX_PATH);
	m_strPathName = szStr;
	AfxGetMainWnd()->SetWindowText(L"照妖镜：接收了一个拖拽文件");
	::DragFinish(hDropInfo);
	   
	m_pSettingDlg->SetParametersToParentWnd(m_nWidth, m_nHeight, m_nFps, m_nYuvFormat);
	KillTimer(1);
	m_bPause1 = FALSE;
	m_bEnd1 = FALSE;
	m_bPlay1 = FALSE;
	m_StartStop.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PLAY)));
	m_Stop.EnableWindow(FALSE);
	m_Slider1.EnableWindow(TRUE);
	// 显示第一帧
	ShowFistFrame();
}

void CVideoStartDlg::Open()
{
	// 打开文件
	if (CFile::hFileNull != m_cFile.m_hFile)
	{
		m_cFile.Close();
	}
	if (!m_cFile.Open(m_strPathName.GetBuffer(), CFile::modeRead))
	{
		AfxGetMainWnd()->SetWindowText(L"打开文件失败");
	}
}

BOOL CVideoStartDlg::IsOpen()
{
	if (CFile::hFileNull != m_cFile.m_hFile)
		return TRUE;
	else
		return FALSE;
}

void CVideoStartDlg::SetParentParameters(int width, int height, int fps, int fmt)
{
	m_nWidth = width;
	m_nHeight = height;
	m_nFps = fps;
	m_nYuvFormat = fmt;

	// 恢复原始状态
	if (!IsOpen()) return;

	ShowFistFrame();
}

void CVideoStartDlg::Malloc()
{
	// 根据YUV格式分配内存
	switch (m_nYuvFormat)
	{
	case FMT_YUV420:
	case FMT_YV12:
	case FMT_NV12:
	case FMT_NV21:
		m_nYuvSize = m_nWidth * m_nHeight * 3 / 2;
		break;
	case FMT_YUV422:
	case FMT_YV16:
	case FMT_YUYV:
	case FMT_YVYU:
	case FMT_UYVY:
	case FMT_VYUY:
	case FMT_NV16:
	case FMT_NV61:
		m_nYuvSize = m_nWidth * m_nHeight * 2;
		break;
	case FMT_YUV444:
	case FMT_RGB24:
	case FMT_BGR24:
		m_nYuvSize = m_nWidth * m_nHeight * 3;
		break;
	case FMT_Y:
		m_nYuvSize = m_nWidth * m_nHeight;
		break;
	default:
		m_nYuvSize = m_nWidth * m_nHeight * 3;
		break;
	}
	if (m_pbYuvData != NULL)
	{
		delete[] m_pbYuvData;
		m_pbYuvData = NULL;
	}

	// 此处有问题，更改YUV格式后会挂掉
	if (m_pbBmpData != NULL)
	{
		delete[] m_pbBmpData;
		m_pbBmpData = NULL;
	}

	if (m_pbBmpData2 != NULL)
	{
		delete[] m_pbBmpData2;
		m_pbBmpData2 = NULL;
	}

	m_nBmpSize = m_nWidth * m_nHeight * 3 + 54; // 这里申请BMP图片的空间，方便保存

	m_pbYuvData = new char[m_nYuvSize];
	m_pbBmpData = new char[m_nBmpSize];
	m_pbBmpData2 = new char[m_nBmpSize];

	m_nTotalFrame = (UINT)(m_cFile.GetLength() / m_nYuvSize); // 计算总帧数
	m_Slider1.SetRange(1, m_nTotalFrame); // 设置滑竿最小最大值
	m_Slider1.SetPos(m_nCurrentFrame);	  // 设置滑竿值

	// 显示帧数
	ShowFrameCount(m_nCurrentFrame);
}


void CVideoStartDlg::Read(INT nCurrentFrame)
{
	// 防止越界	
	if (m_nCurrentFrame > m_nTotalFrame || m_nCurrentFrame < 1)
	{
		if (m_bLoop1)
		{
			if (!m_bReverse1)
				m_nCurrentFrame = 1;
			else
				m_nCurrentFrame = m_nTotalFrame;
		}
		else
		{
			m_bPlay1 = FALSE;
			m_bEnd1 = TRUE;
			m_Stop.EnableWindow(FALSE);
			m_StartStop.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PLAY)));
			m_StartStop.EnableWindow(TRUE);
			KillTimer(1);
			if (m_bReverse1)
				m_nCurrentFrame = 1;
			else
				m_nCurrentFrame = m_nTotalFrame;
			AfxGetMainWnd()->SetWindowText(L"照妖镜：播放完毕");
			return;
		}
	}	
	   	
	this->ShowFrameCount(nCurrentFrame); // 读一帧时，顺便显示出当前帧
	m_cFile.Seek(m_nYuvSize * (nCurrentFrame - 1), SEEK_SET);
	m_cFile.Read(m_pbYuvData, m_nYuvSize);
}

void CVideoStartDlg::Show()
{
	// 先添加BMP头
	m_bmHeader.bfType = 'MB';
	m_bmHeader.bfSize = m_nBmpSize;// + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	m_bmHeader.bfReserved1 = 0;
	m_bmHeader.bfReserved2 = 0;
	m_bmHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	m_bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_bmInfo.bmiHeader.biWidth = m_nWidth;
	// note BMP图像是倒过来的
	m_bmInfo.bmiHeader.biHeight = -m_nHeight;
	m_bmInfo.bmiHeader.biPlanes = 1;
	m_bmInfo.bmiHeader.biBitCount = 24;
	m_bmInfo.bmiHeader.biCompression = BI_RGB;
	m_bmInfo.bmiHeader.biSizeImage = m_nBmpSize - 54;
	m_bmInfo.bmiHeader.biXPelsPerMeter = 0;
	m_bmInfo.bmiHeader.biYPelsPerMeter = 0;
	m_bmInfo.bmiHeader.biClrUsed = 0;
	m_bmInfo.bmiHeader.biClrImportant = 0;

	memcpy(m_pbBmpData, &m_bmHeader, sizeof(BITMAPFILEHEADER));
	memcpy(m_pbBmpData + sizeof(BITMAPFILEHEADER), &m_bmInfo, sizeof(BITMAPINFOHEADER));

	// 再转换格式
	if (m_nYuvFormat == FMT_RGB24 || m_nYuvFormat == FMT_BGR24)
	{
		memcpy(m_pbBmpData + 54, (unsigned char *)m_pbYuvData, m_nBmpSize - 54);
	}
	else
	{
		yuv_to_rgb24((YUV_TYPE)m_nYuvFormat, (unsigned char *)m_pbYuvData, (unsigned char *)m_pbBmpData + 54, m_nWidth, m_nHeight);
	}

	// BMP是BGR格式的，要转换 rgb->bgr
	if (m_nYuvFormat != FMT_BGR24)
	{
		swaprgb((BYTE*)(m_pbBmpData + 54), m_nBmpSize - 54);
	}
	// 显示
	ShowPicture((BYTE *)m_pbBmpData, m_nBmpSize);
}

// 设置按钮
void CVideoStartDlg::OnBnClickedSetting()
{
	// TODO: 在此添加控件通知处理程序代码
		// 非模态对话框
	if (m_pSettingDlg == NULL)
	{
		m_pSettingDlg = new Setting();
		m_pSettingDlg->Create(IDD_SETTING, this);
	}
	m_pSettingDlg->SetSettingWndPos();
	m_pSettingDlg->ShowWindow(SW_SHOW);
}

// 顯示第一幀
void CVideoStartDlg::ShowFistFrame()
{
	m_nCurrentFrame = 1;

	this->Open();
	this->Malloc();
	this->Read(m_nCurrentFrame);
	this->Show();

	m_StartStop.EnableWindow(TRUE);
	if (!m_bPlay1)
		m_Stop.EnableWindow(FALSE);
	else
	{
		SetTimer(1, 1000 / m_nFps, NULL);
	}
	m_pros_cons.EnableWindow(TRUE);
	m_Loop.EnableWindow(TRUE);
	m_First.EnableWindow(TRUE);
	m_Last.EnableWindow(TRUE);
	m_F_B.EnableWindow(TRUE);
	m_F_F.EnableWindow(TRUE);
	m_Prev.EnableWindow(TRUE);
	m_Next.EnableWindow(TRUE);
}

inline void RenderBitmap(CWnd *pWnd, Bitmap* pbmp)
{
	RECT rect;
	pWnd->GetClientRect(&rect);

	Graphics grf(pWnd->m_hWnd);
	if (grf.GetLastStatus() == Ok)
	{
		Rect rc(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
		grf.DrawImage(pbmp, rc);
	}
}

// 显示图片
void CVideoStartDlg::ShowPicture(BYTE* pbData, int iSize)
{
	if (pbData == NULL) return;
	CWnd* pWnd = GetDlgItem(IDC_PICTURE);   // IDC_VIDEO：picture contral 控件ID
	IStream* pPicture = NULL;
	CreateStreamOnHGlobal(NULL, TRUE, &pPicture);
	if (pPicture != NULL) 
	{
		pPicture->Write(pbData, iSize, NULL);
		LARGE_INTEGER liTemp = { 0 };
		pPicture->Seek(liTemp, STREAM_SEEK_SET, NULL);
		Bitmap TempBmp(pPicture);
		RenderBitmap(pWnd, &TempBmp);
	}
	if (pPicture != NULL)
	{
		pPicture->Release();
		pPicture = NULL;
	}

	m_fShowBlack = FALSE;
}

void CVideoStartDlg::OnStnClickedPicture()
{
	// TODO: 在此添加控件通知处理程序代码
}

// 显示当前帧/总帧数
void CVideoStartDlg::ShowFrameCount(int nCurrentFrame)
{
	CString strTemp;
	strTemp.Format(_T("%d/%d"), nCurrentFrame, m_nTotalFrame);
	GetDlgItem(IDC_STATIC_FRAME)->SetWindowText(strTemp);
	m_Slider1.SetPos(nCurrentFrame);
}


// @@@@@@@@@@@定时器处理线程
void CVideoStartDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		if (!m_bReverse1)
			++m_nCurrentFrame;
		else
			--m_nCurrentFrame;
		Read(m_nCurrentFrame);
		Show();
	}

	CDialogEx::OnTimer(nIDEvent);
}

// 0 打开文件
void CVideoStartDlg::OnBnClickedOpenFile()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("dav file (*.dav)|*.yuv|directory (*.*)|*.*||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		// 打开成功，保存文件路径
		m_Slider1.EnableWindow(TRUE);
		m_strPathName = dlg.GetPathName();
		m_pSettingDlg->SetParametersToParentWnd(m_nWidth, m_nHeight, m_nFps, m_nYuvFormat);
		KillTimer(1);
		m_bPause1 = FALSE;
		m_bEnd1 = FALSE;
		m_bPlay1 = FALSE;
		m_StartStop.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PLAY)));
		m_Stop.EnableWindow(FALSE);
		// 显示第一帧
		ShowFistFrame();
		AfxGetMainWnd()->SetWindowText(L"照妖镜：打开了一个文件");
	}
	else
	{
		AfxGetMainWnd()->SetWindowText(L"照妖镜：打开文件错误");
	}
}

// 1 播放暂停按钮
void CVideoStartDlg::OnBnClickedStartStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Stop.EnableWindow(TRUE);

	if (!m_bPlay1)
	{
		SetTimer(1, 1000 / m_nFps, NULL);
		m_StartStop.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PAUSE)));
		m_Stop.EnableWindow(TRUE);
		m_bPlay1 = TRUE;
		AfxGetMainWnd()->SetWindowText(L"照妖镜：已开始播放");
	}
	else
	{
		KillTimer(1);
		m_StartStop.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PLAY)));

		if (m_bPlay1 == TRUE)
		{
			m_Stop.EnableWindow(FALSE);
			m_bPlay1 = FALSE;
		}
		if (m_bEnd1 == TRUE)
		{
			m_StartStop.EnableWindow(FALSE);
			m_bPlay1 = FALSE;
		}
		AfxGetMainWnd()->SetWindowText(L"照妖镜：已暂停播放");
	}

	if (m_bPause1)
	{
		SetTimer(1, 1000 / m_nFps, NULL);
		m_StartStop.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PAUSE)));
		m_StartStop.EnableWindow(TRUE);
		m_Stop.EnableWindow(TRUE);
		m_bPause1 = FALSE;
		AfxGetMainWnd()->SetWindowText(L"照妖镜：已开始播放");
	}
	if (m_bEnd1)
	{
		SetTimer(1, 1000 / m_nFps, NULL);
		m_StartStop.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PAUSE)));
		m_StartStop.EnableWindow(TRUE);
		m_Stop.EnableWindow(TRUE);
		if (!m_bReverse1)
			m_nCurrentFrame = 1;
		else
			m_nCurrentFrame = m_nTotalFrame;
		m_bEnd1 = FALSE;
		AfxGetMainWnd()->SetWindowText(L"照妖镜：已开始播放");
	}
}

// 2 停止
void CVideoStartDlg::OnBnClickedStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bPlay1 = FALSE;
	m_bPause1 = FALSE;
	m_bEnd1 = FALSE;

	KillTimer(1);
	m_StartStop.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PLAY)));
	m_StartStop.EnableWindow(TRUE);
	m_Stop.EnableWindow(FALSE);
	if (!m_bReverse1)
		OnBnClickedFirst();
	else
		OnBnClickedLast();
	AfxGetMainWnd()->SetWindowText(L"照妖镜：停止播放");
}

// 3 倒放与正放
void CVideoStartDlg::OnBnClickedProsCons()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_bReverse1)
	{
		m_bReverse1 = TRUE;
		m_pros_cons.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_FORWARD)));
		AfxGetMainWnd()->SetWindowText(L"照妖镜：已设置为逆序播放");
	}
	else
	{
		m_bReverse1 = FALSE;
		m_pros_cons.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BACK)));
		AfxGetMainWnd()->SetWindowText(L"照妖镜：已设置为正序播放");
	}
	m_pros_cons.EnableWindow(TRUE);
}

// 4 循环
void CVideoStartDlg::OnBnClickedLoop()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_bLoop1)
	{
		m_bLoop1 = TRUE;
		m_Loop.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SINGLE)));
		AfxGetMainWnd()->SetWindowText(L"照妖镜：已设置为循环播放");
	}
	else
	{
		m_bLoop1 = FALSE;
		m_Loop.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_LOOP)));
		AfxGetMainWnd()->SetWindowText(L"照妖镜：已取消循环播放");
	}
	m_Loop.EnableWindow(TRUE);
}

// 5 停留在第一帧
void CVideoStartDlg::OnBnClickedFirst()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nCurrentFrame = 1;
	this->Read(m_nCurrentFrame);
	this->Show();
	if (m_bReverse1)
	{
		m_bPlay1 = FALSE;
		m_bEnd1 = TRUE;
		m_Stop.EnableWindow(FALSE);
		m_StartStop.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PLAY)));
		m_StartStop.EnableWindow(TRUE);
		KillTimer(1);
	}
	AfxGetMainWnd()->SetWindowText(L"照妖镜：已跳转到第一帧");
}

// 6 快退
void CVideoStartDlg::OnBnClickedFB()
{
	// TODO: 在此添加控件通知处理程序代码
	int tempFps = 0;
	CString temp;
	if (!m_bReverse1)
	{
		tempFps = m_nFps / 2;
		if (tempFps <= 1)
			m_nFps = 1;
		else
			m_nFps = tempFps;
		if (m_bPlay1)
			SetTimer(1, 1000 / m_nFps, NULL);		
		temp.Format(L"照妖镜：减速到 %d帧/s", m_nFps);
		AfxGetMainWnd()->SetWindowText(temp.GetBuffer());
	}
	else
	{
		tempFps = m_nFps * 2;
		if (tempFps >= 120)
			m_nFps = 120;
		else
			m_nFps = tempFps;
		if (m_bPlay1)
			SetTimer(1, 1000 / m_nFps, NULL);
		temp.Format(L"照妖镜：加速到 %d帧/s", m_nFps);
		AfxGetMainWnd()->SetWindowText(temp.GetBuffer());
	}		
}

// 7 快进
void CVideoStartDlg::OnBnClickedFF()
{
	// TODO: 在此添加控件通知处理程序代码
	int tempFps = 0;
	CString temp;
	if (m_bReverse1)
	{
		tempFps = m_nFps / 2;
		if (tempFps <= 1)
			m_nFps = 1;
		else
			m_nFps = tempFps;
		if (m_bPlay1)
			SetTimer(1, 1000 / m_nFps, NULL);
		temp.Format(L"照妖镜：减速到 %d帧/s", m_nFps);
		AfxGetMainWnd()->SetWindowText(temp.GetBuffer());
	}
	else
	{
		tempFps = m_nFps * 2;
		if (tempFps >= 120)
			m_nFps = 120;
		else
			m_nFps = tempFps;
		if (m_bPlay1)
			SetTimer(1, 1000 / m_nFps, NULL);
		temp.Format(L"照妖镜：加速到 %d帧/s", m_nFps);
		AfxGetMainWnd()->SetWindowText(temp.GetBuffer());
	}
}

// 8 停留在最后一帧
void CVideoStartDlg::OnBnClickedLast()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nCurrentFrame = m_nTotalFrame;
	this->Read(m_nCurrentFrame);
	this->Show();
	if (!m_bReverse1)
	{
		m_bPlay1 = FALSE;
		m_bEnd1 = TRUE;
		m_Stop.EnableWindow(FALSE);
		m_StartStop.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PLAY)));
		m_StartStop.EnableWindow(TRUE);
		KillTimer(1);
	}
	AfxGetMainWnd()->SetWindowText(L"照妖镜：已跳转到最后帧");
}

// 9 步退
void CVideoStartDlg::OnBnClickedPrev()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);
	m_bPlay1 = FALSE;
	m_bPause1 = FALSE;
	m_Stop.EnableWindow(FALSE);
	m_StartStop.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PLAY)));
	--m_nCurrentFrame;
	if (m_nCurrentFrame < 1)
	{
		m_nCurrentFrame = 1;
		return;
	}
	this->Read(m_nCurrentFrame);
	this->Show();
	AfxGetMainWnd()->SetWindowText(L"照妖镜：已步退一帧");
}

// 10 步进
void CVideoStartDlg::OnBnClickedNext()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);
	m_bPlay1 = FALSE;
	m_bPause1 = FALSE;
	m_Stop.EnableWindow(FALSE);
	m_StartStop.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PLAY)));
	++m_nCurrentFrame;
	if (m_nCurrentFrame > m_nTotalFrame)
	{
		m_nCurrentFrame = m_nTotalFrame;
		return;
	}
	this->Read(m_nCurrentFrame);
	this->Show();
	AfxGetMainWnd()->SetWindowText(L"照妖镜：已步进一帧");
}

// 11 响应滚动条处理
void CVideoStartDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	
	m_nCurrentFrame = m_Slider1.GetPos();	
	if (nPos)
		m_nCurrentFrame = nPos;
	this->Read(m_nCurrentFrame);
	this->Show();

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);	
}


BOOL CVideoStartDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_Cttc.RelayEvent(pMsg);

	return CDialogEx::PreTranslateMessage(pMsg);
}

// 窗口大小变化是会被调用
void CVideoStartDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 窗口最小显示
	//lpMMI->ptMinTrackSize.x = m_Point.x;   //x宽度  
	//lpMMI->ptMinTrackSize.y = m_Point.y;   //y高度   

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

// 12.1自适应窗口大小
void CVideoStartDlg::MyWinAdaption()
{
	// TODO: 在此处添加实现代码.

#if 1
	int woc;
	float fx, fy;

	POINT currSize; // 当前对话框大小
	CRect rt, crt;
	CPoint OldTLPoint, TLPoint; //左上角  
	CPoint OldBRPoint, BRPoint; //右下角  

	GetClientRect(&rt);

	m_Point.x;
	m_Point.y;

	currSize.x = rt.right - rt.left;
	currSize.y = rt.bottom - rt.top;
	   
	fx = (float)currSize.x / m_Point.x;
	fy = (float)currSize.y / m_Point.y;

	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件    
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//取得ID  
		GetDlgItem(woc)->GetWindowRect(crt);
		ScreenToClient(crt);
		OldTLPoint = crt.TopLeft();
		TLPoint.x = long(OldTLPoint.x*fx);
		TLPoint.y = long(OldTLPoint.y*fy);
		OldBRPoint = crt.BottomRight();
		BRPoint.x = long(OldBRPoint.x *fx);
		BRPoint.y = long(OldBRPoint.y *fy);
		crt.SetRect(TLPoint, BRPoint);
		GetDlgItem(woc)->MoveWindow(crt, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
	m_Point = currSize;
#endif
}

// 12.2窗口大小改变后立即知行如下函数
void CVideoStartDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	
	if (nType != SIZE_MINIMIZED)
		MyWinAdaption();
	// TODO: 在此处添加消息处理程序代码
}

// 13.1 鼠标移动到特定控件后的消息响应
BOOL CVideoStartDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	
	// 移动到滑竿控件内
	if (pWnd->GetDlgCtrlID() == IDC_SLIDER1 && m_bShowPreview)
	{
		CString str;
		RECT rect, rt, crt;
		CPoint point, point1;
		float fNum = 0;
		int posFrame = 0;

		pWnd->GetWindowRect(&rect);// 获取的坐标为屏幕坐标系中的坐标
		this->ScreenToClient(&rect);// 转换为客户区坐标
		int nWhide = rect.right - rect.left;

		GetCursorPos(&point);
		point1 = point;
		ScreenToClient(&point);

		if (point.x > 0 && point.x < rect.right)
		{
			fNum = (float)point.x / nWhide;
			posFrame = (int)(fNum * m_nTotalFrame);
		}

		m_pPreview->GetWindowRect(&crt);

		if (m_pPreview)
		{
			// 显示预览小窗口
			m_bWithinSlider = FALSE;
			PreviewShow(posFrame);
			m_pPreview->SetWindowPos(NULL, point1.x + 5, point1.y - 8 - crt.bottom + crt.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
			m_pPreview->ShowWindow(SW_SHOW);
		}

		//str.Format(L"x=%d, y=%d, w=%d.	f=%.4f, fps=%d", point.x, point.y, nWhide, fNum, posFrame);
		//AfxGetMainWnd()->SetWindowText(str);
	}
	else
	{
		if (m_pPreview && !m_bWithinSlider)
		{
			m_bWithinSlider = TRUE;
			m_pPreview->ShowWindow(SW_HIDE);
		}
	}
	
	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}

// 13.2 显示预览小窗口
void CVideoStartDlg::PreviewShow(int nCurrentFrame)
{
	// TODO: 在此处添加实现代码.
	// 防止越界
	if (nCurrentFrame < 1 || nCurrentFrame > m_nTotalFrame)
		return;

	m_cFile.Seek(m_nYuvSize * (nCurrentFrame - 1), SEEK_SET);
	m_cFile.Read(m_pbYuvData, m_nYuvSize);

	// 先添加BMP头
	m_bmHeader2.bfType = 'MB';
	m_bmHeader2.bfSize = m_nBmpSize;// + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	m_bmHeader2.bfReserved1 = 0;
	m_bmHeader2.bfReserved2 = 0;
	m_bmHeader2.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	m_bmInfo2.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_bmInfo2.bmiHeader.biWidth = m_nWidth;
	// note BMP图像是倒过来的
	m_bmInfo2.bmiHeader.biHeight = -m_nHeight;
	m_bmInfo2.bmiHeader.biPlanes = 1;
	m_bmInfo2.bmiHeader.biBitCount = 24;
	m_bmInfo2.bmiHeader.biCompression = BI_RGB;
	m_bmInfo2.bmiHeader.biSizeImage = m_nBmpSize - 54;
	m_bmInfo2.bmiHeader.biXPelsPerMeter = 0;
	m_bmInfo2.bmiHeader.biYPelsPerMeter = 0;
	m_bmInfo2.bmiHeader.biClrUsed = 0;
	m_bmInfo2.bmiHeader.biClrImportant = 0;

	memcpy(m_pbBmpData2, &m_bmHeader2, sizeof(BITMAPFILEHEADER));
	memcpy(m_pbBmpData2 + sizeof(BITMAPFILEHEADER), &m_bmInfo2, sizeof(BITMAPINFOHEADER));

	// 再转换格式
	if (m_nYuvFormat == FMT_RGB24 || m_nYuvFormat == FMT_BGR24)
	{
		memcpy(m_pbBmpData2 + 54, (unsigned char *)m_pbYuvData, m_nBmpSize - 54);
	}
	else
	{
		yuv_to_rgb24((YUV_TYPE)m_nYuvFormat, (unsigned char *)m_pbYuvData, (unsigned char *)m_pbBmpData2 + 54, m_nWidth, m_nHeight);
	}

	// BMP是BGR格式的，要转换 rgb->bgr
	if (m_nYuvFormat != FMT_BGR24)
	{
		swaprgb((BYTE*)(m_pbBmpData2 + 54), m_nBmpSize - 54);
	}

	m_pPreview->ShowPicture((BYTE*)m_pbBmpData2, m_nBmpSize);
}

// 14 是否启用预览小界面
void CVideoStartDlg::OnBnClickedPreviewScreen()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bShowPreview)
	{
		m_bShowPreview = FALSE;
		m_PreScreen.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PREVIEW)));
		m_PreScreen.EnableWindow(TRUE);
		AfxGetMainWnd()->SetWindowText(L"照妖镜：已关闭鼠标悬停滑动条上显示预览画面");
	}
	else
	{
		m_bShowPreview = TRUE;
		m_PreScreen.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_NOT_PREVIEW)));
		m_PreScreen.EnableWindow(TRUE);
		AfxGetMainWnd()->SetWindowText(L"照妖镜：已开启鼠标悬停滑动条上显示预览画面");
	}
}

// 自定义消息
afx_msg LRESULT CVideoStartDlg::OnSettingWnd(WPARAM wParam, LPARAM lParam)
{

	return 0;
}
