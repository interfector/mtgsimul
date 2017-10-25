std::map<std::string, unsigned int>
countPossibleMana(Environment* env)
{
	std::map<std::string, unsigned int> manapool;
	bool onExile[8] = { false, false, false, false, false, false, false, false };

	manapool["R"] = 0;
	manapool["W"] = 0;
	manapool["U"] = 0;
	manapool["G"] = 0;
	manapool["B"] = 0;
	manapool["C"] = 0;
	manapool["ANY"] = 0;
	manapool["MOX"] = 0;
	manapool["TOTAL"] = 0;

	manapool["R"] += env->countInZone("Simian Spirit Guide", HAND);
	manapool["G"] += env->countInZone("Elvish Spirit Guide", HAND);
	manapool["G"] += (env->countInZone("Summoner's Pact", HAND) > 1) ? env->countInZone("Summoner's Pact", HAND) - 1 : 0;
	manapool["ANY"] += env->countInZone("Lotus Petal", HAND);

	int moxCount = env->countInZone("Chrome Mox", HAND);
	for(int k = 0;k < moxCount;k++) {
		if( (env->countInZone("Balustrade Spy", HAND) + env->countInZone("Undercity Informer", HAND) >= 2 && 
					!onExile[0] && !onExile[1])
				|| (env->isInZone("Cabal Therapy", HAND) && !onExile[2]) 
				|| (env->isInZone("Bridge from Below", HAND) && !onExile[6])
				|| (env->isInZone("Street Wraith", HAND) && !onExile[7])) {
			if( env->isInZone("Bridge from Below", HAND) && !onExile[6] )
				onExile[6] = true;
			else if( env->isInZone("Cabal Therapy", HAND) && !onExile[2] )
				onExile[2] = true;
			else if( env->isInZone("Street Wraith", HAND) && !onExile[7] ) {
				onExile[7] = true;
				manapool["MOX_STREETWRAITH"] = 1;
			}
			else if (env->isInZone("Undercity Informer", HAND) && !onExile[1] )
				onExile[1] = true;
			else if (env->isInZone("Balustrade Spy", HAND) && !onExile[0] )
				onExile[0] = true;

			manapool["B"]++;
			manapool["MOX"] = 1;
		}
		else if ( env->countInZone("Chancellor of the Annex", HAND) >= 1 && !onExile[3]) {
			onExile[3] = true;
			manapool["W"]++;
			manapool["MOX"] = 1;
		}
		else if ( env->countInZone("Narcomoeba", HAND) >= 1 && !onExile[4]) {
			onExile[4] = true;
			manapool["U"]++;
			manapool["MOX"] = 1;
		}
		else if ( env->countInZone("Manamorphose", HAND) >= 1 && !onExile[5]) {
			manapool["R"]++;
			onExile[5] = true;
			manapool["MOX"] = 1;
		}
	}

	if( env->isInZone("Summoner's Pact", HAND) ) {
		if( manapool["ANY"] <= 0 && manapool["B"] <= 0 && (manapool["G"] > 0 || manapool["R"] > 0))
			env->getFromZone(LIBRARY, HAND, env->searchZone("Wild Cantor", LIBRARY));
		else	
			manapool["G"]++;
	}

	if( env->isInZone("Wild Cantor", HAND) || 
			((env->isInZone("Manamorphose", HAND) && 
			  !onExile[5] && 
			  (manapool["G"] + manapool["R"] + manapool["ANY"]) >= 2) && (manapool["ANY"] + manapool["B"]) <= 1)) {
		if( manapool["ANY"] <= 0 && manapool["B"] <= 0 ) {
			if( manapool["R"] > 0 ) {
				manapool["R"]--;
				manapool["B"]++;
			} else if( env->manapool["G"] > 0 ) {
				manapool["G"]--;
				manapool["B"]++;
			}
		}
	}

	if( manapool["ANY"] > 0 || manapool["B"] > 0 ) {
		int c1 = env->countInZone("Dark Ritual", HAND);
		int c2 = env->countInZone("Cabal Ritual", HAND);

		if( env->isInZone("Dark Ritual", HAND) ) {
			if( manapool["B"] <= 0 ) {
				manapool["B"] += 2 * c1 + 1;
				manapool["ANY"]--;
			} else
				manapool["B"] += 2 * c1;
		}


		if( env->isInZone("Cabal Ritual", HAND) ) {
			if( manapool["ANY"] > 0 || manapool["R"] > 0 || manapool["G"] > 0 || manapool["B"] >= 2) {
				if (manapool["B"] >= 2 ) {
					manapool["B"] += c2;
				} else {
					if( manapool["R"] > 0 )
						manapool["R"]--;
					else if (env->manapool["G"] > 0 )
						manapool["G"]--;
					else
						manapool["ANY"]--;

					manapool["B"] += c2 + 1;
				}
			}
		}
	}

	manapool["TOTAL"] = manapool["R"] + manapool["G"] + manapool["B"] + manapool["U"] + manapool["W"] + manapool["ANY"];

	return manapool;
}

bool victoryFilter(Environment* env)
{
	if(env->totalMana() >= 4 && env->manapool["B"] >= 1)
	{
		if( (env->isInZone("Balustrade Spy", HAND) || env->isInZone("Undercity Informer", HAND)) && env->countInZone("Narcomoeba", LIBRARY) >= 3)
			return true;
	} else if (env->totalMana() >= 1 && env->isInZone("Undercity Informer", BATTLEFIELD)) {
		return true;
	}

	return false;
}

std::vector<std::pair<MTGCard*,int>> libraryQuantity = {
	std::pair<MTGCard*,int>(new MTGCard("Street Wraith", "3BB", 5), /* 1 */ 8),
	std::pair<MTGCard*,int>(new MTGCard("Pact of Negation", "0", 0), /* 3 */ 0),
	std::pair<MTGCard*,int>(new MTGCard("Lotus Petal", "0", 0), 4),
	std::pair<MTGCard*,int>(new MTGCard("Simian Spirit Guide", "2R", 3), 4),
	std::pair<MTGCard*,int>(new MTGCard("Elvish Spirit Guide", "2G", 3), 4),
	std::pair<MTGCard*,int>(new MTGCard("Dark Ritual", "B", 1), 4),
	std::pair<MTGCard*,int>(new MTGCard("Cabal Ritual", "1B", 2), 4),
	std::pair<MTGCard*,int>(new MTGCard("Summoner's Pact", "0", 0), 4),
	std::pair<MTGCard*,int>(new MTGCard("Manamorphose", "1R", 2), 0),
	std::pair<MTGCard*,int>(new MTGCard("Chancellor of the Annex", "4WWW", 7), /* 4 */ 4),	
	std::pair<MTGCard*,int>(new MTGCard("Balustrade Spy", "3B", 4), 4),	
	std::pair<MTGCard*,int>(new MTGCard("Wild Cantor", "R", 1), 1),
	std::pair<MTGCard*,int>(new MTGCard("Undercity Informer", "2B", 3) , 4),	
	std::pair<MTGCard*,int>(new MTGCard("Narcomoeba", "1U", 2) , 4),	
	std::pair<MTGCard*,int>(new MTGCard("Chrome Mox", "0", 0) , 4),	
	std::pair<MTGCard*,int>(new MTGCard("Laboratory Maniac", "2U", 3) , 1),	
	std::pair<MTGCard*,int>(new MTGCard("Underworld Cerberus", "3BR", 5) , 1),	
	std::pair<MTGCard*,int>(new MTGCard("Bridge from Below", "BBB", 3) , 1),	
	std::pair<MTGCard*,int>(new MTGCard("Cabal Therapy", "B", 1) , 3),	
	std::pair<MTGCard*,int>(new MTGCard("Dread Return", "2BB", 4) , 1),	
};

class DeckEnvironment : public Environment 
{
	public:
		DeckEnvironment() : Environment() {
			int check = 0;
			mulliganCount = 3;

			priorityList.push_back("Pact Of Negation");
			priorityList.push_back("Chancellor of the Annex");
			priorityList.push_back("Street Wraith");
			priorityList.push_back("Gitaxian Probe");
			priorityList.push_back("Lotus Petal");
			priorityList.push_back("Simian Spirit Guide");
			priorityList.push_back("Elvish Spirit Guide");
			priorityList.push_back("Summoner's Pact");
			priorityList.push_back("Wild Cantor");
			priorityList.push_back("Dark Ritual");
			priorityList.push_back("Cabal Ritual");
			priorityList.push_back("Balustrade Spy");
			priorityList.push_back("Undercity Informer");

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
			if( countInZone("Narcomoeba", HAND) + countInZone("Underworld Cerberus", HAND) + 
			    countInZone("Dread Return", HAND) + countInZone("Laboratory Maniac", HAND) >= 3)
				return true;
		
			if( countInZone("Undercity Informer", HAND) + countInZone("Balustrade Spy", HAND) <= 0 )
				return true;

			if( countInZone("Narcomoeba", HAND) == 2 ) {
				if( countInZone("Bridge from Below", LIBRARY) >= 2 )
					return false;
				else if( !isInZone("Balustrade Spy", HAND) )
					return true;
			}

			if( countInZone("Narcomoeba", HAND) > 2 )
				return true;


			if( countPossibleMana( this )["TOTAL"] < 3 )
				return true;
		
			return false;
		}

		bool evaluateGame()
		{
			int count;
			std::map<std::string, unsigned int> pool;

			if( countInZone("Undercity Informer", HAND) + countInZone("Balustrade Spy", HAND) <= 0 )
				return false;

			pool = countPossibleMana( this );
			count = pool["TOTAL"];

			if( count == 3 && isInZone("Undercity Informer", HAND) && pool["MOX"] == 1 ) {
				counter["TURN"]++;
				manapool = pool;
				return true;
			}

			if( count >= 4 && pool["B"] > 0 ) {
				manapool = pool;
				return true;
			}

			while(countInZone("Street Wraith", HAND) > 0) {
				getFromZone(HAND, GRAVEYARD, searchZone("Street Wraith", HAND));
				drawCard(1);
				counter["LIFE"] -= 2;
			}

			pool = countPossibleMana( this );
			count = pool["TOTAL"];

			if( count == 3 && isInZone("Undercity Informer", HAND) && pool["MOX"] == 1 ) {
				counter["TURN"]++;
				manapool = pool;
				return true;
			}

		#ifdef _DEBUG
			std::cout << "[DEBUG] Manacount: " << count << " Black mana: " << pool["B"] << "\n";
		#endif

			if( count >= 4 && pool["B"] > 0 ) {
				manapool = pool;
				return true;
			}

			while(count < 4 || pool["B"] < 1) {
				counter["TURN"]++;

				drawCard(1);

				while(countInZone("Street Wraith", HAND) > 0) {
					getFromZone(HAND, GRAVEYARD, searchZone("Street Wraith", HAND));
					drawCard(1);
					counter["LIFE"] -= 2;
				}

				pool = countPossibleMana( this );
				count = pool["TOTAL"];
			}

			manapool = pool;

			return true;
		}
};

std::map<std::string, int> result;
extern char**	arguments;
extern int	arguments_count;

void
initializeResult()
{
	result["TURN1"] = 0;
	result["TURN2"] = 0;
	result["TURN3"] = 0;
	result["TURN4"] = 0;
	result["TURN5"] = 0;
	result["OVERTURN"] = 0;
	result["LIFE"] = 0;
	result["TURN1_WITH_PROTECTION"] = 0;

	result["SAMPLE"] = 0;
	result["HAND_SIZE"] = 0;
	result["MANA_REQUEST"] = 0;
	result["CATCH_CARD"] = 0;
	result["MULLIGAN"] = 0;
	result["BOUND"] = 0;
	result["BOUND_VAR"] = 3210;

	result["TRY_TURN"] = 0;
	result["TURN_COUNT"] = 0;
	result["HAND_COUNT"] = 0;
	result["MANA_REQ_COUNT"] = 0;
	result["CATCHC_COUNT"] = 0;

	result["SHOW_RESULT"] = 0;

	for(int i = 2;i < arguments_count;i++) {
		if(!strncmp(arguments[i], "--sample-t", 10))
			result["SAMPLE"] = strtol(arguments[i]+10, NULL, 10);

		if(!strncmp(arguments[i], "--turn=", 7))
			result["TRY_TURN"] = strtol(arguments[i]+7, NULL, 10);

		if(!strncmp(arguments[i], "--hand-size=", 12))
			result["HAND_SIZE"] = strtol(arguments[i]+12, NULL, 10);

		if(!strncmp(arguments[i], "--total-mana=", 13))
			result["MANA_REQUEST"] = strtol(arguments[i]+13, NULL, 10);

		if(!strncmp(arguments[i], "--has-card=", 11))
			result["CATCH_CARD"] = i;

		if(!strcmp(arguments[i], "--show-result"))
			result["SHOW_RESULT"] = 1;

		if(!strcmp(arguments[i], "--unbound-var"))
			result["BOUND"] = 1;

		if(!strncmp(arguments[i], "--bound-var=", 12)) {
			char boundVar[] = "TSMC";
			result["BOUND_VAR"] = 0;

			for(int j = 0, k = 0;j  < 4;j++) {
				for(k = 0;k < 4;k++) {
					if(arguments[i][12+j] == boundVar[k]) {
						break;
					}
				}
				result["BOUND_VAR"] = result["BOUND_VAR"] * 10 + k;
			}
			//result["BOUND_VAR"] = strtol(arguments[i]+12, NULL, 10);
		}
	}
}

void
parseResult(Environment* env)
{
	bool bound[4] = { false, false, false, false };

	if( env->counter["TURN"] == (unsigned)result["SAMPLE"] ) {
		if(!result["HAND_SIZE"] || env->zone[HAND].size() == (unsigned)result["HAND_SIZE"]) {
			if(!result["MANA_REQUEST"] || env->manapool["TOTAL"] == (unsigned)result["MANA_REQUEST"]) {
				if(!result["CATCH_CARD"] || env->isInZone(arguments[result["CATCH_CARD"]]+11, HAND)) {
					std::cout << "\t== SAMPLE HAND ==\n";
					for (auto &card : env->zone[HAND])
						std::cout << "-> " << card->name << "\n";
					std::cout << "===================\n";
				}
			}
		}
	}

	if( result["BOUND"] ) {
		bound[0] = bound[1] = bound[2] = bound[3] = true;
	}

	for(int j = 0, last = 0, seq = result["BOUND_VAR"];j < 4;j++)
	{
		switch(seq % 10) {
			case 0:
			if( result["TRY_TURN"] && env->counter["TURN"] == (unsigned)result["TRY_TURN"] && (bound[last] || !j)) {
				result["TURN_COUNT"]++;
				bound[seq % 10] = true;
			}
			break;
			case 1:
			if( result["HAND_SIZE"] && env->zone[HAND].size() == (unsigned)result["HAND_SIZE"] && (bound[last] || !j)) {
				result["HAND_COUNT"]++;
				bound[seq % 10] = true;
			}
			break;
			case 2:
			if( result["MANA_REQUEST"] && env->manapool["TOTAL"] == (unsigned)result["MANA_REQUEST"] && (bound[last] || !j)) {
				result["MANA_REQ_COUNT"]++;
				bound[seq % 10] = true;
			}
			break;
			case 3:
			if( result["CATCH_CARD"] && env->isInZone(arguments[result["CATCH_CARD"]]+11, HAND) && (bound[last] || !j)) {
				result["CATCHC_COUNT"]++;
				bound[seq % 10] = true;
			}
			break;
		}

		last = seq % 10;
		seq /= 10;
	}
	/*
	if( result["TRY_TURN"] && env->counter["TURN"] == (unsigned)result["TRY_TURN"] ) {
		result["TURN_COUNT"]++;
		bound[0] = true;
	}
	
	if( result["HAND_SIZE"] && env->zone[HAND].size() == (unsigned)result["HAND_SIZE"] && bound[0]) {
		result["HAND_COUNT"]++;
		bound[1] = true;
	}

	if( result["MANA_REQUEST"] && env->manapool["TOTAL"] == (unsigned)result["MANA_REQUEST"] && bound[1]) {
		result["MANA_REQ_COUNT"]++;
		bound[2] = true;
	}

	if( result["CATCH_CARD"] && env->isInZone(arguments[result["CATCH_CARD"]]+11, HAND) && bound[2]) {
			result["CATCHC_COUNT"]++;
	}
*/
	if( env->counter["TURN"] <= 5 )
	{
		result["TURN" + std::to_string( env->counter["TURN"] )]++;

		if( env->counter["TURN"] == 1 ) {
			result["MULLIGAN"] += env->counter["MULLIGAN"];

			if( env->isInZone("Pact of Negation", HAND) || env->isInZone("Chancellor of the Annex", HAND) )
				result["TURN1_WITH_PROTECTION"]++;
		}
	} else
		result["OVERTURN"]++;

	result["LIFE"] += env->counter["LIFE"] - 2;
}

void
printResult1(Environment* env)
{
	double pTurn[6], pMulligan, pProtection[2], aLife;
	int count = env->counter["TOTAL_TRY"];

	for(int i = 0;i < 5;i++)
		pTurn[i] = (double)(result["TURN" + std::to_string( i + 1 )])/(double)(count) * 100.0;

	pTurn[5] = (double)(result["OVERTURN"])/(double)(count) * 100.0;

	pMulligan = (double)(result["MULLIGAN"])/(double)(result["TURN1"]) * 100.0;
	pProtection[0] = (double)(result["TURN1_WITH_PROTECTION"])/(double)(count) * 100.0;
	pProtection[1] = (double)(result["TURN1_WITH_PROTECTION"])/(double)(result["TURN1"]) * 100.0;
	aLife = (double)(result["LIFE"])/(double)(count);

	std::cout << "Probability of Turn 1 Win: " << result["TURN1"] << "/" << count << " ~ " << pTurn[0] << "%\n";
	std::cout << "\tProbability of Turn 1 Win with Protection (absolute): " << result["TURN1_WITH_PROTECTION"] << "/" << count << " ~ " << pProtection[0] << "%\n";
	std::cout << "\tProbability of Turn 1 Win with Protection (relative): " << result["TURN1_WITH_PROTECTION"] << "/" << result["TURN1"] << " ~ " << pProtection[1] << "%\n";
	std::cout << "\tNumber of Mulligan with T1 Win / Turn 1 Win: " << result["MULLIGAN"] << "/" << result["TURN1"] << " ~ " << pMulligan << "%\n";
	std::cout << "Probability of Turn 2 Win: " << result["TURN2"] << "/" << count << " ~ " << pTurn[1] << "%\n";
	std::cout << "Probability of Turn 3 Win: " << result["TURN3"] << "/" << count << " ~ " << pTurn[2] << "%\n";
	std::cout << "Probability of Turn 4 Win: " << result["TURN4"] << "/" << count << " ~ " << pTurn[3] << "%\n";
	std::cout << "Probability of Turn 5 Win: " << result["TURN5"] << "/" << count << " ~ " << pTurn[4] << "%\n";
	std::cout << "Probability of winning before Turn 5: " << (count - result["OVERTURN"]) << "/" << count << " ~ " << 100.0 - pTurn[5] << "%\n";	
	std::cout << "Probability of winning after Turn 5: " << result["OVERTURN"] << "/" << count << " ~ " << pTurn[5] << "%\n";	
	std::cout << "Average life after winning: " << result["LIFE"] << "/" << count << " ~ " << aLife << "%\n";
}

void
printResult2(Environment* env)
{
	double turn, handsize, mana, wcard;
	int count = env->counter["TOTAL_TRY"];
	
	turn = (double)(result["TURN_COUNT"])/(double)(count) * 100.0;
	handsize = (double)(result["HAND_COUNT"])/(double)(count) * 100.0;
	mana = (double)(result["MANA_REQ_COUNT"])/(double)(count) * 100.0;
	wcard = (double)(result["CATCHC_COUNT"])/(double)(count) * 100.0;

	if( result["TRY_TURN"] )
		std::cout << "Probability of winning at turn " << result["TRY_TURN"] << ": " << result["TURN_COUNT"] << "/" << count << " ~ " << turn << "%\n";
	if( result["HAND_SIZE"] )
		std::cout << "Probability of winning with hand size " << result["HAND_SIZE"] << ": " << result["HAND_COUNT"] << "/" << count << " ~ " << handsize << "%\n";
	if( result["MANA_REQUEST"] )
		std::cout << "Probability of winning with " << result["MANA_REQUEST"] << " mana: " << result["MANA_REQ_COUNT"] << "/" << count << " ~ " << mana << "%\n";
	if( result["CATCHC_COUNT"] )
		std::cout << "Probability of winning with card \'" << arguments[result["CATCH_CARD"]]+11 << "\': " << result["CATCHC_COUNT"] << "/" << count << " ~ " << wcard << "%\n";
}

void
printResult(Environment* env)
{
	if(result["SHOW_RESULT"] == 0)
		printResult1( env );
	else
		printResult2( env );
}
