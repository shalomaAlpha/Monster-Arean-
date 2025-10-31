#pragma once
#include "MonsterType.h"
#include <iostream>
#include <string>

class Monster
{
public:
    Monster(const MonsterType& Type, const std::string TeamName);
    virtual ~Monster() = default;

public:
    void BeginPlay();

    bool IsAlive();
    int GetHP();
    std::string GetName() const;
    
    virtual void Attack(Monster& target);
    virtual void EndOfTurnEffect();
    virtual void TakeDamage(int amount, std::string source_type, Monster* attacker =nullptr);
    std::string GetTeamName() const { return m_TeamName; }
    
public:
    const MonsterType m_Type;
    const int m_MaxHP;
    int m_HP;
    std::string m_TeamName;
    
};

//Children Classes of Monster
class Goblin :public Monster
{
public:
    Goblin(const MonsterType& Type, int NumAttacks, std::string TeamName);
    void Attack(Monster& target) override;

private:
    int m_NumAttacks;
};

class Troll : public Monster
{
public:
    Troll(const MonsterType& Type, int RegenAmount, std::string TeamName);
    void EndOfTurnEffect() override;

private:
    int m_Regen;
};

class Orc : public Monster
{
public:
    Orc(const MonsterType& Type, int BlockAmount, int ReflectAmount, std::string TeamName);
    void TakeDamage(int amount,std::string Source_type, Monster* attacker) override;

private:
    int m_Block;
    int m_Reflect;
};