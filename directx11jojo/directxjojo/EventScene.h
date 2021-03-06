#pragma once
#include "Scene.h"
class EventBaseMap;
class EventCharacter;
class EventTalk;
class EventStageMap;
class EventScene : public Scene
{
private:
	E_SCENE mEventScene;

public:
	EventScene();
	virtual ~EventScene();
protected:
	UINT		mScreenWidth;
	UINT		mScreenHeight;
private:
	EventBaseMap * mpEventMap;
	EventCharacter* mpEventChr;
	EventTalk *		mpEventTalk;
	EventStageMap* mpEventStageMap;
public:
	virtual HRESULT Initialize(SpriteBatch* spriteBatch, SpriteFont* spriteFont);
	virtual E_SCENE Update(float dt);
	virtual void Render();
	virtual void Release();

public:
	virtual E_SCENE GetScene() { return mEventScene; }
	virtual void SetScene(E_SCENE eScene) { mEventScene = eScene; }
};

