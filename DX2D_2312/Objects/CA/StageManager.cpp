#include "Framework.h"

StageManager::StageManager()
{
}

void StageManager::SetStageData()
{
	stageData = DataManager::Get()->GetStageData(WaitRoomUI::stageKey);
	userHp = PLAYER_SPAWN_HP;
}

void StageManager::SetPlayer(Character* playerCharacter)
{
	this->playerCharacter = playerCharacter; 
}

void StageManager::LoadStage()
{
	SetStageData();
	countStage = 0;

	if (stageData.stage1 != "")
	{
		maxStage++;
	}
	if (stageData.stage2 != "")
	{
		maxStage++;
	}
	if (stageData.stage3 != "")
	{
		maxStage++;
	}
	stageMapNames.resize(maxStage);

	stageMapNames[0] = stageData.stage1;
	stageMapNames[1] = stageData.stage2;
	stageMapNames[2] = stageData.stage3;

	SpawnPlayer();

	NextStage();
}

void StageManager::NextStage()
{
	if (countStage >= maxStage)
	{
		SCENE->ChangeScene("WaitRoom");
		return;
	}

	if (countStage <= 0)
	{
		if (!Audio::Get()->IsPlaySound("GameStart"))
			Audio::Get()->Play("GameStart");
	}
	else 
	{
		if (!Audio::Get()->IsPlaySound("NextLevelReady"))
			Audio::Get()->Play("NextLevelReady");
	}
		
	TileManager::Get()->SetMap(stageMapNames[countStage]);
	TileManager::Get()->LoadMapData();
	TileManager::Get()->LoadMapSize();

	MonsterManager::Get()->LoadMonster();

	Start();

	countStage++;
}

void StageManager::SpawnPlayer()
{
	playerCharacter->SetInit();
	Player* player = (Player*)playerCharacter;
	player->LoadPos();
}

void StageManager::SetGameUIState(GameUI::State state)
{
	UI* curUI = UIManager::Get()->GetUI("Game");
	gameUI = (GameUI*)curUI;
	gameUI->SetGameTxt(state);
}

void StageManager::Start()
{
	SetGameUIState(GameUI::State::START);
}

void StageManager::Clear()
{
	if (!Audio::Get()->IsPlaySound("Clear"))
		Audio::Get()->Play("Clear");

	SetGameUIState(GameUI::State::CLEAR);
}

void StageManager::Gameover()
{
	if (userHp > 0)
		SpawnPlayer();
	else
	{
		if (!Audio::Get()->IsPlaySound("Lose"))
			Audio::Get()->Play("Lose");

		SetGameUIState(GameUI::State::GAMEOVER);
	}

	userHp--;
}

void StageManager::End()
{
	//if (!Audio::Get()->IsPlaySound(""))
	//	Audio::Get()->Play("");

	SCENE->ChangeScene("WaitRoom");
}
