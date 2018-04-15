#include "stdafx.h"
#include "World.h"
#include <iostream>
#include "tinyXml2.h"


World::World(SpriteBatch *pBatch, SpriteFont * pFont,int screenWidth, int screenHeight)
{
	mpBatch = pBatch;
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;
	mpFont = pFont;
	visible = false;
	mCameraPosition = Vector2(0.0f, 0.0f);
}


World::~World()
{
}

void World::Init(string filename, E_SORTID eSortID)
{
	tinyxml2::XMLDocument doc;

	doc.LoadFile(filename.c_str());

	auto mapElement = doc.FirstChildElement("map");
	mWidth = mapElement->IntAttribute("width");
	mHeight = mapElement->IntAttribute("height");
	mTileWidth = mapElement->IntAttribute("tilewidth");
	mTileHeight = mapElement->IntAttribute("tileheight");

	auto tilesetElement = mapElement->FirstChildElement("tileset");
	mTileSet.width = tilesetElement->IntAttribute("tilewidth");
	mTileSet.height = tilesetElement->IntAttribute("tileheight");
	mTileSet.count = tilesetElement->IntAttribute("tilecount");
	mTileSet.columns = tilesetElement->IntAttribute("columns");

	auto imageElement = tilesetElement->FirstChildElement("image");
	string imageFile = filename.substr(0, filename.find_last_of("\\") + 1);
	imageFile.append(imageElement->Attribute("source"));
	mPictureSize.x = imageElement->IntAttribute("width");
	mPictureSize.y = imageElement->IntAttribute("height");

	mpTileTexture = ResourceManager::Instance().GetShaderResource(wstring(imageFile.begin(), imageFile.end()));

	Vector2 renderPos(0, 0);

	//mTiles.assign(mHeight, vector<int>(mWidth, 0));
	//mTilepos.assign(mHeight, vector<Vector2>(mWidth, renderPos));
	auto tile = mapElement->FirstChildElement("layer")->FirstChildElement("data")->FirstChildElement("tile");
	auto layerName = mapElement->FirstChildElement("layer");
	
	auto tilebyDrawId = tilesetElement->FirstChildElement("tile");
	auto test = tilebyDrawId->FirstChildElement("properties")->FirstChildElement("property")->IntAttribute("value");
	//->FirstChildElement("properties")
	//for (int y = 0; y < mHeight; ++y)
	//{
	//	renderPos.x = 0;
	//	for (int x = 0; x < mWidth; ++x)
	//	{
	//		mTiles[x][y] = tile->IntAttribute("gid") - 1;
	//		tile = tile->NextSiblingElement();
	//		mTileInfo[x][y] = renderPos;
	//		renderPos.x += mTileWidth;
	//		//mLayerName.push_back(layerName->Attribute("name"));
	//		//if(layerName->NextSiblingElement() != nullptr)
	//		//	layerName = layerName->NextSiblingElement();
	//	}
	//	renderPos.y += mTileHeight;
	//}
	
	
	for (int y = 0; y < mHeight; ++y)
	{
		renderPos.x = 0;
		for (int x = 0; x < mWidth; ++x)
		{
			mTiles.push_back(tile->IntAttribute("gid") - 1);
			tile = tile->NextSiblingElement();
			TILE tileinfo;
			tileinfo.vPos = renderPos;
			tileinfo.byDrawID = tilebyDrawId->FirstChildElement("properties")->FirstChildElement("property")->IntAttribute("value");
			tileinfo.byOption = tilebyDrawId->FirstChildElement("properties")->FirstChildElement("property")->IntAttribute("value");
			tilebyDrawId = tilebyDrawId->NextSiblingElement();
			tileinfo.underObject = 0;
			tileinfo.moveNum = 0;
			tileinfo.AttackNum = 0;
			//mTileInfo.push_back(tileinfo);
			mTileInfo.push_back(unique_ptr<TILE>(new TILE(tileinfo)));
			//mTileInfo[x][y] = renderPos;
			renderPos.x += mTileWidth;
			//mLayerName.push_back(layerName->Attribute("name"));
			//if(layerName->NextSiblingElement() != nullptr)
			//	layerName = layerName->NextSiblingElement();
		}
		renderPos.y += mTileHeight;
	}

	mScreenTileCountX = mScreenWidth / mTileWidth;
	mScreenTileCountY = mScreenHeight / mTileHeight;

	MActor::SetSortID(eSortID);
}

E_SCENE World::Update(float dt)
{
	offset = Vector2(0.0f, 0.0f);
	float speed = 200;

	auto state = Keyboard::Get().GetState();

	//타일이동
	if (state.W)
	{
		//offset.y = -speed * dt;
		//mCameraPosition.y = -speed * dt;
		ScrollMgr::Instance().SetAddScroll(0.0f, -speed*dt);
	}
	else if (state.S)
	{
		//offset.y = speed * dt;
		//mCameraPosition.y = speed * dt;
		ScrollMgr::Instance().SetAddScroll(0.0f, speed*dt);
	}

	if (state.A)
	{
		//offset.x = -speed * dt;
		//mPosition.x = -speed * dt;
		ScrollMgr::Instance().SetAddScroll(-speed*dt,0.0f);
	}
	else if (state.D)
	{
		//offset.x = speed * dt;
		//mPosition.x = speed * dt;
		ScrollMgr::Instance().SetAddScroll(speed*dt, 0.0f);
	}

	//타일의 계산으로 맵 위치를 제한
	//if ((mCameraPosition.x < 0))
	//{
	//	mCameraPosition = Vector2(0.0f, mCameraPosition.y);
	//}
	//else if (mCameraPosition.y < 0)
	//{
	//	mCameraPosition = Vector2(mCameraPosition.x, 0.0f);
	//}
	//else if (((mCameraPosition.y + mScreenTileCountY) * mTileHeight) > (mPictureSize.y))
	//{
	//	//mCameraPosition = Vector2(mCameraPosition.x, floor(mPictureSize.y / mTileHeight - mScreenTileCountY));
	//	mCameraPosition = Vector2(mCameraPosition.x, (mTilepos[mCameraPosition.x][(mHeight - 1)].y / mTileHeight - (mScreenTileCountY)+1));
	//}
	//else if (((mCameraPosition.x + mScreenTileCountX) * mTileWidth) > mPictureSize.x)
	//{
	//	//mCameraPosition = Vector2(mPictureSize.x / mTileWidth - mScreenTileCountX, mCameraPosition.y);
	//	mCameraPosition = Vector2((mTilepos[(mWidth - 1)][mCameraPosition.y].x / mTileWidth - (mScreenTileCountX)+1), mCameraPosition.y);
	//}
	//else
	//Vector2 offset2 = Vector2(ScrollMgr::Instance().GetScroll().x/48, ScrollMgr::Instance().GetScroll().y / 48);
	//	Scroll(offset2);
		//Scroll(offset.x, offset.y);
	
	//GetTIle 인덱스를 가져와서 사용하기
	//const vector<unique_ptr<TILE>>* vec = MActorManager::Instance().GetTileInfo();
	//TILE a;
	//a.vPos = (*vec)[0].get()->vPos;


	if (state.D2)
		meScene = E_SCENE_LOGO;
	else if (state.D3)
		meScene = E_SCENE_STAGE;
	else
		meScene = E_SCENE_NONPASS;
	
	if (state.D0)
	{
		visible = !visible;
	}
	return meScene;
}

void World::Render()
{

	//XMFLOAT2 renderPos(0, 0);

	//for (int y = 0; y < mHeight; ++y)
	//{
	//	renderPos.x = 0;
	//	for (int x = 0; x < mWidth; ++x)
	//	{
	//		RECT src = { 0,0,48,48 };

	//		int offX = (mTiles[x][y] % mTileSet.columns)*mTileSet.width;
	//		int offY = (mTiles[x][y] / mTileSet.columns)*mTileSet.height;

	//		OffsetRect(&src, offX, offY);

	//		mpBatch->Draw(mpTileTexture, renderPos, &src);
	//		renderPos.x += mTileWidth;
	//	}
	//	renderPos.y += mTileHeight;
	//}
	Vector2 renderPos(0, 0);

	RECT src;

	//for (int y = mCameraPosition.y; y < mCameraPosition.y + mScreenTileCountY; ++y)
	//{
	//	renderPos.x = 0;
	//	for (int x = mCameraPosition.x; x < mCameraPosition.x + mScreenTileCountX; ++x)
	//	{
	//		if (GetSourceRectFromMapIndex(x, y, src) == true)
	//		{
	//			mpBatch->Draw(mpTileTexture, renderPos, &src);
	//		}
	//		renderPos.x += mTileWidth;
	//	}
	//	renderPos.y += mTileHeight;
	//}
}

void World::Draw()
{
	float fScrollx = ScrollMgr::Instance().GetScroll().x;
	float fScrolly = ScrollMgr::Instance().GetScroll().y;

	RECT src;

	//타일 출력부분 화면만큼만 그린다
	//mCameraPosition = this->mPosition;
	//for (int y = mCameraPosition.y; y < mCameraPosition.y + mScreenTileCountY; ++y)
	//{
	//	//renderPos.x = 0;
	//	for (int x = mCameraPosition.x; x < mCameraPosition.x + mScreenTileCountX; ++x)
	//	{
	//		if (GetSourceRectFromMapIndex(x, y, src) == true)
	//		{
	//			mpBatch->Draw(mpTileTexture, mTilepos[x-(int)mCameraPosition.x][y - (int)mCameraPosition.y], &src);
	//		}
	//		//renderPos.x += mTileWidth;
	//	}
	//	//renderPos.y += mTileHeight;
	//}

	
	for (int y = 0; y < mScreenTileCountY + 1; ++y)
	{
		for (int x = 0; x < mScreenTileCountX + 1; ++x)
		{
			int index = (x + (int)fScrollx / mTileWidth) + (y + (int)fScrolly / mTileHeight) * mHeight;

			if (index >= mHeight*mWidth || index < 0)
				continue;

			if (GetSourceRectFromMapIndex(index, src) == true)
			{
				mPosition.x = mTileInfo[index]->vPos.x - fScrollx;
				mPosition.y = mTileInfo[index]->vPos.y - fScrolly;
				mpBatch->Draw(mpTileTexture, mWorldPos + mPosition , &src);
				if(visible)
				{
					wchar_t wch[64];
					//swprintf_s(wch, L"%d", index);
					swprintf_s(wch, L"%d", index);//mTileInfo[index]->underObject
					mpFont->DrawString(mpBatch, wch, XMFLOAT2(mPosition), DirectX::Colors::Black, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f));
					
					int x = mTileInfo[index]->vPos.x;
					int y = mTileInfo[index]->vPos.y;
					swprintf_s(wch, L"x=%d \n y=%d",x , y);
					mpFont->DrawString(mpBatch, wch, XMFLOAT2(mPosition + XMFLOAT2(-10.0f, 10.0f)), DirectX::Colors::Black, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.4f, 0.4f));
				}
			}
		}
	}
	//char buf[512];
	//sprintf(buf, "mCameraPosition.x = %f \n", mCameraPosition.x);
	////wsprintf(sre, L"%f", mspStage->GetmCameraPosition().x);
	//
	//OutputDebugStringA(buf);
	//sprintf(buf, "mCameraPosition.y = %f \n", mCameraPosition.y);
	//OutputDebugStringA(buf);

}
