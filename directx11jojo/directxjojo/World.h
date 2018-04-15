#pragma once

class World
	: public MActor
{
public:
	World(SpriteBatch *pBatch, SpriteFont * pFont,int screenWidth,int screenHeight);
	virtual ~World();

private:
	SpriteBatch*	mpBatch;
	int			mScreenWidth;
	int			mScreenHeight;
	XMFLOAT2 mPictureSize;
	//int			mPictureWidth;
	//int			mPictureHeight;
	int			mWidth;
	int			mHeight;
	int			mTileWidth;
	int			mTileHeight;
	bool			visible;
	TileSet		mTileSet;
	ID3D11ShaderResourceView*	mpTileTexture;
	//vector<vector<int>>			mTiles;
	vector<unique_ptr<TILE>>			mTileInfo;
	vector<int>				mTiles;
	//vector<vector<Vector2>>			mTileInfo;
	vector<string> mLayerName;
private:
	Vector2			mCameraPosition;
	Vector2			offset;
	int				mScreenTileCountX;
	int				mScreenTileCountY;
public:
	void Init(string filename, E_SORTID eSortID);
	E_SCENE Update(float dt);
	void Render();
	void Draw();
	//bool inline GetSourceRectFromMapIndex(int x, int y, RECT& result)
	//{
	//	if (x < 0 || x >= mWidth || y < 0 || y >= mHeight)
	//		return false;
	//	float index;
	//	index = x + y * mHeight;

	//	int gid = mTiles[index];
	//	result = { 0, 0, 48, 48 };

	//	int offX = (gid % mTileSet.columns) * mTileSet.width;
	//	int offY = (gid / mTileSet.columns) * mTileSet.height;

	//	OffsetRect(&result, offX, offY);

	//	return true;
	//}
	bool inline GetSourceRectFromMapIndex(int x, RECT& result)
	{
		if (x < 0 || x >= mWidth * mHeight)
			return false;

		int gid = mTiles[x];
		result = { 0, 0, 48, 48 };

		int offX = (gid % mTileSet.columns) * mTileSet.width;
		int offY = (gid / mTileSet.columns) * mTileSet.height;

		OffsetRect(&result, offX, offY);

		return true;
	}
	void inline Scroll(float offX, float offY)
	{ 
		//mCameraPosition.x += offX; mCameraPosition.y = offY; 
		mPosition.x = offX;
		mPosition.y = offY;
	}
	void inline Scroll(const Vector2& offset) { mCameraPosition += offset; }

	void SetmCameraPosition(float offX, float offY) { mCameraPosition.x = offX; mCameraPosition.y = offY; }
	Vector2 GetmCameraPosition() { return mCameraPosition; }
	int GetmTileCountY() { return mScreenTileCountY; }
	int GetmTileCountX() { return mScreenTileCountX; }
	int GetmTileWidth() { return mTileWidth; }
	int GetmTileHeight() { return mTileHeight; }

	XMFLOAT2 GetWorldSize() { return mPictureSize; }
	vector<unique_ptr<TILE>>* GetvecTileInfo() { return &mTileInfo; }
};

