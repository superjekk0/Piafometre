#pragma once
#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include "ressources.h"
#include "Plateforme.h"

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

	void miseAJourCheckpoint(const sf::Vector2f& pCoordonneesCamera, const sf::Sprite& pCoordonneesJoueur, const std::vector<sf::Sprite>& pEnsembleArrierePlan, const Plateforme& pCheckpoint)
	{
		m_CoordonneesCamera = pCoordonneesCamera;
		m_CoordonneesJoueur = pCheckpoint.coinSpriteGaucheBas() + sf::Vector2f(0.f, -getHeight(pCoordonneesJoueur));
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

struct Moteur {
	int positionJoueurX;
	int positionJoueurY;
	int maxCameraX; // Se situe par rapport à l'extrémité droite de l'écran
	int maxCameraY; // Se situe par rapport à l'extrémité basse de l'écran
	int minCameraX;	// Se situe par rapport à l'extrémité gauche de l'écran
	int minCameraY; // Se situe par rapport à l'extrémité haute de l'écran
	int niveau;
	int nbVie;
	Checkpoint checkpoint;
};


#endif

