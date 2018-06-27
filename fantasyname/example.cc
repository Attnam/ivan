#include "namegen.h"

#include <stdio.h>
#include <clocale>
#include <iostream>


int main(int argc, char **argv)
{
	int num = 1;
	std::setlocale(LC_CTYPE, "");
	if (argc < 2 || argc > 3 || (argc == 3 && (num = atoi(argv[2])) && num == -1)) {
		std::cerr << "Usage: " << argv[0] << " <pattern> [num]\n";
		std::cerr << "  pattern - Template for names to generate.\n";
		std::cerr << "  num     - Number of names to generate.\n";
		return 64;
	}

	char * pattern = argv[1];
	// std::cerr << "> pattern = " << pattern << "\n";

	NameGen::Generator generator(pattern);

	std::cerr << "> combinations = " << generator.combinations() << "\n";
	// std::cerr << "> min = " << generator.min() << "\n";
	// std::cerr << "> max = " << generator.max() << "\n";

	for (int i = 0; i < num; i++) {
		std::cout << generator.toString() << "\n";
	}

	return 0;
}
