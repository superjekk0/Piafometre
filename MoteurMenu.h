#pragma once
#ifndef MOTEURMENU_H
#define MOTEURMENU_H

#include "ressources.h"
#include "Jeu.h"
//#include "Checkpoint.h"
#include <Tile.h>
#include <Level.h>
#include "Plateforme.h"

class MoteurMenu {
private:
#pragma region PROPRIETES
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

#pragma endregion

#pragma region METHODES_PRIVEES

	void chargementTexteHUD()
	{
		std::string chemin{ chargementTextures(Jeu::symboleLangue(m_sprites.langue), m_sprites.positionDansJeu) };
		verifFichierExiste(chemin);
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
		case PositionJeu::chargement:
			m_textesHUD[0] = L"";
			for (std::wstring& texte : m_descriptionsNiveau)
			{
				auto indexNiveau{ texte.find(L"- " + std::to_wstring(m_moteur.niveau)) };
				if (indexNiveau != texte.npos)
				{
					m_textesHUD[0] += texte.substr(0, indexNiveau - 1) + L'\n';
				}
			}
			m_sprites.hud[0].setString(m_textesHUD[0]);
			break;
		case PositionJeu::pause:
		case PositionJeu::quitter_pause:
			for (int i{ 0 }; i < m_textesHUD.size() && i < m_sprites.hud.size() && fichier; ++i)
			{
				std::wstring ligneTemp;
				std::getline(fichier, ligneTemp);
				m_textesHUD[i] = ligneTemp;
				m_sprites.hud[i].setString(m_textesHUD[i]);
			}
			break;
		default:
			break;
		}
	}

	// Placement des objets du HUD en dehors du moteur de jeu
	void placementObjets()
	{
		//m_sprites.camera.setCenter(m_sprites.camera.getSize().x / 2, m_sprites.camera.getSize().y / 2);
		switch (m_sprites.positionDansJeu)
		{
		case PositionJeu::accueil:
			for (int i{ 0 }; i < m_sprites.hud.size(); ++i)
			{
				m_sprites.hud[i].setPosition(800.f, 100.f + i * 100.f);
			}
			break;
		case PositionJeu::options:
			for (int i{ 0 }; i < m_sprites.hud.size(); i += 2)
			{
				m_sprites.hud[i].setPosition(sf::Vector2f(250.f, 50.f + 40.f * i));
			}
			for (int i{ 1 }; i < m_sprites.hud.size(); i += 2)
			{
				m_sprites.hud[i].setPosition(sf::Vector2f(900.f, m_sprites.hud[i - 1].getPosition().y));
			}
			break;
		case PositionJeu::remmapage:
			m_sprites.hud[0].setFillColor(sf::Color());
			m_sprites.hud[0].setPosition(400.f, 300.f);
			//if (spritesEtTextures.hud.size() >= 2) spritesEtTextures.hud[1].setPosition(850, 580);
			break;
		case PositionJeu::chargement:
			m_sprites.hud[0].setPosition(400.f, 400.f);
			//spritesEtTextures.hud[1].setPosition(600, 400);
			break;
		case PositionJeu::credits:
			m_sprites.hud[0].setPosition(280.f, 200.f);
			break;
		case PositionJeu::pause:
			m_sprites.hud[0].setPosition(m_sprites.camera.getCenter() + sf::Vector2f(-200.f, -50.f));
			m_sprites.hud[1].setPosition(m_sprites.camera.getCenter() + sf::Vector2f(-160.f, 50.f));
			break;
		case PositionJeu::quitter_pause:
			m_sprites.hud[0].setPosition(m_sprites.camera.getCenter() + sf::Vector2f(-250.f, 0.f));
			m_sprites.hud[1].setPosition(m_sprites.camera.getCenter() + sf::Vector2f(250.f, 0.f));
			break;
		}
	}

	/// <summary>
	/// Permet d'afficher le menu présentement sélectionné.
	/// </summary>
	void affichageHUD()
	{
		//===================================
		//
		// Très important! L'index pour le texte affiché lorsque le menu est "options" est pair tandis que pour l'image de touche
		// correspondante, elle est impaire.
		//
		//===================================

		for (int i{ 0 }; i < m_sprites.hud.size() && i < m_textesHUD.size(); ++i)
		{
			m_sprites.hud[i].setFont(m_sprites.police);
			switch (m_sprites.positionDansJeu)
			{
			case PositionJeu::accueil:
				m_sprites.hud[i].setString(m_textesHUD[i]);
				m_sprites.hud[i].setCharacterSize(80u);
				if (m_index == i)
				{
					m_sprites.hud[i].setCharacterSize(m_sprites.hud[i].getCharacterSize() * 1.15);
					m_sprites.hud[i].setFillColor(sf::Color(222, 215, 43));
					m_sprites.hud[i].setStyle(sf::Text::Style::Bold);
				}
				else
				{
					m_sprites.hud[i].setFillColor(sf::Color(210, 210, 210));
					m_sprites.hud[i].setStyle(sf::Text::Style::Regular);
				}
				break;
			case PositionJeu::options:
				m_sprites.hud[i].setCharacterSize(30u);
				if (m_index * 2 == i)
				{
					m_sprites.hud[i].setCharacterSize(m_sprites.hud[i].getCharacterSize() * 1.15);
					m_sprites.hud[i].setFillColor(sf::Color(222, 215, 43));
					m_sprites.hud[i].setStyle(sf::Text::Style::Bold);
				}
				else
				{
					m_sprites.hud[i].setFillColor(sf::Color(210, 210, 210));
					m_sprites.hud[i].setStyle(sf::Text::Style::Regular);
				}
				break;
			case PositionJeu::credits:
				m_sprites.hud[i].setCharacterSize(45u);
				break;
			case PositionJeu::remmapage:
				m_sprites.hud[i].setFillColor(sf::Color(210, 210, 210));
				m_sprites.hud[i].setCharacterSize(45u);
				break;
			case PositionJeu::chargement:
				m_sprites.hud[i].setFillColor(sf::Color(210, 210, 210));
				m_sprites.hud[i].setCharacterSize(45u);
				break;
			case PositionJeu::pause:
			case PositionJeu::quitter_pause:
				m_sprites.hud[i].setString(m_textesHUD[i]);
				m_sprites.hud[i].setCharacterSize(60u);
				if (m_index == i)
				{
					m_sprites.hud[i].setCharacterSize(m_sprites.hud[i].getCharacterSize() * 1.15);
					m_sprites.hud[i].setFillColor(sf::Color(222, 215, 43));
					m_sprites.hud[i].setStyle(sf::Text::Style::Bold);
				}
				else
				{
					m_sprites.hud[i].setFillColor(sf::Color(210, 210, 210));
					m_sprites.hud[i].setStyle(sf::Text::Style::Regular);
				}
				break;
			}
		}
		placementObjets();
	}

	void creationLimiteCamera(Moteur& moteur)
	{
		switch (moteur.niveau)
		{
		case 1:
			moteur.minCameraY = -500;
			moteur.maxCameraX = 3000;
			moteur.maxCameraY = 2000;
			break;
		case 2:
			moteur.minCameraY = -600;
			moteur.maxCameraY = 720;
			moteur.maxCameraX = 3800;
			break;
		case 3:
			moteur.minCameraY = -1600;
			moteur.maxCameraY = 720;
			moteur.maxCameraX = 1280;
			break;
		case 4:
			moteur.minCameraY = -2000;
			moteur.maxCameraY = 720;
			moteur.maxCameraX = 1280;
			break;
		default:
			//Il s'agit de la fin, donc aucune règle!
			break;
		}
	}

	void chargementNiveau()
	{
		chargementTexturesNiveau();
		m_sprites.joueur.setTexture(m_sprites.textures[0]);
		m_sprites.joueur.setTextureRect(sf::IntRect(0,
			marche_droite * getHeight(m_sprites.textures[0]) / max,
			getWidth(m_sprites.textures[0]) / 3,
			getHeight(m_sprites.textures[0]) / max));
		m_sprites.joueur.setScale(3, 3);

		switch (m_moteur.niveau)
		{
		case 1:
			m_sprites.couleur = sf::Color(119, 181, 254);
			m_sprites.positionDansJeu = PositionJeu::jeu;

			m_sprites.avantPlan.resetTiles();
			m_sprites.arrierePlan.resize(2);

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				1,
				sf::Vector2f(3000.f, m_sprites.avantPlan.getSubTexture(1).height),
				sf::Vector2f(0.f, 720.f - m_sprites.avantPlan.getSubTexture(1).height),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::solide));
			m_sprites.avantPlan[0]->setScale(0.75f, 0.75f, opt::TextureRule::adjustable_size);

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0, 
				sf::Vector2f(m_sprites.avantPlan.getSubTexture(0).width, m_sprites.avantPlan.getSubTexture(0).height * 2.5f),
				sf::Vector2f(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::solide));
			m_sprites.avantPlan[1]->setScale(0.75f, 0.75f, opt::TextureRule::adjustable_size);
			m_sprites.avantPlan[1]->setPosition(3.f * m_sprites.avantPlan[1]->subTextureSize().x,
				m_sprites.avantPlan[0]->getPosition().y + m_sprites.avantPlan[0]->height() - m_sprites.avantPlan[1]->height());

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				2,
				sf::Vector2f(m_sprites.avantPlan.getSubTextureSize(2).x * 4.f, m_sprites.avantPlan.getSubTextureSize(2).y),
				sf::Vector2f(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::pics));
			m_sprites.avantPlan[2]->setPosition(m_sprites.avantPlan[1]->getPosition().x + m_sprites.avantPlan[1]->width(), m_sprites.avantPlan[0]->getPosition().y - m_sprites.avantPlan[2]->height());

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(m_sprites.avantPlan[0]->width(), m_sprites.avantPlan.getSubTexture(0).height * 3.f),
				sf::Vector2f(m_sprites.avantPlan[0]->getPosition().x, m_sprites.avantPlan[0]->getPosition().y + m_sprites.avantPlan[0]->height()),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));
			m_sprites.avantPlan[3]->setScale(0.75f, 0.75f, opt::TextureRule::keep_size);

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				1,
				sf::Vector2f(3000.f, m_sprites.avantPlan.getSubTexture(1).height),
				sf::Vector2f(0.f, m_sprites.avantPlan[3]->getPosition().y + m_sprites.avantPlan[3]->height() + 500.f),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				2,
				sf::Vector2f(m_sprites.avantPlan.getSubTextureSize(2)),
				sf::Vector2f(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::pics));
			m_sprites.avantPlan[5]->setPosition(1500.f, m_sprites.avantPlan[4]->getPosition().y - m_sprites.avantPlan[5]->height());

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				2,
				m_sprites.avantPlan[5]->getSize(),
				m_sprites.avantPlan[5]->getPosition() + sf::Vector2f(-750.f, 0.f),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::pics));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				4,
				sf::Vector2f(m_sprites.avantPlan.getSubTextureSize(4)),
				sf::Vector2f(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::checkPoint));
			m_sprites.avantPlan[7]->changeColor(sf::Color(0xFFEEFFFF));
			m_sprites.avantPlan[7]->setScale(2.f, opt::TextureRule::keep_height);
			m_sprites.avantPlan[7]->setPosition(m_sprites.avantPlan[5]->getPosition().x + 500.f,
				m_sprites.avantPlan[4]->getPosition().y - m_sprites.avantPlan[7]->height());

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				7,
				m_sprites.avantPlan.getSubTextureSize(7),
				sf::Vector2f(),
				opt::TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::objet));
			m_sprites.avantPlan[8]->setPosition(m_sprites.avantPlan[1]->getPosition().x - m_sprites.avantPlan[8]->width() -100.f,
				m_sprites.avantPlan[0]->getPosition().y - m_sprites.avantPlan[8]->height());

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				7,
				m_sprites.avantPlan.getSubTextureSize(7),
				sf::Vector2f(),
				opt::TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::objet
			));
			m_sprites.avantPlan[9]->setPosition(sf::Vector2f(m_sprites.avantPlan[7]->getPosition().x - m_sprites.avantPlan[9]->getSize().x - 40.f, 
				m_sprites.avantPlan[4]->getPosition().y - m_sprites.avantPlan[9]->height()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				3,
				m_sprites.avantPlan.getSubTextureSize(3),
				m_sprites.avantPlan[4]->getPosition() + sf::Vector2f(0.f, -m_sprites.avantPlan.getSubTextureSize(3).y),
				opt::TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::finTableau
			));

			m_sprites.joueur.setPosition(m_sprites.avantPlan[0]->getPosition().x,
				m_sprites.avantPlan[0]->topLeftCorner().y - getHeight(m_sprites.joueur));

			m_sprites.arrierePlan[0].setTexture(m_sprites.textures[1], true);
			m_sprites.arrierePlan[0].setScale(7, 7);
			m_sprites.arrierePlan[0].setPosition(0, -200);

			m_sprites.arrierePlan[1].setTexture(m_sprites.textures[2], true);
			m_sprites.arrierePlan[1].setScale(7, 7);
			m_sprites.arrierePlan[1].setPosition(0, -150);

			break;
		case 2:
			m_sprites.couleur = sf::Color(0x2D100DFF);
			m_sprites.positionDansJeu = PositionJeu::jeu;

			m_sprites.arrierePlan.resize(1);
			m_sprites.avantPlan.resetTiles();

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(500.f, m_sprites.avantPlan.getSubTextureSize(0).y),
				sf::Vector2f(0.f, 400.f),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				2,
				sf::Vector2f(m_sprites.avantPlan.getSubTexture(2).width, 200.f),
				sf::Vector2f(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));
			m_sprites.avantPlan[1]->setPosition(m_sprites.avantPlan[0]->topRightCorner() - m_sprites.avantPlan[1]->getSize());

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				3,
				sf::Vector2f(400.f, m_sprites.avantPlan.getSubTextureSize(3).y),
				m_sprites.avantPlan[0]->topRightCorner(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::pics));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(400.f, 200.f),
				m_sprites.avantPlan[2]->bottomLeftCorner(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				3,
				sf::Vector2f(400.f, m_sprites.avantPlan[2]->height()),
				m_sprites.avantPlan[0]->topRightCorner() + sf::Vector2f(600.f, 0.f),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::pics));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(400.f, 200.f),
				m_sprites.avantPlan[4]->bottomLeftCorner(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				3,
				sf::Vector2f(400.f, m_sprites.avantPlan[2]->height()),
					m_sprites.avantPlan[4]->topRightCorner() + sf::Vector2f(400.f, 0.f),
					opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::pics));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(400.f, 200.f),
				m_sprites.avantPlan[6]->bottomLeftCorner(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(400.f, 200.f),
				m_sprites.avantPlan[2]->bottomLeftCorner(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				4,
				m_sprites.avantPlan.getSubTextureSize(4),
				sf::Vector2f(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::objet));
			m_sprites.avantPlan[9]->setPosition(((m_sprites.avantPlan[2]->topRightCorner().x + m_sprites.avantPlan[4]->topLeftCorner().x) / 2.f) - m_sprites.avantPlan[9]->width() / 2.f, 250.f);

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				4,
				m_sprites.avantPlan.getSubTextureSize(4),
				sf::Vector2f(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::objet));
			m_sprites.avantPlan[10]->setPosition(((m_sprites.avantPlan[4]->topRightCorner().x + m_sprites.avantPlan[6]->topLeftCorner().x) / 2.f) - m_sprites.avantPlan[10]->width() / 2.f, 250.f);

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(700.f, m_sprites.avantPlan.getSubTextureSize(0).y),
				m_sprites.avantPlan[7]->topRightCorner(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				6,
				m_sprites.avantPlan.getSubTextureSize(6),
				sf::Vector2f(),
				opt::TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::checkPoint));
			m_sprites.avantPlan[12]->setScale(2.f);
			m_sprites.avantPlan[12]->setPosition(m_sprites.avantPlan[11]->getPosition() + sf::Vector2f(50.f, -m_sprites.avantPlan[12]->width()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				4,
				m_sprites.avantPlan.getSubTextureSize(4),
				sf::Vector2f(),
				opt::TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::objet));
			m_sprites.avantPlan[13]->setPosition(m_sprites.avantPlan[1]->bottomLeftCorner() - sf::Vector2f(75.f, m_sprites.avantPlan[13]->height()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				2,
				sf::Vector2f(3000.f, 400.f),
				sf::Vector2f(0.f, -500.f),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				2,
				sf::Vector2f(200.f, 1000.f),
				sf::Vector2f(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));
			m_sprites.avantPlan[15]->setPosition(m_sprites.avantPlan[11]->topRightCorner() - sf::Vector2f(m_sprites.avantPlan[15]->width(), 1200.f));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				2,
				sf::Vector2f(300.f, 700.f),
				m_sprites.avantPlan[11]->topRightCorner() + sf::Vector2f(500.f, -400.f),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				4,
				m_sprites.avantPlan.getSubTextureSize(4),
				sf::Vector2f(),
				opt::TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::objet));
			m_sprites.avantPlan[17]->setPosition(m_sprites.avantPlan[11]->topRightCorner() - sf::Vector2f(200.f, m_sprites.avantPlan[17]->height()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				4,
				m_sprites.avantPlan.getSubTextureSize(4),
				(m_sprites.avantPlan[11]->topRightCorner() + m_sprites.avantPlan[16]->topLeftCorner()) / 2.f,
				opt::TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::objet));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				1,
				sf::Vector2f(1000.f, m_sprites.avantPlan.getSubTextureSize(1).y),
				m_sprites.avantPlan[15]->topRightCorner() + sf::Vector2f(0.f, 300.f),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				2,
				sf::Vector2f(1000.f, 500.f),
				sf::Vector2f(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));
			m_sprites.avantPlan[20]->setPosition(m_sprites.avantPlan[19]->getPosition() + sf::Vector2f(0.f, -m_sprites.avantPlan[20]->height()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				5,
				m_sprites.avantPlan.getSubTextureSize(5) * 1.5f,
				sf::Vector2f(),
				opt::TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::finTableau));
			m_sprites.avantPlan[21]->setPosition(m_sprites.avantPlan[16]->getPosition() + sf::Vector2f(0.f, -m_sprites.avantPlan[21]->height()));

			m_sprites.joueur.setPosition(m_sprites.avantPlan[0]->topLeftCorner() + sf::Vector2f(0.f, -getHeight(m_sprites.joueur)));

			m_sprites.arrierePlan[0].setTexture(m_sprites.textures[1], true);
			m_sprites.arrierePlan[0].setTextureRect(sf::IntRect(0, 0, 8000, getHeight(m_sprites.textures[1])));
			m_sprites.arrierePlan[0].setPosition(-100.f, -250.f);
			m_sprites.arrierePlan[0].setScale(0.25f, 0.25f);
			break;
		case 3:
			m_sprites.couleur = sf::Color(0xD4E7FFFF);
			m_sprites.positionDansJeu = PositionJeu::jeu;

			m_sprites.arrierePlan.resize(1);
			m_sprites.avantPlan.resetTiles();

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(1280.f, 200.f),
				sf::Vector2f(0.f, 550.f),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				2,
				m_sprites.avantPlan.getSubTextureSize(2),
				sf::Vector2f(),
				opt::TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::objet));
			m_sprites.avantPlan[1]->setPosition(m_sprites.avantPlan[0]->topRightCorner() - m_sprites.avantPlan[1]->getSize() + sf::Vector2f(-120.f, 0.f));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(300.f, 500.f),
				sf::Vector2f(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::semiSolide));
			m_sprites.avantPlan[2]->changeColor(sf::Color(0xAAAAAAFF));
			m_sprites.avantPlan[2]->setPosition((m_sprites.avantPlan[0]->topRightCorner() + m_sprites.avantPlan[0]->topLeftCorner()) / 2.f + sf::Vector2f(0.f, -m_sprites.avantPlan[2]->height()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(400.f, 150.f),
				m_sprites.avantPlan[2]->topRightCorner(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(500.f, 250.f),
				sf::Vector2f(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				2,
				m_sprites.avantPlan.getSubTextureSize(2),
				sf::Vector2f(),
				opt::TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::objet));
			m_sprites.avantPlan[5]->setPosition(m_sprites.avantPlan[4]->getPosition() + sf::Vector2f(200.f, -m_sprites.avantPlan[5]->height()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(300.f, 500.f),
				sf::Vector2f(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::semiSolide));
			m_sprites.avantPlan[6]->setPosition(m_sprites.avantPlan[4]->topRightCorner() - sf::Vector2f(150.f, m_sprites.avantPlan[6]->height()));
			m_sprites.avantPlan[6]->changeColor(sf::Color(0xAAAAAAFF));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(300.f, 150.f),
				m_sprites.avantPlan[6]->topRightCorner(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				1,
				sf::Vector2f(350.f, m_sprites.avantPlan.getSubTextureSize(1).y),
				sf::Vector2f(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::pics));
			m_sprites.avantPlan[8]->setPosition(m_sprites.avantPlan[3]->topRightCorner() - m_sprites.avantPlan[8]->getSize());

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				3,
				m_sprites.avantPlan.getSubTextureSize(3),
				m_sprites.avantPlan[7]->topLeftCorner() + sf::Vector2f(0.f, -350.f),
				opt::TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::semiSolide));
			m_sprites.avantPlan[9]->setScale(10.f, 5.f);

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				4,
				sf::Vector2f(400.f, 180.f),
				m_sprites.avantPlan[9]->topRightCorner(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				2,
				m_sprites.avantPlan.getSubTextureSize(2),
				sf::Vector2f(),
				opt::TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::objet));
			m_sprites.avantPlan[11]->setPosition((m_sprites.avantPlan[7]->topLeftCorner() + m_sprites.avantPlan[7]->topRightCorner()) / 2.f + sf::Vector2f(0.f, -m_sprites.avantPlan[11]->height()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				5,
				m_sprites.avantPlan.getSubTextureSize(5) * 2.f,
				sf::Vector2f(),
				opt::TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::finTableau));
			m_sprites.avantPlan[12]->setPosition(m_sprites.avantPlan[10]->topLeftCorner() + sf::Vector2f(0.f, -m_sprites.avantPlan[12]->height()));

			m_sprites.joueur.setPosition(m_sprites.avantPlan[0]->topLeftCorner() + sf::Vector2f(0.f, -getHeight(m_sprites.joueur)));

			m_sprites.arrierePlan[0].setTexture(m_sprites.textures[1]);
			m_sprites.arrierePlan[0].setScale(4.25f, 4.25f);
			m_sprites.arrierePlan[0].setTextureRect(sf::IntRect(0, 0, 1500, getHeight(m_sprites.textures[1])));
			m_sprites.arrierePlan[0].setPosition(0.f, -400.f);
			break;
		case 4:
			m_sprites.couleur = sf::Color(0xFEFEE2FF);
			m_sprites.positionDansJeu = PositionJeu::jeu;

			m_sprites.arrierePlan.resize(1);
			m_sprites.avantPlan.resetTiles();

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(1280.f, m_sprites.avantPlan.getSubTextureSize(0).y),
				sf::Vector2f(0.f, 720.f - m_sprites.avantPlan.getSubTextureSize(0).y),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				1,
				sf::Vector2f(1280.f, m_sprites.avantPlan.getSubTextureSize(1).y),
				m_sprites.avantPlan[0]->getPosition() + sf::Vector2f(0.f, -m_sprites.avantPlan.getSubTextureSize(1).y),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				1,
				sf::Vector2f(600.f, m_sprites.avantPlan.getSubTextureSize(1).y),
				sf::Vector2f(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));
			m_sprites.avantPlan[2]->setPosition((m_sprites.avantPlan[1]->topRightCorner() + m_sprites.avantPlan[1]->topLeftCorner()) / 2.f - sf::Vector2f(m_sprites.avantPlan[2]->width() / 2.f, 150.f + m_sprites.avantPlan[2]->height()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				8,
				m_sprites.avantPlan.getSubTextureSize(8),
				sf::Vector2f(),
				opt::TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::objet));
			m_sprites.avantPlan[3]->setPosition(m_sprites.avantPlan[2]->bottomLeftCorner().x - m_sprites.avantPlan[3]->width(), m_sprites.avantPlan[1]->getPosition().y - m_sprites.avantPlan[3]->height());

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				8,
				m_sprites.avantPlan.getSubTextureSize(8),
				sf::Vector2f(m_sprites.avantPlan[2]->bottomRightCorner().x, m_sprites.avantPlan[1]->getPosition().y - m_sprites.avantPlan[3]->height()),
				opt::TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::objet));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				2,
				sf::Vector2f(m_sprites.avantPlan[2]->width(), m_sprites.avantPlan.getSubTextureSize(2).y),
				m_sprites.avantPlan[2]->getPosition() - sf::Vector2f(0.f, m_sprites.avantPlan.getSubTextureSize(2).y),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::pics));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(125.f, m_sprites.avantPlan.getSubTextureSize(0).y * 5.f),
				sf::Vector2f(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));
			m_sprites.avantPlan[6]->setPosition((m_sprites.avantPlan[2]->bottomLeftCorner() + m_sprites.avantPlan[2]->bottomRightCorner()) / 2.f - sf::Vector2f(m_sprites.avantPlan[6]->width() / 2.f, m_sprites.avantPlan[6]->height()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				8,
				m_sprites.avantPlan.getSubTextureSize(6),
				m_sprites.avantPlan[6]->getPosition() + sf::Vector2f(-100.f, 200.f),
				opt::TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::objet));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				8,
				m_sprites.avantPlan.getSubTextureSize(6),
				m_sprites.avantPlan[6]->topRightCorner() + sf::Vector2f(50.f, 200.f),
				opt::TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::objet));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				3,
				m_sprites.avantPlan.getSubTextureSize(3),
				sf::Vector2f(),
				opt::TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::semiSolide));
			m_sprites.avantPlan[9]->setScale(10.f, 5.f);
			m_sprites.avantPlan[9]->setPosition((m_sprites.avantPlan[6]->topRightCorner() + m_sprites.avantPlan[6]->getPosition()) / 2.f - sf::Vector2f(m_sprites.avantPlan[9]->width() / 2.f, m_sprites.avantPlan[9]->height()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				1,
				sf::Vector2f(300.f, m_sprites.avantPlan.getSubTextureSize(1).y - 180.f),
				sf::Vector2f(0.f, m_sprites.avantPlan[9]->getPosition().y - 180.f),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				1,
				sf::Vector2f(600.f, m_sprites.avantPlan.getSubTextureSize(1).y),
				m_sprites.avantPlan[10]->topRightCorner() + sf::Vector2f(0.f, -250.f),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				7,
				m_sprites.avantPlan.getSubTextureSize(7) * 1.5f,
				sf::Vector2f(),
				opt::TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::decor));
			m_sprites.avantPlan[12]->setPosition(m_sprites.avantPlan[11]->getPosition() - sf::Vector2f(0.f, m_sprites.avantPlan[12]->height()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				7,
				m_sprites.avantPlan.getSubTextureSize(7) * 1.5f,
				sf::Vector2f(),
				opt::TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::decor));
			m_sprites.avantPlan[13]->setPosition(m_sprites.avantPlan[11]->topRightCorner() - m_sprites.avantPlan[13]->getSize());
			
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				4,
				m_sprites.avantPlan.getSubTextureSize(4) * 2.f,
				sf::Vector2f(),
				opt::TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::checkPoint));
			m_sprites.avantPlan[14]->setPosition((m_sprites.avantPlan[12]->bottomRightCorner() + m_sprites.avantPlan[13]->bottomLeftCorner()) / 2.f - sf::Vector2f(m_sprites.avantPlan[14]->width() / 2.f, m_sprites.avantPlan[14]->height()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(150.f, m_sprites.avantPlan.getSubTextureSize(0).y),
				sf::Vector2f(0.f, m_sprites.avantPlan[12]->getPosition().y),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				8,
				m_sprites.avantPlan.getSubTextureSize(8),
				sf::Vector2f(),
				opt::TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::objet));
			m_sprites.avantPlan[16]->setPosition(m_sprites.avantPlan[12]->bottomLeftCorner() - sf::Vector2f(0.f, m_sprites.avantPlan[16]->height()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(1000.f, 350.f),
				sf::Vector2f(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));
			m_sprites.avantPlan[17]->setPosition(m_sprites.avantPlan[15]->topRightCorner() - sf::Vector2f(-150.f, m_sprites.avantPlan[17]->height()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				8,
				m_sprites.avantPlan.getSubTextureSize(8),
				sf::Vector2f(),
				opt::TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::objet));
			m_sprites.avantPlan[18]->setPosition(m_sprites.avantPlan[15]->getPosition() - sf::Vector2f(0.f, m_sprites.avantPlan[18]->height()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				2,
				sf::Vector2f(150.f, m_sprites.avantPlan.getSubTextureSize(2).y),
				sf::Vector2f(),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::pics));
			m_sprites.avantPlan[19]->setPosition(m_sprites.avantPlan[17]->getPosition() - sf::Vector2f(0.f, m_sprites.avantPlan[19]->height()));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				9,
				m_sprites.avantPlan.getSubTextureSize(9) * 5.f,
				sf::Vector2f(),
				opt::TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::finTableau));
			m_sprites.avantPlan[20]->setPosition(m_sprites.avantPlan[17]->topRightCorner() - m_sprites.avantPlan[20]->getSize());

			m_sprites.joueur.setPosition((m_sprites.avantPlan[3]->bottomRightCorner() + m_sprites.avantPlan[4]->bottomLeftCorner()) / 2.f - sf::Vector2f(getWidth(m_sprites.joueur) / 2.f, getHeight(m_sprites.joueur)));

			m_sprites.arrierePlan[0].setTexture(m_sprites.textures[1], true);
			m_sprites.arrierePlan[0].setScale(4.25f, 4.25f);
			m_sprites.arrierePlan[0].setTextureRect(sf::IntRect(0, 0, 1500, getHeight(m_sprites.textures[1])));
			m_sprites.arrierePlan[0].setPosition(0.f, -475.f);
			break;
		default:
			m_sprites.arrierePlan.resize(0);
			m_sprites.avantPlan.resetTiles();
			m_sprites.positionDansJeu = PositionJeu::fin;
			m_sprites.couleur = sf::Color(0xB32400FF);

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(m_sprites.avantPlan.getSubTextureSize(0).x * 3.f, m_sprites.avantPlan.getSubTextureSize(0).y),
				sf::Vector2f(0.f, 720.f - m_sprites.avantPlan.getSubTextureSize(0).y),
				opt::TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

			m_sprites.joueur.setPosition(375.f, 50.f);
			break;
		}
		m_sprites.avantPlan.reloadVertexes();
	}

	void verifFichierDisponible(const std::string& chemin, int index)
	{
		if (!m_sprites.textures[index].loadFromFile(chemin))
			PLOGE << "Unable to load " << chemin;
		m_sprites.textures[index].setRepeated(false);
	}

	void chargementTexturesNiveau()
	{
		m_sprites.textures.resize(1);
		verifFichierDisponible("resources/sprites/player.png", 0);
		//l'index 0 est réservé pour le sprite du joueur
		switch (m_moteur.niveau)
		{
		case 1:
			m_sprites.textures.resize(3);
			//avant-plan
			m_sprites.avantPlan.loadTexture("resources/textures/Niveau1.png", "resources/textures/Level1.txt");

			//arrière-plan
			verifFichierDisponible("resources/textures/sky.png", 1);
			verifFichierDisponible("resources/textures/mountain.png", 2);

			break;
		case 2:
			//avant-plan
			m_sprites.textures.resize(2);
			m_sprites.avantPlan.loadTexture("resources/textures/Niveau2.png", "resources/textures/Level2.txt");

			//arrière-plan
			verifFichierDisponible("resources/textures/cave.png", 1);

			m_sprites.textures[1].setRepeated(true);
			break;
		case 3:
			m_sprites.textures.resize(2);
			
			//avant-plan
			m_sprites.avantPlan.loadTexture("resources/textures/Niveau3.png", "resources/textures/Level3.txt");

			//arrière-plan
			verifFichierDisponible("resources/textures/repeatedMountains.png", 1);

			m_sprites.textures[1].setRepeated(true);
			//m_sprites.avantPlan.loadTexture(m_sprites.textures[1], "resources/textures/Level3.txt");
			break;
		case 4:
			m_sprites.textures.resize(2);

			//avant-plan
			m_sprites.avantPlan.loadTexture("resources/textures/Niveau4.png", "resources/textures/Level4.txt");

			//arrière-plan
			verifFichierDisponible("resources/textures/repeatedMountains.png", 1);

			m_sprites.textures[1].setRepeated(true);
			break;
		default:
			m_sprites.avantPlan.loadTexture("resources/textures/montagneFond.png", 1);
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
		verifFichierExiste(chargementTextures(Jeu::symboleLangue(m_sprites.langue), PositionJeu::chargement));
		std::wfstream lectureNomsNiveau{ chargementTextures(Jeu::symboleLangue(m_sprites.langue), PositionJeu::chargement) };
		std::wstring temp;
		while (lectureNomsNiveau)
		{
			std::getline(lectureNomsNiveau, temp);
			m_descriptionsNiveau.push_back(temp);
		}
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
			if (m_touchesActionnees[0] && m_sprites.positionDansJeu == PositionJeu::quitter_pause)
				--m_index;
			if (m_touchesActionnees[1] && m_sprites.positionDansJeu == PositionJeu::quitter_pause)
				++m_index;
			if (m_touchesActionnees[2] && m_sprites.positionDansJeu != PositionJeu::quitter_pause)
				--m_index;
			if (m_touchesActionnees[3] && m_sprites.positionDansJeu != PositionJeu::quitter_pause)
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
						//affichageHUD(m_sprites, index, ptrFcnFichier, pTouches);
						m_moteur.nbVie = 3;
						m_sprites.positionDansJeu = PositionJeu::chargement;
						//peutDeplacer = true;
						m_moteur.niveau = 1; //Dès que la construction des niveaux est terminée, remettre à 1
						ecranChargement();
						return;
						break;
					case 1: //Entrer dans les options
						//affichageHUD(m_sprites, index, ptrFcnFichier, pTouches);
						m_sprites.ecranNoir.setFillColor(sf::Color::Color(0x00000080));
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
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
						//touchesActionnees.set(7);
						return;
						break;
					case 3: //Entrer dans les crédits
						//affichageHUD(m_sprites, index, ptrFcnFichier, pTouches);
						m_sprites.ecranNoir.setFillColor(sf::Color(0x00000080));
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
				case PositionJeu::pause:
					switch (m_index)
					{
					case 0:
						m_sprites.ecranNoir.setFillColor(sf::Color(0x0));
						m_peutDeplacer = !m_peutDeplacer;
						m_sprites.hud.resize(0);
						return;
					case 1:
						m_sprites.positionDansJeu = PositionJeu::quitter_pause;
						chargementTexteHUD();
						break;
					}
					break;
				case PositionJeu::quitter_pause:
					switch (m_index)
					{
					case 0:
						m_sprites.arrierePlan.resize(0);
						m_sprites.avantPlan.resetTiles();
						m_sprites.textures.resize(0);
						m_textesHUD.resize(4);
						m_sprites.hud.resize(4);
						m_sprites.positionDansJeu = PositionJeu::accueil;
						m_sprites.couleur = couleur;
						m_sprites.ecranNoir.setFillColor(sf::Color(0, 0, 0, 0));
						m_indexMax = 3;
						m_sprites.camera.setCenter(m_sprites.camera.getSize() / 2.f);
						chargementTexteHUD();
						break;
					case 1:
						m_sprites.positionDansJeu = PositionJeu::pause;
						chargementTexteHUD();
						m_index = 0;
						break;
					}
				}

			}
			if (m_touchesActionnees[6] && !m_touchesNonRepetables.test(1))
			{
				m_touchesNonRepetables.set(1);
				switch (m_sprites.positionDansJeu)
				{
				case PositionJeu::options:
					//affichageHUD(m_sprites, index, ptrFcnFichier, pTouches);
					m_sprites.ecranNoir.setFillColor(sf::Color(0x0));
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
					//affichageHUD(m_sprites, index, ptrFcnFichier, pTouches);
					m_sprites.ecranNoir.setFillColor(sf::Color(0x0));
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
				case PositionJeu::pause:
					m_sprites.ecranNoir.setFillColor(sf::Color(0x0));
					m_peutDeplacer = !m_peutDeplacer;
					m_index = 1;
					m_sprites.hud.resize(0);
					return;
				case PositionJeu::quitter_pause:
					m_sprites.positionDansJeu = PositionJeu::pause;
					chargementTexteHUD();
					m_index = 0;
					break;
				}
			}

			//chargementTextures(m_sprites.textures, m_sprites.langue, m_sprites.positionDansJeu);
			if (m_index > m_indexMax)
				m_index = m_indexMax;
			//0 ici représente le minimum de l'index
			else if (m_index < 0)
				m_index = 0;
			affichageHUD();

			std::this_thread::sleep_for(std::chrono::milliseconds(100 - debutCycle.restart().asMilliseconds()));
		}
	}

#pragma endregion

	void ecranChargement()
	{
		PLOGI << "Launching level " << m_moteur.niveau;
		m_sprites.hud.resize(1);
		m_sprites.positionDansJeu = PositionJeu::chargement;
		m_sprites.camera.setCenter(m_sprites.camera.getSize() / 2.f);
		m_sprites.ecranNoir.setFillColor(sf::Color(0, 0, 0, 255));
		m_sprites.ecranNoir.setPosition(m_sprites.camera.getCenter() - m_sprites.camera.getSize() / 2.f);
		chargementTexteHUD();
		affichageHUD();
		chargementNiveau();
		creationLimiteCamera(m_moteur);
		std::this_thread::sleep_for(std::chrono::seconds(2));
		m_sprites.ecranNoir.setFillColor(sf::Color(0, 0, 0, 0));
		m_peutDeplacer = true;
		m_sprites.hud.resize(0);
		//m_sprites.positionDansJeu = PositionJeu::jeu;
	}

	void pause()
	{
		m_index = 0;
		m_indexMax = 1;
	}
};


#endif