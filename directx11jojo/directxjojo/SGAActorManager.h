#pragma once
class SGAActorManager final
{
public:
	// ������ǥ��
	/*static SGAActorManager* INstance2()
	{
		static SGAActorManager instance;
		return &instance;
	}*/
	//���۷��� ǥ��
	static SGAActorManager& Instance()
	{
		static SGAActorManager instance;
		return instance;
	}
private:
	SGAActorManager();
	SGAActorManager(SGAActorManager const&);
	void operator = (SGAActorManager const&);
public:
	~SGAActorManager();
private:
	E_SCENE meScene;
private:
	int mClickCount;
public:
	E_SCENE Update(float dt);
	void CheckCollidion();
	void CheckAction();
	void RePosAndVisi();
	void UIRePosAndVisi();
	void AtRePosAndVisi();

	void Draw();
	void Release();
	void SortActors();
	
	E_SCENE GetScene();
	bool GetMBSeekScope();
	void SetMBVisible(bool visible);
	vector<unique_ptr<TILE>>* GetTileInfo();
	vector<unique_ptr<int>>* GetCharacterPosIndex();
	void SetUIVisible(bool visible);
	void SetClickCount(int count) { mClickCount = count; }
	void SetAtVisible(bool visible);
	//void InsertMap(string str, unique_ptr<SGAActor> actor);
public:
	//���ø��� ����ȭ ��� ���ڸ� �޾Ƶ� ��밡���ϵ���
	template<class T,class...Types> inline
		T* Create(Types&&...args)
	{
		mActors.push_back(
			unique_ptr<T>(						//����Ʈ�����ͷ� ����� ���Ϳ� �������
				new T(forward<Types>(args)...)	//�����ڿ� ���� �Ķ���Ͱ�
				)
		);
		return dynamic_cast<T*>(mActors.back().get());
	}

private:
	//map<string, unique_ptr <SGAActor>> mMapActors;
	list<unique_ptr <SGAActor>> mActors;
	//vector<TILE*> mVectile;
};