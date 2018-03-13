int
min(int a, int b)
{
	return (a > b) ? b : a;
}

std::map<std::string, unsigned int>
countPossibleMana(Environment* env)
{
	std::map<std::string, unsigned int> manapool;
	bool onExile[4] = { false, false, false, false };

	manapool["R"] = 0;
	manapool["W"] = 0;
	manapool["U"] = 0;
	manapool["G"] = 0;
	manapool["B"] = 0;
	manapool["C"] = 0;
	manapool["ANY"] = 0;
	manapool["MOX"] = 0;
	manapool["MOX_STREETWRAITH"] = 0;
	manapool["STORM"] = 0;
	manapool["TOTAL"] = 0;

	manapool["R"] += env->countInZone("Simian Spirit Guide", HAND);
	manapool["G"] += env->countInZone("Elvish Spirit Guide", HAND);
	manapool["ANY"] += env->countInZone("Lotus Petal", HAND);
	manapool["STORM"] += env->countInZone("Lotus Petal", HAND);

	if( env->isInZone("Stomping Ground", HAND) || (!env->isInZone("Stomping Ground", HAND) && env->isInZone("Land Grant", HAND)) ) {
		if( env->isInZone("Tinder Wall", HAND) && manapool["G"] < 1 )
			manapool["G"]++;
		else
			manapool["R"]++;

		if(env->isInZone("Land Grant", HAND) && !env->isInZone("Stomping Ground", HAND))
			manapool["STORM"]++;
	}

	int c0 = env->countInZone("Tinder Wall", HAND);
	int cTW = min(c0, manapool["G"]);

	manapool["G"] -= cTW;
	manapool["R"] += (2 * cTW);
	manapool["STORM"] += cTW;

	int c1 = env->countInZone("Rite of Flame", HAND);
	int c2 = env->countInZone("Desperate Ritual", HAND);
	int c3 = env->countInZone("Pyretic Ritual", HAND);
	int c4 = env->countInZone("Seething Song", HAND);

	if( env->isInZone("Rite of Flame", HAND) ) {
		if( manapool["ANY"] > 0 || manapool["R"] > 0 ) {
			if( manapool["R"] <= 0 )
				manapool["ANY"]--;
			else
				manapool["R"]--;
			
			manapool["R"] += 1 + (c1 * (c1 + 1)) / 2;
			manapool["STORM"] += c1;
		}
	}

	manapool["TOTAL"] = manapool["R"] + manapool["G"] + manapool["B"] + manapool["U"] + manapool["W"] + manapool["ANY"];

	if( env->isInZone("Desperate Ritual", HAND) ) {
		if( (manapool["ANY"] > 0 || manapool["R"] > 0 || manapool["G"] > 0) && manapool["TOTAL"] >= 2) {
			if (manapool["R"] >= 2 ) {
				manapool["R"] += c2;
			} else {
				if( manapool["R"] > 0 )
					manapool["R"]--;
				else if (env->manapool["G"] > 0 )
					manapool["G"]--;
				else
					manapool["ANY"]--;

				manapool["R"] += c2 + 1;
			}

			manapool["STORM"] += c2;
		}
	}

	manapool["TOTAL"] = manapool["R"] + manapool["G"] + manapool["B"] + manapool["U"] + manapool["W"] + manapool["ANY"];

	if( env->isInZone("Pyretic Ritual", HAND) ) {
		if( (manapool["ANY"] > 0 || manapool["R"] > 0 || manapool["G"] > 0) && manapool["TOTAL"] >= 2) {
			if (manapool["R"] >= 2 ) {
				manapool["R"] += c3;
			} else {
				if( manapool["R"] > 0 )
					manapool["R"]--;
				else if (env->manapool["G"] > 0 )
					manapool["G"]--;
				else
					manapool["ANY"]--;

				manapool["R"] += c3 + 1;
			}

			manapool["STORM"] += c3;
		}
	}

	manapool["TOTAL"] = manapool["R"] + manapool["G"] + manapool["B"] + manapool["U"] + manapool["W"] + manapool["ANY"];

	if( env->isInZone("Seething Song", HAND) ) {
		if( (manapool["ANY"] > 0 || manapool["R"] > 0 || manapool["G"] > 0) && manapool["TOTAL"] >= 3) {
			if (manapool["R"] >= 3 ) {
				manapool["R"] += 2 * c4;
			} else {
				if( manapool["R"] > 0 )
					manapool["R"]--;
				else if (env->manapool["G"] > 0 )
					manapool["G"]--;
				else
					manapool["ANY"]--;

				manapool["R"] += 2 * c4 + 1;
			}

			manapool["STORM"] += c4;
		}
	}

	manapool["TOTAL"] = manapool["R"] + manapool["G"] + manapool["B"] + manapool["U"] + manapool["W"] + manapool["ANY"];

	int moxCount = env->countInZone("Chrome Mox", HAND);

	for(int k = 0;k < moxCount;k++) {
		if( env->isInZone("Land Grant", HAND) && env->isInZone("Stomping Ground", HAND) && !onExile[0] ) {
			onExile[0] = true;
			manapool["G"]++;
			manapool["MOX"] = 1;
		}
		else if ( env->isInZone("Burning Wish", HAND) && !onExile[1] && (manapool["TOTAL"] < 6 || manapool["R"] < 2) ) {
			onExile[1] = true;
			manapool["R"]++;
			manapool["MOX"] = 1;
		}
		else if ( env->isInZone("Street Wraith", HAND) && !onExile[2] ) {
			onExile[2] = true;
			manapool["B"]++;
			manapool["MOX"] = 1;
			manapool["MOX_STREETWRAITH"]++;
		}
		else if ( env->countInZone("Manamorphose", HAND) >= 1 && !onExile[3]) {
			manapool["R"]++;
			onExile[3] = true;
			manapool["MOX"] = 1;
		}
	}

	manapool["STORM"] += moxCount;

	if( !onExile[0] && !env->isInZone("Stomping Ground", HAND) )
		manapool["STORM"]++;

	if(((env->isInZone("Manamorphose", HAND) && 
			  !onExile[3] && 
			  (manapool["G"] + manapool["R"] + manapool["ANY"]) >= 2) && (manapool["ANY"] + manapool["R"]) <= 1)) {
		if( manapool["ANY"] <= 0 && manapool["R"] <= 0 ) {
			if( manapool["G"] > 0 ) {
				manapool["G"]--;
				manapool["R"]++;
			}

			manapool["STORM"] += env->countInZone("Manamorphose", HAND);
		}
	}

	manapool["TOTAL"] = manapool["R"] + manapool["G"] + manapool["B"] + manapool["U"] + manapool["W"] + manapool["ANY"];

	return manapool;
}

std::vector<std::pair<MTGCard*,int>> libraryQuantity = {
	std::pair<MTGCard*,int>(new MTGCard("Chrome Mox", "0", 0) , 3),	
	std::pair<MTGCard*,int>(new MTGCard("Goblin Charbelcher", "4", 4) , 4),	
	std::pair<MTGCard*,int>(new MTGCard("Lotus Petal", "0", 0) , 4),	
	std::pair<MTGCard*,int>(new MTGCard("Elvish Spirit Guide", "0", 0) , 4),	
	std::pair<MTGCard*,int>(new MTGCard("Simian Spirit Guide", "0", 0) , 4),	
	std::pair<MTGCard*,int>(new MTGCard("Street Wraith", "0", 0) , 4),
	std::pair<MTGCard*,int>(new MTGCard("Tinder Wall", "G", 1) , 4),
	std::pair<MTGCard*,int>(new MTGCard("Desperate Ritual", "1R", 2) , 4),
	std::pair<MTGCard*,int>(new MTGCard("Pyretic Ritual", "1R", 2) , 4),
	std::pair<MTGCard*,int>(new MTGCard("Seething Song", "2R", 3) , 4),
	std::pair<MTGCard*,int>(new MTGCard("Manamorphose", "1R", 2) , 4),
	std::pair<MTGCard*,int>(new MTGCard("Stomping Ground", "0", 0) , 1),
	std::pair<MTGCard*,int>(new MTGCard("Burning Wish", "1R", 2) , 2),
	std::pair<MTGCard*,int>(new MTGCard("Empty the Warrens", "3R", 4) , 2),
	std::pair<MTGCard*,int>(new MTGCard("Gitaxian Probe", "0", 0) , 4),
	std::pair<MTGCard*,int>(new MTGCard("Land Grant", "0", 0) , 4),
	std::pair<MTGCard*,int>(new MTGCard("Rite of Flame", "R", 1) , 4),
};

class DeckEnvironment : public Environment 
{
	public:
		DeckEnvironment() : Environment() {
			int check = 0;
			mulliganCount = 2;

			for (auto &cardq : libraryQuantity) // access by reference to avoid copying
			{
				cardq.first->env = this;

				for(int i = 0;i < cardq.second;i++)
					zone[LIBRARY].push_back(cardq.first->clone());

				check += cardq.second;
			}

			shuffleDeck();

			if( check < 60 ) {
				std::cout << "Error: Wrong size deck, " << check << " cards found!\n";
				exit(2);
			}
		};

		bool mulliganFilter()
		{
			if( countInZone("Goblin Charbelcher", HAND) + countInZone("Empty the Warrens", HAND) + countInZone("Burning Wish", HAND) <= 0 )
				return true;

			std::map<std::string, unsigned int> pool = countPossibleMana( this );

			if( pool["TOTAL"] < 3 )
				return true;

			if( !isInZone("Empty the Warrens", HAND) && !isInZone("Burning Wish", HAND) && pool["TOTAL"] < 6 )
				return true;
		
			return false;
		}

		bool evaluateGame()
		{
			int count;
			std::map<std::string, unsigned int> pool;

			pool = countPossibleMana( this );
			count = pool["TOTAL"];

			if( isInZone("Goblin Charbelcher", HAND) && count >= 7 ) {
				return true;
			}

			if( isInZone("Burning Wish", HAND) && count >= 6 && pool["R"] >= 2 )
			{
				counter["TURN"] += 1 + (10 / (pool["STORM"] + 2));
				return true;
			}

			if( isInZone("Empty the Warrens", HAND) && count >= 4 && pool["R"] >= 1 )
			{
				counter["TURN"] += 1 + (10 / (pool["STORM"] + 1));
				return true;
			}

			while(countInZone("Street Wraith", HAND) > 0) {
				getFromZone(HAND, GRAVEYARD, searchZone("Street Wraith", HAND));
				drawCard(1);
				counter["LIFE"] -= 2;
			}

			while(countInZone("Gitaxian Probe", HAND) > 0) {
				getFromZone(HAND, GRAVEYARD, searchZone("Gitaxian Probe", HAND));
				drawCard(1);
				pool["STORM"]++;
				counter["LIFE"] -= 2;
			}

			pool = countPossibleMana( this );
			count = pool["TOTAL"];

		#ifdef _DEBUG
			std::cout << "[DEBUG] Manacount: " << count << " Red mana: " << pool["R"] << "\n";
		#endif

			if( isInZone("Goblin Charbelcher", HAND) && count >= 7 ) {
				return true;
			}

			if( isInZone("Burning Wish", HAND) && count >= 6 && pool["R"] >= 2 )
			{
				counter["TURN"] += 1 + (10 / (pool["STORM"] + 2));
				return true;
			}

			if( isInZone("Empty the Warrens", HAND) && count >= 4 && pool["R"] >= 1 )
			{
				counter["TURN"] += 1 + (10 / (pool["STORM"] + 1));
				return true;
			}

/*			while( (count < 4 && isInZone("Empty the Warrens", HAND)) ||
					(count < 6 && isInZone("Burning Wish", HAND)) ||
					(count < 7 && isInZone("Goblin Charbelcher", HAND)) ) {*/
			do {
				if( count >= 7 && isInZone("Goblin Charbelcher", HAND) ) {
					break;
				} else if (count >= 6 && isInZone("Burning Wish", HAND) ) {
					counter["TURN"] += 1 + (10 / (pool["STORM"] + 2));
					break;
				} else if (count >= 4 && isInZone("Empty the Warrens", HAND) ) {
					counter["TURN"] += 1 + (10 / (pool["STORM"] + 1));
					break;
				}

				counter["TURN"]++;

				drawCard(1);

				pool = countPossibleMana( this );
				count = pool["TOTAL"];
			} while( (count < 4 && isInZone("Empty the Warrens", HAND)) ||
						(count < 6 && isInZone("Burning Wish", HAND)) ||
						(count < 7 && isInZone("Goblin Charbelcher", HAND)) );

			return true;
		}
};

std::map<std::string, int> result;
extern char** arguments;

void
initializeResult()
{
	result["TURN1"] = 0;
	result["TURN2"] = 0;
	result["TURN3"] = 0;
	result["TURN4"] = 0;
	result["TURN5"] = 0;
	result["OVERTURN"] = 0;

	result["MULLIGAN"] = 0;
	result["LIFE"] = 0;

	if(arguments[2] && !strcmp(arguments[2], "--sample-t1"))
		result["SAMPLE_T1"] = 1;
}

void
parseResult(Environment* env)
{
	if( env->counter["TURN"] <= 5 )
	{
		result["TURN" + std::to_string( env->counter["TURN"] )]++;

		if( env->counter["TURN"] == 1 ) {
			result["MULLIGAN"] += env->counter["MULLIGAN"];

			if(result["SAMPLE_T1"]) {
				std::cout << "\t== SAMPLE HAND ==\n";
				for (auto &card : env->zone[HAND])
					std::cout << "-> " << card->name << "\n";
				std::cout << "===================\n";
			}
		}
	} else
		result["OVERTURN"]++;

	result["LIFE"] += env->counter["LIFE"];
}

void
printResult(Environment* env)
{
	double pTurn[6], pMulligan, aLife;
	int count = env->counter["TOTAL_TRY"];

	for(int i = 0;i < 5;i++)
		pTurn[i] = (double)(result["TURN" + std::to_string( i + 1 )])/(double)(count) * 100.0;

	pTurn[5] = (double)(result["OVERTURN"])/(double)(count) * 100.0;

	pMulligan = (double)(result["MULLIGAN"])/(double)(result["TURN1"]) * 100.0;
	aLife = (double)(result["LIFE"])/(double)(count);

	std::cout << "Probability of Turn 1 Win: " << result["TURN1"] << "/" << count << " ~ " << pTurn[0] << "%\n";
	std::cout << "\tNumber of Mulligan with T1 Win / Turn 1 Win: " << result["MULLIGAN"] << "/" << result["TURN1"] << " ~ " << pMulligan << "%\n";
	std::cout << "Probability of Turn 2 Win: " << result["TURN2"] << "/" << count << " ~ " << pTurn[1] << "%\n";
	std::cout << "Probability of Turn 3 Win: " << result["TURN3"] << "/" << count << " ~ " << pTurn[2] << "%\n";
	std::cout << "Probability of Turn 4 Win: " << result["TURN4"] << "/" << count << " ~ " << pTurn[3] << "%\n";
	std::cout << "Probability of Turn 5 Win: " << result["TURN5"] << "/" << count << " ~ " << pTurn[4] << "%\n";
	std::cout << "Probability of winning before Turn 5: " << count - result["OVERTURN"] << "/" << count << " ~ " << 100.0 - pTurn[5] << "%\n";
	std::cout << "Probability of winning after Turn 5: " << result["OVERTURN"] << "/" << count << " ~ " << pTurn[5] << "%\n";
	std::cout << "Average life after winning: " << result["LIFE"] << "/" << count << " ~ " << aLife << "%\n";
}
