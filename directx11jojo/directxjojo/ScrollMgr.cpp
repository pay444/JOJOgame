#include "stdafx.h"
#include "ScrollMgr.h"


ScrollMgr::ScrollMgr():
	mfloat2Scroll(0.0f,0.0f)
{
}


ScrollMgr::~ScrollMgr()
{
}

void ScrollMgr::SetScroll(float x_, float y_)
{
	 mfloat2Scroll.x = x_; 
	 mfloat2Scroll.y = y_;
}

void ScrollMgr::SetAddScroll(float x_, float y_)
{
	mfloat2Scroll.x += x_; 
	mfloat2Scroll.y += y_;

	if (mfloat2Scroll.x < 0)
		mfloat2Scroll.x = 0;
	if (mfloat2Scroll.y < 0)
		mfloat2Scroll.y = 0;
	//넘어가는 범위지정
	if (mfloat2Scroll.x > -(DEFAULT_SCREEN_WIDTH - 960))
		mfloat2Scroll.x = -(DEFAULT_SCREEN_WIDTH - 960);
	if (mfloat2Scroll.y > -(DEFAULT_SCREEN_HEIGHT - 960))
		mfloat2Scroll.y = -(DEFAULT_SCREEN_HEIGHT - 960);
}
