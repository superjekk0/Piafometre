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
	decor
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

//class Checkpoint;

//struct Moteur;

using Clv = sf::Keyboard;									//La classe sf::Keyboard (Clavier), en somme
using ensembleTouches = std::array<sf::Keyboard::Key, 7>;
//using fonctionsRessources = std::function<std::string(const int&)>;
using touchesActives = std::vector<bool>;

constexpr float vecteurNul{ 0.f };
const sf::Color couleur{ sf::Color(128, 128, 128) };
	constexpr int tempsParImage{ 1000000 / 60 };				//Temps en microsecondes
namespace utilitaire {
	constexpr unsigned int limiteFramerate{ 60 };
	constexpr float deplacement{ 3.f };
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
//void creationLimiteCamera(Moteur& moteur);
void verifFichierExiste(const std::string& chemin);
sf::Keyboard::Key assignationTouche(const sf::Event& pEvenement, const sf::Keyboard::Key& toucheExclue, sf::Clock& cycle);
bool confirmerReiniTouches(sf::Event& pEvenement, sf::Clock& cycle);
std::vector<std::string> splitString(std::string str, const char separator);

#endif 