#ifndef MOTEUR_H
#define MOTEUR_H

#include "ressources.h"
#include "MoteurMenu.h"
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

float getWidth(const sf::Texture& texture)
{
	return texture.getSize().x;
}
float getWidth(const sf::Sprite& sprite)
{
	//return sprite.getLocalBounds().width;
	return sprite.getTextureRect().width * sprite.getScale().x;
}
float getHeight(const sf::Texture& texture)
{
	return texture.getSize().y;
}
float getHeight(const sf::Sprite& sprite)
{
	//return sprite.getGlobalBounds().height;
	return sprite.getTextureRect().height * sprite.getScale().y;
}
bool procheBordGauche(const sf::Sprite& joueur, const sf::Sprite& bloc)
{
	return joueur.getPosition().x - utilitaire::deplacement >= bloc.getPosition().x + getWidth(bloc);
}
bool procheBordDroite(const sf::Sprite& joueur, const sf::Sprite& bloc)
{
	return joueur.getPosition().x + getWidth(joueur) + utilitaire::deplacement <= bloc.getPosition().x;
}
bool prochePlafond(const sf::Sprite& joueur, const sf::Sprite& bloc)
{
	return joueur.getPosition().y + utilitaire::deplacement >= bloc.getPosition().y + getHeight(bloc);
}
bool procheSol(const sf::Sprite& joueur, const sf::Sprite& bloc)
{
	return joueur.getPosition().y + getHeight(joueur) - utilitaire::deplacement <= bloc.getPosition().y;
}
bool cameraToucheBordDroite(const sf::View& camera, const Moteur& moteur)
{
	return (camera.getCenter().x + camera.getSize().x / 2 >= moteur.maxCameraX);
}
bool cameraToucheBordGauche(const sf::View& camera, const Moteur& moteur)
{
	return (camera.getCenter().x - camera.getSize().x / 2 <= moteur.minCameraX);
}
bool cameraToucheBordHaut(const sf::View& camera, const Moteur& moteur)
{
	return (camera.getCenter().y - camera.getSize().y / 2 <= moteur.minCameraY);
}
bool cameraToucheBordBas(const sf::View& camera, const Moteur& moteur)
{
	return (camera.getCenter().y + camera.getSize().y / 2 >= moteur.maxCameraY);
}
bool cameraPeutContinuerGauche(const sf::Sprite& joueur, const sf::View& camera, const Moteur& moteur)
{
	return (joueur.getPosition().x - getWidth(joueur) / 2 <= camera.getCenter().x && !cameraToucheBordGauche(camera, moteur));
}
bool cameraPeutContinuerDroite(const sf::Sprite& joueur, const sf::View& camera, const Moteur& moteur)
{
	return (joueur.getPosition().x + getWidth(joueur) / 2 >= camera.getCenter().x && !cameraToucheBordDroite(camera, moteur));
}
bool cameraPeutContinuerHaut(const sf::Sprite& joueur, const sf::View& camera, const Moteur& moteur)
{
	return (joueur.getPosition().y - getHeight(joueur) / 2 <= camera.getCenter().y && !cameraToucheBordHaut(camera, moteur));
}
bool cameraPeutContinuerBas(const sf::Sprite& joueur, const sf::View& camera, const Moteur& moteur)
{
	return (joueur.getPosition().y + getHeight(joueur) / 2 >= camera.getCenter().y && !cameraToucheBordBas(camera, moteur));
}
bool collisionBas(const sf::Sprite& joueur, const Plateforme& bloc)
{
	return (joueur.getPosition().x + getWidth(joueur) > bloc.sprite.getPosition().x)
		&& (joueur.getPosition().x < bloc.sprite.getPosition().x + getWidth(bloc.sprite))
		&& (joueur.getPosition().y + getHeight(joueur) >= bloc.sprite.getPosition().y)
		&& (joueur.getPosition().y + getHeight(joueur) <= bloc.sprite.getPosition().y + getHeight(bloc.sprite));
}
bool collisionHaut(const sf::Sprite& joueur, const Plateforme& bloc)
{
	return (joueur.getPosition().x > bloc.sprite.getPosition().x)
		&& (joueur.getPosition().x < bloc.sprite.getPosition().x + getWidth(bloc.sprite))
		&& (joueur.getPosition().y > bloc.sprite.getPosition().y)
		&& (joueur.getPosition().y < bloc.sprite.getPosition().y + getHeight(bloc.sprite));
}
bool collisionDroite(const sf::Sprite& joueur, const Plateforme& bloc)
{
	return (joueur.getPosition().x + getWidth(joueur) >= bloc.sprite.getPosition().x)
		&& (joueur.getPosition().x + getWidth(joueur) <= bloc.sprite.getPosition().x + getWidth(bloc.sprite))
		&& (joueur.getPosition().y + getHeight(joueur) > bloc.sprite.getPosition().y)
		&& (joueur.getPosition().y < bloc.sprite.getPosition().y + getHeight(bloc.sprite));
}
bool collisionGauche(const sf::Sprite& joueur, const Plateforme& bloc)
{
	return (joueur.getPosition().x >= bloc.sprite.getPosition().x)
		&& (joueur.getPosition().x <= bloc.sprite.getPosition().x + getWidth(bloc.sprite))
		&& (joueur.getPosition().y + getHeight(joueur) > bloc.sprite.getPosition().y)
		&& (joueur.getPosition().y < bloc.sprite.getPosition().y + getHeight(bloc.sprite));
}

bool dansEcran(const sf::View& camera, const sf::Sprite& plateforme)
{
	static constexpr int longueurSecurite{ 50 }; //Je charge tout objet qui est dans une portée de 50 pixels en plus de la zone de caméra
	return ((plateforme.getPosition().x < camera.getCenter().x + camera.getSize().x / 2 + longueurSecurite)
		&& (plateforme.getPosition().x + getWidth(plateforme) > camera.getCenter().x - camera.getSize().x / 2 - longueurSecurite)
		&& (plateforme.getPosition().y < camera.getCenter().y + camera.getSize().y / 2 + longueurSecurite)
		&& (plateforme.getPosition().y + getHeight(plateforme) > camera.getCenter().y - camera.getSize().y / 2 - longueurSecurite));
}

void doitAfficher(const sf::View& camera, std::vector<Plateforme>& plateformes, const bool& threadsActifs, const bool& pPeutDeplacer)
{
	sf::Clock cycle{};
	while (threadsActifs && pPeutDeplacer)
	{
		for (auto& plateforme : plateformes)
		{
			plateforme.visible = (plateforme.comportement == TypePlateforme::objet) ? dansEcran(camera, plateforme.sprite) && !plateforme.touche : dansEcran(camera, plateforme.sprite);
		}
		std::this_thread::sleep_for(std::chrono::microseconds(tempsParImage - cycle.restart().asMicroseconds()));
	}
}

void rendreObjetVisible(Plateforme& plateforme, const bool& threadsActifs, const bool& pPeutDeplacer)
{
	sf::Clock minuterie;
	while (threadsActifs && pPeutDeplacer)
	{
		if (plateforme.touche)
		{
			std::this_thread::sleep_for(std::chrono::seconds(3));
			if (!(&plateforme))
				return;
			plateforme.touche = false;
		}
		std::this_thread::sleep_for(std::chrono::microseconds(tempsParImage));
	}
}


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
	int m_tempsDixiemeSeconde{ 0 };						//Sert à mesurer le temps de saut de le faire achever plus vite
	//Le bit 0 correspond à si le joueur peut sauter
	//Le bit 1 correspond à si le joueur peut faire un autre saut
	//Le bit 2 correspond à si un saut est en cours.
	//Le bit 3 correspond à si un saut est possible en appuyant sur le bouton saut
	std::bitset<4> m_autorisationsSaut{ 0b0000 };
	int m_DixiemeSecondePeutSauter{ 0 };				//Sert à vérifier quand le joueur peut sauter

#pragma region COLLISIONS
	Collision touchePlateformeBas()
	{
		for (auto& plateforme : m_sprites.avantPlan)
		{

			if (collisionBas(m_sprites.joueur, plateforme))
			{
				switch (plateforme.comportement)
				{
				case TypePlateforme::solide:
				case TypePlateforme::semiSolide:
					if (procheSol(m_sprites.joueur, plateforme.sprite))
						m_sprites.joueur.setPosition(m_sprites.joueur.getPosition().x, plateforme.sprite.getPosition().y - getHeight(m_sprites.joueur));
					return Collision::normale;
					break;
				case TypePlateforme::pics:
					--m_moteur.nbVie;
					return Collision::pics;
					break;
				case TypePlateforme::checkPoint:
					return Collision::checkpoint;
					break;
				case TypePlateforme::objet:
					if (!plateforme.touche)
					{
						m_autorisationsSaut.set(3);
						m_autorisationsSaut.set(1);
						m_DixiemeSecondePeutSauter = 0;
						plateforme.touche = true;
					}
					break;
				case TypePlateforme::finTableau:
					return Collision::fin;
					break;
				default:
					break;
				}

			}
			//if (plateformes[i].comportement != TypePlateforme::objet)
		}
		return Collision::aucune;
	}

	Collision touchePlateformeGauche()
	{
		for (auto& plateforme : m_sprites.avantPlan)
		{
			if (collisionGauche(m_sprites.joueur, plateforme))
			{
				switch (plateforme.comportement)
				{
				case TypePlateforme::solide:
				case TypePlateforme::pics:
					if (procheBordGauche(m_sprites.joueur, plateforme.sprite))
						m_sprites.joueur.setPosition(plateforme.sprite.getPosition().x + getWidth(plateforme.sprite), m_sprites.joueur.getPosition().y);
					return Collision::normale;
					break;
				case TypePlateforme::checkPoint:
					return Collision::checkpoint;
					break;
				case TypePlateforme::objet:
					if (!plateforme.touche)
					{
						m_autorisationsSaut.set(3);
						m_autorisationsSaut.set(1);
						m_DixiemeSecondePeutSauter = 0;
						plateforme.touche = true;
					}
					break;
				case TypePlateforme::finTableau:
					return Collision::fin;
					break;
				default:
					break;
				}

			}
		}
		return Collision::aucune;
	}

	Collision touchePlateformeDroite()
	{
		for (auto& plateforme : m_sprites.avantPlan)
		{
			if (collisionDroite(m_sprites.joueur, plateforme))
			{
				switch (plateforme.comportement)
				{
				case TypePlateforme::solide:
				case TypePlateforme::pics:
					if (procheBordDroite(m_sprites.joueur, plateforme.sprite))
						m_sprites.joueur.setPosition(plateforme.sprite.getPosition().x - getWidth(m_sprites.joueur),
							m_sprites.joueur.getPosition().y);
					return Collision::normale;
				case TypePlateforme::checkPoint:
					return Collision::checkpoint;
				case TypePlateforme::objet:
					if (!plateforme.touche)
					{
						m_autorisationsSaut.set(3);
						m_autorisationsSaut.set(1);
						m_DixiemeSecondePeutSauter = 0;
						plateforme.touche = true;
					}
					break;
				case TypePlateforme::finTableau:
					return Collision::fin;
					break;
				default:
					break;
				}
			}
		}
		return Collision::aucune;
	}

	Collision touchePlateformeHaut()
	{
		for (auto& plateforme : m_sprites.avantPlan)
		{
			if (collisionHaut(m_sprites.joueur, plateforme))
			{
				switch (plateforme.comportement)
				{
				case TypePlateforme::solide:
					m_tempsDixiemeSeconde = finTempsSaut + 1;
					if (prochePlafond(m_sprites.joueur, plateforme.sprite))
						m_sprites.joueur.setPosition(m_sprites.joueur.getPosition().x, plateforme.sprite.getPosition().y + getHeight(plateforme.sprite));
					return Collision::normale;
				case TypePlateforme::pics:
					return Collision::pics;
				case TypePlateforme::objet:
					if (!plateforme.touche)
					{
						m_autorisationsSaut.set(3);
						m_autorisationsSaut.set(1);
						m_DixiemeSecondePeutSauter = 0;
						plateforme.touche = true;
					}
					break;
				case TypePlateforme::checkPoint:
					return Collision::checkpoint;
				case TypePlateforme::finTableau:
					return Collision::fin;
					break;
				default:
					break;
				}
			}
		}
		return Collision::aucune;
	}

#pragma endregion

	void desactiverSaut()
	{
		sf::Clock cycle{};
		while (m_peutDeplacer && m_threadsActifs)
		{
			if (m_tempsDixiemeSeconde < finTempsSaut) //Équivalent de 3 secondes
			{
				++m_tempsDixiemeSeconde;
			}
			else
			{
				//if (*tempsDixiemeSeconde == finTempsSaut) PLOGD << L"A arrêté de sauter";
				m_tempsDixiemeSeconde = 0;
				m_autorisationsSaut.reset(2);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100 - cycle.restart().asMilliseconds()));
		}
	}

	/// <summary>
/// 
/// </summary>
/// <param name="joueur">Sprite du joueur</param>
/// <param name="threadsActifs">Sémaphore pour le fil d'exécution</param>
/// <param name="mouvementsJoueur">Mouvements vectoriels du joueur</param>
/// <param name="gauche">Indique si la direction de l'oiseau est à gauche</param>
	void animationJoueur(const sf::Vector2f& mouvementsJoueur, long pFrameAnimation)
	{
		static int frameAnimation{ 0 };
		//L'animation du joueur est changée à chaque 1/10 de seconde
		if (pFrameAnimation % 8 == 0)
		{
			if (mouvementsJoueur.y < 0)
			{
				//Pourquoi je divise la taille de la texture par 3? Il y a 3 frames d'animation
				m_sprites.joueur.setTextureRect(sf::IntRect(frameAnimation % 3 * getWidth(*m_sprites.joueur.getTexture()) / 3,
					((m_gauche) ? Oiseau::vole_gauche : Oiseau::vole_droite) * getHeight(*m_sprites.joueur.getTexture()) / Oiseau::max,
					getWidth(*m_sprites.joueur.getTexture()) / 3, getHeight(*m_sprites.joueur.getTexture()) / Oiseau::max));
				++frameAnimation;
			}
			else if (mouvementsJoueur.y >= 0 && touchePlateformeBas() != Collision::normale)
			{
				//Puisque l'image de l'oiseau où il plane est à la position 1, il est inutile de faire le modulo
				m_sprites.joueur.setTextureRect(sf::IntRect(getWidth(*m_sprites.joueur.getTexture()) / 3,
					((m_gauche) ? Oiseau::vole_gauche : Oiseau::vole_droite) * getHeight(*m_sprites.joueur.getTexture()) / Oiseau::max,
					getWidth(*m_sprites.joueur.getTexture()) / 3, getHeight(*m_sprites.joueur.getTexture()) / Oiseau::max));
			}
			else if (mouvementsJoueur.x != vecteurNul)
			{
				m_sprites.joueur.setTextureRect(sf::IntRect(frameAnimation % 3 * getWidth(*m_sprites.joueur.getTexture()) / 3,
					((m_gauche) ? Oiseau::marche_gauche : Oiseau::marche_droite) * getHeight(*m_sprites.joueur.getTexture()) / Oiseau::max,
					getWidth(*m_sprites.joueur.getTexture()) / 3, getHeight(*m_sprites.joueur.getTexture()) / Oiseau::max));
				++frameAnimation;
			}
			else if (mouvementsJoueur.x == vecteurNul)
			{
				m_sprites.joueur.setTextureRect(sf::IntRect(0, ((m_gauche) ? Oiseau::marche_gauche : Oiseau::marche_droite) * getHeight(*m_sprites.joueur.getTexture()) / Oiseau::max,
					getWidth(*m_sprites.joueur.getTexture()) / 3, getHeight(*m_sprites.joueur.getTexture()) / Oiseau::max));
			}
		}
	}

	void animationCheckpoint(sf::Sprite& pSpriteCheckpoint, long pFrameAnimation)
	{
		//À chaque 1/6 d'image, on change de frame
		static int frame{ 0 };
		if (pFrameAnimation % 15 == 0)
			if (m_moteur.checkpoint.checkpointActif())
			{
				pSpriteCheckpoint.setTextureRect(sf::IntRect(getWidth(*pSpriteCheckpoint.getTexture()) / 3 + getWidth(*pSpriteCheckpoint.getTexture()) / 3 * (frame % 2), 0, getWidth(*pSpriteCheckpoint.getTexture()) / 3, getHeight(*pSpriteCheckpoint.getTexture())));
				++frame;
			}
	}

	void peutSauter()
	{
		sf::Clock minuterie;
		while (m_threadsActifs && m_peutDeplacer)
		{
			if (m_autorisationsSaut.test(3) && m_DixiemeSecondePeutSauter < finPeutSauter)
			{
				++m_DixiemeSecondePeutSauter;
			}
			else
			{
				m_autorisationsSaut.reset(3);
				m_DixiemeSecondePeutSauter = 0;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100 - minuterie.restart().asMilliseconds()));
		}
	}

	int indexCheckpoint()
	{
		//Il regarde quel objet a le comportement Checkpoint. Puis, une fois qu'il l'a trouvé, il vérifie quel est l'index de la texture correspondante au checkpoint
		for (int i{ 0 }; i < m_sprites.avantPlan.size(); ++i)
		{
			if (m_sprites.avantPlan[i].comportement == TypePlateforme::checkPoint)
			{
				return i;
			}
		}
		return -1;
	}

	bool personnagePeutSauter()
	{
		return m_autorisationsSaut.test(3) && (m_autorisationsSaut.test(0) || m_autorisationsSaut.test(1));
	}
public:

#pragma region CONSTRUCTEUR
	MoteurPhysique(const touchesActives& pTouchesActionnees, ObjetADessiner& pSprites,
		bool& pPeutDeplacer, const bool& pThreadsActifs, Moteur& pMoteur,
		std::bitset<3>& m_touchesNonRepetables, MoteurMenu& pMenus) : m_touchesActionnees{ pTouchesActionnees },
		m_sprites{ pSprites }, m_peutDeplacer{ pPeutDeplacer }, m_threadsActifs{ pThreadsActifs },
		m_moteur{ pMoteur }, m_touchesNonRepetables{ m_touchesNonRepetables }, m_menus{ pMenus }, m_tempsDixiemeSeconde{ 0 }
	{}

#pragma endregion

	void deplacement()
	{
		int positionTableauCheckpoint{ indexCheckpoint() };
		sf::Vector2f deplacementVectoriel{ 0.f, 0.f };
		const int nbVieDebut{ m_moteur.nbVie };
		std::unique_ptr<std::thread> sautEffectif{ new std::thread{ [&]() { desactiverSaut(); }} };
		std::unique_ptr<std::thread> reglerVisible{ new std::thread {doitAfficher, std::ref(m_sprites.camera), std::ref(m_sprites.avantPlan), std::ref(m_threadsActifs), std::ref(m_peutDeplacer)} };
		std::vector<std::thread> minuterieObjetsTouches;
		//std::unique_ptr<std::thread> animationDrapeau{ new std::thread {[&]() {animationCheckpoint(m_sprites.avantPlan[indexCheckpoint()].sprite); }} };
		//std::unique_ptr<std::thread> animerJoueur{ new std::thread{ [&]() {animationJoueur(deplacementVectoriel); }} };
		std::unique_ptr<std::thread> joueurPeutSauter{ new std::thread{[&]() {peutSauter(); }} };
		//for (int i{ 0 }; i < m_sprites.avantPlan.size(); ++i)
		for (auto& plateforme : m_sprites.avantPlan)
			if (plateforme.comportement == TypePlateforme::objet)
				minuterieObjetsTouches.push_back(std::thread(rendreObjetVisible, std::ref(plateforme), std::ref(m_threadsActifs), std::ref(m_peutDeplacer)));

		for (auto& minuterie : minuterieObjetsTouches)
			minuterie.detach();

		reglerVisible->detach();
		//animationDrapeau->detach();
		//animerJoueur->detach();
		joueurPeutSauter->detach();
		sautEffectif->detach();
		sf::Clock debutCycle{};
		long frameAnimation{ 0 };
		while (m_peutDeplacer)
		{
			deplacementVectoriel.x = 0.f;
			deplacementVectoriel.y = 0.f;
			if ((m_touchesActionnees[0] && m_sprites.joueur.getPosition().x > m_moteur.minCameraX))
			{
				switch (touchePlateformeGauche())
				{
				case Collision::aucune:
					deplacementVectoriel.x -= utilitaire::deplacement;
					break;
				case Collision::checkpoint:
					deplacementVectoriel.x -= utilitaire::deplacement;
					m_moteur.checkpoint.miseAJourCheckpoint(m_sprites.camera.getCenter(),
						m_sprites.joueur.getPosition(),
						m_sprites.arrierePlan);
					break;
				case Collision::fin:
					++m_moteur.niveau;
					m_moteur.checkpoint.reinitialiser();
					m_menus.ecranChargement();
					return;
					break;
				default:
					break;
				}
			}
			if (m_touchesActionnees[1] && m_sprites.joueur.getPosition().x + getWidth(m_sprites.joueur) < m_moteur.maxCameraX)
			{
				switch (touchePlateformeDroite())
				{
				case Collision::aucune:
					deplacementVectoriel.x += utilitaire::deplacement;
					break;
				case Collision::checkpoint:
					deplacementVectoriel.x += utilitaire::deplacement;
					if (!m_moteur.checkpoint.checkpointActif()) m_moteur.checkpoint.miseAJourCheckpoint(m_sprites.camera.getCenter(), m_sprites.joueur.getPosition(), m_sprites.arrierePlan);
					break;
				case Collision::objet:
					deplacementVectoriel.x += utilitaire::deplacement;
					break;
				case Collision::fin:
					++m_moteur.niveau;
					m_moteur.checkpoint.reinitialiser();
					m_menus.ecranChargement();
					return;
				default:
					break;
				}
			}
			if ((m_autorisationsSaut.test(2)) && (m_sprites.joueur.getPosition().y > m_moteur.minCameraY - m_sprites.camera.getSize().y / 2))
			{
				switch (touchePlateformeHaut())
				{
				case Collision::aucune:
					deplacementVectoriel.y -= utilitaire::deplacement;
					break;
				case Collision::checkpoint:
					deplacementVectoriel.y -= utilitaire::deplacement;
					m_moteur.checkpoint.miseAJourCheckpoint(m_sprites.camera.getCenter(), m_sprites.joueur.getPosition(), m_sprites.arrierePlan);
					break;
				case Collision::fin:
					++m_moteur.niveau;
					m_moteur.checkpoint.reinitialiser();
					m_menus.ecranChargement();
					return;
				default:
					break;
				}
			}
			if (!m_autorisationsSaut.test(2))
			{
				switch (touchePlateformeBas())
				{
				case Collision::aucune:
					deplacementVectoriel.y += utilitaire::deplacement;
					break;
				case Collision::pics:
					m_sprites.hud.resize(2);
					m_tempsDixiemeSeconde = finTempsSaut;
					m_peutDeplacer = !m_peutDeplacer;
					m_menus.ecranChargement();
					if (m_moteur.checkpoint.checkpointActif())
					{
						m_sprites.joueur.setPosition(m_moteur.checkpoint.coordonneesJoueur());
						m_sprites.camera.setCenter(m_moteur.checkpoint.coordonneesCamera());

						std::vector<sf::Vector2f> coordoneesArriere{ m_moteur.checkpoint.coordonneesArrierePlan() };
						for (int i{ 0 }; i < m_sprites.arrierePlan.size(); ++i)
						{
							m_sprites.arrierePlan[i].setPosition(coordoneesArriere[i]);
						}
					}
					m_sprites.hud.resize(0);
					sautEffectif.release();
					return;
					break;
				case Collision::checkpoint:
					deplacementVectoriel.y += utilitaire::deplacement;
					m_moteur.checkpoint.miseAJourCheckpoint(m_sprites.camera.getCenter(), m_sprites.joueur.getPosition(), m_sprites.arrierePlan);
					break;
				case Collision::normale:
					m_autorisationsSaut.set(0);
					m_autorisationsSaut.set(1);
					break;
				case Collision::fin:
					++m_moteur.niveau;
					m_moteur.checkpoint.reinitialiser();
					m_menus.ecranChargement();
					return;
				default:
					break;
				}
				//deplacementVectoriel.y += utilitaire::deplacement;
			}
			if (m_touchesActionnees[5] && !m_touchesNonRepetables.test(2) && personnagePeutSauter())
			{
				//PLOGD << L"A commencé à sauter";
				m_touchesNonRepetables.set(2);
				(m_autorisationsSaut.test(0)) ? m_autorisationsSaut.reset(0) : m_autorisationsSaut.reset(1);
				m_autorisationsSaut.set(2);
				//if (sautEffectif->joinable())
				//	sautEffectif->detach();
				//else
				m_tempsDixiemeSeconde = 0;
			}
			if (m_touchesActionnees[6] && !m_touchesNonRepetables.test(1))
			{
				m_sprites.ecranNoir.setFillColor(sf::Color(0, 0, 0, 127));
				m_peutDeplacer = !m_peutDeplacer;
				m_sprites.positionDansJeu = PositionJeu::pause;
				std::this_thread::sleep_for(std::chrono::microseconds(tempsParImage * 10));
				sautEffectif.release();
				m_sprites.hud.resize(2);
				m_menus.pause();
				return;
			}
			if (deplacementVectoriel.x > vecteurNul)
			{
				m_gauche = false;
				if (cameraPeutContinuerDroite(m_sprites.joueur, m_sprites.camera, m_moteur))
				{
					m_sprites.camera.move(deplacementVectoriel.x, 0);
					for (int i{ 0 }; i < m_sprites.arrierePlan.size(); ++i)
					{
						m_sprites.arrierePlan[i].move(deplacementVectoriel.x * .75, 0);
					}
				}
			}
			else if (deplacementVectoriel.x < vecteurNul)
			{
				m_gauche = true;
				if (cameraPeutContinuerGauche(m_sprites.joueur, m_sprites.camera, m_moteur))
				{
					m_sprites.camera.move(deplacementVectoriel.x, 0);
					for (auto& arrierePlan : m_sprites.arrierePlan)
					{
						arrierePlan.move(deplacementVectoriel.x * .75, 0);
					}
				}
			}
			//PLOGD << L"Déplacement vertical: " << deplacementVectoriel.y;
			if (deplacementVectoriel.y < vecteurNul)
			{
				if (cameraPeutContinuerHaut(m_sprites.joueur, m_sprites.camera, m_moteur))
				{
					m_sprites.camera.move(0, deplacementVectoriel.y);
					for (auto& arrierePlan : m_sprites.arrierePlan)
					{
						arrierePlan.move(0, deplacementVectoriel.y * .75);
					}
				}
			}
			else if (deplacementVectoriel.y > vecteurNul)
			{
				if (cameraPeutContinuerBas(m_sprites.joueur, m_sprites.camera, m_moteur))
				{
					m_sprites.camera.move(0, deplacementVectoriel.y);
					for (auto& arrierePlan : m_sprites.arrierePlan)
					{
						arrierePlan.move(0, deplacementVectoriel.y * .75);
					}
				}
			}
			m_sprites.joueur.move(deplacementVectoriel);
			m_sprites.ecranNoir.setPosition(m_sprites.camera.getCenter() - m_sprites.camera.getSize() / 2.f);
			animationCheckpoint(m_sprites.avantPlan[positionTableauCheckpoint].sprite, frameAnimation);
			animationJoueur(deplacementVectoriel, frameAnimation);
			++frameAnimation;
			std::this_thread::sleep_for(std::chrono::microseconds(tempsParImage - debutCycle.restart().asMicroseconds()));
		}
		//std::this_thread::sleep_for(std::chrono::seconds(1));
		m_tempsDixiemeSeconde = 31;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		//tempsDixiemeSeconde.release();
		reglerVisible.release();
		sautEffectif.release();
	}

};


#endif