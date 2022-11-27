#pragma once
#include <afxcmn.h>
class MySlider :
	public CSliderCtrl
{
	DECLARE_DYNAMIC(MySlider)
public:
	MySlider();
	virtual ~MySlider();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

