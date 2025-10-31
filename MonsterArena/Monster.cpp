#include "Monster.h"
#include <iostream>

Monster::Monster(const MonsterType& Type, std::string TeamName)
    : m_Type(Type)
    , m_MaxHP(Type.m_InitialHP)
    , m_HP(Type.m_InitialHP)
    , m_TeamName(TeamName)
{
}

void Monster::BeginPlay()
{
    m_HP = m_MaxHP;
}

bool Monster::IsAlive()
{
    return m_HP>0;
}

int Monster::GetHP()
{
    return m_HP;
}

std::string Monster::GetName() const
{
    switch (m_Type.m_TypeId)
    {
    case 0: return "Goblin";
    case 1: return "Troll";
    case 2: return "Orc";
    default: return "Unknown";
    }
}

void Monster::TakeDamage(int amount,std::string sourceType, Monster* attacker)
{
    m_HP -= amount;
    if (m_HP < 0) m_HP = 0;
}

void Monster::Attack(Monster& target)
{
    std::cout << GetTeamName() << " ";
    std::cout << GetName() << " attacks " << target.GetName()
              << " for " << m_Type.m_BaseDamage << " damage.\n";
    target.TakeDamage(m_Type.m_BaseDamage, "direct",this);
}

void Monster::EndOfTurnEffect()
{
    // Default: no effect
}

//Goblin
Goblin::Goblin(const MonsterType& Type, int NumAttacks, std::string TeamName)
    : Monster(Type, TeamName), m_NumAttacks(NumAttacks)
{
}

void Goblin::Attack(Monster& target)
{
    for (int i = 0; i < m_NumAttacks; ++i)
    {
        target.TakeDamage(m_Type.m_BaseDamage,"direct",this);
        std::cout << GetTeamName() << " ";
        std::cout << GetName() << " attacks " << target.GetName()
                  << " for " << m_Type.m_BaseDamage << " damage.\n";
    }
}

//Troll
Troll::Troll(const MonsterType& Type, int RegenAmount, std::string TeamName)
    : Monster(Type, TeamName), m_Regen(RegenAmount)
{
}

void Troll::EndOfTurnEffect()
{
    int before = m_HP;
    if (before<=0)
    {
        std::cout <<GetTeamName()<<" "
              << GetName() << " dead, can't regenerate health.\n";
        return;
    }
    m_HP += m_Regen;
    if (m_HP > m_MaxHP) m_HP = m_MaxHP;
    std::cout <<GetTeamName()<<" "
              << GetName() << " regenerates " << (m_HP - before) << " health.\n";
}

//Orc
Orc::Orc(const MonsterType& Type, int BlockAmount, int ReflectAmount, std::string TeamName)
    : Monster(Type,TeamName), m_Block(BlockAmount), m_Reflect(ReflectAmount)
{
}

void Orc::TakeDamage(int amount, std::string source_type, Monster* attacker)
{
    int reducedDamage=amount -m_Block;
    if (reducedDamage<0) reducedDamage = 0;

    m_HP -= reducedDamage;
    if (m_HP < 0) m_HP = 0;

    if (attacker)
    {
        // we check source type first to avoid recursive calls
        if (source_type == "direct")
        {
            attacker->TakeDamage(m_Reflect, "",this);
        }
        std::cout << GetTeamName() << " " << GetName()
                  << " blocks " << m_Block
                  << " damage and reflects " << m_Reflect
                  << " damage back to " << attacker->GetName() << "!\n";
    }
}



