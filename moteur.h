#ifndef MOTEUR_H
#define MOTEUR_H

#include <Level.h>
#include <Tile.h>
#include "ressources.h"
#include "Plateforme.h"
#include "Checkpoint.h"
#include "MoteurMenu.h"
//#include "Plateformes.h"

constexpr int finTempsSaut{ 30 }; //30 dixièmes de secondes (3 secondes)
constexpr int finPeutSauter{ 150 };//150 dixièmes de seconde (15 secondes)

enum class Collision {
	aucune,
	pics,
	checkpoint,
	objet,
	normale,
	fin,
	max
};

enum class PositionCollision {
	droite,
	gauche,
	haut,
	bas
};

/// <summary>
/// Indique une collision qui a lieu entre le joueur et une plateforme
/// </summary>
class InfosCollision {
private:
	const PlateformeOptimisee* const m_plateforme;
	const PositionCollision m_positionColision;
	const Collision m_collision;
	const int m_indexPlateforme;
public:
	InfosCollision(const PlateformeOptimisee& pPlateforme, const PositionCollision pPositionCollision, const Collision pCollision, const int pIndex) : m_plateforme{ &pPlateforme }, m_collision{ pCollision }, m_positionColision{ pPositionCollision }, m_indexPlateforme{ pIndex }
	{}

	InfosCollision() : m_plateforme{ nullptr }, m_positionColision{ PositionCollision::bas }, m_collision{ Collision::aucune }, m_indexPlateforme{ -1 }
	{}

	const PlateformeOptimisee& plateforme() const
	{
		return *m_plateforme;
	}

	PositionCollision positionCollision() const
	{
		return m_positionColision;
	}

	Collision collision() const
	{
		return m_collision;
	}

	int indexPlateforme() const
	{
		return m_indexPlateforme;
	}
};

float getWidth(const sf::Texture& texture);

float getWidth(const sf::Sprite& sprite);

float getHeight(const sf::Texture& texture);

float getHeight(const sf::Sprite& sprite);

/// <summary>
/// Le joueur est proche du bloc par son bord gauche
/// </summary>
bool procheBordGauche(const sf::Sprite& joueur, const sf::Sprite& bloc);

/// <summary>
/// Le joueur est proche du bloc par son bord droit
/// </summary>
bool procheBordDroite(const sf::Sprite& joueur, const sf::Sprite& bloc);

bool prochePlafond(const sf::Sprite& joueur, const sf::Sprite& bloc);

bool procheSol(const sf::Sprite& joueur, const sf::Sprite& bloc);

bool procheSol(const sf::Sprite& joueur, const opt::Tile& bloc);

bool cameraToucheBordDroite(const sf::View& camera, const Moteur& moteur);

bool cameraToucheBordGauche(const sf::View& camera, const Moteur& moteur);

bool cameraToucheBordHaut(const sf::View& camera, const Moteur& moteur);

bool cameraToucheBordBas(const sf::View& camera, const Moteur& moteur);

bool cameraPeutContinuerGauche(const sf::Sprite& joueur, const sf::View& camera, const Moteur& moteur);

bool cameraPeutContinuerDroite(const sf::Sprite& joueur, const sf::View& camera, const Moteur& moteur);

bool cameraPeutContinuerHaut(const sf::Sprite& joueur, const sf::View& camera, const Moteur& moteur);

bool cameraPeutContinuerBas(const sf::Sprite& joueur, const sf::View& camera, const Moteur& moteur);
/// <summary>
/// Le joueur entre en collision avec le bloc par son côté bas
/// </summary>
bool collisionBas(const sf::Sprite& joueur, const Plateforme& bloc);

bool collisionBas(const sf::Sprite& joueur, const opt::Tile& bloc);

/// <summary>
/// Le joueur entre en collision avec le bloc par son côté haut
/// </summary>
bool collisionHaut(const sf::Sprite& joueur, const Plateforme& bloc);

bool collisionHaut(const sf::Sprite& joueur, const opt::Tile& bloc);

/// <summary>
/// Le joueur entre en collision avec le bloc par son côté droit
/// </summary>
bool collisionDroite(const sf::Sprite& joueur, const Plateforme& bloc);

bool collisionDroite(const sf::Sprite& joueur, const opt::Tile& bloc);

/// <summary>
/// Le joueur entre en collision avec le bloc par son côté gauche
/// </summary>
bool collisionGauche(const sf::Sprite& joueur, const Plateforme& bloc);

bool collisionGauche(const sf::Sprite& joueur, const opt::Tile& bloc);

bool dansEcran(const sf::View& camera, const sf::Sprite& plateforme);


class MoteurPhysique {
private:
	//index 0: touche pour le bouton de gauche
	//index 1: touche pour le bouton de droite
	//index 2: touche pour le bouton de haut
	//index 3: touche pour le bouton du bas
	//index 4: touche pour le bouton de confirmation
	//index 5: touche pour le bouton de saut
	//index 6: touche pour le bouton de pause
	//index 7: indique lorsque le programme doit arrêter
	const touchesActives& m_touchesActionnees;
	ObjetADessiner& m_sprites;							//Structure permettant de dessiner les sprites à l'écran
	bool& m_peutDeplacer;								//Lorsque vrai, ça active le moteur physique
	const bool& m_threadsActifs;						//Sémaphore pour indiquer si les fonctions doivent se désactiver
	Moteur& m_moteur;									//Ensemble de règles pour le moteur de jeu
	//Touches ne devant pas être actionnées en même temps.
	//Les touches non répétables correspondent à l'index 4 (Entrée), l'index 6 (Echap) et l'index 5 (Espace)
	std::bitset<3>& m_touchesNonRepetables;
	MoteurMenu& m_menus;								//Pour appeler l'écran de chargement
	bool m_gauche{ false };								//Le joueur regarde à gauche (Si aucune touche n'est appuyée, garde la direction actuelle)
	int m_tempsDixiemeSeconde{ 0 };						//Indique combien de dixièmes de secondes se sont écoulées au saut
	//Le bit 0 correspond à si le joueur peut sauter
	//Le bit 1 correspond à si le joueur peut faire un autre saut
	//Le bit 2 correspond à si un saut est en cours.
	//Le bit 3 correspond à si un saut est possible en appuyant sur le bouton saut
	std::bitset<4> m_autorisationsSaut{ 0b0000 };
	int m_DixiemeSecondePeutSauter{ 0 };				//Sert à vérifier quand le joueur peut sauter
	std::vector<InfosCollision> m_collisions;
	std::binary_semaphore& m_semaphore;

	/// <summary>
	/// Indique si la plateforme recherchée est déjà inscrite dans le vecteur de collisions
	/// </summary>
	bool collisionPresente(const int pPlateformeRecherchee);

	void touchePlateformeBas();

	void touchePlateformeGauche();

	void touchePlateformeDroite();

	void touchePlateformeHaut();

	void desactiverSaut();

	/// <summary>
	/// Indique si le joueur a pied sur une plateforme
	/// </summary>
	bool piedSurPlateforme();

	void rendreObjetVisible(int index);

	/// <summary>
/// 
/// </summary>
/// <param name="mouvementsJoueur">Mouvements vectoriels du joueur</param>
	void animationJoueur(const sf::Vector2f& mouvementsJoueur, long pFrameAnimation);

	void animationCheckpoint(int indexCheckpoint, long pFrameAnimation, const int& subTexture);

	void peutSauter();

	int indexCheckpoint();

	bool personnagePeutSauter();

	void mort(std::unique_ptr<std::thread>& sautEffectif);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="pIndexCheckpoint">Index du checkpoint</param>
	/// <returns>Lorsque vrai, il faut par la suite terminer le thread principal</returns>
	bool gestionCollisions(int pIndexCheckpoint, std::unique_ptr<std::thread>& pSautEffectif, sf::Vector2f& pDeplacements);
public:

	MoteurPhysique(const touchesActives& pTouchesActionnees, ObjetADessiner& pSprites,
		bool& pPeutDeplacer, const bool& pThreadsActifs, Moteur& pMoteur,
		std::bitset<3>& m_touchesNonRepetables, MoteurMenu& pMenus, std::binary_semaphore& semaphore);

	void deplacement();
};


#endif