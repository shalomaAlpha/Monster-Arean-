#pragma once

constexpr int MONSTER_TYPE_MAX_ID = 2;

class MonsterType
{
public:
    static MonsterType Goblin;
    static MonsterType Troll;
    static MonsterType Orc;

    static MonsterType* GetTypeById(int TypeId);

public:
    MonsterType(int TypeId, int InitialHP, int BaseDamage);
    MonsterType(const MonsterType& Other);

public:
    const int m_TypeId;
    const int m_InitialHP;
    const int m_BaseDamage;
};
