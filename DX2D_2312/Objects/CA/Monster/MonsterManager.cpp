#include "Framework.h"

MonsterManager::MonsterManager()
{
	boss = new Boss(101, 30.0f, true, 10);
	RenderManager::Get()->Add("GameObject", boss);
	boss->Spawn(CENTER);

}

MonsterManager::~MonsterManager()
{
}

void MonsterManager::Update()
{
	for (const auto& pair : totalObject) 
	{
		const vector<GameObject*>& gameObjects = pair.second;
		for (GameObject* object : gameObjects) 
		{
			if(object)
				object->Update();
		}
	}
	boss->Update();
}

void MonsterManager::Render()
{
	boss->PostRender();
}

void MonsterManager::LoadBossMonster()
{
	totalObject["SpawnBossMonster"].resize(10);


	for (GameObject*& monsterObj : totalObject["SpawnBossMonster"])
	{
		int key = Random(0,4);
		Vector2 curIdx = Random({ 0,0 }, {(float)TileManager::Get()->SIZE_X ,(float)TileManager::Get()->SIZE_Y });
		Vector2 pos = TileManager::Get()->GetBgTile(curIdx)->GetGlobalPosition();

		MonsterData monsterData;
		monsterData = DataManager::Get()->GetMonsterData(key);

		monsterObj = new Monster(monsterData.key, monsterData.speed, monsterData.isBubble, monsterData.hp);

		Monster* monster = (Monster*)monsterObj;
		monster->Spawn(pos);

		RenderManager::Get()->Add("GameObject", monster);
	}
}

void MonsterManager::Spawn(const Vector2& pos)
{

	for (GameObject*& monsterObj : totalObject["SpawnBossMonster"])
	{
		if (!monsterObj->IsActive())
			monsterObj->SetActive(true);
	}
}

void MonsterManager::LoadMonster()
{
	BinaryReader* reader = new BinaryReader(PATH + TileManager::mapNameStr + ".map");

	if (reader->IsFailed())
	{
		delete reader;
		return;
	}

	int size = reader->UInt();

	totalObject["LoadMonster"].resize(size);

	for (GameObject*& monsterObj : totalObject["LoadMonster"])
	{
		int key = reader->Int();
		Vector2 curIdx = reader->Vector();
		Vector2 pos = TileManager::Get()->GetBgTile(curIdx)->GetGlobalPosition();

		MonsterData monsterData;
		monsterData = DataManager::Get()->GetMonsterData(key);

		monsterObj = new Monster(monsterData.key, monsterData.speed, monsterData.isBubble, monsterData.hp);

		Monster* monster = (Monster*)monsterObj;
		monster->Spawn(pos);

		RenderManager::Get()->Add("GameObject", monster);
	}

	delete reader;
}

void MonsterManager::ClearMonster()
{
	for (auto& pair : totalObject)
	{
		vector<GameObject*>& gameObjects = pair.second;
		for (GameObject*& object : gameObjects)
		{
			RenderManager::Get()->Remove("GameObject", object);
			delete object;
		}
	}
}
