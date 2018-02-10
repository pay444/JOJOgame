#pragma once

class Scene 
{
public:
	Scene();
	virtual ~Scene();
public:
	virtual HRESULT Initialize(SpriteBatch *spritBatch, SpriteFont* spriteFont)PURE;
	virtual E_SCENE Update(float dt)PURE;
	virtual void Render()PURE;
	virtual void Release()PURE;

public:
	virtual E_SCENE GetScene()PURE;
	virtual void SetScene(E_SCENE eScene)PURE;
};

