// MonsterArean.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include "Team.h"

class MonsterArena
{
public:
	MonsterArena();

	void InitTest1Teams();
	void InitTest2Teams();
	void InitTest3Teams();
	void InitTest4Teams();
	void InitTest5Teams();
	void InitTest6Teams();
	void InitTestRandomTeams();

	void ClearTeams();
	void RunTurns();
	
private:
	Team m_Team1;
	Team m_Team2;
};
