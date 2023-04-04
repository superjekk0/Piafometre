#ifndef MOTEURMENU_H
#define MOTEURMENU_H

#include "ressources.h"
#include "Jeu.h"

class MoteurMenu {
private:
#pragma region PROPRIETES

	touchesActives& m_touchesActionnees;	// Ensemble des touches actionn�es par le clavier
	int m_index;							// Index actuel du HUD
	int m_indexMax;							// Index maximal du HUD selon un contexte donn�
	bool& m_peutDeplacer;					// Indique si le joueur peut bouger. Lorsque faux, le HUD est actif
	bool& m_threadsActifs;					// S�maphore permettant de d�sactiver tous les fils d'ex�cution
	ObjetADessiner& m_sprites;				// Ensemble des �l�ments graphiques
	ensembleTouches& m_touches;				// Ensemble des touches pouvant activer les touches actionn�es
	Moteur& m_moteur;						// Ensemble de r�gles r�gissant le moteur de jeu
	//Touches ne devant pas �tre actionn�es en m�me temps.
	//Les touches non r�p�tables correspondent � l'index 4 (Entr�e), l'index 6 (Echap) et l'index 5 (Espace)
	std::bitset<3>& m_touchesNonRepetables;
	sf::Event& m_evenements;				// Objet permettant de d�tecter les �v�nements
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
			for (int i{ 0 }; i < m_textesHUD.size() && fichier; ++i)
			{
				std::wstring ligneTemp;
				std::getline(fichier, ligneTemp);
				m_textesHUD[i] = ligneTemp;
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
		case PositionJeu::pause:
			m_sprites.hud[0].setPosition(400, 200);
			break;
		}
	}

	/// <summary>
	/// Permet d'afficher le menu pr�sentement s�lectionn�.
	/// </summary>
	void affichageHUD()
	{
		//===================================
		//
		// Tr�s important! L'index pour le texte affich� lorsque le menu est "options" est pair tandis que pour l'image de touche
		// correspondante, elle est impaire.
		//
		//===================================

		bool toucheImage{ false };
		//std::wfstream chemin{ chargementTextures(Jeu::symboleLangue(m_sprites.langue), m_sprites.positionDansJeu) };
		std::wstring ligneFinale{ L"" };

		for (int i{ 0 }; i < m_sprites.hud.size() && i < m_textesHUD.size(); ++i)
		{
			m_sprites.hud[i].setFont(m_sprites.police);
			switch (m_sprites.positionDansJeu)
			{
			case PositionJeu::accueil:
				m_sprites.hud[i].setString(static_cast<std::wstring>(m_textesHUD[i]));
				m_sprites.hud[i].setCharacterSize(80);
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
				m_sprites.hud[i].setCharacterSize(30);
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
				m_sprites.hud[i].setFillColor(sf::Color(210, 210, 210));
				m_sprites.hud[i].setCharacterSize(45u);
				break;
			}
		}
		placementObjets();
	}

	void chargementNiveau()
	{
		//m_sprites.hud.resize(1);
		//chargementTexteHUD();
		//affichageHUD();

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
		//� l'index 0 et 1 sont r�serv�s pour l'�cran de chargement
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
			//arri�re-plan
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
			sf::Clock debutCycle; //TR�S IMPORTANT! NE PAS D�PLACER
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
						//affichageHUD(m_sprites, index, ptrFcnFichier, pTouches);
						m_moteur.nbVie = 3;
						m_sprites.positionDansJeu = PositionJeu::chargement;
						//peutDeplacer = true;
						m_moteur.niveau = 1;
						ecranChargement();
						return;
						break;
					case 1: //Entrer dans les options
						//affichageHUD(m_sprites, index, ptrFcnFichier, pTouches);
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
					case 3: //Entrer dans les cr�dits
						//affichageHUD(m_sprites, index, ptrFcnFichier, pTouches);
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
					//affichageHUD(m_sprites, index, ptrFcnFichier, pTouches);
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
					//affichageHUD(m_sprites, index, ptrFcnFichier, pTouches);
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
				case PositionJeu::pause:
					m_sprites.ecranNoir.setFillColor(sf::Color(0, 0, 0, 0));
					m_peutDeplacer = !m_peutDeplacer;
					break;
				}
			}

			//chargementTextures(m_sprites.textures, m_sprites.langue, m_sprites.positionDansJeu);
			if (m_index > m_indexMax)
				m_index = m_indexMax;
			//0 ici repr�sente le minimum de l'index
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
		m_sprites.ecranNoir.setFillColor(sf::Color(0, 0, 0, 255));
		chargementTexteHUD();
		affichageHUD();
		chargementNiveau();
		creationLimiteCamera(m_moteur);
		std::this_thread::sleep_for(std::chrono::seconds(2));
		m_sprites.ecranNoir.setFillColor(sf::Color(0, 0, 0, 0));
		m_peutDeplacer = true;
		m_sprites.hud.resize(0);
		m_sprites.positionDansJeu = PositionJeu::jeu;
	}


};


#endif