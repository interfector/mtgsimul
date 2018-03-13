#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <map>
#include <cassert>
#include <Environment.hpp>
#include "TestDeck.hpp"
//#include "Charbelcher.hpp"

char** arguments;
int arguments_count;

int
main(int argc,char** argv)
{
	int k, count;
	DeckEnvironment* env = new DeckEnvironment();

	if( argc < 2 )
		return 1;

	count = strtol(argv[1], NULL, 10);
	arguments = argv;
	arguments_count = argc;

	initializeResult();

	for(k = 0;k < count;k++)
	{
		double progress = (double)k/(double)count;

		if( k != count-1 )
			std::cout << "[" << "/-\\|"[k % 4] << "] ";
		else
			std::cout << "[!] ";
		std::cout << int(progress * 100.0) << " %\r";
		std::cout.flush();

		for(int i = 0;i <= env->mulliganCount;i++) {
			env = new DeckEnvironment();

			env->drawCard(7-i);

		#ifdef _DEBUG
			std::cout << "\n===== HAND =====\n";
			for(auto card = env->zone[HAND].begin(); card != env->zone[HAND].end(); ++card) {
				std::cout << (*card)->name << "\n";
			}
		#endif

			if(!env->mulliganFilter() || i == env->mulliganCount) {
				if( env->evaluateGame() ) {
				#ifdef _DEBUG
					std::cout << "\nHAND CAN WIN AT TURN " << env->counter["TURN"] << " with " << i << " mulligan!!!\n";
				#endif
					env->counter["MULLIGAN"] = i;
	
					parseResult( env );
	
					break;
				}
			}
		}	
	}

	env->counter["TOTAL_TRY"] = count;

	std::cout << std::endl;

	printResult( env );

	return 0;
}
