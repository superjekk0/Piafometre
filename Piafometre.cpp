#include "Jeu.h"

int WinMain()
{
	const std::array <std::string, 3> constantesTextuelles{ "reglages.txt", "ik" };
	//La première constante correspond au fichier de réglages
	std::fstream ouvertureReglages{ constantesTextuelles[0] };
	//ouvertureReglages.open(constantesTextuelles[0], std::ios::out);

	plog::init(plog::debug, "log.txt");
	PLOGI << "Program started";
	Jeu* jeu{ new (std::nothrow) Jeu {ouvertureReglages} };
	if (!jeu)
	{
		PLOGE << "No available memory for main object";
		return EXIT_FAILURE;
	}
	if (jeu->principal() != 0)
	{
		return EXIT_FAILURE;
	}
	delete jeu;
	PLOGI << "Program shut down";
	return 0;
}