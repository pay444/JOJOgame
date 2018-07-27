#pragma once
class EventStageMap : public MActor
{
public:
	EventStageMap();
	EventStageMap(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~EventStageMap();
private:
	wstring mStageName;
	bool mVisble;
	float mfElpTime;		//일정시간이 지난후에 전투 씬으로 넘겨줌
	int mScene;			//어떤 씬으로 넘어갈지
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();

public:
	void SetVisible(bool visible) { mVisble = visible; }
	bool GetVisible() { return mVisble; }

	void SetStageName(wstring name) { mStageName = name; }
	wstring GetStageName() { return mStageName; }

	void SetScene(int scene) { mScene = scene; }
	int GetScene() { return mScene; }
};

