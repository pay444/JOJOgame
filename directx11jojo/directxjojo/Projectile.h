#pragma once
//#include "MActor.h"
class Projectile :
	public MActor
{
public:
	Projectile();
	Projectile(SpriteBatch *pBatch, SpriteSheet *pSheet,SpriteFont * pFont);
	virtual ~Projectile();

public:
	virtual void Init(XMFLOAT2 position, XMFLOAT2 direction,
		float speed, float lifeTime, float ditance);
	virtual E_SCENE Update(float dt);

protected:
	XMFLOAT2	mvDirection;		//x,y성분으로된 총알이 어디로 날라갈지
	float		mfSpeed;			//속도
	float		mfLifeTIme;			//시간이 지나면 자동삭제됨
	float		mfMaxDistance;		//이만큼 이동하면 삭제됨

protected:
	float		mfElapsedTime;		//경과시간
	float		mfDistance;			
};

