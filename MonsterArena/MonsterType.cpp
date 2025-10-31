#include "MonsterType.h"
#include "MonsterType.h"

MonsterType MonsterType::Goblin(0, 35, 20);
MonsterType MonsterType::Troll(1, 100, 15);
MonsterType MonsterType::Orc(2, 70, 10);

MonsterType* MonsterType::GetTypeById(int TypeId)
{
    if (TypeId == MonsterType::Goblin.m_TypeId) return &MonsterType::Goblin;
    if (TypeId == MonsterType::Troll.m_TypeId) return &MonsterType::Troll;
    if (TypeId == MonsterType::Orc.m_TypeId) return &MonsterType::Orc;
    return nullptr;
}

MonsterType::MonsterType(int TypeId, int InitialHP, int BaseDamage)
    : m_TypeId(TypeId)
    , m_InitialHP(InitialHP)
    , m_BaseDamage(BaseDamage)
{
}

MonsterType::MonsterType(const MonsterType& Other)
    : m_TypeId(Other.m_TypeId)
    , m_InitialHP(Other.m_InitialHP)
    , m_BaseDamage(Other.m_BaseDamage)
{
}
