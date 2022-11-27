
// VideoStartDlg.h: 头文件
//

#pragma once

#include "Setting.h"
#include "MySlider.h"
#include "CPreview.h"

#define WM_SETTING_WND (WM_USER + 1)  // 自定义消息

// CVideoStartDlg 对话框
class CVideoStartDlg : public CDialogEx
{

// 构造
public:
	CVideoStartDlg(CWnd* pParent = nullptr);	// 标准构造函数
	~CVideoStartDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIDEOSTART_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	BOOL m_fInit;
	BOOL m_fShowBlack;

	Setting *m_pSettingDlg; // 设置窗口对象指针
	CPreview *m_pPreview;	// 预览小窗口对象指针
	static BOOL m_bPause1;	// 是否暂停状态，1是，0否
	static BOOL m_bEnd1;	// 是否结束状态
	static BOOL m_bPlay1;	// 是否播放状态
	static BOOL m_bReverse1;// 是否倒放状态
	static BOOL m_bLoop1;	// 是否循环状态
	BOOL m_bWithinSlider;	// 鼠标是否在滑动条区域内
	BOOL m_bShowPreview;	// 是否显示预览小画面

	int m_nWidth;
	int m_nHeight;
	int m_nFps;
	int m_nYuvFormat;

	int m_nStartX[2][11];   // 窗口拖动所需位置

	char*   m_pbYuvData;    // YUV数据
	char*   m_pbBmpData;    // RGB数据
	char*   m_pbBmpData2;    // RGB数据
	UINT    m_nYuvSize; // 一幅图像大小
	UINT    m_nBmpSize; // BMP图的大小

	BITMAPFILEHEADER m_bmHeader;
	BITMAPINFO       m_bmInfo;

	BITMAPFILEHEADER m_bmHeader2;
	BITMAPINFO       m_bmInfo2;
	

	INT m_nTotalFrame;   // 总帧数，由文件大小及YUV大小计算而得
	INT m_nCurrentFrame; // 当前帧，从1开始计算，但seek文件时会自动减1

	CFile m_cFile;
	CString m_strPathName;

	POINT m_Point;
	
	// 共用内部函数
	BOOL IsOpen();
	void Open();
	void Malloc();
	void Read(INT nCurrentFrame);
	void Show();	
	void ShowFrameCount(int nCurrentFrame);
	void ShowFistFrame();
	void ShowPicture(BYTE* pbData, int iSize);
	void PreviewShow(int nCurrentFrame);
	void SetParentParameters(int width, int height, int fps, int fmt);
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);

	CToolTipCtrl m_Cttc;

	// 滑杆1
	MySlider m_Slider1;

	CButton m_StartStop;
	CButton m_OpenFile;
	CButton m_Stop;
	CButton m_pros_cons;
	CButton m_Loop;
	CButton m_First;
	CButton m_Last;
	CButton m_F_B;
	CButton m_F_F;
	CButton m_Prev;
	CButton m_Next;
	CButton m_PreScreen;
	CButton m_Setting;
	afx_msg void OnBnClickedSetting();
	CStatic m_Picture;
	afx_msg void OnStnClickedPicture();
	afx_msg void OnBnClickedStartStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedFirst();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedProsCons();
	afx_msg void OnBnClickedLast();
	afx_msg void OnBnClickedLoop();
	afx_msg void OnBnClickedFB();
	afx_msg void OnBnClickedFF();
	afx_msg void OnBnClickedPrev();
	afx_msg void OnBnClickedNext();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedOpenFile();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	void MyWinAdaption();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	
	afx_msg void OnBnClickedPreviewScreen();
	
protected:
	afx_msg LRESULT OnSettingWnd(WPARAM wParam, LPARAM lParam);
};
