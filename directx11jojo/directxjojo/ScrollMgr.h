#pragma once
class ScrollMgr final
{
public:
	//레퍼런스 표현
	static ScrollMgr& Instance()
	{
		static ScrollMgr instance;
		return instance;
	}
private:
	ScrollMgr();
public:
	~ScrollMgr();

public:
private:
	Vector2 mfloat2Scroll;
public:
	void SetScroll(float x_, float y_);// { mfloat2Scroll.x = x_; mfloat2Scroll.y = y_; }
	void SetAddScroll(float x_, float y_);// { mfloat2Scroll.x += x_; mfloat2Scroll.y += y_; }
	Vector2 GetScroll() { return mfloat2Scroll; }
};

