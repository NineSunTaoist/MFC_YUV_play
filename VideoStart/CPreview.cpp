// CPreview.cpp: 实现文件
//

#include "pch.h"
#include "VideoStart.h"
#include "CPreview.h"
#include "afxdialogex.h"


// CPreview 对话框

IMPLEMENT_DYNAMIC(CPreview, CDialogEx)

CPreview::CPreview(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PREVIEW, pParent)
{

}

CPreview::~CPreview()
{
}

void CPreview::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPreview, CDialogEx)
END_MESSAGE_MAP()


// CPreview 消息处理程序
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

// // 显示图片
void CPreview::ShowPicture(BYTE* pbData, int iSize)
{
	// TODO: 在此处添加实现代码.
		// 显示
	CWnd* pWnd = GetDlgItem(IDC_PREVIEW);   // IDC_VIDEO：picture contral 控件ID
	IStream* pPicture = NULL;
	CreateStreamOnHGlobal(NULL, TRUE, &pPicture);
	if (pPicture != NULL)
	{
		pPicture->Write((BYTE *)pbData, iSize, NULL);
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
}
