// MonsterArean.cpp : Defines the entry point for the application.

#include "MonsterArena.h"

#include <chrono>
#include <thread>

int main()
{
	MonsterArena Arena{};
	
	Arena.InitTest1Teams();
	Arena.RunTurns();
	Arena.ClearTeams();
	
	Arena.InitTest2Teams();
	Arena.RunTurns();
	Arena.ClearTeams();
	
	Arena.InitTest3Teams();
	Arena.RunTurns();
	Arena.ClearTeams();

	Arena.InitTest4Teams();
	Arena.RunTurns();
	Arena.ClearTeams();
    
	Arena.InitTest5Teams();
	Arena.RunTurns();
	Arena.ClearTeams();

	Arena.InitTest6Teams();
	Arena.RunTurns();
	Arena.ClearTeams();
	
	Arena.InitTestRandomTeams();
	Arena.RunTurns();
	Arena.ClearTeams();
	
	return 0;
}

MonsterArena::MonsterArena(){}

void MonsterArena::InitTest1Teams()
{
	m_Team1.AddEntity(MonsterType::Goblin,"Red");
	m_Team2.AddEntity(MonsterType::Troll,"Blue");
}

void MonsterArena::InitTest2Teams()
{
	m_Team1.AddEntity(MonsterType::Goblin,"Red");
	m_Team2.AddEntity(MonsterType::Troll,"Blue");
	m_Team2.AddEntity(MonsterType::Troll,"Blue");
}

void MonsterArena::InitTest3Teams()
{
	m_Team1.AddEntity(MonsterType::Troll,"Red");
	m_Team2.AddEntity(MonsterType::Orc,"Blue");
}

void MonsterArena::InitTest4Teams()
{
	m_Team1.AddEntity(MonsterType::Troll,"Red");
	m_Team2.AddEntity(MonsterType::Orc,"Blue");
	m_Team2.AddEntity(MonsterType::Orc,"Blue");
}

void MonsterArena::InitTest5Teams()
{
	m_Team1.AddEntity(MonsterType::Orc,"Red");
	m_Team2.AddEntity(MonsterType::Goblin,"Blue");
}

void MonsterArena::InitTest6Teams()
{
	m_Team1.AddEntity(MonsterType::Orc,"Red");
	m_Team2.AddEntity(MonsterType::Goblin,"Blue");
	m_Team2.AddEntity(MonsterType::Goblin,"Blue");
}
void MonsterArena::ClearTeams()
{
	m_Team1.ClearEntity();
	m_Team2.ClearEntity();
}

float RndFloat()
{
	return (float)(std::rand()) / (float)(RAND_MAX + 1); // [0, 1)
}

int RndIntInRange(int minInclusive, int maxInclusive)
{
	return (int)((float)(minInclusive) + RndFloat() * (float)(maxInclusive + 1 - minInclusive));
}

void MonsterArena::InitTestRandomTeams()
{
	for (int i = 0; i < 4; i++)
	{
		{
			MonsterType* RND_TYPE = MonsterType::GetTypeById(RndIntInRange(0, MONSTER_TYPE_MAX_ID));
			m_Team1.AddEntity(*RND_TYPE,"Red");
		}
		{
			MonsterType* RND_TYPE = MonsterType::GetTypeById(RndIntInRange(0, MONSTER_TYPE_MAX_ID));
			m_Team2.AddEntity(*RND_TYPE,"Blue");
		}
	}
}
void MonsterArena::PrintInfo()
{
	std::cout<<"[ Red | ";
	for (size_t i = m_Team1.m_Entities.size(); i-->0;)
	{
		auto& monster = m_Team1.m_Entities[i];
		std::cout << monster->GetName()<<" ("<<monster->GetHP()<<") ";
	}
	std::cout<<"] VS [ ";
	for (size_t i = 0; i < m_Team2.m_Entities.size(); i++)
	{
		auto& monster = m_Team2.m_Entities[i];
		std::cout << monster->GetName()<<" ("<<monster->GetHP()<<") ";
	}
	std::cout<<"| Blue ] \n";
	std::cout <<"---------------------\n";
}

void MonsterArena::RunTurns()
{
	std::cout <<"Battle Start \n";
	PrintInfo();
	auto Leader1 = m_Team1.GetLead();
	auto Leader2 = m_Team2.GetLead();
	int turn=1;
	while (true)
	{
		auto Lead1 = m_Team1.GetLead();
		auto Lead2 = m_Team2.GetLead();
		if (!Lead1)
		{
			std::cout << "Team Blue Win!.\n";
			break;
		}

		if (!Lead2)
		{
			std::cout << "Team Red Win!.\n";
			break;
		}
		std::cout<<"Turn "<<turn<<"\n";
		PrintInfo();
		Lead1->Attack(*Lead2);
		Lead1->EndOfTurnEffect();
		Lead2->Attack(*Lead1);
		Lead2->EndOfTurnEffect();
		if (!Lead1->IsAlive() || !Lead2->IsAlive())
		{
			if (!Lead1->IsAlive())
			{
				m_Team1.RemoveOnDead(Lead1);
			}
			if (!Lead2->IsAlive())
			{
				m_Team2.RemoveOnDead(Lead2);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			continue;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		turn++;
		PrintInfo();
	}
	std::cout << "GameFinished.\n";
	PrintInfo();
}

