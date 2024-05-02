#include "Framework.h"

MonsterMove::MonsterMove()
{
    SetMoveTime();
}

MonsterMove::MonsterMove(Transform* target, int key, float speed, int max_frameX)
{
    SetTarget(target);

    LoadClip(ToWString(PATH) + L"Monster/" + to_wstring(key) + L"/Move_Down.png", max_frameX, 1, true);
    LoadClip(ToWString(PATH) + L"Monster/" + to_wstring(key) + L"/Move_Up.png", max_frameX, 1, true);
    LoadClip(ToWString(PATH) + L"Monster/" + to_wstring(key) + L"/Move_Right.png", max_frameX, 1, true);
    LoadClip(ToWString(PATH) + L"Monster/" + to_wstring(key) + L"/Move_Right.png", max_frameX, 1, true);

    SetMoveTime();
    SetMoveSpeed(speed);
}

void MonsterMove::Update()
{
    CharacterAction::Update();

    playTime += DELTA;

    if (playTime > moveTime)
    {
        playTime -= moveTime; 
        RandomCompass();
        SetMoveTime();
    }

    Move();

    Monster* monster = (Monster*)target;
    if (monster->GetIsBoss())
    {
        if (!Audio::Get()->IsPlaySound("BossMove"))
            Audio::Get()->Play("BossMove");
    }


    target->UpdateWorld();
}

void MonsterMove::Move()
{
    velocity = { 0, 0 };

    Character* character = (Character*)target;

    switch (compass)
    {
    case CharacterAction::S:
        velocity.y = -1.0f;
        target->SetLocalScale(1, 1);
        break;
    case CharacterAction::N:
        velocity.y = 1.0f;
        target->SetLocalScale(1, 1);
        break;
    case CharacterAction::E:
        velocity.x = 1.0f;
        target->SetLocalScale(1, 1);
        break;
    case CharacterAction::W:
        velocity.x = -1.0f;
        target->SetLocalScale(-1, 1);
        break;
    default:
        break;
    }

    int speed = (character->GetStat().speed > MAX_SPEED) ? MAX_SPEED : character->GetStat().speed;
    target->Translate(velocity * speed * moveSpeed * DELTA);

    if (TileManager::Get()->PushGameObject(character))
        RandomCompass();

    if (TileManager::Get()->CheckMapPosPlayer(character))
        RandomCompass();

    BubbleManager::Get()->PushPlayer(character);

    //if (compass == W) velocity.x = 1.0f;
}

void MonsterMove::SetMoveTime()
{
    moveTime = Random(1.0f, 5.0f);
}

void MonsterMove::RandomCompass()
{
    velocity = { 0, 0 };

    int randomCompassType = Random(0, 2);

    if (randomCompassType) //사용자 방향
    {
        Monster* monster = (Monster*)target;
        Character* player = monster->GetPlayer();

        if (!player) return;

        Vector2 distance = monster->GetGlobalPosition() - player->GetGlobalPosition();

        if (distance.x < distance.y) //좌우
        {
            if (distance.x > 0)
                SetCompass(Compass::W);
            else
                SetCompass(Compass::E);
        }
        else //상하
        {
            if (distance.y > 0)
                SetCompass(Compass::S);
            else
                SetCompass(Compass::N);
        }
    }
    else //무작위 방향
    {
        int randomCompass = Random(0, 4);

        switch (randomCompass)
        {
        case CharacterAction::S:
            SetCompass(Compass::S);
            break;
        case CharacterAction::N:
            SetCompass(Compass::N);
            break;
        case CharacterAction::E:
            SetCompass(Compass::E);
            break;
        case CharacterAction::W:
            SetCompass(Compass::W);
            break;
        default:
            break;
        }
    }
}

void MonsterMove::End()
{
    
}
