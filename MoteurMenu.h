#ifndef MOTEURMENU_H
#define MOTEURMENU_H

#include "ressources.h"
//#include "Jeu.h"
//#include "Checkpoint.h"
#include <Tile.h>
#include <Level.h>
#include "Plateforme.h"

struct Moteur;

class MoteurMenu {
private:
	//Ensemble des touches actionnées par le clavier
	//index 0: touche pour le bouton de gauche
	//index 1: touche pour le bouton de droite
	//index 2: touche pour le bouton de haut
	//index 3: touche pour le bouton du bas
	//index 4: touche pour le bouton de confirmation
	//index 5: touche pour le bouton de saut
	//index 6: touche pour le bouton de pause
	//index 7: indique lorsque le programme doit arrêter
	touchesActives& m_touchesActionnees;
	int m_index;							// Index actuel du HUD
	int m_indexMax;							// Index maximal du HUD selon un contexte donné
	bool& m_peutDeplacer;					// Indique si le joueur peut bouger. Lorsque faux, le HUD est actif
	bool& m_threadsActifs;					// Sémaphore permettant de désactiver tous les fils d'exécution
	ObjetADessiner& m_sprites;				// Ensemble des éléments graphiques
	ensembleTouches& m_touches;				// Ensemble des touches pouvant activer les touches actionnées
	Moteur& m_moteur;						// Ensemble de règles régissant le moteur de jeu
	//Touches ne devant pas être actionnées en même temps.
	//Les touches non répétables correspondent à l'index 4 (Entrée), l'index 6 (Echap) et l'index 5 (Espace)
	std::bitset<3>& m_touchesNonRepetables;
	sf::Event& m_evenements;				// Objet permettant de détecter les événements
	std::vector<std::wstring> m_textesHUD;	// Ensemble des textes composant le HUD actuel
	std::vector<std::wstring> m_descriptionsNiveau;
	std::binary_semaphore& m_semaphore;

	void chargementTexteHUD();

	// Placement des objets du HUD en dehors du moteur de jeu
	void placementObjets();

	/// <summary>
	/// Permet d'afficher le menu présentement sélectionné.
	/// </summary>
	void affichageHUD();

	void chargementNiveau();

	void verifFichierDisponible(const std::string& chemin, int index);

	void chargementTexturesNiveau();

public:

	MoteurMenu(touchesActives& pTouchesActionnees, int pIndex,
		int pIndexMax, bool& pPeutDeplacer, ObjetADessiner& pSprites,
		ensembleTouches& pTouches, Moteur& pMoteur, std::bitset<3>& pTouchesNonRepetables,
		bool& pThreadsActifs, sf::Event& pEvenement, std::binary_semaphore& semaphore);

	void deplacementMenus();

	void ecranChargement();

	void pause();
};

#endif