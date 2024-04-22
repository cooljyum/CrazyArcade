#include "Framework.h"

TileManager::TileManager()
{
    SetTag("TileManager");

    CreateBGTile();

    Transform::Load(); 

    UpdateWorld();

    EffectManager::Get()->Add("RemoveObstacle", 50, L"ResourcesCA/Textures/Effect/RemoveEffect.png",
        Vector2(4, 1));
}

TileManager::~TileManager()
{
}

void TileManager::Render()
{
    RenderManager::Get()->Render("BGTile");
    RenderManager::Get()->Render("GameObject");
    RenderManager::Get()->Render("BGTileTxt");

    EffectManager::Get()->Render();
}

void TileManager::Update()
{
    FOR_X(SIZE_X)
        FOR_Y(SIZE_Y)
            bgTiles[x][y]->UpdateWorld();
        
    for (Tile* tile : objTiles)
    {
        tile->Update();
        tile->UpdateWorld();
    }
       

    UpdateWorld();

    EffectManager::Get()->Update();
}

void TileManager::CreateBGTile()
{
    wstring baseFile = L"ResourcesCA/Textures/Tiles/Basic/Tile (1).png";
    Texture* baseTile = Texture::Add(baseFile);
    tileSize = baseTile->GetSize();

    bgTiles.resize(SIZE_X);

    for (UINT y = 0; y < SIZE_X; y++) {
        bgTiles[y].resize(SIZE_Y);
    }

    Vector2 startPos = { 0, SIZE_X * 0.5f * tileSize.y * 0.5f };

    FOR_X(SIZE_X)
        FOR_Y(SIZE_Y)
        {
            wstring textureFile = baseFile;
            Vector2 xDirection = Vector2(tileSize.x, 0);
            Vector2 yDirection = Vector2(0, -tileSize.y);

            Vector2 pos = startPos + xDirection * x + yDirection * y;

            BasicTile* tile = new BasicTile(textureFile, pos);
            tile->SetParent(this);
            tile->SetCurIdx(Vector2{ (float)x,(float)y });
            bgTiles[x][y] = tile;

            RenderManager::Get()->Add("BGTile",tile);
            RenderManager::Get()->Add("BGTileTxt",tile);
        }
}

void TileManager::LoadMapData(string file)
{
    BinaryReader* reader = new BinaryReader(file);

    if (reader->IsFailed())
    {
        delete reader;
        return;
    }

    UINT SIZE_X = reader->UInt();
    UINT SIZE_Y = reader->UInt();

    FOR_Y(SIZE_Y)
        FOR_X(SIZE_X)
        bgTiles[x][y]->SetTexture(reader->WString());

    
    int size = reader->UInt();

    ClearObjTile();

    FOR(size)
    {
        wstring textureFile = reader->WString();
        Vector2 pos = reader->Vector();
        Vector2 curIdx = reader->Vector();

        AddObjTile(pos, tileSize, curIdx, textureFile);
    }

    delete reader;
}

void TileManager::ClearObjTile()
{
    for (ObstacleTile* tile : objTiles)
        delete tile;

    objTiles.clear();

}

void TileManager::AddObjTile(const Vector2& pos, const Vector2& size, const Vector2 idx, const wstring textureFile)
{
    ObstacleTile* tile = new ObstacleTile(textureFile, pos, true);
    tile->SetParent(this);
    tile->Translate(Vector2::Up() * (tile->GetSize().y - size.y) * 0.5);
    tile->Update();
    tile->SetCurIdx(idx);
    
    BasicTile* bgTile = (BasicTile*)bgTiles[idx.x][idx.y];
    bgTile->SetObstacleTile(tile);
    bgTile->SetType(Tile::OBSTACLE);


    objTiles.push_back(tile);
    tile->UpdateWorld();

    RenderManager::Get()->Add("GameObject", tile);
}

void TileManager::PopObjTile()
{
    for (Tile* objTile : objTiles)
    {
        Tile::Type tiletype = TileManager::Get()->GetIdxBgTileType(objTile->GetCurIdx());
        if (tiletype == Tile::ATTACK)
        {
            ObstacleTile* tile = (ObstacleTile*)objTile;
            tile->End();
        }
    }
}


Tile* TileManager::GetNearPosTileState(GameObject* target, Tile::Type type)
{
    float minDistance = Distance(bgTiles[0][0]->GetGlobalPosition(), target->GetGlobalPosition());
    Tile* minDistanceTile = nullptr;

    for (UINT y = 0; y < SIZE_Y; y++)
    {
        for (UINT x = 0; x < SIZE_X; x++)
        {
            Tile* tile = bgTiles[x][y];

            float distance = Distance(tile->GetGlobalPosition(), target->GetGlobalPosition());

            if (distance < minDistance)
            {
                minDistance = distance;
                minDistanceTile = tile;
            }
        }
    }

    if(minDistanceTile)
        minDistanceTile->SetType(type);

    RenderManager::Get()->Add("GameObject", target);
        

    return minDistanceTile;
}

Tile* TileManager::GetNearPosTileState(Vector2 pos)
{
    Vector2 firstTilePos = bgTiles[0][0]->GetGlobalPosition();

    Vector2 calPos = { pos.x - firstTilePos.x,firstTilePos.y-pos.y };
     calPos /=  Tile::TILE_SIZE;
     calPos= { round(calPos.x), round(calPos.y) };

    return bgTiles[calPos.x][calPos.y];
}

void TileManager::PushPlayer(Character* player)
{
    for (Tile* objTile : objTiles)
    {
        Vector2 overlab;

        if (objTile->GetCollider()->IsCollision(player->GetCollider(), &overlab))
        {
            if (overlab.x < overlab.y) 
            {
                if (player->GetGlobalPosition().x > objTile->GetGlobalPosition().x)
                {
                    player->Translate(Vector2::Right()*overlab.x);
                }
                else 
                {
                    player->Translate(Vector2::Left() * overlab.x);
                }
            }
            else 
            {
                if (player->GetGlobalPosition().y > objTile->GetGlobalPosition().y)
                {
                    player->Translate(Vector2::Up() * overlab.y);
                }
                else
                {
                    player->Translate(Vector2::Down() * overlab.y);
                }
            }

            player->UpdateWorld();
        }
    }
}

void TileManager::AttackPlayer(Character* player)
{

}
