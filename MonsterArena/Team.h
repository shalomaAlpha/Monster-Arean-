#pragma once
#include <vector>
#include "Monster.h"
#include <string>

class Team
{
public:
    std::shared_ptr<Monster> GetLead();
    void AddEntity(MonsterType& Type, std::string TeamName);
    void ClearEntity(){m_Entities.clear();};
    bool RemoveOnDead(std::shared_ptr<Monster> Monster);

public:
    std::vector<std::shared_ptr<Monster>> m_Entities;
};
