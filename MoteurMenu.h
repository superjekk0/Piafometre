#pragma once
#ifndef MOTEURMENU_H
#define MOTEURMENU_H

#include "ressources.h"
#include "Jeu.h"
//#include "Checkpoint.h"
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

			//m_sprites.avantPlan.resize(13);
			m_sprites.avantPlan.resetTiles();
			m_sprites.avantPlan.loadTexture(m_sprites.textures[1], "resources/textures/Level1.txt");
			m_sprites.arrierePlan.resize(2);

			//m_sprites.avantPlan[0].sprite.setTexture(m_sprites.textures[2], true);
			//m_sprites.avantPlan[0].sprite.setScale(0.75f, 0.75f);
			//m_sprites.avantPlan[0].sprite.setPosition(0, 720 - m_sprites.textures[2].getSize().y);
			//m_sprites.avantPlan[0].sprite.setTextureRect(sf::IntRect(0, 0, 3000, m_sprites.textures[2].getSize().y));
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				1,
				sf::Vector2f(3000.f, m_sprites.avantPlan.getSubTexture(1).height),
				sf::Vector2f(0.f, 720.f - m_sprites.avantPlan.getSubTexture(1).height),
				TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::solide));
			m_sprites.avantPlan[0]->setScale(0.75f, 0.75f, TextureRule::adjustable_size);
			
			//m_sprites.avantPlan[1].sprite.setTexture(m_sprites.textures[1], true);
			//m_sprites.avantPlan[1].sprite.setTextureRect(sf::IntRect(0, 0, getWidth(m_sprites.textures[1]), getHeight(m_sprites.textures[1]) * 2.5));
			//m_sprites.avantPlan[1].sprite.setScale(0.75f, 0.75f);
			//m_sprites.avantPlan[1].sprite.setPosition(3 * getWidth(m_sprites.textures[2]),
			//	m_sprites.avantPlan[0].sprite.getPosition().y + getHeight(m_sprites.avantPlan[0].sprite) - getHeight(m_sprites.avantPlan[1].sprite));
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0, 
				sf::Vector2f(m_sprites.avantPlan.getSubTexture(0).width, m_sprites.avantPlan.getSubTexture(0).height * 2.5f),
				sf::Vector2f(),
				TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::solide));
			m_sprites.avantPlan[1]->setScale(0.75f, 0.75f, TextureRule::adjustable_size);
			m_sprites.avantPlan[1]->setPosition(3.f * m_sprites.avantPlan[1]->subTextureSize().x,
				m_sprites.avantPlan[0]->getPosition().y + m_sprites.avantPlan[0]->height() - m_sprites.avantPlan[1]->height());
			//m_sprites.avantPlan[2].sprite.setTexture(m_sprites.textures[3], true);
			//m_sprites.avantPlan[2].sprite.setTextureRect(sf::IntRect(0, 0, getWidth(m_sprites.textures[3]) * 4, getHeight(m_sprites.textures[3])));
			//m_sprites.avantPlan[2].sprite.setPosition(m_sprites.avantPlan[1].sprite.getPosition().x + getWidth(m_sprites.avantPlan[1].sprite),
			//	m_sprites.avantPlan[0].sprite.getPosition().y - getHeight(m_sprites.avantPlan[2].sprite));
			//m_sprites.avantPlan[2].comportement = TypePlateforme::pics;

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				2,
				sf::Vector2f(m_sprites.avantPlan.getSubTextureSize(2).x * 4.f, m_sprites.avantPlan.getSubTextureSize(2).y),
				sf::Vector2f(),
				TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::pics));
			m_sprites.avantPlan[2]->setPosition(m_sprites.avantPlan[1]->getPosition().x + m_sprites.avantPlan[1]->width(), m_sprites.avantPlan[0]->getPosition().y - m_sprites.avantPlan[2]->height());
			//m_sprites.avantPlan[3].sprite.setTexture(m_sprites.textures[1], true);
			//m_sprites.avantPlan[3].sprite.setScale(0.75f, .75f);
			//m_sprites.avantPlan[3].sprite.setTextureRect(sf::IntRect(0, 0, m_sprites.avantPlan[0].sprite.getTextureRect().width, getHeight(m_sprites.textures[1]) * 3));
			//m_sprites.avantPlan[3].sprite.setPosition(m_sprites.avantPlan[0].sprite.getPosition().x,
			//	m_sprites.avantPlan[0].sprite.getPosition().y + getHeight(m_sprites.avantPlan[0].sprite));

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(m_sprites.avantPlan[0]->width(), m_sprites.avantPlan.getSubTexture(0).height * 3.f),
				sf::Vector2f(m_sprites.avantPlan[0]->getPosition().x, m_sprites.avantPlan[0]->getPosition().y + m_sprites.avantPlan[0]->height()),
				TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));
			m_sprites.avantPlan[3]->setScale(0.75f, 0.75f, TextureRule::keep_size);
			//m_sprites.avantPlan[4].sprite.setTexture(m_sprites.textures[2], true);
			//m_sprites.avantPlan[4].sprite.setTextureRect(sf::IntRect(0, 0, 3000, getHeight(m_sprites.textures[2])));
			//m_sprites.avantPlan[4].sprite.setPosition(0,
			//	m_sprites.avantPlan[3].sprite.getPosition().y + getHeight(m_sprites.avantPlan[3].sprite) + 500);

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				1,
				sf::Vector2f(3000.f, m_sprites.avantPlan.getSubTexture(1).height),
				sf::Vector2f(0.f, m_sprites.avantPlan[3]->getPosition().y + m_sprites.avantPlan[3]->height() + 500.f),
				TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));
			//m_sprites.avantPlan[5].sprite.setTexture(m_sprites.textures[3], true);
			//m_sprites.avantPlan[5].sprite.setTextureRect(sf::IntRect(0, 0, m_sprites.textures[3].getSize().x, m_sprites.textures[3].getSize().y));
			//m_sprites.avantPlan[5].sprite.setPosition(1500,
			//	m_sprites.avantPlan[4].sprite.getPosition().y - getHeight(m_sprites.avantPlan[5].sprite));
			//m_sprites.avantPlan[5].comportement = TypePlateforme::pics;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				2,
				sf::Vector2f(m_sprites.avantPlan.getSubTextureSize(2)),
				sf::Vector2f(),
				TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::pics));
			m_sprites.avantPlan[5]->setPosition(1500.f, m_sprites.avantPlan[4]->getPosition().y - m_sprites.avantPlan[5]->height());

			//m_sprites.avantPlan[6].sprite.setTexture(m_sprites.textures[3], true);
			//m_sprites.avantPlan[6].sprite.setTextureRect(m_sprites.avantPlan[5].sprite.getTextureRect());
			//m_sprites.avantPlan[6].sprite.setPosition(m_sprites.avantPlan[5].sprite.getPosition().x - 750,
			//	m_sprites.avantPlan[5].sprite.getPosition().y);
			//m_sprites.avantPlan[6].comportement = TypePlateforme::pics;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				2,
				m_sprites.avantPlan[5]->getSize(),
				m_sprites.avantPlan[5]->getPosition() + sf::Vector2f(-750.f, 0.f),
				TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::pics));

			//m_sprites.avantPlan[7].sprite.setTexture(m_sprites.textures[5], true);
			//m_sprites.avantPlan[7].sprite.setTextureRect(sf::IntRect(0, 0, getWidth(m_sprites.textures[5]) / 3, getHeight(m_sprites.textures[5])));
			//m_sprites.avantPlan[7].sprite.setScale(2, 2);
			//m_sprites.avantPlan[7].sprite.setPosition(m_sprites.avantPlan[5].sprite.getPosition().x + 500,
			//	m_sprites.avantPlan[4].sprite.getPosition().y - getHeight(m_sprites.avantPlan[7].sprite));
			//m_sprites.avantPlan[7].comportement = TypePlateforme::checkPoint;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				4,
				sf::Vector2f(m_sprites.avantPlan.getSubTextureSize(4)),
				sf::Vector2f(),
				TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::checkPoint));
			m_sprites.avantPlan[7]->setScale(2.f, TextureRule::keep_height);
			m_sprites.avantPlan[7]->setPosition(m_sprites.avantPlan[5]->getPosition().x + 500.f,
				m_sprites.avantPlan[4]->getPosition().y - m_sprites.avantPlan[7]->height());

			//m_sprites.avantPlan[8].sprite.setTexture(m_sprites.textures[6], true);
			//m_sprites.avantPlan[8].sprite.setPosition(m_sprites.avantPlan[1].sprite.getPosition().x - 100 - getWidth(m_sprites.avantPlan[8].sprite),
			//	m_sprites.avantPlan[0].sprite.getPosition().y - getHeight(m_sprites.avantPlan[8].sprite));
			//m_sprites.avantPlan[8].comportement = TypePlateforme::objet;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				7,
				m_sprites.avantPlan.getSubTextureSize(7),
				sf::Vector2f(),
				TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::objet));
			m_sprites.avantPlan[8]->setPosition(m_sprites.avantPlan[1]->getPosition().x - m_sprites.avantPlan[8]->width() -100.f,
				m_sprites.avantPlan[0]->getPosition().y - m_sprites.avantPlan[8]->height());
			//m_sprites.avantPlan[9].sprite.setTexture(m_sprites.textures[6], true);
			//m_sprites.avantPlan[9].sprite.setPosition(m_sprites.avantPlan[7].sprite.getPosition().x - getWidth(m_sprites.avantPlan[9].sprite) - 40,
			//	m_sprites.avantPlan[4].sprite.getPosition().y - getHeight(m_sprites.avantPlan[9].sprite));
			//m_sprites.avantPlan[9].comportement = TypePlateforme::objet;

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				7,
				m_sprites.avantPlan.getSubTextureSize(7),
				sf::Vector2f(),
				TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::objet
			));
			m_sprites.avantPlan[9]->setPosition(sf::Vector2f(m_sprites.avantPlan[7]->getPosition().x - m_sprites.avantPlan[9]->getSize().x - 40.f, 
				m_sprites.avantPlan[4]->getPosition().y - m_sprites.avantPlan[9]->height()));
			//m_sprites.avantPlan[10].sprite.setTexture(m_sprites.textures[4], true);
			//m_sprites.avantPlan[10].sprite.setPosition(
			//	m_sprites.avantPlan[4].sprite.getPosition() - sf::Vector2f(0.f, getHeight(m_sprites.textures[4])));
			//m_sprites.avantPlan[10].comportement = TypePlateforme::finTableau;

			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				3,
				m_sprites.avantPlan.getSubTextureSize(3),
				m_sprites.avantPlan[4]->getPosition() + sf::Vector2f(0.f, -m_sprites.avantPlan.getSubTextureSize(3).y),
				TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::finTableau
			));

			m_sprites.joueur.setPosition(m_sprites.avantPlan[0]->getPosition().x,
				m_sprites.avantPlan[0]->topLeftCorner().y - getHeight(m_sprites.joueur));

			m_sprites.arrierePlan[0].setTexture(m_sprites.textures[2], true);
			m_sprites.arrierePlan[0].setScale(7, 7);
			m_sprites.arrierePlan[0].setPosition(0, -200);

			m_sprites.arrierePlan[1].setTexture(m_sprites.textures[3], true);
			m_sprites.arrierePlan[1].setScale(7, 7);
			m_sprites.arrierePlan[1].setPosition(0, -150);

			break;
		case 2:
			m_sprites.couleur = sf::Color(0x2D100DFF);
			m_sprites.positionDansJeu = PositionJeu::jeu;

		//	m_sprites.avantPlan.resize(22);
			m_sprites.arrierePlan.resize(1);
			m_sprites.avantPlan.resetTiles();
			m_sprites.avantPlan.loadTexture(m_sprites.textures[1], "resources/textures/Level2.txt");

		//	m_sprites.avantPlan[0].sprite.setTexture(m_sprites.textures[1], true);
		//	m_sprites.avantPlan[0].sprite.setTextureRect(sf::IntRect(0, 0, 500, getHeight(m_sprites.textures[2])));
		//	m_sprites.avantPlan[0].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[0].sprite.setPosition(sf::Vector2f(0.f, 400.f));
		//	m_sprites.avantPlan[0].comportement = TypePlateforme::solide;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(500.f, m_sprites.avantPlan.getSubTextureSize(0).y),
				sf::Vector2f(0.f, 400.f),
				TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

		//	m_sprites.avantPlan[1].sprite.setTexture(m_sprites.textures[3], true);
		//	m_sprites.avantPlan[1].sprite.setTextureRect(sf::IntRect(0, 0, getWidth(m_sprites.textures[3]), 200));
		//	m_sprites.avantPlan[1].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[1].sprite.setPosition(
		//		m_sprites.avantPlan[0].coinSpriteDroitHaut()
		//		+ sf::Vector2f(-getWidth(m_sprites.avantPlan[1].sprite), -getHeight(m_sprites.avantPlan[1].sprite))
		//	);
		//	m_sprites.avantPlan[1].comportement = TypePlateforme::solide;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				2,
				sf::Vector2f(m_sprites.avantPlan.getSubTexture(2).width, 200.f),
				sf::Vector2f(),
				TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));
			m_sprites.avantPlan[1]->setPosition(m_sprites.avantPlan[0]->topRightCorner() - m_sprites.avantPlan[1]->getSize());
		//	m_sprites.avantPlan[2].sprite.setTexture(m_sprites.textures[4], true);
		//	m_sprites.avantPlan[2].sprite.setTextureRect(sf::IntRect(0, 0, 400, (int)getHeight(m_sprites.avantPlan[2].sprite)));
		//	m_sprites.avantPlan[2].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[2].sprite.setPosition(sf::Vector2f(m_sprites.avantPlan[0].coinSpriteDroitHaut()));
		//	m_sprites.avantPlan[2].comportement = TypePlateforme::pics;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				3,
				sf::Vector2f(400.f, m_sprites.avantPlan.getSubTextureSize(3).y),
				m_sprites.avantPlan[0]->topRightCorner(),
				TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::pics));
		//	m_sprites.avantPlan[3].sprite.setTexture(m_sprites.textures[1], true);
		//	m_sprites.avantPlan[3].sprite.setTextureRect(sf::IntRect(0, 0, 400, 200));
		//	m_sprites.avantPlan[3].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[3].sprite.setPosition(m_sprites.avantPlan[2].coinSpriteGaucheBas());
		//	m_sprites.avantPlan[3].comportement = TypePlateforme::solide;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(400.f, 200.f),
				m_sprites.avantPlan[2]->bottomLeftCorner(),
				TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

		//	m_sprites.avantPlan[4].sprite.setTexture(m_sprites.textures[4], true);
		//	m_sprites.avantPlan[4].sprite.setTextureRect(sf::IntRect(0, 0, 400, (int)getHeight(m_sprites.avantPlan[2].sprite)));
		//	m_sprites.avantPlan[4].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[4].sprite.setPosition(sf::Vector2f(m_sprites.avantPlan[0].coinSpriteDroitHaut()) + sf::Vector2f(600.f, 0.f));
		//	m_sprites.avantPlan[4].comportement = TypePlateforme::pics;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				3,
				sf::Vector2f(400.f, m_sprites.avantPlan[2]->height()),
				m_sprites.avantPlan[0]->topRightCorner() + sf::Vector2f(600.f, 0.f),
				TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::pics));

		//	m_sprites.avantPlan[5].sprite.setTexture(m_sprites.textures[1], true);
		//	m_sprites.avantPlan[5].sprite.setTextureRect(sf::IntRect(0, 0, 400, 200));
		//	m_sprites.avantPlan[5].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[5].sprite.setPosition(m_sprites.avantPlan[4].coinSpriteGaucheBas());
		//	m_sprites.avantPlan[5].comportement = TypePlateforme::solide;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(400.f, 200.f),
				m_sprites.avantPlan[4]->bottomLeftCorner(),
				TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

		//	m_sprites.avantPlan[6].sprite.setTexture(m_sprites.textures[4], true);
		//	m_sprites.avantPlan[6].sprite.setTextureRect(sf::IntRect(0, 0, 400, static_cast<int>(getHeight(m_sprites.avantPlan[2].sprite))));
		//	m_sprites.avantPlan[6].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[6].sprite.setPosition(sf::Vector2f(m_sprites.avantPlan[4].coinSpriteDroitHaut() + sf::Vector2f(400.f, 0.f)));
		//	m_sprites.avantPlan[6].comportement = TypePlateforme::pics;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				3,
				sf::Vector2f(400.f, m_sprites.avantPlan[2]->height()),
					m_sprites.avantPlan[4]->topRightCorner() + sf::Vector2f(400.f, 0.f),
					TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::pics));

		//	m_sprites.avantPlan[7].sprite.setTexture(m_sprites.textures[1], true);
		//	m_sprites.avantPlan[7].sprite.setTextureRect(sf::IntRect(0, 0, 400, 200));
		//	m_sprites.avantPlan[7].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[7].sprite.setPosition(m_sprites.avantPlan[6].coinSpriteGaucheBas());
		//	m_sprites.avantPlan[7].comportement = TypePlateforme::solide;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(400.f, 200.f),
				m_sprites.avantPlan[6]->bottomLeftCorner(),
				TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));
		//	m_sprites.avantPlan[8].sprite.setTexture(m_sprites.textures[1], true);
		//	m_sprites.avantPlan[8].sprite.setTextureRect(sf::IntRect(0, 0, 400, 200));
		//	m_sprites.avantPlan[8].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[8].sprite.setPosition(m_sprites.avantPlan[2].coinSpriteGaucheBas());
		//	m_sprites.avantPlan[8].comportement = TypePlateforme::solide;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(400.f, 200.f),
				m_sprites.avantPlan[2]->bottomLeftCorner(),
				TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

		//	m_sprites.avantPlan[9].sprite.setTexture(m_sprites.textures[5], true);
		//	m_sprites.avantPlan[9].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[9].sprite.setPosition(sf::Vector2f(((m_sprites.avantPlan[2].coinSpriteDroitHaut().x + m_sprites.avantPlan[4].coinSpriteGaucheHaut().x) / 2.f) - getWidth(m_sprites.avantPlan[9].sprite) / 2.f, 250.f));
		//	m_sprites.avantPlan[9].comportement = TypePlateforme::objet;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				4,
				m_sprites.avantPlan.getSubTextureSize(4),
				sf::Vector2f(),
				TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::objet));
			m_sprites.avantPlan[9]->setPosition(((m_sprites.avantPlan[2]->topRightCorner().x + m_sprites.avantPlan[4]->topLeftCorner().x) / 2.f) - m_sprites.avantPlan[9]->width() / 2.f, 250.f);

		//	m_sprites.avantPlan[10].sprite.setTexture(m_sprites.textures[5], true);
		//	m_sprites.avantPlan[10].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[10].sprite.setPosition(((m_sprites.avantPlan[4].coinSpriteDroitHaut().x + m_sprites.avantPlan[6].coinSpriteGaucheHaut().x) / 2.f) - getWidth(m_sprites.avantPlan[10].sprite) / 2.f, 250.f);
		//	m_sprites.avantPlan[10].comportement = TypePlateforme::objet;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				4,
				m_sprites.avantPlan.getSubTextureSize(4),
				sf::Vector2f(),
				TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::objet));
			m_sprites.avantPlan[10]->setPosition(((m_sprites.avantPlan[4]->topRightCorner().x + m_sprites.avantPlan[6]->topLeftCorner().x) / 2.f) - m_sprites.avantPlan[10]->width() / 2.f, 250.f);

		//	m_sprites.avantPlan[11].sprite.setTexture(m_sprites.textures[1], true);
		//	m_sprites.avantPlan[11].sprite.setTextureRect(sf::IntRect(0, 0, 700, getHeight(m_sprites.textures[1])));
		//	m_sprites.avantPlan[11].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[11].sprite.setPosition(m_sprites.avantPlan[7].coinSpriteDroitHaut());
		//	m_sprites.avantPlan[11].comportement = TypePlateforme::solide;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				0,
				sf::Vector2f(700.f, m_sprites.avantPlan.getSubTextureSize(0).y),
				m_sprites.avantPlan[7]->topRightCorner(),
				TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

		//	m_sprites.avantPlan[12].sprite.setTexture(m_sprites.textures[7], true);
		//	m_sprites.avantPlan[12].sprite.setTextureRect(sf::IntRect(0, 0, getWidth(m_sprites.textures[7]) / 3, getHeight(m_sprites.textures[7])));
		//	m_sprites.avantPlan[12].sprite.setScale(2.f, 2.f);
		//	m_sprites.avantPlan[12].sprite.setPosition(m_sprites.avantPlan[11].coinSpriteGaucheHaut() + sf::Vector2f(50.f, -getWidth(m_sprites.avantPlan[12].sprite)));
		//	m_sprites.avantPlan[12].comportement = TypePlateforme::checkPoint;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				6,
				m_sprites.avantPlan.getSubTextureSize(6),
				sf::Vector2f(),
				TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::checkPoint));
			m_sprites.avantPlan[12]->setScale(2.f);
			m_sprites.avantPlan[12]->setPosition(m_sprites.avantPlan[11]->getPosition() + sf::Vector2f(50.f, -m_sprites.avantPlan[12]->width()));

		//	m_sprites.avantPlan[13].sprite.setTexture(m_sprites.textures[5], true);
		//	m_sprites.avantPlan[13].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[13].sprite.setPosition(m_sprites.avantPlan[1].coinSpriteGaucheBas() + sf::Vector2f(-75.f, -getHeight(m_sprites.avantPlan[13].sprite)));
		//	m_sprites.avantPlan[13].comportement = TypePlateforme::objet;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				4,
				m_sprites.avantPlan.getSubTextureSize(4),
				sf::Vector2f(),
				TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::objet));
			m_sprites.avantPlan[13]->setPosition(m_sprites.avantPlan[1]->bottomLeftCorner() - sf::Vector2f(75.f, m_sprites.avantPlan[13]->height()));

		//	m_sprites.avantPlan[14].sprite.setTexture(m_sprites.textures[3], true);
		//	m_sprites.avantPlan[14].sprite.setTextureRect(sf::IntRect(0, 0, 3000, 400));
		//	m_sprites.avantPlan[14].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[14].sprite.setPosition(sf::Vector2f(0.f, -500.f));
		//	m_sprites.avantPlan[14].comportement = TypePlateforme::solide;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				2,
				sf::Vector2f(3000.f, 400.f),
				sf::Vector2f(0.f, -500.f),
				TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

		//	m_sprites.avantPlan[15].sprite.setTexture(m_sprites.textures[3], true);
		//	m_sprites.avantPlan[15].sprite.setTextureRect(sf::IntRect(0, 0, 200, 1000));
		//	m_sprites.avantPlan[15].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[15].sprite.setPosition(m_sprites.avantPlan[11].coinSpriteDroitHaut() + sf::Vector2f(-getWidth(m_sprites.avantPlan[15].sprite), -1200.f));
		//	m_sprites.avantPlan[15].comportement = TypePlateforme::solide;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				2,
				sf::Vector2f(200.f, 1000.f),
				sf::Vector2f(),
				TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));
			m_sprites.avantPlan[15]->setPosition(m_sprites.avantPlan[11]->topRightCorner() - sf::Vector2f(m_sprites.avantPlan[15]->width(), 1200.f));

		//	m_sprites.avantPlan[16].sprite.setTexture(m_sprites.textures[3], true);
		//	m_sprites.avantPlan[16].sprite.setTextureRect(sf::IntRect(0, 0, 300, 700));
		//	m_sprites.avantPlan[16].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[16].sprite.setPosition(m_sprites.avantPlan[11].coinSpriteDroitHaut() + sf::Vector2f(500.f, -400.f));
		//	m_sprites.avantPlan[16].comportement = TypePlateforme::solide;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				2,
				sf::Vector2f(300.f, 700.f),
				m_sprites.avantPlan[11]->topRightCorner() + sf::Vector2f(500.f, -400.f),
				TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

		//	m_sprites.avantPlan[17].sprite.setTexture(m_sprites.textures[5], true);
		//	m_sprites.avantPlan[16].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[17].sprite.setPosition(m_sprites.avantPlan[11].coinSpriteDroitHaut() + sf::Vector2f(-200.f, -getHeight(m_sprites.avantPlan[17].sprite)));
		//	m_sprites.avantPlan[17].comportement = TypePlateforme::objet;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				4,
				m_sprites.avantPlan.getSubTextureSize(4),
				sf::Vector2f(),
				TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::objet));
			m_sprites.avantPlan[17]->setPosition(m_sprites.avantPlan[11]->topRightCorner() - sf::Vector2f(200.f, m_sprites.avantPlan[17]->height()));

		//	m_sprites.avantPlan[18].sprite.setTexture(m_sprites.textures[5], true);
		//	m_sprites.avantPlan[18].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[18].sprite.setPosition((m_sprites.avantPlan[11].coinSpriteDroitHaut() + m_sprites.avantPlan[16].coinSpriteGaucheHaut()) / 2.f);
		//	m_sprites.avantPlan[18].comportement = TypePlateforme::objet;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				4,
				m_sprites.avantPlan.getSubTextureSize(4),
				(m_sprites.avantPlan[11]->topRightCorner() + m_sprites.avantPlan[16]->topLeftCorner()) / 2.f,
				TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::objet));

		//	m_sprites.avantPlan[19].sprite.setTexture(m_sprites.textures[2], true);
		//	m_sprites.avantPlan[19].sprite.setTextureRect(sf::IntRect(0, 0, 1000, getHeight(m_sprites.textures[2])));
		//	m_sprites.avantPlan[19].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[19].sprite.setPosition(m_sprites.avantPlan[15].coinSpriteDroitHaut() + sf::Vector2f(0.f, 300.f));
		//	m_sprites.avantPlan[19].comportement = TypePlateforme::solide;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				1,
				sf::Vector2f(1000.f, m_sprites.avantPlan.getSubTextureSize(1).y),
				m_sprites.avantPlan[15]->topRightCorner() + sf::Vector2f(0.f, 300.f),
				TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));

		//	m_sprites.avantPlan[20].sprite.setTexture(m_sprites.textures[3], true);
		//	m_sprites.avantPlan[20].sprite.setTextureRect(sf::IntRect(0, 0, 1000, 500));
		//	m_sprites.avantPlan[20].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[20].sprite.setPosition(m_sprites.avantPlan[19].coinSpriteGaucheHaut() + sf::Vector2f(0.f, -getHeight(m_sprites.avantPlan[20].sprite)));
		//	m_sprites.avantPlan[20].comportement = TypePlateforme::solide;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				2,
				sf::Vector2f(1000.f, 500.f),
				sf::Vector2f(),
				TextureRule::repeat_texture,
				m_sprites.avantPlan.getSubTextures()));
			m_sprites.avantPlan[20]->setPosition(m_sprites.avantPlan[19]->getPosition() + sf::Vector2f(0.f, -m_sprites.avantPlan[20]->height()));

		//	m_sprites.avantPlan[21].sprite.setTexture(m_sprites.textures[6], true);
		//	m_sprites.avantPlan[21].sprite.setScale(1.5f, 1.5f);
		//	m_sprites.avantPlan[21].sprite.setPosition(m_sprites.avantPlan[16].coinSpriteGaucheHaut() + sf::Vector2f(0.f, -getHeight(m_sprites.avantPlan[21].sprite)));
		//	m_sprites.avantPlan[21].comportement = TypePlateforme::finTableau;
			m_sprites.avantPlan.add(PlateformeOptimisee(m_sprites.avantPlan.getTexture(),
				5,
				m_sprites.avantPlan.getSubTextureSize(5) * 1.5f,
				sf::Vector2f(),
				TextureRule::fill_space,
				m_sprites.avantPlan.getSubTextures(),
				TypePlateforme::finTableau));
			m_sprites.avantPlan[21]->setPosition(m_sprites.avantPlan[16]->getPosition() + sf::Vector2f(0.f, -m_sprites.avantPlan[21]->height()));

			m_sprites.joueur.setPosition(m_sprites.avantPlan[0]->topLeftCorner() + sf::Vector2f(0.f, -getHeight(m_sprites.joueur)));

			m_sprites.arrierePlan[0].setTexture(m_sprites.textures[8], true);
			m_sprites.arrierePlan[0].setTextureRect(sf::IntRect(0, 0, 8000, getHeight(m_sprites.textures[8])));
			m_sprites.arrierePlan[0].setPosition(-100.f, -250.f);
			m_sprites.arrierePlan[0].setScale(0.25f, 0.25f);
			break;
		//case 3:
		//	m_sprites.couleur = sf::Color(0xD4E7FFFF);
		//	m_sprites.positionDansJeu = PositionJeu::jeu;
		//	

		//	m_sprites.arrierePlan.resize(1);
		//	m_sprites.avantPlan.resize(13);

		//	m_sprites.avantPlan[0].sprite.setTexture(m_sprites.textures[1], true);
		//	m_sprites.avantPlan[0].sprite.setTextureRect(sf::IntRect(0, 0, 1280, 200));
		//	m_sprites.avantPlan[0].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[0].sprite.setPosition(0.f, 550.f);
		//	m_sprites.avantPlan[0].comportement = TypePlateforme::solide;

		//	m_sprites.avantPlan[1].sprite.setTexture(m_sprites.textures[3], true);
		//	m_sprites.avantPlan[1].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[1].sprite.setPosition(m_sprites.avantPlan[0].coinSpriteDroitHaut() + sf::Vector2f(-getWidth(m_sprites.avantPlan[1].sprite) - 120.f, -getHeight(m_sprites.avantPlan[1].sprite)));
		//	m_sprites.avantPlan[1].comportement = TypePlateforme::objet;

		//	m_sprites.avantPlan[2].sprite.setTexture(m_sprites.textures[1], true);
		//	m_sprites.avantPlan[2].sprite.setColor(sf::Color(0xAAAAAAFF));
		//	m_sprites.avantPlan[2].sprite.setTextureRect(sf::IntRect(0, 0, 300, 500));
		//	m_sprites.avantPlan[2].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[2].sprite.setPosition((m_sprites.avantPlan[0].coinSpriteDroitHaut() + m_sprites.avantPlan[0].coinSpriteGaucheHaut()) / 2.f + sf::Vector2f(0.f, -getHeight(m_sprites.avantPlan[2].sprite)));
		//	m_sprites.avantPlan[2].comportement = TypePlateforme::semiSolide;

		//	m_sprites.avantPlan[3].sprite.setTexture(m_sprites.textures[1], true);
		//	m_sprites.avantPlan[3].sprite.setTextureRect(sf::IntRect(0, 0, 400, 150));
		//	m_sprites.avantPlan[3].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[3].sprite.setPosition(m_sprites.avantPlan[2].coinSpriteDroitHaut());
		//	m_sprites.avantPlan[3].comportement = TypePlateforme::solide;

		//	m_sprites.avantPlan[4].sprite.setTexture(m_sprites.textures[1], true);
		//	m_sprites.avantPlan[4].sprite.setTextureRect(sf::IntRect(0, 0, 500, 250));
		//	m_sprites.avantPlan[4].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[4].sprite.setPosition(0.f, 0.f);
		//	m_sprites.avantPlan[4].comportement = TypePlateforme::solide;

		//	m_sprites.avantPlan[5].sprite.setTexture(m_sprites.textures[3], true);
		//	m_sprites.avantPlan[5].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[5].sprite.setPosition(m_sprites.avantPlan[4].coinSpriteGaucheHaut() + sf::Vector2f(200.f, -getHeight(m_sprites.avantPlan[5].sprite)));
		//	m_sprites.avantPlan[5].comportement = TypePlateforme::objet;

		//	m_sprites.avantPlan[6].sprite.setTexture(m_sprites.textures[1], true);
		//	m_sprites.avantPlan[6].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[6].sprite.setColor(sf::Color(0xAAAAAAFF));
		//	m_sprites.avantPlan[6].sprite.setTextureRect(sf::IntRect(0, 0, 300, 500));
		//	m_sprites.avantPlan[6].sprite.setPosition(m_sprites.avantPlan[4].coinSpriteDroitHaut() + sf::Vector2f(-150.f, -getHeight(m_sprites.avantPlan[6].sprite)));
		//	m_sprites.avantPlan[6].comportement = TypePlateforme::semiSolide;

		//	m_sprites.avantPlan[7].sprite.setTexture(m_sprites.textures[1], true);
		//	m_sprites.avantPlan[7].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[7].sprite.setTextureRect(sf::IntRect(0, 0, 300, 150));
		//	m_sprites.avantPlan[7].sprite.setPosition(m_sprites.avantPlan[6].coinSpriteDroitHaut());
		//	m_sprites.avantPlan[7].comportement = TypePlateforme::solide;

		//	m_sprites.avantPlan[8].sprite.setTexture(m_sprites.textures[2], true);
		//	m_sprites.avantPlan[8].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[8].sprite.setTextureRect(sf::IntRect(0, 0, 350, getHeight(m_sprites.avantPlan[8].sprite)));
		//	m_sprites.avantPlan[8].sprite.setPosition(m_sprites.avantPlan[3].coinSpriteDroitHaut() + sf::Vector2f(-getWidth(m_sprites.avantPlan[8].sprite), -getHeight(m_sprites.avantPlan[8].sprite)));
		//	m_sprites.avantPlan[8].comportement = TypePlateforme::pics;

		//	m_sprites.avantPlan[9].sprite.setTexture(m_sprites.textures[4], true);
		//	m_sprites.avantPlan[9].sprite.setScale(10.f, 5.f);
		//	m_sprites.avantPlan[9].sprite.setPosition(m_sprites.avantPlan[7].coinSpriteGaucheHaut() + sf::Vector2f(0.f, -350.f));
		//	m_sprites.avantPlan[9].comportement = TypePlateforme::semiSolide;

		//	m_sprites.avantPlan[10].sprite.setTexture(m_sprites.textures[5], true);
		//	m_sprites.avantPlan[10].sprite.setTextureRect(sf::IntRect(0, 0, 400, 180));
		//	m_sprites.avantPlan[10].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[10].sprite.setPosition(m_sprites.avantPlan[9].coinSpriteDroitHaut());
		//	m_sprites.avantPlan[10].comportement = TypePlateforme::solide;

		//	m_sprites.avantPlan[11].sprite.setTexture(m_sprites.textures[3], true);
		//	m_sprites.avantPlan[11].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[11].sprite.setPosition((m_sprites.avantPlan[7].coinSpriteGaucheHaut() + m_sprites.avantPlan[7].coinSpriteDroitHaut()) / 2.f + sf::Vector2f(0.f, -getHeight(m_sprites.avantPlan[11].sprite)));
		//	m_sprites.avantPlan[11].comportement = TypePlateforme::objet;

		//	m_sprites.avantPlan[12].sprite.setTexture(m_sprites.textures[6], true);
		//	m_sprites.avantPlan[12].sprite.setScale(2.f, 2.f);
		//	m_sprites.avantPlan[12].sprite.setPosition(m_sprites.avantPlan[10].coinSpriteGaucheHaut() + sf::Vector2f(0.f, -getHeight(m_sprites.avantPlan[12].sprite)));
		//	m_sprites.avantPlan[12].comportement = TypePlateforme::finTableau;

		//	m_sprites.joueur.setPosition(m_sprites.avantPlan[0].coinSpriteGaucheHaut() + sf::Vector2f(0.f, -getHeight(m_sprites.joueur)));

		//	m_sprites.arrierePlan[0].setTexture(m_sprites.textures[7]);
		//	m_sprites.arrierePlan[0].setScale(4.25f, 4.25f);
		//	m_sprites.arrierePlan[0].setTextureRect(sf::IntRect(0, 0, 1500, getHeight(m_sprites.textures[7])));
		//	m_sprites.arrierePlan[0].setPosition(0.f, -400.f);
		//	break;
		//case 4:
		//	m_sprites.couleur = sf::Color(0xFEFEE2FF);
		//	m_sprites.positionDansJeu = PositionJeu::jeu;

		//	m_sprites.avantPlan.resize(21);
		//	m_sprites.arrierePlan.resize(1);

		//	m_sprites.avantPlan[0].sprite.setTexture(m_sprites.textures[1], true);
		//	m_sprites.avantPlan[0].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[0].sprite.setTextureRect(sf::IntRect(0, 0, 1280, getHeight(m_sprites.textures[1])));
		//	m_sprites.avantPlan[0].sprite.setPosition(0.f, 720.f - getHeight(m_sprites.avantPlan[0].sprite));
		//	m_sprites.avantPlan[0].comportement = TypePlateforme::solide;

		//	m_sprites.avantPlan[1].sprite.setTexture(m_sprites.textures[2], true);
		//	m_sprites.avantPlan[1].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[1].sprite.setTextureRect(sf::IntRect(0, 0, 1280, getHeight(m_sprites.avantPlan[1].sprite)));
		//	m_sprites.avantPlan[1].sprite.setPosition(m_sprites.avantPlan[0].coinSpriteGaucheHaut() + sf::Vector2f(0.f, -getHeight(m_sprites.avantPlan[1].sprite)));
		//	m_sprites.avantPlan[1].comportement = TypePlateforme::solide;

		//	m_sprites.avantPlan[2].sprite.setTexture(m_sprites.textures[2], true);
		//	m_sprites.avantPlan[2].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[2].sprite.setTextureRect(sf::IntRect(0, 0, 600, getHeight(m_sprites.textures[2])));
		//	m_sprites.avantPlan[2].sprite.setPosition((m_sprites.avantPlan[1].coinSpriteDroitHaut() + m_sprites.avantPlan[1].coinSpriteGaucheHaut()) / 2.f + sf::Vector2f(-getWidth(m_sprites.avantPlan[2].sprite) / 2.f, -150.f - getHeight(m_sprites.avantPlan[2].sprite)));
		//	m_sprites.avantPlan[2].sprite.setColor(sf::Color(0xFFFFFFFF));
		//	m_sprites.avantPlan[2].comportement = TypePlateforme::solide;

		//	m_sprites.avantPlan[3].sprite.setTexture(m_sprites.textures[7], true);
		//	m_sprites.avantPlan[3].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[3].sprite.setPosition(m_sprites.avantPlan[2].coinSpriteGaucheBas().x - getWidth(m_sprites.avantPlan[3].sprite)
		//		, m_sprites.avantPlan[1].coinSpriteGaucheHaut().y - getHeight(m_sprites.avantPlan[3].sprite));
		//	m_sprites.avantPlan[3].comportement = TypePlateforme::objet;

		//	m_sprites.avantPlan[4].sprite.setTexture(m_sprites.textures[7], true);
		//	m_sprites.avantPlan[4].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[4].sprite.setPosition(m_sprites.avantPlan[2].coinSpriteDroitBas().x,
		//		m_sprites.avantPlan[1].coinSpriteGaucheHaut().y - getHeight(m_sprites.avantPlan[3].sprite));
		//	m_sprites.avantPlan[4].comportement = TypePlateforme::objet;
		//	
		//	m_sprites.avantPlan[5].sprite.setTexture(m_sprites.textures[3], true);
		//	m_sprites.avantPlan[5].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[5].sprite.setTextureRect(sf::IntRect(0, 0, getWidth(m_sprites.avantPlan[2].sprite), getHeight(m_sprites.textures[3])));
		//	m_sprites.avantPlan[5].sprite.setPosition(m_sprites.avantPlan[2].coinSpriteGaucheHaut() + sf::Vector2f(0.f, -getHeight(m_sprites.avantPlan[5].sprite)));
		//	m_sprites.avantPlan[5].comportement = TypePlateforme::pics;

		//	m_sprites.avantPlan[6].sprite.setTexture(m_sprites.textures[1], true);
		//	m_sprites.avantPlan[6].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[6].sprite.setTextureRect(sf::IntRect(0, 35, 125, getHeight(m_sprites.textures[1]) * 5.f));
		//	m_sprites.avantPlan[6].sprite.setPosition((m_sprites.avantPlan[2].coinSpriteGaucheBas() + m_sprites.avantPlan[2].coinSpriteDroitBas()) / 2.f + sf::Vector2f(-getWidth(m_sprites.avantPlan[6].sprite) / 2.f, -getHeight(m_sprites.avantPlan[6].sprite)));
		//	m_sprites.avantPlan[6].sprite.setColor(sf::Color(0xFFFFFFFF));
		//	m_sprites.avantPlan[6].comportement = TypePlateforme::solide;

		//	m_sprites.avantPlan[7].sprite.setTexture(m_sprites.textures[7], true);
		//	m_sprites.avantPlan[7].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[7].sprite.setPosition(m_sprites.avantPlan[6].coinSpriteGaucheHaut() + sf::Vector2f(-100.f, 200.f));
		//	m_sprites.avantPlan[7].comportement = TypePlateforme::objet;

		//	m_sprites.avantPlan[8].sprite.setTexture(m_sprites.textures[7], true);
		//	m_sprites.avantPlan[8].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[8].sprite.setPosition(m_sprites.avantPlan[6].coinSpriteDroitHaut() + sf::Vector2f(50.f, 200.f));
		//	m_sprites.avantPlan[8].comportement = TypePlateforme::objet;

		//	m_sprites.avantPlan[9].sprite.setTexture(m_sprites.textures[4], true);
		//	m_sprites.avantPlan[9].sprite.setScale(10.f, 5.f);
		//	m_sprites.avantPlan[9].sprite.setPosition((m_sprites.avantPlan[6].coinSpriteDroitHaut() + m_sprites.avantPlan[6].coinSpriteGaucheHaut()) / 2.f + sf::Vector2f(-getWidth(m_sprites.avantPlan[9].sprite) / 2.f, -getHeight(m_sprites.avantPlan[9].sprite)));
		//	m_sprites.avantPlan[9].comportement = TypePlateforme::semiSolide;

		//	m_sprites.avantPlan[10].sprite.setTexture(m_sprites.textures[2], true);
		//	m_sprites.avantPlan[10].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[10].sprite.setTextureRect(sf::IntRect(0, 0, 300, getHeight(m_sprites.textures[2])));
		//	m_sprites.avantPlan[10].sprite.setPosition(0.f, m_sprites.avantPlan[9].coinSpriteGaucheHaut().y - 180.f);
		//	m_sprites.avantPlan[10].comportement = TypePlateforme::solide;

		//	m_sprites.avantPlan[11].sprite.setTexture(m_sprites.textures[2], true);
		//	m_sprites.avantPlan[11].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[11].sprite.setTextureRect(sf::IntRect(0, 0, 600, getHeight(m_sprites.textures[2])));
		//	m_sprites.avantPlan[11].sprite.setPosition(m_sprites.avantPlan[10].coinSpriteDroitHaut() + sf::Vector2f(0.f, -250.f));
		//	m_sprites.avantPlan[11].comportement = TypePlateforme::solide;

		//	m_sprites.avantPlan[12].sprite.setTexture(m_sprites.textures[6], true);
		//	m_sprites.avantPlan[12].sprite.setScale(1.5f, 1.5f);
		//	m_sprites.avantPlan[12].sprite.setPosition(m_sprites.avantPlan[11].coinSpriteGaucheHaut() + sf::Vector2f(0.f, -getHeight(m_sprites.avantPlan[12].sprite)));
		//	m_sprites.avantPlan[12].comportement = TypePlateforme::decor;

		//	m_sprites.avantPlan[13].sprite.setTexture(m_sprites.textures[6], true);
		//	m_sprites.avantPlan[13].sprite.setScale(1.5f, 1.5f);
		//	m_sprites.avantPlan[13].sprite.setPosition(m_sprites.avantPlan[11].coinSpriteDroitHaut() + sf::Vector2f(-getWidth(m_sprites.avantPlan[13].sprite), -getHeight(m_sprites.avantPlan[13].sprite)));
		//	m_sprites.avantPlan[13].comportement = TypePlateforme::decor;

		//	m_sprites.avantPlan[14].sprite.setTexture(m_sprites.textures[5], true);
		//	m_sprites.avantPlan[14].sprite.setScale(2.f, 2.f);
		//	m_sprites.avantPlan[14].sprite.setTextureRect(sf::IntRect(0, 0, getWidth(m_sprites.textures[5]) / 3, getHeight(m_sprites.textures[5])));
		//	m_sprites.avantPlan[14].sprite.setPosition((m_sprites.avantPlan[12].coinSpriteDroitBas() + m_sprites.avantPlan[13].coinSpriteGaucheBas()) / 2.f + sf::Vector2f(-getWidth(m_sprites.avantPlan[14].sprite) / 2.f, -getHeight(m_sprites.avantPlan[14].sprite)));
		//	m_sprites.avantPlan[14].comportement = TypePlateforme::checkPoint;

		//	m_sprites.avantPlan[15].sprite.setTexture(m_sprites.textures[1], true);
		//	m_sprites.avantPlan[15].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[15].sprite.setTextureRect(sf::IntRect(0, 0, 150, getHeight(m_sprites.avantPlan[15].sprite)));
		//	m_sprites.avantPlan[15].sprite.setPosition(0.f, m_sprites.avantPlan[12].coinSpriteGaucheHaut().y);
		//	m_sprites.avantPlan[15].comportement = TypePlateforme::solide;

		//	m_sprites.avantPlan[16].sprite.setTexture(m_sprites.textures[7], true);
		//	m_sprites.avantPlan[16].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[16].sprite.setPosition(m_sprites.avantPlan[12].coinSpriteGaucheBas() + sf::Vector2f(0.f, -getHeight(m_sprites.avantPlan[16].sprite)));
		//	m_sprites.avantPlan[16].comportement = TypePlateforme::objet;

		//	m_sprites.avantPlan[17].sprite.setTexture(m_sprites.textures[1], true);
		//	m_sprites.avantPlan[17].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[17].sprite.setTextureRect(sf::IntRect(0, 0, 1000, 350));
		//	m_sprites.avantPlan[17].sprite.setPosition(m_sprites.avantPlan[15].coinSpriteDroitHaut() + sf::Vector2f(150.f, -getHeight(m_sprites.avantPlan[17].sprite)));
		//	m_sprites.avantPlan[17].comportement = TypePlateforme::solide;

		//	m_sprites.avantPlan[18].sprite.setTexture(m_sprites.textures[7], true);
		//	m_sprites.avantPlan[18].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[18].sprite.setPosition(m_sprites.avantPlan[15].coinSpriteGaucheHaut() + sf::Vector2f(0.f, -getHeight(m_sprites.avantPlan[18].sprite)));
		//	m_sprites.avantPlan[18].comportement = TypePlateforme::objet;

		//	m_sprites.avantPlan[19].sprite.setTexture(m_sprites.textures[3], true);
		//	m_sprites.avantPlan[19].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[19].sprite.setTextureRect(sf::IntRect(0, 0, 150, getHeight(m_sprites.textures[3])));
		//	m_sprites.avantPlan[19].sprite.setPosition(m_sprites.avantPlan[17].coinSpriteGaucheHaut() + sf::Vector2f(0.f, -getHeight(m_sprites.avantPlan[19].sprite)));
		//	m_sprites.avantPlan[19].comportement = TypePlateforme::pics;

		//	m_sprites.avantPlan[20].sprite.setTexture(m_sprites.textures[8], true);
		//	m_sprites.avantPlan[20].sprite.setScale(5.f, 5.f);
		//	m_sprites.avantPlan[20].sprite.setPosition(m_sprites.avantPlan[17].coinSpriteDroitHaut() + sf::Vector2f(-getWidth(m_sprites.avantPlan[20].sprite), -getHeight(m_sprites.avantPlan[20].sprite)));
		//	m_sprites.avantPlan[20].comportement = TypePlateforme::finTableau;

		//	m_sprites.joueur.setPosition((m_sprites.avantPlan[3].coinSpriteDroitBas() + m_sprites.avantPlan[4].coinSpriteGaucheBas()) / 2.f + sf::Vector2f(-getWidth(m_sprites.joueur) / 2.f, -getHeight(m_sprites.joueur)));
		//	
		//	m_sprites.arrierePlan[0].setTexture(m_sprites.textures[9], true);
		//	m_sprites.arrierePlan[0].setScale(4.25f, 4.25f);
		//	m_sprites.arrierePlan[0].setTextureRect(sf::IntRect(0, 0, 1500, getHeight(m_sprites.textures[9])));
		//	m_sprites.arrierePlan[0].setPosition(0.f, -475.f);
		//	break;
		//default:
		//	m_sprites.arrierePlan.resize(0);
		//	m_sprites.avantPlan.resize(1);
		//	m_sprites.positionDansJeu = PositionJeu::fin;
		//	m_sprites.couleur = sf::Color(0xB32400FF);

		//	m_sprites.avantPlan[0].sprite.setTexture(m_sprites.textures[1], true);
		//	m_sprites.avantPlan[0].sprite.setScale(1.f, 1.f);
		//	m_sprites.avantPlan[0].sprite.setTextureRect(sf::IntRect(0, 0, getWidth(m_sprites.textures[1]) * 3, getHeight(m_sprites.textures[1])));
		//	m_sprites.avantPlan[0].sprite.setPosition(0.f, 720.f - getHeight(m_sprites.avantPlan[0].sprite));

		//	m_sprites.joueur.setPosition(375.f, 50.f);
		//	break;
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
			m_sprites.textures.resize(4);
			//avant-plan
			//verifFichierDisponible("resources/textures/dirt.png", 1);
			//verifFichierDisponible("resources/textures/grass.png", 2);
			//verifFichierDisponible("resources/textures/spikes.png", 3);
			//verifFichierDisponible("resources/textures/caveEntry.png", 4);
			//verifFichierDisponible("resources/sprites/checkpoint.png", 5);
			//verifFichierDisponible("resources/sprites/powerUp.png", 6);

			//m_sprites.textures[1].setRepeated(true);
			//m_sprites.textures[2].setRepeated(true);
			//m_sprites.textures[3].setRepeated(true);
			verifFichierDisponible("resources/textures/Niveau1.png", 1);
			//arrière-plan
			verifFichierDisponible("resources/textures/sky.png", 2);
			verifFichierDisponible("resources/textures/mountain.png", 3);
			break;
		case 2:
			//avant-plan
			m_sprites.textures.resize(9);
			//verifFichierDisponible("resources/textures/caveGround.png", 1);
			//verifFichierDisponible("resources/textures/caveRoof.png", 2);
			//verifFichierDisponible("resources/textures/caveSoil.png", 3);
			//verifFichierDisponible("resources/textures/spikes.png", 4);
			//verifFichierDisponible("resources/sprites/powerUp.png", 5);
			//verifFichierDisponible("resources/sprites/caveExit.png", 6);
			//verifFichierDisponible("resources/sprites/checkpoint.png", 7);

			//m_sprites.textures[1].setRepeated(true);
			//m_sprites.textures[2].setRepeated(true);
			//m_sprites.textures[3].setRepeated(true);
			//m_sprites.textures[4].setRepeated(true);
			verifFichierDisponible("resources/textures/Niveau2.png", 1);
			//arrière-plan
			verifFichierDisponible("resources/textures/cave.png", 8);

			m_sprites.textures[8].setRepeated(true);
			break;
		case 3:
			m_sprites.textures.resize(8);
			
			//avant-plan
			verifFichierDisponible("resources/textures/solMontagne.png", 1);
			verifFichierDisponible("resources/textures/spikes.png", 2);
			verifFichierDisponible("resources/sprites/powerUp.png", 3);
			verifFichierDisponible("resources/textures/movingplatform.png", 4);
			verifFichierDisponible("resources/textures/snowDirt.png", 5);
			verifFichierDisponible("resources/sprites/snow_97.png", 6);

			m_sprites.textures[1].setRepeated(true);
			m_sprites.textures[2].setRepeated(true);
			m_sprites.textures[5].setRepeated(true);

			//arrière-plan
			verifFichierDisponible("resources/textures/repeatedMountains.png", 7);

			m_sprites.textures[7].setRepeated(true);
			break;
		case 4:
			m_sprites.textures.resize(10);

			//avant-plan
			verifFichierDisponible("resources/textures/snowDirt.png", 1);
			verifFichierDisponible("resources/textures/snowGrass.png", 2);
			verifFichierDisponible("resources/textures/iceSpikes.png", 3);
			verifFichierDisponible("resources/textures/movingPlatform.png", 4);
			verifFichierDisponible("resources/sprites/checkpoint.png", 5);
			verifFichierDisponible("resources/sprites/snow_97.png", 6);
			verifFichierDisponible("resources/sprites/powerUp.png", 7);
			verifFichierDisponible("resources/sprites/Panneau_direction.png", 8);

			m_sprites.textures[1].setRepeated(true);
			m_sprites.textures[2].setRepeated(true);
			m_sprites.textures[3].setRepeated(true);

			//arrière-plan

			verifFichierDisponible("resources/textures/repeatedMountains.png", 9);

			m_sprites.textures[9].setRepeated(true);
			break;
		default:
			m_sprites.textures.resize(2);
			verifFichierDisponible("resources/textures/montagneFond.png", 1);

			m_sprites.textures[1].setRepeated(true);
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
						//m_sprites.avantPlan.resize(0);
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