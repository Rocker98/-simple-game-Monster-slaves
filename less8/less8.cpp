// less8.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

int getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	// равномерно распределяем генерацию числа в диапазоне
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

class Creature
{
	string m_name;
	char m_char;
	int m_Health;
	int m_damage;
	int m_gold;
public:
	Creature(string name="", char symb='@', int HP=0, int damage=0, int gold=0)
		:m_name(name), m_char(symb), m_Health(HP), m_damage(damage), m_gold(gold)
	{

	}

	string getName() { return m_name; }
	char getSymbol() { return m_char; }
	int getHealth() { return m_Health; }
	int getDamage() { return m_damage; }
	int getGold() { return m_gold; }
	void setDamage(int dam) { m_damage = dam; }

	void reduceHP(int dmg)
	{
		m_Health -= dmg;
	}
	bool isDead()
	{
		return m_Health <= 0;
	}
	void addGold(int gold)
	{
		m_gold += gold;
	}

};

class Player :public Creature
{
	int m_lvl = 1;
public:
	Player(string name = "", char symb = '@', int HP = 10, int damage = 1, int gold = 0):Creature(name,symb,HP,damage,gold)
	{

	}
	void levelUp()
	{
		m_lvl++;
		setDamage(m_lvl);
		cout << "Now you are " << m_lvl <<" level"<< endl;
	}
	int getLvl() { return m_lvl; }
	bool hasWon() { return m_lvl >= 20; }

	bool fled()
	{
		return getRandomNumber(0, 1);
	}

	void attackPlayer(int power)
	{
		reduceHP(power);
	}
};

class Monster:public Creature
{
public:
	enum Type
	{
		DRAGON,
		ORC,
		SLIME,
		MAX_TIPES
	};

	

	typedef struct 
	{
		string name;
		char symbol;
		int health;
		int damage;
		int gold;
	}MonsterData;

	static const MonsterData data[MAX_TIPES];

	static Monster getRandomMonster()
	{
		return Monster(static_cast<Type>(getRandomNumber(0, MAX_TIPES - 1)));
	}

	void attackMonster(int power)
	{
		reduceHP(power);
	}

	Monster(Type type) :
		Creature(data[type].name, data[type].symbol, data[type].health, data[type].damage, data[type].gold)
	{

	}
	
};

const Monster::MonsterData Monster::data[Monster::MAX_TIPES]
{
	{"dragon", 'D', 20, 4, 100},
	{ "orc",'o',4,2,25 },
	{ "slime",'s',1,1,10 }
};

void fightMonster(Player& player, Monster& monster)
{
	cout << "You have encountered a " << monster.getName()<<' ' << '(' << monster.getSymbol() << ')' << endl;
	while (1)
	{
		
		char choice;
		do
		{
			cout << "(R)un or (F)ight?";
			cin.ignore();
			cin >> choice;
		} while (choice != 'R'&&choice != 'r'&&choice != 'F'&&choice != 'f');
		if (choice == 'R' || choice == 'r')
		{
			if (player.fled())
			{
				cout << "You sucssesfully fled!" << endl;
				return;
			}
			else
			{
				player.attackPlayer(monster.getDamage());
				cout << monster.getName() << " hit you by " << monster.getDamage() << endl;
				if(!player.isDead())
				cout << "Now you have " << player.getHealth() << "HP" << endl;
				if (player.isDead())
				{
					cout << "You died with level " << player.getLvl() << " with " << player.getGold() << " gold." << endl;
					return;
				}
				monster.attackMonster(player.getDamage());
				cout << "You hit " << monster.getName() << "by " << player.getDamage() << endl;
				if (!monster.isDead())
				cout << monster.getName()<<" have " << monster.getHealth() << "HP" << endl;
				if (monster.isDead())
				{
					cout << "You killed the " << monster.getName() << " and found " << monster.getGold() << " gold!" << endl;
					player.addGold(monster.getGold());
					player.levelUp();
					return;
				}
			}
		}
		else
		{
			monster.attackMonster(player.getDamage());
			cout << "You hit " << monster.getName() << "by " << player.getDamage() << endl;
			if(!monster.isDead())
			cout << monster.getName() << " have " << monster.getHealth() << "HP" << endl;
			if (monster.isDead())
			{
				cout << "You killed the " << monster.getName() << " and found " << monster.getGold() << " gold!" << endl;
				player.addGold(monster.getGold());
				player.levelUp();
				return;
			}
			player.attackPlayer(monster.getDamage());
			cout << monster.getName() << " hit you by " << monster.getDamage() << endl;
			if (!player.isDead())
			cout << "Now you have " << player.getHealth() << "HP" << endl;
			if (player.isDead())
			{
				cout << "You died with level " << player.getLvl() << " with " << player.getGold() << " gold." << endl;
				return;
			}
		}
	}
}

int main()
{
	srand(static_cast<unsigned int>(time(0))); // устанавливаем значение системных часов в качестве стартового числа
	rand(); // сбрасываем первый результат

	string userName;
	cout << "Enter your name :";
	cin >> userName;
	cout << "Welcome, " << userName << endl;

	Player user(userName);

	do
	{
		Monster monster = Monster::getRandomMonster();
		fightMonster(user, monster);
	} while (!user.isDead()&&!user.hasWon());
	if(!user.isDead())
	cout << "You won!!! Conglatulation.\a" << endl;

	return 0;
}