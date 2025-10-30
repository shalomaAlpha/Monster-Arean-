#include "Team.h"

void Team::AddEntity(MonsterType& type, std::string TeamName)
{
    std::shared_ptr<Monster> newMonster = nullptr;
    switch (type.m_TypeId)
    {
    case 0: // Goblin
        newMonster = std::make_shared<Goblin>(type, 3, TeamName); 
        break;
    case 1: // Troll
        newMonster = std::make_shared<Troll>(type, 10,TeamName); 
        break;
    case 2: // Orc
        newMonster = std::make_shared<Orc>(type, 10, 5,TeamName); 
        break;
    default:
        std::cerr << "Unknown monster type!\n";
        return;
    }
    m_Entities.emplace_back(newMonster);
    //m_Entities.emplace_back(std::make_shared<Monster>(type));
}

std::shared_ptr<Monster> Team::GetLead()
{
    return m_Entities.empty() ? nullptr : m_Entities[0];
}

bool Team::RemoveOnDead(std::shared_ptr<Monster> Monster)
{
    for (auto it = m_Entities.begin(); it != m_Entities.end(); it++)
    {
        if (*it == Monster)
        {
            m_Entities.erase(it);
            std::cout<<Monster->GetTeamName()<<" "<<Monster->GetName()<<" has died \n";
            return true;
        }
    }
    return false;
}
