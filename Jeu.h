#ifndef JEU_H
#define JEU_H

#include "ressources.h"
class Jeu {
private:

	ensembleTouches touches{};
	int vitesseDeBase{ 0 };
	int indexMenus{ 0 }; //Indique l'index dans la section du jeu
	int indexMaxMenu{ 3 };			//Indique dans quelle section du jeu on est
	bool threadsActifs{ true };
	bool deplacementActif{ false };
	//int niveau{ 0 };

	std::unique_ptr<sf::RenderWindow> fenetre{ new (std::nothrow) sf::RenderWindow {sf::VideoMode(1280, 720), "Piafometre", sf::Style::Default} };

	//Index 0: touche de confirmation
	//Index 1: touche de pause
	//Index 2: touche de saut
	std::bitset<3> toucheNonRepetables{ 0b000 };

	//PositionJeu sectionJeu{ PositionJeu::accueil };
	//sf::Text test{};
	//index 0: touche pour le bouton de gauche
	//index 1: touche pour le bouton de droite
	//index 2: touche pour le bouton de haut
	//index 3: touche pour le bouton du bas
	//index 4: touche pour le bouton de confirmation
	//index 5: touche pour le bouton de saut
	//index 6: touche pour le bouton de pause
	//index 7: indique lorsque le programme doit arrêter
	touchesActives touchesActionnees{ false, false, false, false, false, false, false, false };
	std::fstream* fichierReglages; //Pour être capable d'écrire dedans lors de la fermeture
	std::unique_ptr<ObjetADessiner> sprites{ new (std::nothrow) ObjetADessiner };
	std::unique_ptr<Moteur> moteurJeu{ new (std::nothrow) Moteur{} };
	
	void rendu()
	{
		fenetre->clear(sprites->couleur);
		fenetre->setView(sprites->camera);
		for (int i{0}; i < sprites->arrierePlan.size(); ++i)
		{
			fenetre->draw(sprites->arrierePlan[i]);
		}
		for (int i{0}; i < sprites->avantPlan.size(); ++i)
		{
			if (sprites->avantPlan[i].visible)
				fenetre->draw(sprites->avantPlan[i].sprite);
		}
		if (deplacementActif)
			fenetre->draw(sprites->joueur);
		fenetre->draw(sprites->ecranNoir);
		for (int i{ 0 }; i < sprites->hud.size(); ++i)
		{
			fenetre->draw(sprites->hud[i]);
		}
		fenetre->display();
	}

	Langue traductionSymboleLangue(const std::string& langue)
	{
		if (langue == "fr")
			return Langue::fr;
		else if (langue == "en")
			return Langue::en;
		else
			PLOGE << "Unknown language. Language settings reset to French (fr)";
		return Langue::fr;
	}


public:

	static std::string symboleLangue(const Langue& langue)
	{
		switch (langue)
		{
		case Langue::fr:
			return "fr";
		case Langue::en:
			return "en";
		default:
			PLOGE << "Unknown language. Will return French (fr)";
			return "fr";
		}
	}


	Jeu(std::fstream& reglages)
	{
		sf::Image icone{};
		fichierReglages = &reglages;
		if (!fenetre)
		{
			PLOGE << "Not enough memory for the window class";
			std::abort();
		}
		if (!moteurJeu)
		{
			PLOGE << "Not enough memory for the Game Engine";
			std::abort();
		}
		fenetre->setTitle(L"Piafomètre");
		if (!icone.loadFromFile("resources/icone.png"))
		{
			PLOGE << "Unable to load resources/icone.png";
			std::abort();
		}
		fenetre->setIcon(icone.getSize().x, icone.getSize().y, icone.getPixelsPtr());
		std::string temp{};
		for (int i{ 0 }; fichierReglages && i < touches.size(); ++i)
		{

			reglages >> temp;
			std::stringstream conversion{ temp };
			int produit{};

			if (!(conversion >> produit)) produit = 0;

			touches[i] = static_cast<Clv::Key>(produit);
		}

		//sf::Font police{};
		//if (!police.loadFromFile("resources/font/times-new-roman.ttf"))
		//	PLOGE << "Font file not found";

		if (!sprites)
		{
			PLOGE << "Not enough memory for Sprite structure";
			std::abort();
		}
		if (fichierReglages)
			reglages >> temp;

		sprites->langue = traductionSymboleLangue(temp);
		sprites->ecranNoir.setFillColor(sf::Color(0, 0, 0, 0));
		sprites->ecranNoir.setSize(static_cast<sf::Vector2f>(fenetre->getSize()));
		sprites->camera.setCenter(sprites->ecranNoir.getSize() / 2.f);
		sprites->camera.setSize(fenetre->getSize().x, fenetre->getSize().y);

		sprites->textures.resize(2);
		sprites->textures.at(0).loadFromFile("resources/texts/" + symboleLangue(sprites->langue) + "/home/normal.png");
		sprites->textures.at(1).loadFromFile("resources/texts/" + symboleLangue(sprites->langue) + "/home/selected.png");
		sprites->couleur = sf::Color(128, 128, 128);

		//sprites->hud.resize(4);

		if (verifTouches(touches))
			resetTouches(touches);
		fichierReglages->close();

		PLOGI << "Main object initialized";
	}

	~Jeu()
	{
		fichierReglages->open("reglages.txt", std::ios::out);
		fichierReglages->clear();
		for (int i{ 0 }; i < touches.size() && fichierReglages; ++i)
		{
			*fichierReglages << touches[i] << ' ';
		}
		*fichierReglages << symboleLangue(sprites->langue);
		PLOGI << "sprites Structure deleted";
		PLOGI << "Main object deleted";
	}

	static void preparerQuitter(bool& pThreadsActifs, touchesActives& pLogicielContinue, bool& pDeplacementActif)
	{
		pThreadsActifs = false;
		pDeplacementActif = !pDeplacementActif;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		//pLogicielContinue[7] = true;
	}

	int principal()
	{
		sprites->positionDansJeu = PositionJeu::accueil;

		std::unique_ptr<sf::Event> evenementFenetre{ new (std::nothrow) sf::Event };
		std::unique_ptr<std::thread> mouvementMenu{ new (std::nothrow) std::thread
		{deplacementAChoisir, std::ref(touchesActionnees), std::ref(indexMenus),
			std::ref(indexMaxMenu) , std::ref(deplacementActif),
			std::ref(*sprites),
			std::ref(touches), std::ref(threadsActifs),
			std::ref(*evenementFenetre), std::ref(*moteurJeu), std::ref(toucheNonRepetables)} };
		//Permet de changer entre les deux types de déplacement


		if ((!evenementFenetre) || (!fenetre) || (!mouvementMenu) || (!sprites))
		{
			PLOGE << "A dynamic variable is NULL";
			return -1;
		}

		sprites->arrierePlan.resize(0);
		sprites->avantPlan.resize(0);
		sprites->hud.resize(4);
		sprites->police.loadFromFile("resources/font/verdanai.ttf");

		//evenementValide = 
			//fenetre->pollEvent(*evenementFenetre);
		//detecTouches->detach();
		mouvementMenu->detach();

		sf::Clock debutCycle;
		while (threadsActifs)
		{
			detectionEvenement(*evenementFenetre, threadsActifs, deplacementActif,
				touchesActionnees, touches, *fenetre, toucheNonRepetables);
			rendu();
			std::this_thread::sleep_for(std::chrono::microseconds(tempsParImage - debutCycle.restart().asMicroseconds())); //Se met à jour à chaque 1/60ème de seconde
		}

		fenetre->close();
		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		return 0;
	}
};


#endif
