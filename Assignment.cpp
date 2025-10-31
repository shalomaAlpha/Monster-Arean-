#include <chrono>
#include <thread>
#include <string>
#include <iostream>
#include <vector>
constexpr int MONSTER_TYPE_MAX_ID = 2;

//Region MonsterTypes:
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
{}
MonsterType::MonsterType(const MonsterType& Other)
    : m_TypeId(Other.m_TypeId)
    , m_InitialHP(Other.m_InitialHP)
    , m_BaseDamage(Other.m_BaseDamage)
{}
//Endregion MonsterTypes

//Region Monster
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
{}

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
//Endregion Monster

//Region Team
class Team
{
public:
    std::shared_ptr<Monster> GetLead();
    void AddEntity(MonsterType& Type, std::string TeamName);
    void ClearEntity(){m_Entities.clear();}
    bool RemoveOnDead(std::shared_ptr<Monster> Monster);

public:
    std::vector<std::shared_ptr<Monster>> m_Entities;
};
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
//EndRegion Team

//Region MonsterArena
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
	void PrintInfo();
	
private:
    Team m_Team1;
    Team m_Team2;
};
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
//Endregion MonsterArena

int main(int argc, char* argv[])
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
