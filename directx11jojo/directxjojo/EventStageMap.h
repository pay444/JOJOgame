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
	float mfElpTime;		//�����ð��� �����Ŀ� ���� ������ �Ѱ���
	int mScene;			//� ������ �Ѿ��
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

