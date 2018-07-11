#include "stdafx.h"
#include "Skill.h"


Skill::Skill()
{
}

Skill::Skill(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	: MActor(pBatch, pSheet, pFont)
{

}

Skill::~Skill()
{
}
