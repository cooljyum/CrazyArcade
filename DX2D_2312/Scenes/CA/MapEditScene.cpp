#include "Framework.h"
#include "MapEditScene.h"

MapEditScene::MapEditScene()
{
    CAM->SetFix(false);

    tileMap = new EditTileMap();
}

MapEditScene::~MapEditScene()
{
    delete tileMap;
}

void MapEditScene::Update()
{
    tileMap->Update();
}

void MapEditScene::Render()
{
    tileMap->Render();
}

void MapEditScene::PostRender()
{
    if (GameManager::isGUI) 
    {
        tileMap->RenderSampleButtons();
    }
}

void MapEditScene::Start()
{
    GameManager::isGUI = true;
}
