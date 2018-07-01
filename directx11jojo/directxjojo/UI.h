#pragma once

#include "UiSkills.h"

class UI : public MActor
{
public:
	UI();
	UI(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	~UI();
private:
	bool mUIVisible;
	vector<XMFLOAT2> mVecUiPos;
	Player* mpPlayer;
	unique_ptr<UiSkills> mspUiSkill;
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();

	bool CheckAttackArea();
	bool CheckWaitArea();
	bool CheckSkillArea();
	bool CheckCancelArea();
public:
	void SetPlayer(Player * player) { mpPlayer = player; }
	Player * GetPlayer() { return mpPlayer; }

	void SetVisible(bool visible) { mUIVisible = visible; }
	bool GetVisible() { return mUIVisible; }

	UiSkills* GetUiSkills() { return mspUiSkill.get(); }

};


