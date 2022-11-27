#pragma once

class CVideoStartDlg;

// Setting 对话框

class Setting : public CDialogEx
{
	DECLARE_DYNAMIC(Setting)

public:
	Setting(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Setting();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CVideoStartDlg* m_pParentWnd;

public:
	CComboBox m_cbFps;
	CComboBox m_cbYuvFormat;
	CComboBox m_cbResolution;

	int m_nHeight;
	int m_nWidth;
	int m_nFps;
	int m_nYuvFormat;
	int m_nFpsIndex;

	CString m_strResolution;
	CStringArray m_ResolutionList;

	virtual BOOL OnInitDialog();

	void SetParentWnd(CVideoStartDlg* pWnd) { m_pParentWnd = pWnd; }
	void SetParametersToParentWnd(int& width, int& height, int& fps, int& fmt)
	{
		width = m_nWidth;
		height = m_nHeight;
		fps = m_nFps;
		fmt = m_nYuvFormat;
	}

	afx_msg void OnBnClickedSetOk();
	void UpdateParam();
	afx_msg void OnCbnSelchangeResolution();
	afx_msg void OnCbnSelchangeFps();
	afx_msg void OnCbnSelchangeYuvFormat();
	afx_msg void OnBnClickedSetCancel();
	afx_msg void OnBnClickedSetApply();
	void SetSettingWndPos();
};
