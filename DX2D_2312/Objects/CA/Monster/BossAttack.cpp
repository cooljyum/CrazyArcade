#include "Framework.h"

BossAttack::BossAttack(Transform* target, int key, int max_frameX)
{
	SetTarget(target);

	LoadClip(ToWString(PATH) + L"Monster/" + to_wstring(key) + L"/Attack.png", max_frameX, 1, false, 1.7f);

	EffectManager::Get()->Add("MonsterSpawn", 50, L"MonsterSpawn.png",
		Vector2(4, 1));
}

void BossAttack::Start()
{
	CharacterAction::Start();
}

void BossAttack::End()
{
	Boss* boss = (Boss*)target;
	boss->SetAction(boss->curType);

	Vector2 curIdx = Random({ 0,0 }, { (float)TileManager::Get()->SIZE_X ,(float)TileManager::Get()->SIZE_Y });
	Vector2 pos = TileManager::Get()->GetBgTile(curIdx)->GetGlobalPosition();

	MonsterManager::Get()->Spawn(pos);
	EffectManager::Get()->Play("MonsterSpawn", pos);
}

