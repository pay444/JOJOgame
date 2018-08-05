#include "stdafx.h"
#include "Skill.h"


Skill::Skill()
{
}

Skill::Skill(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	: MActor(pBatch, pSheet, pFont)
{
	mfAnimTime = 0.0f;
	mfDelayAnimTime = 0.0f;
	mbAnimSkill = true;
}

Skill::~Skill()
{
}
