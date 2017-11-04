std::vector<std::string>
split(const char *phrase, std::string delimiter)
{
	std::vector<std::string> list;
	std::string s = std::string(phrase);
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos)
	{
		token = s.substr(0, pos);
		list.push_back(token);
		s.erase(0, pos + delimiter.length());
	}

	list.push_back(s);
	
	return list;
}

Environment*
countPossibleMana(Environment* env)
{
	Environment* sbox = env->clone();

	sbox->manapool["R"] = 0;
	sbox->manapool["W"] = 0;
	sbox->manapool["U"] = 0;
	sbox->manapool["G"] = 0;
	sbox->manapool["B"] = 0;
	sbox->manapool["C"] = 0;
	sbox->manapool["ANY"] = 0;
	sbox->manapool["TOTAL"] = 0;

	while(sbox->countInZone("Simian Spirit Guide", HAND) > 0) {
		sbox->getFromZone(HAND, EXILE, sbox->searchZone("Simian Spirit Guide", HAND));
		sbox->manapool["R"]++;
	}

	while(sbox->countInZone("Elvish Spirit Guide", HAND) > 0) {
		sbox->getFromZone(HAND, EXILE, sbox->searchZone("Elvish Spirit Guide", HAND));
		sbox->manapool["G"]++;
	}

	if(sbox->countInZone("Summoner's Pact", HAND) > 1) {
		while(sbox->countInZone("Elvish Spirit Guide", HAND) > 1) {
			sbox->getFromZone(HAND, GRAVEYARD, sbox->searchZone("Summoner's Pact", HAND));
			sbox->manapool["G"]++;
		}
	}

	while(sbox->countInZone("Lotus Petal", HAND) > 0) {
		sbox->getFromZone(HAND, EXILE, sbox->searchZone("Lotus Petal", HAND));
		sbox->manapool["ANY"]++;
	}

	int moxCount = sbox->countInZone("Chrome Mox", HAND);
	for(int k = 0;k < moxCount;k++) {
		if( (sbox->countInZone("Balustrade Spy", HAND) + sbox->countInZone("Undercity Informer", HAND) >= 2)
				|| sbox->isInZone("Cabal Therapy", HAND)
				|| sbox->isInZone("Bridge from Below", HAND)
				|| sbox->isInZone("Street Wraith", HAND)) {
			if( sbox->isInZone("Bridge from Below", HAND) )
				sbox->getFromZone(HAND, EXILE, sbox->searchZone("Bridge from Below", HAND));
			else if( sbox->isInZone("Cabal Therapy", HAND) )
				sbox->getFromZone(HAND, EXILE, sbox->searchZone("Cabal Therapy", HAND));
			else if( sbox->isInZone("Street Wraith", HAND) )
				sbox->getFromZone(HAND, EXILE, sbox->searchZone("Street Wraith", HAND));
			else if (sbox->isInZone("Undercity Informer", HAND) )
				sbox->getFromZone(HAND, EXILE, sbox->searchZone("Undercity Informer", HAND));
			else if (sbox->isInZone("Balustrade Spy", HAND) )
				sbox->getFromZone(HAND, EXILE, sbox->searchZone("Balustrade Spy", HAND));

			sbox->manapool["B"]++;
			sbox->manapool["MOX"] = 1;
		}
		else if ( sbox->countInZone("Chancellor of the Annex", HAND) >= 1 ) {
			sbox->manapool["W"]++;
			sbox->manapool["MOX"] = 1;
			sbox->getFromZone(HAND, EXILE, sbox->searchZone("Chancellor of the Annex", HAND));
		}
		else if ( sbox->countInZone("Narcomoeba", HAND) >= 1 ) {
			sbox->manapool["U"]++;
			sbox->manapool["MOX"] = 1;
			sbox->getFromZone(HAND, EXILE, sbox->searchZone("Narcomoeba", HAND));
		}
		else if ( sbox->countInZone("Gitaxian Probe", HAND) >= 1 ) {
			sbox->manapool["U"]++;
			sbox->manapool["MOX"] = 1;
			sbox->getFromZone(HAND, EXILE, sbox->searchZone("Gitaxian Probe", HAND));
		}
		else if ( sbox->countInZone("Manamorphose", HAND) >= 1) {
			sbox->manapool["R"]++;
			sbox->manapool["MOX"] = 1;
			sbox->getFromZone(HAND, EXILE, sbox->searchZone("Manamorphose", HAND));
		} 
		else if ( sbox->countInZone("Wild Cantor", HAND) >= 1 && sbox->manapool["ANY"] > 0) {
			sbox->manapool["R"]++;
			sbox->manapool["MOX"] = 1;
			sbox->getFromZone(HAND, EXILE, sbox->searchZone("Wild Cantor", HAND));
		} 
	}

	if( sbox->isInZone("Summoner's Pact", HAND) ) {
		if( sbox->manapool["ANY"] <= 0 && sbox->manapool["B"] <= 0 && (sbox->manapool["G"] > 0 || sbox->manapool["R"] > 0))
			sbox->getFromZone(LIBRARY, HAND, sbox->searchZone("Wild Cantor", LIBRARY));
		else	
			sbox->manapool["G"]++;

		sbox->getFromZone(HAND, GRAVEYARD, sbox->searchZone("Summoner's Pact", HAND));
	}

	if( sbox->isInZone("Wild Cantor", HAND) ) {
		if( sbox->manapool["ANY"] <= 0 && sbox->manapool["B"] <= 0 ) {
			if( sbox->manapool["R"] > 0 ) {
				sbox->manapool["R"]--;
				sbox->manapool["B"]++;
			} else if( sbox->manapool["G"] > 0 ) {
				sbox->manapool["G"]--;
				sbox->manapool["B"]++;
			}
		}
	}

	sbox->manapool["TOTAL"] = sbox->manapool["R"] + sbox->manapool["G"] + sbox->manapool["B"] + sbox->manapool["U"] + sbox->manapool["W"] + sbox->manapool["ANY"];

	if(sbox->manapool["TOTAL"] >= 2 && (sbox->manapool["R"] > 0 || sbox->manapool["G"] > 0)) {
		while(sbox->countInZone("Manamorphose", HAND) > 0) {
			if(sbox->manapool["B"] <= 0 && sbox->manapool["ANY"] <= 0) {
				if( sbox->manapool["R"] > 0 )
					sbox->manapool["R"]--;
				else if ( sbox->manapool["G"] > 0 )
					sbox->manapool["G"]--;

				sbox->manapool["B"]++;
			}

			sbox->drawCard(1);

			sbox->getFromZone(HAND, GRAVEYARD, sbox->searchZone("Manamorphose", HAND));
		}
	}

	if( sbox->manapool["ANY"] > 0 || sbox->manapool["B"] > 0) {
		while(sbox->countInZone("Dark Ritual", HAND) > 0) {
			if(sbox->manapool["B"] <= 0)
				sbox->manapool["ANY"]--;
			else
				sbox->manapool["B"]--;

			sbox->manapool["B"] += 3;
			sbox->getFromZone(HAND, GRAVEYARD, sbox->searchZone("Dark Ritual", HAND));
		}

		sbox->manapool["TOTAL"] = sbox->manapool["R"] + sbox->manapool["G"] + sbox->manapool["B"] + sbox->manapool["U"] + sbox->manapool["W"] + sbox->manapool["ANY"];

		while(sbox->countInZone("Cabal Ritual", HAND) > 0) {
			if((sbox->manapool["B"] > 0 || sbox->manapool["ANY"] > 0) && (sbox->manapool["TOTAL"] >= 2) )
				sbox->manapool["B"] += 1;

			sbox->getFromZone(HAND, GRAVEYARD, sbox->searchZone("Cabal Ritual", HAND));
		}
	}

	sbox->manapool["TOTAL"] = sbox->manapool["R"] + sbox->manapool["G"] + sbox->manapool["B"] + sbox->manapool["U"] + sbox->manapool["W"] + sbox->manapool["ANY"];

	return sbox;
}

std::vector<std::pair<MTGCard*,int>> libraryQuantity = {
	std::pair<MTGCard*,int>(new MTGCard("Street Wraith", "3BB", 5), /* 1 */ 4),
//	std::pair<MTGCard*,int>(new MTGCard("Gitaxian Probe", "U", 1), 4),
	std::pair<MTGCard*,int>(new MTGCard("Pact of Negation", "0", 0), /* 3 */ 2),
	std::pair<MTGCard*,int>(new MTGCard("Lotus Petal", "0", 0), 4),
	std::pair<MTGCard*,int>(new MTGCard("Simian Spirit Guide", "2R", 3), 4),
	std::pair<MTGCard*,int>(new MTGCard("Elvish Spirit Guide", "2G", 3), 4),
	std::pair<MTGCard*,int>(new MTGCard("Dark Ritual", "B", 1), 4),
	std::pair<MTGCard*,int>(new MTGCard("Cabal Ritual", "1B", 2), 4),
	std::pair<MTGCard*,int>(new MTGCard("Summoner's Pact", "0", 0), 4),
	std::pair<MTGCard*,int>(new MTGCard("Manamorphose", "1R", 2), /* 0 */ 4),
	std::pair<MTGCard*,int>(new MTGCard("Chancellor of the Annex", "4WWW", 7), /* 4 */ 2),	
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


			if( countPossibleMana( this )->manapool["TOTAL"] < 3 )
				return true;
		
			return false;
		}

		bool evaluateGame()
		{
			int count;
			std::map<std::string, unsigned int> pool;

			if( countInZone("Undercity Informer", HAND) + countInZone("Balustrade Spy", HAND) <= 0 )
				return false;

			pool = countPossibleMana( this )->manapool;
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

			while(countInZone("Gitaxian Probe", HAND) > 0) {
				getFromZone(HAND, GRAVEYARD, searchZone("Gitaxian Probe", HAND));
				drawCard(1);
				counter["LIFE"] -= 2;
			}

			pool = countPossibleMana( this )->manapool;
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

				while(countInZone("Street Wraith", HAND) > 0) {
					getFromZone(HAND, GRAVEYARD, searchZone("Street Wraith", HAND));
					drawCard(1);
					counter["LIFE"] -= 2;
				}

				while(countInZone("Gitaxian Probe", HAND) > 0) {
					getFromZone(HAND, GRAVEYARD, searchZone("Gitaxian Probe", HAND));
					drawCard(1);
					counter["LIFE"] -= 2;
				}

				drawCard(1);

				pool = countPossibleMana( this )->manapool;
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

	result["LAST_CHOSEN"] = 0;

	for(int i = 2;i < arguments_count;i++) {
		if(!strncmp(arguments[i], "--sample", 10))
			result["SAMPLE"] = 1;

		if(!strncmp(arguments[i], "--turn=", 7)) {
			result["TRY_TURN"] = strtol(arguments[i]+7, NULL, 10);
			result["LAST_CHOSEN"] = 0;
		}

		if(!strncmp(arguments[i], "--hand-size=", 12)) {
			result["HAND_SIZE"] = strtol(arguments[i]+12, NULL, 10);
			result["LAST_CHOSEN"] = 1;
		}

		if(!strncmp(arguments[i], "--total-mana=", 13)) {
			result["MANA_REQUEST"] = strtol(arguments[i]+13, NULL, 10);
			result["LAST_CHOSEN"] = 2;
		}

		if(!strncmp(arguments[i], "--has-card=", 11)) {
			result["CATCH_CARD"] = i;
			result["LAST_CHOSEN"] = 3;
		}

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
		}
	}
}

void
parseResult(Environment* env)
{
	bool bound[4] = { false, false, false, false };
	bool cardc = true;

	if( result["BOUND"] ) {
		bound[0] = bound[1] = bound[2] = bound[3] = true;
	}

	for(int j = 0, last = 0, seq = result["BOUND_VAR"];j < 4;j++)
	{
		switch(seq % 10) {
			case 0:
			if( result["TRY_TURN"] && env->counter["TURN"] == (unsigned)result["TRY_TURN"] && (bound[last] || !j)) {
				result["TURN_COUNT"]++;
				if( result["SAMPLE"] == 1 && result["LAST_CHOSEN"] == 0)
					env->showZone(HAND, "\e[0;36mTurn\e[0m");
				bound[seq % 10] = true;
			}
			break;
			case 1:
			if( result["HAND_SIZE"] && env->zone[HAND].size() == (unsigned)result["HAND_SIZE"] && (bound[last] || !j)) {
				result["HAND_COUNT"]++;
				if( result["SAMPLE"] == 1 && result["LAST_CHOSEN"] == 1)
					env->showZone(HAND, "\e[0;31mHand Count\e[0m");
				bound[seq % 10] = true;
			}
			break;
			case 2:
			if( result["MANA_REQUEST"] && env->manapool["TOTAL"] == (unsigned)result["MANA_REQUEST"] && (bound[last] || !j)) {
				result["MANA_REQ_COUNT"]++;
				if( result["SAMPLE"] == 1 && result["LAST_CHOSEN"] == 2)
					env->showZone(HAND, "\e[0;32mMana Pool\e[0m");
				bound[seq % 10] = true;
			}
			break;
			case 3:
			if( result["CATCH_CARD"] && (bound[last] || !j)) {
				std::vector<std::string> tokens1 = split( arguments[result["CATCH_CARD"]]+11, "," );

				for(unsigned int i = 0;i < tokens1.size() && cardc;i++) {
					bool cardc2 = false;

					if(tokens1[i].find('|') != std::string::npos) {
						std::vector<std::string> tokens2 = split( tokens1[i].c_str(), "|" );
						for(unsigned int j = 0;j < tokens2.size() && !cardc2;j++) {
							cardc2 = cardc2 || env->isInZone(tokens2[i], HAND);
						}
					} else
						cardc2 = env->isInZone(tokens1[i], HAND);

					cardc = cardc && cardc2;
				}

				if( cardc ) {
					result["CATCHC_COUNT"]++;
					if( result["SAMPLE"] == 1 && result["LAST_CHOSEN"] == 3)
						env->showZone(HAND, "\e[0;33mWITH CARDS\e[0m");
					bound[seq % 10] = true;
				}
			}
			break;
		}

		last = seq % 10;
		seq /= 10;
	}

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
	std::cout << "========= OOPS ALL SPELLS!! ==========\n";
	if(result["SHOW_RESULT"] == 0)
		printResult1( env );
	else
		printResult2( env );
}
