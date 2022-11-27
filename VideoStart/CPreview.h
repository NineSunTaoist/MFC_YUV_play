#pragma once


// CPreview 对话框

class CPreview : public CDialogEx
{
	DECLARE_DYNAMIC(CPreview)

public:
	CPreview(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPreview();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PREVIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// // 显示图片
	void ShowPicture(BYTE* pbData, int iSize);
};
