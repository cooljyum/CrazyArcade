#pragma once

class Character : public GameObject
{
public:
    enum State
    {
        IDLE, MOVE, //,INCARCERATION, LIVE, DIE
    };

public:
    Character();
    ~Character();

    void Update();
    void Render(); 
    void PostRender();

    RectCollider* GetCollider() { return collider; }

private:
    void Move();
    void Jump();
    void Attack();

    void Landing();

    void SetAction(int state);

    void CreateActions();

    void AddAction(string file, int frameX, int frameY);

private:

    RectCollider* collider;

    map<State, Action*> actions;

    State curState = IDLE;
};