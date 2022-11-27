#include "pch.h"
#include "MySlider.h"


IMPLEMENT_DYNAMIC(MySlider, CSliderCtrl)
MySlider::MySlider()
{
}

MySlider::~MySlider()
{
}

BEGIN_MESSAGE_MAP(MySlider, CSliderCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void MySlider::OnLButtonDown(UINT nFlags, CPoint point)
{
#if 1
	CRect rc, trc;
	GetChannelRect(rc);
	GetThumbRect(trc);
	rc.InflateRect(0, (trc.Height() - rc.Height()) / 2);

	if (!PtInRect(rc, point))
		return;

	LONG range = GetRangeMax();
	LONG pos = point.x - rc.left - trc.Width() / 2;
	LONG width = rc.Width() - trc.Width();
	CSliderCtrl::SetPos(int(DOUBLE(pos) * range / width + 0.5));

	CSliderCtrl::OnLButtonDown(nFlags, point);
#endif	
}