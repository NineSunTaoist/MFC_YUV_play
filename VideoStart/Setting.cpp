// Setting.cpp: 实现文件
//

#include "pch.h"
#include "VideoStart.h"
#include "VideoStartDlg.h"
#include "Setting.h"
#include "afxdialogex.h"


// Setting 对话框

IMPLEMENT_DYNAMIC(Setting, CDialogEx)

Setting::Setting(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETTING, pParent)
{
	m_pParentWnd = NULL;
	m_strResolution = L"160x120;176x144;320x240;352x288;640x480;704x576;720x480;1280x720;1600x1200;1920x1080;2560x1440";
	m_nWidth = 352;
	m_nHeight = 288;
	m_nFps = 25;
	m_nFpsIndex = 8; // index 7 == 25
	m_nYuvFormat = 1;   // default yuv420
}

Setting::~Setting()
{
}

void Setting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FPS, m_cbFps);
	DDX_Control(pDX, IDC_YUV_FORMAT, m_cbYuvFormat);
	DDX_Control(pDX, IDC_RESOLUTION, m_cbResolution);
}


BEGIN_MESSAGE_MAP(Setting, CDialogEx)
	ON_BN_CLICKED(IDC_SET_OK, &Setting::OnBnClickedSetOk)
	ON_CBN_SELCHANGE(IDC_RESOLUTION, &Setting::OnCbnSelchangeResolution)
	ON_CBN_SELCHANGE(IDC_FPS, &Setting::OnCbnSelchangeFps)
	ON_CBN_SELCHANGE(IDC_YUV_FORMAT, &Setting::OnCbnSelchangeYuvFormat)
	ON_BN_CLICKED(IDC_SET_CANCEL, &Setting::OnBnClickedSetCancel)
	ON_BN_CLICKED(IDC_SET_APPLY, &Setting::OnBnClickedSetApply)
END_MESSAGE_MAP()


// Setting 消息处理程序


BOOL Setting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 1、初始化分辨率下拉列表
	int pos = 0;
	CString strTemp;
	strTemp = m_strResolution.Tokenize(L";", pos);
	m_ResolutionList.Add(strTemp);
	while (!strTemp.IsEmpty())
	{
		strTemp = m_strResolution.Tokenize(L";", pos);
		m_ResolutionList.Add(strTemp);
	}
	// 不知为何，最后一个是空的，这里删除，否则后面再添加时会多一个空字符串
	m_ResolutionList.RemoveAt(m_ResolutionList.GetCount() - 1);
	
	int nIndex = -1;
	for (int i = 0; i < m_ResolutionList.GetCount(); i++)
	{
		m_cbResolution.AddString(m_ResolutionList[i]);
		int width = 0;
		int height = 0;
		// 把字符串的值赋给整数
		swscanf_s(m_ResolutionList[i].GetBuffer(), _T("%dx%d"), &width, &height);
		if (width == m_nWidth && height == m_nHeight)
			nIndex = i;
	}
		
	// 从下拉列表中选择项
	m_cbFps.SetCurSel(7);
	m_cbYuvFormat.SetCurSel(1);
	m_cbResolution.SetCurSel(3);
		   
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 1 确定按钮
void Setting::OnBnClickedSetOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pParentWnd->SetParentParameters(m_nWidth, m_nHeight, m_nFps, m_nYuvFormat);
	AfxGetMainWnd()->SetWindowText(L"照妖镜：已完成设置");
	CDialogEx::OnOK();
}

// 2 取消按钮
void Setting::OnBnClickedSetCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxGetMainWnd()->SetWindowText(L"照妖镜：已关闭设置窗口");
	CDialogEx::OnCancel();
}

// 3 应用按钮
void Setting::OnBnClickedSetApply()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pParentWnd->SetParentParameters(m_nWidth, m_nHeight, m_nFps, m_nYuvFormat);
	AfxGetMainWnd()->SetWindowText(L"照妖镜：已应用设置");
}


// 更新参数
void Setting::UpdateParam()
{
	// TODO: 在此处添加实现代码.
	int nResolutionIdx = -1;
	for (int i = 0; i < m_ResolutionList.GetCount(); i++)
	{
		int width = 0;
		int height = 0;
		swscanf_s(m_ResolutionList[i].GetBuffer(), _T("%dx%d"), &width, &height);
		if (width == m_nWidth && height == m_nHeight)
			nResolutionIdx = i;
	}

	m_cbResolution.SetCurSel(nResolutionIdx);
}

// 响应分辨率下拉列表
void Setting::OnCbnSelchangeResolution()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;
	m_cbResolution.GetLBText(m_cbResolution.GetCurSel(), strTemp);
	swscanf_s(strTemp.GetBuffer(), _T("%dx%d"), &m_nWidth, &m_nHeight);

	UpdateData(FALSE);
}

// 响应FPS下拉列表
void Setting::OnCbnSelchangeFps()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;
	m_nFpsIndex = m_cbFps.GetCurSel();
	m_cbFps.GetLBText(m_cbFps.GetCurSel(), strTemp);
	swscanf_s(strTemp.GetBuffer(), _T("%d"), &m_nFps);
}

// 响应YUV格式下拉列表
void Setting::OnCbnSelchangeYuvFormat()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nYuvFormat = m_cbYuvFormat.GetCurSel();
}



// 设置本窗口坐标
void Setting::SetSettingWndPos()
{
	// TODO: 在此处添加实现代码.
	// 设置窗口位置
	CString str;
	RECT rt, prt;
	GetWindowRect(&rt);
	m_pParentWnd->GetWindowRect(&prt);

	int childWhide = rt.right - rt.left;
	int parentWhide = prt.right - prt.left;
	int whide = (parentWhide - childWhide) / 2;
	int high = (prt.bottom - prt.top) / 5;

	SetWindowPos(NULL, prt.left + whide, prt.top + high, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}
