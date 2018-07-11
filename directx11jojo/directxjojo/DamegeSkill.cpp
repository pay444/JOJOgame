#include "stdafx.h"
#include "DamegeSkill.h"


DamegeSkill::DamegeSkill()
{
}


DamegeSkill::DamegeSkill(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	:Skill(pBatch,pSheet,pFont)
{

}

DamegeSkill::~DamegeSkill()
{
}

void DamegeSkill::Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible)
{

}

E_SCENE DamegeSkill::Update(float dt)
{
	E_SCENE eResult = MActor::Update(dt);

	return E_SCENE_NONPASS;
}

void DamegeSkill::Draw()
{

}
