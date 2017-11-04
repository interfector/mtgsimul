#ifndef _ENVIRONMENT_
#define _ENVIRONMENT_

#include <vector>
#include <string>
#include <map>
#include <cassert>
#include <algorithm> 
#include <functional>
#include <random>

enum PlayerTrigger {
	GAIN_LIFE,
	LOSE_LIFE,
	DAMAGE,
	CAST,
	DRAW,
	DISCARD,
	PLAYER_TRIGGER,
};

enum Zone {
	HAND,
	LIBRARY,
	BATTLEFIELD,
	GRAVEYARD,
	EXILE,
	ZONE_COUNT
};

typedef enum Zone ZoneType;

class Environment;

class MTGCard {
	public:
		std::string name;
		std::string cost;
		unsigned int cmc;
		Environment* env;

		ZoneType currentZone;

		virtual bool canPlay() { return true; };
		virtual void play() {};
		virtual MTGCard* clone() { MTGCard* i = new MTGCard(name, cost, cmc); i->currentZone = currentZone; return i; };

		MTGCard(std::string str, std::string cc, unsigned int cost) : name(str), cost(cc), cmc(cost) { currentZone = LIBRARY; };
};

class MTGTrigger
{
	public:
		MTGCard* source;
		std::function<void (Environment*)> ability;

		MTGTrigger(MTGCard* card, std::function<void (Environment*)> func) : source(card), ability(func) {}
};

class Environment
{
	public:
		std::vector<MTGCard*> zone[ZONE_COUNT];
		std::map<std::string, unsigned int> manapool;
		std::map<std::string, unsigned int> counter;
		std::map<std::string, int> result;

		std::vector<std::string> priorityList;

		int mulliganCount;

//		std::map<std::string, MTGCard*> classMap;

		Environment() {
			manapool["R"] = 0;
			manapool["W"] = 0;
			manapool["U"] = 0;
			manapool["G"] = 0;
			manapool["B"] = 0;
			manapool["C"] = 0;
			manapool["ANY"] = 0;

			counter["LIFE"] = 20;
			counter["OPPONENT"] = 20;
			counter["TURN"] = 1;
			counter["MULLIGAN"] = 0;

			mulliganCount = 6;
		}

		virtual int countInZone(std::string str, ZoneType t) {
			int i = 0;

			for(auto &card : zone[t]) {
				if(!card->name.compare(str))
					i++;
			}

			return i;
		}

		virtual bool isInZone(std::string str, ZoneType t) {
			return (countInZone(str, t) > 0) ? true : false;
		}

		virtual int searchZone(std::string str, ZoneType t) {
			for(unsigned int i = 0;i < zone[t].size();i++) {
				if(!zone[t].at(i)->name.compare(str))
					return (int)i;
			}

			return -1;
		}

		virtual int totalMana() {
			return manapool["R"] + manapool["W"] + manapool["U"] + manapool["G"] + manapool["B"] + manapool["C"];
		}

		virtual std::string maxManaPool()
		{
			std::string max = "R";

			for(auto &pool : manapool)
			{
				if(pool.second > manapool[max])
					max = pool.first;
			}

			return max;
		}

		virtual void getFromZone(ZoneType from, ZoneType to, int idx) {
			if (idx >= 0 )
			{
				zone[from].at(idx)->currentZone = to;

				zone[to].push_back(zone[from].at(idx));
				zone[from].erase(zone[from].begin() + idx);
			}
		}

		virtual void shuffleDeck() {
			std::random_device rd;
			std::mt19937 g(rd());

			std::shuffle ( zone[LIBRARY].begin(), zone[LIBRARY].end(), g );
		}

		virtual void drawCard(int count) {
			for(int i = 0;i < count;i++) {
				zone[HAND].push_back(zone[LIBRARY].back());
				zone[LIBRARY].pop_back();
				zone[HAND].back()->currentZone = HAND;
			}
		}

		virtual void showZone(ZoneType sZone, std::string msg) {
			std::cout << "\n===== " + msg + " ======\n";
			for (auto &card : zone[sZone])
					std::cout << "-> " << card->name << "\n";
			std::cout << "===================\n";
		}

		virtual Environment* clone() {
			Environment* nEnv = new Environment();

			nEnv->manapool = manapool;
			nEnv->counter = counter;
			nEnv->result = result;

			for(int i = 0;i < ZONE_COUNT;i++)
				nEnv->zone[i] = zone[i];

			return nEnv;
		}

		virtual bool mulliganFilter() { return false; }
		virtual bool evaluateGame() { return false; }
};



#endif
