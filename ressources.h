#ifndef RESSOURCES
#define RESSOURCES

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <fstream>
#include <array>
#include <vector>
#include <thread>
#include <bitset>
#include <iostream>
#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <string_view>
#include <chrono>
#include <functional>
#include <cassert>
#include <algorithm>
#include <memory>

enum class Langue {
	fr,
	en,
	max
};

class Checkpoint {
private:
	sf::Vector2f m_CoordonneesCamera{ 0.f, 0.f };
	sf::Vector2f m_CoordonneesJoueur{ 0.f, 0.f };
	std::vector<sf::Vector2f> m_CoordonneesArrierePlan;
	bool m_checkpointActif{ false };
public:
	Checkpoint() = default;
	Checkpoint(const sf::Vector2f& pCoordonneesCamera, const sf::Vector2f& pCoordonneesJoueur, const std::vector<sf::Sprite>& pEnsembleArrierePlan)
	{
		m_CoordonneesCamera = pCoordonneesCamera;
		m_CoordonneesJoueur = pCoordonneesJoueur;
		m_CoordonneesArrierePlan.resize(pEnsembleArrierePlan.size());
		for (int i{ 0 }; i < pEnsembleArrierePlan.size(); ++i)
			m_CoordonneesArrierePlan[i] = pEnsembleArrierePlan[i].getPosition();
	}

	void reinitialiser()
	{
		m_CoordonneesCamera = sf::Vector2f(0.f, 0.f);
		m_CoordonneesJoueur = sf::Vector2f(0.f, 0.f);
		m_checkpointActif = false;
	}

	void miseAJourCheckpoint(const sf::Vector2f& pCoordonneesCamera, const sf::Vector2f& pCoordonneesJoueur, const std::vector<sf::Sprite>& pEnsembleArrierePlan)
	{
		m_CoordonneesCamera = pCoordonneesCamera;
		m_CoordonneesJoueur = pCoordonneesJoueur;
		m_checkpointActif = true;
		m_CoordonneesArrierePlan.resize(pEnsembleArrierePlan.size());
		for (int i{ 0 }; i < pEnsembleArrierePlan.size(); ++i)
			m_CoordonneesArrierePlan[i] = pEnsembleArrierePlan[i].getPosition();
	}

	const sf::Vector2f coordonneesCamera()
	{
		return m_CoordonneesCamera;
	}

	const sf::Vector2f coordonneesJoueur()
	{
		return m_CoordonneesJoueur;
	}

	const std::vector<sf::Vector2f> coordonneesArrierePlan()
	{
		return m_CoordonneesArrierePlan;
	}

	const bool checkpointActif()
	{
		return m_checkpointActif;
	}
};

enum class PositionJeu {
	accueil,
	credits,
	options,
	remmapage,
	jeu,
	chargement,
	pause,
	max
};

enum class TypePlateforme {
	solide,
	semiSolide,
	objet,
	checkPoint,
	pics,
	finTableau,
	max
};

class Plateforme {
public:
	bool visible;
	sf::Sprite sprite;
	TypePlateforme comportement;
};

//class FilExecution {
//private:
//	std::thread m_filExecution;
//
//public:
//	FilExecution(const std::thread& pFilExecution)
//	{
//		m_filExecution = pFilExecution;
//	}
//
//	~FilExecution()
//	{
//		if (m_filExecution.joinable())
//			m_filExecution.join();
//	}
//};
//
enum Oiseau {
	vole_gauche = 0,
	vole_droite = 1,
	marche_gauche = 2,
	marche_droite = 3,
	max = 4
};

enum FrameDrapeau {
	inactif = -1,
	frame1 = 0,
	frame2 = 1
};

struct ObjetADessiner {
	std::vector <sf::Sprite> arrierePlan;
	std::vector <Plateforme> avantPlan;
	sf::Sprite joueur;
	std::vector <sf::Text> hud;
	Langue langue;
	sf::Color couleur;
	sf::RectangleShape ecranNoir;
	std::vector<sf::Texture> textures;
	sf::View camera;
	sf::Font police;
	PositionJeu positionDansJeu;
};

struct Moteur {
	int positionJoueurX;
	int positionJoueurY;
	int maxCameraX;
	int maxCameraY;
	int minCameraX;
	int minCameraY;
	int niveau;
	int nbVie;
	Checkpoint checkpoint;
};


using Clv = sf::Keyboard;									//La classe sf::Keyboard (Clavier), en somme
using ensembleTouches = std::array<sf::Keyboard::Key, 7>;
using fonctionsRessources = std::function<std::string(const int&)>;
using touchesActives = std::vector<bool>;

constexpr int vecteurNul{ 0 };

namespace utilitaire {
	constexpr unsigned int limiteFramerate{ 60 };
	constexpr int tempsParImage{ 1000000 / 60 };				//Temps en microsecondes
	constexpr int deplacement{ 3 };
}


bool verifTouches(const ensembleTouches& pTouches);
void resetTouches(ensembleTouches& pTouches);
//void surveillanceFPS(bool& pThreadsActifs, sf::Text& pMoniteurFPS, const sf::CircleShape& cercle);
void deplacementAChoisir(touchesActives& touchesActionnees, int& index, int& indexMax, bool& peutDeplacer, ObjetADessiner& spritesEtFond, fonctionsRessources& ptrFcnFichier, ensembleTouches& pTouches, bool& threadsActifs, sf::Event& pEvenement, Moteur& moteur, std::bitset<3>& touchesNonRepetables);
void deplacement(const touchesActives& touchesActionnees, ObjetADessiner& spritesEtFond, bool& peutDeplacer, const bool& threadActifs, Moteur& moteur, std::bitset<3>& touchesNonRepetables);
void detectionEvenement(const sf::Event& evenementJeu, bool& threadsActifs, bool& peutDeplacer, touchesActives& touchesActionnees, const ensembleTouches& pTouches, sf::RenderWindow& pFenetre, std::bitset<3>& touchesNonRepetables);
void deplacementMenus(touchesActives& touchesActionnees, int& index, int& indexMax, bool& peutDeplacer, ObjetADessiner& spritesEtFond,fonctionsRessources& ptrFcnFichier, ensembleTouches& pTouches, bool& threadsActifs, sf::Event& pEvenement, Moteur& moteur, std::bitset<3>& toucheNonRepetables);
std::string nomDossierJeu(const PositionJeu& positionDansJeu, fonctionsRessources& fncFichiersTexte, const bool& pImageTouche);
void verifMenu(ObjetADessiner& ensemble, int& index, std::vector<std::wstring>& textesHUD, const ensembleTouches& pTouches);
std::wstring nomFichierImageTouches(const Clv::Key& pTouche, const Langue& langue);
void placementObjets(ObjetADessiner& spritesEtTextures);
std::string chargementTextures(const std::string& langue, const PositionJeu& position);
float getWidth(const sf::Texture& texture);
float getWidth(const sf::Sprite& sprite);
float getHeight(const sf::Texture& texture);
float getHeight(const sf::Sprite& sprite);
void creationLimiteCamera(Moteur& moteur);
void ecranChargement(ObjetADessiner& spritesEtFond, Moteur& moteur, bool& peutDeplacer);
void chargementTexteHUD(std::vector<std::wstring>& textesHUD, ObjetADessiner& ensemble, const ensembleTouches& pTouches, const int index);

class Jeu {
private:

	ensembleTouches touches{};
	int vitesseDeBase{ 0 };
	int indexMenus{ 0 }; //Indique l'index dans la section du jeu
	int indexMaxMenu{ 3 };			//Indique dans quelle section du jeu on est
	bool threadsActifs{ true };
	bool deplacementActif{ false };
	//int niveau{ 0 };

	sf::RenderWindow* fenetre{ new (std::nothrow) sf::RenderWindow {sf::VideoMode(1280, 720), "Piafometre", sf::Style::Default} };

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
	//std::bitset<8> touchesActionnees{ 0b0000'0000 };
	std::fstream* fichierReglages; //Pour être capable d'écrire dedans lors de la fermeture
	ObjetADessiner* sprites{ new (std::nothrow) ObjetADessiner };
	//sf::View camera{ sf::FloatRect(0,0, 1280, 720) };
	Moteur* moteurJeu{ new (std::nothrow) Moteur{} };
	void rendu()
	{
		fenetre->clear(sprites->couleur);
		fenetre->setView(sprites->camera);
		for (int i{ 0 }; i < sprites->arrierePlan.size(); ++i)
		{
			fenetre->draw(sprites->arrierePlan[i]);
		}
		for (int i{ 0 }; i < sprites->avantPlan.size(); ++i)
		{
			if (sprites->avantPlan[i].visible)
				fenetre->draw(sprites->avantPlan[i].sprite);
		}
		fenetre->draw(sprites->joueur);
		fenetre->draw(sprites->ecranNoir);
		for (int i{ 0 }; i < sprites->hud.size(); ++i)
		{
			//PLOGD << "index : " << i;
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
		sprites->camera.setCenter(fenetre->getSize().x / 2, fenetre->getSize().y / 2);
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
		delete sprites;
		PLOGI << "sprites Structure deleted";
		PLOGI << "Main object deleted";
	}

	static void preparerQuitter(bool& pThreadsActifs, touchesActives& pLogicielContinue, bool& pDeplacementActif)
	{
		pThreadsActifs = false;
		pDeplacementActif = !pDeplacementActif;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		pLogicielContinue[7] = true;
	}

	int principal()
	{
		sprites->positionDansJeu = PositionJeu::accueil;
		std::function<std::string(const int&)> ptrFcn{};

		std::unique_ptr<sf::Event> evenementFenetre{ new (std::nothrow) sf::Event };
		std::unique_ptr<std::thread> detecTouches{ new (std::nothrow) std::thread {
			detectionEvenement, std::ref(*evenementFenetre),
			std::ref(threadsActifs), std::ref(deplacementActif) , std::ref(touchesActionnees),
			std::ref(touches), std::ref(*fenetre), std::ref(toucheNonRepetables)
		}};
		std::unique_ptr<std::thread> mouvementMenu{ new (std::nothrow) std::thread
		{deplacementAChoisir, std::ref(touchesActionnees), std::ref(indexMenus),
			std::ref(indexMaxMenu) , std::ref(deplacementActif),
			std::ref(*sprites), std::ref(ptrFcn),
			std::ref(touches), std::ref(threadsActifs),
			std::ref(*evenementFenetre), std::ref(*moteurJeu), std::ref(toucheNonRepetables)} };
		//Permet de changer entre les deux types de déplacement

		if ((!evenementFenetre) || (!fenetre) || (!detecTouches) || (!mouvementMenu) || (!sprites))
		{
			PLOGE << "A dynamic variable is NULL";
			return -1;
		}

		sprites->arrierePlan.resize(0);
		sprites->avantPlan.resize(0);
		sprites->hud.resize(4);
		sprites->police.loadFromFile("resources/font/verdanai.ttf");
		//fenetre->setFramerateLimit(60);

		//moteurJeu.longueurEcran = fenetre->getSize().x;
		//moteurJeu.hauteurEcran = fenetre->getSize().y;

		detecTouches->detach();
		mouvementMenu->detach();
		//PLOGD << L"Fil de détection des touches: " << detecTouches->;
		//PLOGD << "Fil des mouvements: " << mouvementMenu->get_id();

		sf::Clock debutCycle{};
		while (!touchesActionnees[7])
		{
			fenetre->pollEvent(*evenementFenetre);
			rendu();
			std::this_thread::sleep_for(std::chrono::microseconds(utilitaire::tempsParImage - debutCycle.restart().asMicroseconds())); //Se met à jour à chaque 1/60ème de seconde
		}

		fenetre->close();
		delete moteurJeu;
		delete fenetre;

		//delete evenementFenetre;
		//delete mouvementMenu;
		//delete detecTouches;
		std::this_thread::sleep_for(std::chrono::seconds(2));
		return 0;
	}
};

#endif 