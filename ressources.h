#ifndef RESSOURCES_H
#define RESSOURCES_H

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
	quitter_pause,
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

constexpr float vecteurNul{ 0.f };
const sf::Color couleur{ sf::Color(128, 128, 128) };
	constexpr int tempsParImage{ 1000000 / 60 };				//Temps en microsecondes
namespace utilitaire {
	constexpr unsigned int limiteFramerate{ 60 };
	constexpr int deplacement{ 3 };
}

bool verifTouches(const ensembleTouches& pTouches);
void resetTouches(ensembleTouches& pTouches);
void detectionEvenement(sf::Event& evenementJeu, bool& threadsActifs, bool& peutDeplacer, touchesActives& touchesActionnees, const ensembleTouches& pTouches, sf::RenderWindow& pFenetre, std::bitset<3>& touchesNonRepetables);
std::wstring nomFichierImageTouches(const Clv::Key pTouche, const Langue langue);
std::string chargementTextures(const std::string& langue, const PositionJeu position);
float getWidth(const sf::Texture& texture);
float getWidth(const sf::Sprite& sprite);
float getHeight(const sf::Texture& texture);
float getHeight(const sf::Sprite& sprite);
void creationLimiteCamera(Moteur& moteur);
void verifFichierExiste(const std::string& chemin);
sf::Keyboard::Key assignationTouche(const sf::Event& pEvenement, const sf::Keyboard::Key& toucheExclue, sf::Clock& cycle);
bool confirmerReiniTouches(sf::Event& pEvenement, sf::Clock& cycle);

#endif 