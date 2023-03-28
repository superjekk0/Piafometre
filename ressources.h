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
#include <filesystem>

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

	sf::Vector2f coordonneesCamera() const
	{
		return m_CoordonneesCamera;
	}

	sf::Vector2f coordonneesJoueur() const
	{
		return m_CoordonneesJoueur;
	}

	std::vector<sf::Vector2f> coordonneesArrierePlan() const
	{
		return m_CoordonneesArrierePlan;
	}

	bool checkpointActif() const
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
	bool touche;
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

	constexpr int tempsParImage{ 1000000 / 60 };				//Temps en microsecondes
namespace utilitaire {
	constexpr unsigned int limiteFramerate{ 60 };
	constexpr int deplacement{ 3 };
}

bool verifTouches(const ensembleTouches& pTouches);
void resetTouches(ensembleTouches& pTouches);
//void surveillanceFPS(bool& pThreadsActifs, sf::Text& pMoniteurFPS, const sf::CircleShape& cercle);
void deplacementAChoisir(touchesActives& touchesActionnees, int& index, int& indexMax, bool& peutDeplacer, ObjetADessiner& spritesEtFond, fonctionsRessources& ptrFcnFichier, ensembleTouches& pTouches, bool& threadsActifs, sf::Event& pEvenement, Moteur& moteur, std::bitset<3>& touchesNonRepetables);
void detectionEvenement(sf::Event& evenementJeu, bool& threadsActifs, bool& peutDeplacer, touchesActives& touchesActionnees, const ensembleTouches& pTouches, sf::RenderWindow& pFenetre, std::bitset<3>& touchesNonRepetables);
std::string nomDossierJeu(const PositionJeu& positionDansJeu, fonctionsRessources& fncFichiersTexte, const bool& pImageTouche);
std::wstring nomFichierImageTouches(const Clv::Key& pTouche, const Langue& langue);
std::string chargementTextures(const std::string& langue, const PositionJeu& position);
float getWidth(const sf::Texture& texture);
float getWidth(const sf::Sprite& sprite);
float getHeight(const sf::Texture& texture);
float getHeight(const sf::Sprite& sprite);
void creationLimiteCamera(Moteur& moteur);
void verifFichierExiste(const std::string& chemin);
sf::Keyboard::Key assignationTouche(const sf::Event& pEvenement, const sf::Keyboard::Key& toucheExclue, sf::Clock& cycle);
bool confirmerReiniTouches(sf::Event& pEvenement, sf::Clock& cycle);


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
		//sf::Clock minuterie;
		//while (threadsActifs)
		//{
			fenetre->clear(sprites->couleur);
			fenetre->setView(sprites->camera);
			for (auto& arrierePlan : sprites->arrierePlan)
			{
				fenetre->draw(arrierePlan);
			}
			for (auto& avantPlan : sprites->avantPlan)
			{
				if (avantPlan.visible)
					fenetre->draw(avantPlan.sprite);
			}
			fenetre->draw(sprites->joueur);
			fenetre->draw(sprites->ecranNoir);
			for (auto& hud : sprites->hud)
			{
				fenetre->draw(hud);
			}
			fenetre->display();
			//std::this_thread::sleep_for(std::chrono::microseconds(tempsParImage - minuterie.restart().asMicroseconds()));
		//}
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
		//bool evenementValide{};
		sprites->positionDansJeu = PositionJeu::accueil;
		std::function<std::string(const int&)> ptrFcn{};

		std::unique_ptr<sf::Event> evenementFenetre{ new (std::nothrow) sf::Event };
		//std::unique_ptr<std::thread> dessinEcran{new (std::nothrow) std::thread {
		//	[this]() {rendu(); }
		//}};
		std::unique_ptr<std::thread> mouvementMenu{ new (std::nothrow) std::thread
		{deplacementAChoisir, std::ref(touchesActionnees), std::ref(indexMenus),
			std::ref(indexMaxMenu) , std::ref(deplacementActif),
			std::ref(*sprites), std::ref(ptrFcn),
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
		//PLOGD << L"Fil de détection des touches: " << detecTouches->;
		//PLOGD << "Fil des mouvements: " << mouvementMenu->get_id();

		sf::Clock debutCycle;
		//dessinEcran->detach();
		while (!touchesActionnees[7])
		{
			//Remplacer par la fonction detectionEvenement
			//fenetre->pollEvent(*evenementFenetre);
			detectionEvenement(*evenementFenetre, threadsActifs, deplacementActif,
				touchesActionnees, touches, *fenetre, toucheNonRepetables);
			rendu();
			std::this_thread::sleep_for(std::chrono::microseconds(tempsParImage - debutCycle.restart().asMicroseconds())); //Se met à jour à chaque 1/60ème de seconde
		}

		fenetre->close();
		//delete moteurJeu;
		//delete fenetre;

		//delete evenementFenetre;
		//delete mouvementMenu;
		//delete detecTouches;
		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		return 0;
	}
};

class MoteurMenu {
private:
#pragma region PROPRIETES

	touchesActives& m_touchesActionnees;
	int m_index;
	int m_indexMax;
	bool& m_peutDeplacer;
	bool& m_threadsActifs;
	ObjetADessiner& m_sprites;
	ensembleTouches& m_touches;
	Moteur& m_moteur;
	std::bitset<3>& m_touchesNonRepetables;
	sf::Event& m_evenements;
	std::vector<std::wstring> m_textesHUD;

#pragma endregion

#pragma region METHODES_PRIVEES

	void chargementTexteHUD()
	{
		std::string chemin{ chargementTextures(Jeu::symboleLangue(m_sprites.langue), m_sprites.positionDansJeu) };
		if (m_sprites.positionDansJeu != PositionJeu::chargement) verifFichierExiste(chemin);
		std::wfstream fichier{ chemin };
		std::wstring texteActuel;

		switch (m_sprites.positionDansJeu)
		{
		case PositionJeu::accueil:
			for (int i{ 0 }; i < m_textesHUD.size() && fichier; ++i)
			{
				std::wstring ligneTemp;
				fichier >> ligneTemp;
				m_textesHUD[i] = ligneTemp;
				m_sprites.hud[i].setString(m_textesHUD[i]);
			}
			break;
		case PositionJeu::options:
			for (int i{ 0 }; i < m_textesHUD.size() && fichier; i += 2)
			{
				std::wstring ligneTemp;
				fichier >> ligneTemp;
				m_textesHUD[i] = ligneTemp;
				m_sprites.hud[i].setString(m_textesHUD[i]);
			}
			for (int i{ 1 }; i < m_textesHUD.size(); i += 2)
			{
				std::wstring ligneTemp{ nomFichierImageTouches(m_touches[i / 2], m_sprites.langue) };
				m_textesHUD[i] = ligneTemp;
				m_sprites.hud[i].setString(m_textesHUD[i]);
			}
			break;
		case PositionJeu::credits:
			while (fichier)
			{
				std::wstring ligneTemp;
				std::getline(fichier, ligneTemp);
				texteActuel += ligneTemp + L'\n';
			}
			if (m_textesHUD.size() > 0)
			{
				m_textesHUD[0] = texteActuel;
				m_sprites.hud[0].setString(m_textesHUD[0]);
			}
			break;
		case PositionJeu::remmapage:
			while (fichier)
			{
				std::wstring ligneTemp;
				std::getline(fichier, ligneTemp);
				auto balise{ (m_index == m_touches.size()) ? ligneTemp.find(L"- 7") : ligneTemp.find(L"- a") };
				if (balise != ligneTemp.npos)
				{
					texteActuel += ligneTemp.substr(0, balise - 1);
					balise = texteActuel.find(L"La touche actuelle est");
					if (balise != texteActuel.npos)
						texteActuel += L' ' + nomFichierImageTouches(m_touches[m_index], m_sprites.langue);
				}
				texteActuel += L'\n';
			}
			if (m_textesHUD.size() > 0)
			{
				m_textesHUD[0] = texteActuel;
				m_sprites.hud[0].setString(m_textesHUD[0]);
			}
		default:
			break;
		}
	}

	// Placement des objets du HUD en dehors du moteur de jeu
	void placementObjets()
	{
		m_sprites.camera.setCenter(m_sprites.camera.getSize().x / 2, m_sprites.camera.getSize().y / 2);
		switch (m_sprites.positionDansJeu)
		{
		case PositionJeu::accueil:
			for (int i{ 0 }; i < m_sprites.hud.size(); ++i)
			{
				m_sprites.hud[i].setPosition(800, 100 + i * 100);
			}
			break;
		case PositionJeu::options:
			for (int i{ 0 }; i < m_sprites.hud.size(); i += 2)
			{
				m_sprites.hud[i].setPosition(sf::Vector2f(250, 50 + 40 * i));
			}
			for (int i{ 1 }; i < m_sprites.hud.size(); i += 2)
			{
				m_sprites.hud[i].setPosition(sf::Vector2f(900, m_sprites.hud[i - 1].getPosition().y));
			}
			break;
		case PositionJeu::remmapage:
			m_sprites.hud[0].setFillColor(sf::Color());
			m_sprites.hud[0].setPosition(400, 300);
			//if (spritesEtTextures.hud.size() >= 2) spritesEtTextures.hud[1].setPosition(850, 580);
			break;
		case PositionJeu::chargement:
			m_sprites.hud[0].setPosition(400, 400);
			//spritesEtTextures.hud[1].setPosition(600, 400);
			break;
		case PositionJeu::credits:
			m_sprites.hud[0].setPosition(280, 200);
			break;
		}
	}

	/// <summary>
/// Permet d'afficher le menu présentement sélectionné.
/// </summary>
/// <param name="ensemble">Ensemble des sprites du HUD et des textures</param>
/// <param name="textesHUD">Ensemble des textes du HUD</param>
/// <param name="index">Indique quel texture doit être celle sélectionnée</param>
/// <param name="menu">Indique dans quelle section du jeu on se trouve. Pourrait être le menu principal, les options, le menu de pause, etc.</param>
	void verifMenu(ObjetADessiner& ensemble, int& index, std::vector<std::wstring>& textesHUD, const ensembleTouches& pTouches)
	{
		//===================================
		//
		// Très important! L'index pour le texte affiché lorsque le menu est "options" est pair tandis que pour l'image de touche
		// correspondante, elle est impaire.
		//
		//===================================

		bool toucheImage{ false };
		//std::wfstream chemin{ chargementTextures(Jeu::symboleLangue(ensemble.langue), ensemble.positionDansJeu) };
		std::wstring ligneFinale{ L"" };

		for (int i{ 0 }; i < ensemble.hud.size() && i < textesHUD.size(); ++i)
		{
			ensemble.hud[i].setFont(ensemble.police);
			switch (ensemble.positionDansJeu)
			{
			case PositionJeu::accueil:
				ensemble.hud[i].setString(static_cast<std::wstring>(textesHUD[i]));
				ensemble.hud[i].setCharacterSize(80);
				if (index == i)
				{
					ensemble.hud[i].setCharacterSize(ensemble.hud[i].getCharacterSize() * 1.15);
					ensemble.hud[i].setFillColor(sf::Color(222, 215, 43));
					ensemble.hud[i].setStyle(sf::Text::Style::Bold);
				}
				else
				{
					ensemble.hud[i].setFillColor(sf::Color(210, 210, 210));
					ensemble.hud[i].setStyle(sf::Text::Style::Regular);
				}
				break;
			case PositionJeu::options:
				ensemble.hud[i].setCharacterSize(30);
				if (index * 2 == i)
				{
					ensemble.hud[i].setCharacterSize(ensemble.hud[i].getCharacterSize() * 1.15);
					ensemble.hud[i].setFillColor(sf::Color(222, 215, 43));
					ensemble.hud[i].setStyle(sf::Text::Style::Bold);
				}
				else
				{
					ensemble.hud[i].setFillColor(sf::Color(210, 210, 210));
					ensemble.hud[i].setStyle(sf::Text::Style::Regular);
				}
				break;
			case PositionJeu::credits:
				ensemble.hud[i].setCharacterSize(45u);
				break;
			case PositionJeu::remmapage:
				ensemble.hud[i].setFillColor(sf::Color(210, 210, 210));
				ensemble.hud[i].setCharacterSize(45u);
				break;
			}
		}
		placementObjets();
	}

	void chargementNiveau()
	{
		m_sprites.hud.resize(1);
		std::string cheminNiveau{ "resources/texts/" + Jeu::symboleLangue(m_sprites.langue) + "/loading.txt" };
		verifFichierExiste(cheminNiveau);
		std::wfstream nomNiveau{ cheminNiveau }; //StreamReader nomNiveau = new StreamReader;
		std::wstring contenuNomNiveau{};
		std::wstring nomFinalNiveau{};

		while (nomNiveau)
		{
			std::getline(nomNiveau, contenuNomNiveau);
			auto indexNiveau{ contenuNomNiveau.find(L"- " + std::to_wstring(m_moteur.niveau)) };
			if (indexNiveau != contenuNomNiveau.npos)
			{
				nomFinalNiveau += contenuNomNiveau.substr(0, indexNiveau - 1) + L'\n';
			}
		}
		m_sprites.hud[0].setCharacterSize(45);
		m_sprites.hud[0].setString(nomFinalNiveau);

		switch (m_moteur.niveau)
		{
		case 1:
			m_sprites.couleur = sf::Color(119, 181, 254);
			m_sprites.textures.resize(11);
			m_sprites.avantPlan.resize(13);
			m_sprites.arrierePlan.resize(2);
			chargementTexturesNiveau();

			m_sprites.avantPlan[0].sprite.setTexture(m_sprites.textures[3]);
			m_sprites.avantPlan[0].sprite.setScale(0.75f, 0.75f);
			m_sprites.avantPlan[0].sprite.setPosition(0, 720 - m_sprites.textures[3].getSize().y);
			m_sprites.avantPlan[0].sprite.setTextureRect(sf::IntRect(0, 0, 3000, m_sprites.textures[3].getSize().y));

			m_sprites.avantPlan[1].sprite.setTexture(m_sprites.textures[2]);
			m_sprites.avantPlan[1].sprite.setTextureRect(sf::IntRect(0, 0, getWidth(m_sprites.textures[2]), getHeight(m_sprites.textures[2]) * 2.5));
			m_sprites.avantPlan[1].sprite.setScale(0.75f, 0.75f);
			m_sprites.avantPlan[1].sprite.setPosition(3 * getWidth(m_sprites.textures[3]),
				m_sprites.avantPlan[0].sprite.getPosition().y + getHeight(m_sprites.avantPlan[0].sprite) - getHeight(m_sprites.avantPlan[1].sprite));

			m_sprites.avantPlan[2].sprite.setTexture(m_sprites.textures[4]);
			m_sprites.avantPlan[2].sprite.setTextureRect(sf::IntRect(0, 0, getWidth(m_sprites.textures[4]) * 4, getHeight(m_sprites.textures[4])));
			m_sprites.avantPlan[2].sprite.setPosition(m_sprites.avantPlan[1].sprite.getPosition().x + getWidth(m_sprites.avantPlan[1].sprite),
				m_sprites.avantPlan[0].sprite.getPosition().y - getHeight(m_sprites.avantPlan[2].sprite));
			m_sprites.avantPlan[2].comportement = TypePlateforme::pics;

			m_sprites.avantPlan[3].sprite.setTexture(m_sprites.textures[2]);
			m_sprites.avantPlan[3].sprite.setScale(0.75f, .75f);
			m_sprites.avantPlan[3].sprite.setTextureRect(sf::IntRect(0, 0, m_sprites.avantPlan[0].sprite.getTextureRect().width, getHeight(m_sprites.textures[2]) * 3));
			m_sprites.avantPlan[3].sprite.setPosition(m_sprites.avantPlan[0].sprite.getPosition().x,
				m_sprites.avantPlan[0].sprite.getPosition().y + getHeight(m_sprites.avantPlan[0].sprite));

			m_sprites.avantPlan[4].sprite.setTexture(m_sprites.textures[3]);
			m_sprites.avantPlan[4].sprite.setTextureRect(sf::IntRect(0, 0, 3000, getHeight(m_sprites.textures[3])));
			m_sprites.avantPlan[4].sprite.setPosition(0,
				m_sprites.avantPlan[3].sprite.getPosition().y + getHeight(m_sprites.avantPlan[3].sprite) + 500);

			m_sprites.avantPlan[5].sprite.setTexture(m_sprites.textures[4]);
			m_sprites.avantPlan[5].sprite.setTextureRect(sf::IntRect(0, 0, m_sprites.textures[4].getSize().x, m_sprites.textures[4].getSize().y));
			m_sprites.avantPlan[5].sprite.setPosition(1500,
				m_sprites.avantPlan[4].sprite.getPosition().y - getHeight(m_sprites.avantPlan[5].sprite));
			m_sprites.avantPlan[5].comportement = TypePlateforme::pics;

			m_sprites.avantPlan[6].sprite.setTexture(m_sprites.textures[4]);
			m_sprites.avantPlan[6].sprite.setTextureRect(m_sprites.avantPlan[5].sprite.getTextureRect());
			m_sprites.avantPlan[6].sprite.setPosition(m_sprites.avantPlan[5].sprite.getPosition().x - 750,
				m_sprites.avantPlan[5].sprite.getPosition().y);
			m_sprites.avantPlan[6].comportement = TypePlateforme::pics;

			m_sprites.avantPlan[7].sprite.setTexture(m_sprites.textures[6]);
			m_sprites.avantPlan[7].sprite.setTextureRect(sf::IntRect(0, 0, getWidth(m_sprites.textures[6]) / 3, getHeight(m_sprites.textures[6])));
			m_sprites.avantPlan[7].sprite.setScale(2, 2);
			m_sprites.avantPlan[7].sprite.setPosition(m_sprites.avantPlan[5].sprite.getPosition().x + 500,
				m_sprites.avantPlan[4].sprite.getPosition().y - getHeight(m_sprites.avantPlan[7].sprite));
			m_sprites.avantPlan[7].comportement = TypePlateforme::checkPoint;

			m_sprites.avantPlan[8].sprite.setTexture(m_sprites.textures[7]);
			m_sprites.avantPlan[8].sprite.setPosition(m_sprites.avantPlan[1].sprite.getPosition().x - 100 - getWidth(m_sprites.avantPlan[8].sprite),
				m_sprites.avantPlan[0].sprite.getPosition().y - getHeight(m_sprites.avantPlan[8].sprite));
			m_sprites.avantPlan[8].comportement = TypePlateforme::objet;

			m_sprites.avantPlan[9].sprite.setTexture(m_sprites.textures[7]);
			m_sprites.avantPlan[9].sprite.setPosition(m_sprites.avantPlan[7].sprite.getPosition().x - getWidth(m_sprites.avantPlan[9].sprite) - 40,
				m_sprites.avantPlan[4].sprite.getPosition().y - getHeight(m_sprites.avantPlan[9].sprite));
			m_sprites.avantPlan[9].comportement = TypePlateforme::objet;

			m_sprites.joueur.setTexture(m_sprites.textures[1]);
			m_sprites.joueur.setTextureRect(sf::IntRect(0,
				marche_droite * getHeight(m_sprites.textures[1]) / max,
				getWidth(m_sprites.textures[1]) / 3,
				getHeight(m_sprites.textures[1]) / max));
			m_sprites.joueur.setScale(3, 3);
			m_sprites.joueur.setPosition(m_sprites.avantPlan[0].sprite.getPosition().x,
				m_sprites.avantPlan[0].sprite.getPosition().y - getHeight(m_sprites.joueur));

			m_sprites.arrierePlan[0].setTexture(m_sprites.textures[9]);
			m_sprites.arrierePlan[0].setScale(7, 7);
			m_sprites.arrierePlan[0].setPosition(0, -200);

			m_sprites.arrierePlan[1].setTexture(m_sprites.textures[10]);
			m_sprites.arrierePlan[1].setScale(7, 7);
			m_sprites.arrierePlan[1].setPosition(0, -150);

			break;
		default:
			break;
		}
	}

	void chargementTexturesNiveau()
	{
		//if (!m_sprites.textures.at(0).loadFromFile("resources/texts/" + langue + "/keyboards/" + std::to_string(niveau) + ".png"))
		//	PLOGE << "Unable to load resources/texts/" << langue << "/keyboards/" << std::to_string(niveau) << ".png";
		if (!m_sprites.textures.at(1).loadFromFile("resources/sprites/player.png"))
			PLOGE << "Unable to load resources/sprites/player.png";
		//à l'index 0 et 1 sont réservés pour l'écran de chargement
		switch (m_moteur.niveau)
		{
		case 1:
			//avant-plan
			if (!m_sprites.textures[2].loadFromFile("resources/textures/dirt.png"))
				PLOGE << "Unable to load resources/textures/dirt.png";
			if (!m_sprites.textures[3].loadFromFile("resources/textures/grass.png"))
				PLOGE << "Unable to load resources/textures/grass.png";
			if (!m_sprites.textures[4].loadFromFile("resources/textures/spikes.png"))
				PLOGE << "Unable to load resources/textures/spikes.png";
			if (!m_sprites.textures[5].loadFromFile("resources/textures/caveEntry.png"))
				PLOGE << "Unable to load resources/textures/caveEntry.png";
			if (!m_sprites.textures[6].loadFromFile("resources/sprites/checkpoint.png"))
				PLOGE << "Unable to load resources/sprites/checkpoint.png";
			if (!m_sprites.textures[7].loadFromFile("resources/sprites/powerUp.png"))
				PLOGE << "Unable to load resources/sprites/powerUp.png";
			if (!m_sprites.textures[8].loadFromFile("resources/sprites/caveEntry.png"))
				PLOGE << "Unable to load resources/sprites/caveEntry.png";

			m_sprites.textures[2].setRepeated(true);
			m_sprites.textures[3].setRepeated(true);
			m_sprites.textures[4].setRepeated(true);
			//arrière-plan
			if (!m_sprites.textures[9].loadFromFile("resources/textures/sky.png"))
				PLOGE << "Unable to load resources/textures/sky.png";
			if (!m_sprites.textures[10].loadFromFile("resources/textures/mountain.png"))
				PLOGE << "Unable to load resources/textures/mountain.png";
		default:
			break;
		}
	}

#pragma endregion


public:

#pragma region CONSTRUCTEUR

	MoteurMenu(touchesActives& pTouchesActionnees, int pIndex,
		int pIndexMax, bool& pPeutDeplacer, ObjetADessiner& pSprites,
		ensembleTouches& pTouches, Moteur& pMoteur, std::bitset<3>& pTouchesNonRepetables,
		bool& pThreadsActifs, sf::Event& pEvenement) :
		m_touchesActionnees{ pTouchesActionnees }, m_index{ pIndex }, m_indexMax{ pIndexMax },
		m_peutDeplacer{ pPeutDeplacer }, m_sprites{ pSprites }, m_touches{ pTouches },
		m_moteur{ pMoteur }, m_touchesNonRepetables{ pTouchesNonRepetables }, m_threadsActifs{ pThreadsActifs },
		m_evenements{ pEvenement }
	{
		m_textesHUD.reserve(4);
	}

#pragma endregion

#pragma region PRINCIPAL

	void deplacementMenus()
	{
		
		m_textesHUD.resize(4);
		chargementTexteHUD();

		while (!m_peutDeplacer)
		{
			sf::Clock debutCycle; //TRÈS IMPORTANT! NE PAS DÉPLACER
			//static bool imageClavier{ false };
			if (m_touchesActionnees[2])
				--m_index;
			if (m_touchesActionnees[3])
				++m_index;
			if (m_touchesActionnees[4] && !m_touchesNonRepetables.test(0))
			{
				m_touchesNonRepetables.set(0);
				switch (m_sprites.positionDansJeu)
				{
				case PositionJeu::accueil:

					switch (m_index)
					{
					case 0: //Entrer dans le jeu
						//verifMenu(m_sprites, index, ptrFcnFichier, pTouches);
						m_moteur.nbVie = 3;
						m_sprites.positionDansJeu = PositionJeu::chargement;
						//peutDeplacer = true;
						m_moteur.niveau = 1;
						ecranChargement();
						chargementTexteHUD();
						return;
						break;
					case 1: //Entrer dans les options
						//verifMenu(m_sprites, index, ptrFcnFichier, pTouches);
						m_sprites.ecranNoir.setFillColor(sf::Color::Color(0, 0, 0, 128));
						m_sprites.hud.resize(15);
						m_textesHUD.resize(15);
						m_sprites.textures.resize(9);
						m_sprites.positionDansJeu = PositionJeu::options;
						m_index = 0;
						m_indexMax = 7;
						chargementTexteHUD();
						PLOGI << "Exiting main menu";
						PLOGI << "Entering in settings region";
						break;
					case 2: //Quitter le logiciel
						Jeu::preparerQuitter(m_threadsActifs, m_touchesActionnees, m_peutDeplacer);
						//threadsActifs = false;
						//std::this_thread::sleep_for(100ms);
						//touchesActionnees.set(7);
						return;
						break;
					case 3: //Entrer dans les crédits
						//verifMenu(m_sprites, index, ptrFcnFichier, pTouches);
						m_sprites.ecranNoir.setFillColor(sf::Color(0, 0, 0, 128));
						m_sprites.positionDansJeu = PositionJeu::credits;
						m_textesHUD.resize(1);
						m_sprites.hud.resize(1);
						m_sprites.hud[0].setPosition(0, 0);
						//m_sprites.textures.resize(1);
						chargementTexteHUD();
						PLOGI << "Exiting main menu";
						PLOGI << "Entering credits";
						break;
					}
					break;
				case PositionJeu::options:
					PLOGI << "Remapping touch number " << m_index;
					m_sprites.positionDansJeu = PositionJeu::remmapage;
					m_sprites.hud.resize(1);
					m_textesHUD.resize(1);
					chargementTexteHUD();
					//std::this_thread::sleep_for(200ms);
					if (m_index < 7)
					{
						Clv::Key toucheTemp{ assignationTouche(m_evenements, m_touches[4], debutCycle) };
						m_touches[m_index] = toucheTemp;
						if (verifTouches(m_touches))
							resetTouches(m_touches);
					}
					else
					{
						if (confirmerReiniTouches(m_evenements, debutCycle)) resetTouches(m_touches);
					}
					m_sprites.positionDansJeu = PositionJeu::options;
					m_textesHUD.resize(15);
					m_sprites.hud.resize(15);
					chargementTexteHUD();
					//m_sprites.textures.resize(9);
				}
			}
			if (m_touchesActionnees[6] && !m_touchesNonRepetables.test(1))
			{
				m_touchesNonRepetables.set(1);
				switch (m_sprites.positionDansJeu)
				{
				case PositionJeu::options:
					//verifMenu(m_sprites, index, ptrFcnFichier, pTouches);
					m_sprites.ecranNoir.setFillColor(sf::Color(0, 0, 0, 0));
					m_sprites.hud.resize(4);
					m_textesHUD.resize(4);
					//m_sprites.textures.resize(2);
					m_sprites.positionDansJeu = PositionJeu::accueil;
					m_index = 0;
					m_indexMax = 3;
					chargementTexteHUD();
					PLOGI << "Exiting settings region";
					PLOGI << "Entering main menu";
					break;
				case PositionJeu::credits:
					//verifMenu(m_sprites, index, ptrFcnFichier, pTouches);
					m_sprites.ecranNoir.setFillColor(sf::Color(0, 0, 0, 0));
					m_sprites.hud.resize(4);
					m_textesHUD.resize(4);
					//m_sprites.textures.resize(2);
					m_sprites.positionDansJeu = PositionJeu::accueil;
					m_index = 0;
					m_indexMax = 3;
					chargementTexteHUD();
					PLOGI << "Exiting credits";
					PLOGI << "Entering main menu";
					break;
				}
			}

			//chargementTextures(m_sprites.textures, m_sprites.langue, m_sprites.positionDansJeu);
			if (m_index > m_indexMax)
				m_index = m_indexMax;
			//0 ici représente le minimum de l'index
			else if (m_index < 0)
				m_index = 0;
			verifMenu(m_sprites, m_index, m_textesHUD, m_touches);

			std::this_thread::sleep_for(std::chrono::milliseconds(100 - debutCycle.restart().asMilliseconds()));
		}
	}

#pragma endregion

	void ecranChargement()
	{
		PLOGI << "Launching level " << m_moteur.niveau;
		m_sprites.positionDansJeu = PositionJeu::chargement;
		m_sprites.ecranNoir.setFillColor(sf::Color(0, 0, 0, 255));
		placementObjets();
		chargementNiveau();
		creationLimiteCamera(m_moteur);
		std::this_thread::sleep_for(std::chrono::seconds(2));
		m_sprites.ecranNoir.setFillColor(sf::Color(0, 0, 0, 0));
		m_peutDeplacer = true;
		m_sprites.hud.resize(0);

	}


};


#endif 