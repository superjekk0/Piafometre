#include "ressources.h"
#include "moteur.h"
#include "MoteurMenu.h"

float getWidth(const sf::Texture& texture)
{
	return texture.getSize().x;
}
float getWidth(const sf::Sprite& sprite)
{
	//return sprite.getLocalBounds().width;
	return sprite.getGlobalBounds().width;
}
float getHeight(const sf::Texture& texture)
{
	return texture.getSize().y;
}
float getHeight(const sf::Sprite& sprite)
{
	//return sprite.getGlobalBounds().height;
	return sprite.getGlobalBounds().height;
}
/// <summary>
/// Le joueur est proche du bloc par son bord gauche
/// </summary>
bool procheBordGauche(const sf::Sprite& joueur, const sf::Sprite& bloc)
{
	return joueur.getPosition().x - utilitaire::deplacement >= bloc.getPosition().x + getWidth(bloc);
}
/// <summary>
/// Le joueur est proche du bloc par son bord droit
/// </summary>
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
bool procheSol(const sf::Sprite& joueur, const opt::Tile& bloc)
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
/// <summary>
/// Le joueur entre en collision avec le bloc par son côté bas
/// </summary>
bool collisionBas(const sf::Sprite& joueur, const Plateforme& bloc)
{
	return (joueur.getPosition().x + getWidth(joueur) > bloc.sprite.getPosition().x)
		&& (joueur.getPosition().x < bloc.sprite.getPosition().x + getWidth(bloc.sprite))
		&& (joueur.getPosition().y + getHeight(joueur) >= bloc.sprite.getPosition().y)
		&& (joueur.getPosition().y <= bloc.sprite.getPosition().y + getHeight(bloc.sprite));
}

bool collisionBas(const sf::Sprite& joueur, const opt::Tile& bloc)
{
	return (joueur.getPosition().x + getWidth(joueur) > bloc.getPosition().x)
		&& (joueur.getPosition().x < bloc.getPosition().x + bloc.width())
		&& (joueur.getPosition().y + getHeight(joueur) >= bloc.getPosition().y)
		&& (joueur.getPosition().y <= bloc.getPosition().y + bloc.height());
}

/// <summary>
/// Le joueur entre en collision avec le bloc par son côté haut
/// </summary>
bool collisionHaut(const sf::Sprite& joueur, const Plateforme& bloc)
{
	return (joueur.getPosition().x + getWidth(joueur) > bloc.sprite.getPosition().x)
		&& (joueur.getPosition().x < bloc.sprite.getPosition().x + getWidth(bloc.sprite))
		&& (joueur.getPosition().y > bloc.sprite.getPosition().y)
		&& (joueur.getPosition().y < bloc.sprite.getPosition().y + getHeight(bloc.sprite));
}

bool collisionHaut(const sf::Sprite& joueur, const opt::Tile& bloc)
{
	return (joueur.getPosition().x + getWidth(joueur) > bloc.getPosition().x)
		&& (joueur.getPosition().x < bloc.getPosition().x + bloc.width())
		&& (joueur.getPosition().y > bloc.getPosition().y)
		&& (joueur.getPosition().y < bloc.getPosition().y + bloc.height());
}
/// <summary>
/// Le joueur entre en collision avec le bloc par son côté droit
/// </summary>
bool collisionDroite(const sf::Sprite& joueur, const Plateforme& bloc)
{
	return (joueur.getPosition().x + getWidth(joueur) >= bloc.sprite.getPosition().x)
		&& (joueur.getPosition().x + getWidth(joueur) <= bloc.sprite.getPosition().x + getWidth(bloc.sprite))
		&& (joueur.getPosition().y + getHeight(joueur) > bloc.sprite.getPosition().y)
		&& (joueur.getPosition().y < bloc.sprite.getPosition().y + getHeight(bloc.sprite));
}

bool collisionDroite(const sf::Sprite& joueur, const opt::Tile& bloc)
{
	return (joueur.getPosition().x + getWidth(joueur) >= bloc.getPosition().x)
		&& (joueur.getPosition().x + getWidth(joueur) <= bloc.getPosition().x + bloc.width())
		&& (joueur.getPosition().y + getHeight(joueur) > bloc.getPosition().y)
		&& (joueur.getPosition().y < bloc.getPosition().y + bloc.height());
}
/// <summary>
/// Le joueur entre en collision avec le bloc par son côté gauche
/// </summary>
bool collisionGauche(const sf::Sprite& joueur, const Plateforme& bloc)
{
	return (joueur.getPosition().x >= bloc.sprite.getPosition().x)
		&& (joueur.getPosition().x <= bloc.sprite.getPosition().x + getWidth(bloc.sprite))
		&& (joueur.getPosition().y + getHeight(joueur) > bloc.sprite.getPosition().y)
		&& (joueur.getPosition().y < bloc.sprite.getPosition().y + getHeight(bloc.sprite));
}

bool collisionGauche(const sf::Sprite& joueur, const opt::Tile& bloc)
{
	return (joueur.getPosition().x >= bloc.getPosition().x)
		&& (joueur.getPosition().x <= bloc.getPosition().x + bloc.width())
		&& (joueur.getPosition().y + getHeight(joueur) > bloc.getPosition().y)
		&& (joueur.getPosition().y < bloc.getPosition().y + bloc.height());
}

bool dansEcran(const sf::View& camera, const sf::Sprite& plateforme)
{
	static constexpr int longueurSecurite{ 50 }; //Je charge tout objet qui est dans une portée de 50 pixels en plus de la zone de caméra
	return ((plateforme.getPosition().x < camera.getCenter().x + camera.getSize().x / 2 + longueurSecurite)
		&& (plateforme.getPosition().x + getWidth(plateforme) > camera.getCenter().x - camera.getSize().x / 2 - longueurSecurite)
		&& (plateforme.getPosition().y < camera.getCenter().y + camera.getSize().y / 2 + longueurSecurite)
		&& (plateforme.getPosition().y + getHeight(plateforme) > camera.getCenter().y - camera.getSize().y / 2 - longueurSecurite));
}

void MoteurPhysique::rendreObjetVisible(int index)
{
	const static sf::Color invisible{sf::Color(0x0)};
	sf::Clock minuterie;
	PlateformeOptimisee* plateforme{ m_sprites.avantPlan.derivedPointer<PlateformeOptimisee>(index) };

	while (m_threadsActifs && m_peutDeplacer)
	{
		if (m_sprites.avantPlan[index].getColour() == invisible)
		{
			std::this_thread::sleep_for(std::chrono::seconds(3));
			plateforme->resetColour();
			m_sprites.avantPlan.reloadBuffer();
		}
		std::this_thread::sleep_for(std::chrono::microseconds(tempsParImage));
	}
}

bool MoteurPhysique::collisionPresente(const int pPlateformeRecherchee)
{
	for (auto& collision : m_collisions)
	{
		if (collision.indexPlateforme() == pPlateformeRecherchee) return true;
	}
	return false;
}

void MoteurPhysique::touchePlateformeBas()
{
	for (int i{ 0 }; i < m_sprites.avantPlan.size(); ++i)
	{
		PlateformeOptimisee* const plateforme{ m_sprites.avantPlan.derivedPointer<PlateformeOptimisee>(i) };
		if (plateforme)
			if (!collisionPresente(i) && collisionBas(m_sprites.joueur, *plateforme) && procheSol(m_sprites.joueur, *plateforme))
			{
				switch (plateforme->comportement())
				{
				case TypePlateforme::solide:
				case TypePlateforme::semiSolide:
					//if (procheSol(m_sprites.joueur, plateforme.sprite))
					//	m_sprites.joueur.setPosition(m_sprites.joueur.getPosition().x, plateforme.sprite.getPosition().y - getHeight(m_sprites.joueur));
					//return Collision::normale;
					m_collisions.push_back(InfosCollision(*plateforme, PositionCollision::bas, Collision::normale, i));
					break;
				case TypePlateforme::pics:
					//if (procheSol(m_sprites.joueur, plateforme.sprite))
					//{
						//--m_moteur.nbVie;
						//return Collision::pics;
					//}
					m_collisions.push_back(InfosCollision(*plateforme, PositionCollision::bas, Collision::pics, i));
					break;
				case TypePlateforme::checkPoint:
					//return Collision::checkpoint;
					m_collisions.push_back(InfosCollision(*plateforme, PositionCollision::bas, Collision::checkpoint, i));
					break;
				case TypePlateforme::objet:
					//if (!plateforme.touche)
					//{
					//	m_autorisationsSaut.set(3);
					//	m_autorisationsSaut.set(1);
					//	m_DixiemeSecondePeutSauter = 0;
					//	plateforme.touche = true;
					//}
					m_collisions.push_back(InfosCollision(*plateforme, PositionCollision::bas, Collision::objet, i));
					break;
				case TypePlateforme::finTableau:
					//return Collision::fin;
					m_collisions.push_back(InfosCollision(*plateforme, PositionCollision::bas, Collision::fin, i));
					break;
				default:
					break;
				}

			}
		//if (plateformes[i].comportement != TypePlateforme::objet)
	}
	//return Collision::aucune;
}

void MoteurPhysique::touchePlateformeGauche()
{
	for (int i{ 0 }; i < m_sprites.avantPlan.size(); ++i)
	{
		PlateformeOptimisee* const plateforme{ m_sprites.avantPlan.derivedPointer<PlateformeOptimisee>(i) };
		if (plateforme)
			if (!collisionPresente(i) && collisionGauche(m_sprites.joueur, *plateforme))
			{
				switch (plateforme->comportement())
				{
				case TypePlateforme::solide:
				case TypePlateforme::pics:
					//if (procheBordGauche(m_sprites.joueur, m_sprites.avantPlan[i].sprite))
					//	m_sprites.joueur.setPosition(m_sprites.avantPlan[i].sprite.getPosition().x + getWidth(m_sprites.avantPlan[i].sprite), m_sprites.joueur.getPosition().y);
					//return Collision::normale;
					m_collisions.push_back(InfosCollision(*plateforme, PositionCollision::gauche, Collision::normale, i));
					break;
				case TypePlateforme::checkPoint:
					//return Collision::checkpoint;
					m_collisions.push_back(InfosCollision(*plateforme, PositionCollision::gauche, Collision::checkpoint, i));
					break;
				case TypePlateforme::objet:
					//if (!m_sprites.avantPlan[i].touche)
					//{
					//	m_autorisationsSaut.set(3);
					//	m_autorisationsSaut.set(1);
					//	m_DixiemeSecondePeutSauter = 0;
					//	m_sprites.avantPlan[i].touche = true;
					//}
					m_collisions.push_back(InfosCollision(*plateforme, PositionCollision::gauche, Collision::objet, i));
					break;
				case TypePlateforme::finTableau:
					//return Collision::fin;
					m_collisions.push_back(InfosCollision(*plateforme, PositionCollision::gauche, Collision::fin, i));
					break;
				default:
					break;
				}

			}
	}
	//return Collision::aucune;
}

void MoteurPhysique::touchePlateformeDroite()
{
	for (int i{ 0 }; i < m_sprites.avantPlan.size(); ++i)
	{
		PlateformeOptimisee* const plateforme{ m_sprites.avantPlan.derivedPointer<PlateformeOptimisee>(i) };
		if (plateforme)
			if (!collisionPresente(i) && collisionDroite(m_sprites.joueur, *plateforme))
			{
				switch (plateforme->comportement())
				{
				case TypePlateforme::solide:
				case TypePlateforme::pics:
					//if (procheBordDroite(m_sprites.joueur, m_sprites.avantPlan[i].sprite))
					//	m_sprites.joueur.setPosition(m_sprites.avantPlan[i].sprite.getPosition().x - getWidth(m_sprites.joueur),
					//		m_sprites.joueur.getPosition().y);
					//return Collision::normale;
					m_collisions.push_back(InfosCollision(*plateforme, PositionCollision::droite, Collision::normale, i));
					break;
				case TypePlateforme::checkPoint:
					//return Collision::checkpoint;
					m_collisions.push_back(InfosCollision(*plateforme, PositionCollision::droite, Collision::checkpoint, i));
					break;
				case TypePlateforme::objet:
					//if (!m_sprites.avantPlan[i].touche)
					//{
					//	m_autorisationsSaut.set(3);
					//	m_autorisationsSaut.set(1);
					//	m_DixiemeSecondePeutSauter = 0;
					//	m_sprites.avantPlan[i].touche = true;
					//}
					m_collisions.push_back(InfosCollision(*plateforme, PositionCollision::droite, Collision::objet, i));
					break;
				case TypePlateforme::finTableau:
					//return Collision::fin;
					m_collisions.push_back(InfosCollision(*plateforme, PositionCollision::droite, Collision::fin, i));
					break;
				default:
					break;
				}
			}
	}
	//return Collision::aucune;
}

void MoteurPhysique::touchePlateformeHaut()
{
	for (int i{ 0 }; i < m_sprites.avantPlan.size(); ++i)
	{
		PlateformeOptimisee* const plateforme{ m_sprites.avantPlan.derivedPointer<PlateformeOptimisee>(i) };
		if (plateforme)
			if (!collisionPresente(i) && collisionHaut(m_sprites.joueur, *plateforme))
			{
				switch (plateforme->comportement())
				{
				case TypePlateforme::solide:
					//m_tempsDixiemeSeconde = finTempsSaut + 1;
					//if (prochePlafond(m_sprites.joueur, m_sprites.avantPlan[i].sprite))
					//	m_sprites.joueur.setPosition(m_sprites.joueur.getPosition().x, m_sprites.avantPlan[i].sprite.getPosition().y + getHeight(m_sprites.avantPlan[i].sprite));
					//return Collision::normale;
					m_collisions.push_back(InfosCollision(*plateforme, PositionCollision::haut, Collision::normale, i));
					break;
				case TypePlateforme::pics:
					//return Collision::pics;
					m_collisions.push_back(InfosCollision(*plateforme, PositionCollision::haut, Collision::pics, i));
					break;
				case TypePlateforme::objet:
					//if (!m_sprites.avantPlan[i].touche)
					//{
					//	m_autorisationsSaut.set(3);
					//	m_autorisationsSaut.set(1);
					//	m_DixiemeSecondePeutSauter = 0;
					//	m_sprites.avantPlan[i].touche = true;
					//}
					m_collisions.push_back(InfosCollision(*plateforme, PositionCollision::haut, Collision::objet, i));
					break;
				case TypePlateforme::checkPoint:
					//return Collision::checkpoint;
					m_collisions.push_back(InfosCollision(*plateforme, PositionCollision::haut, Collision::checkpoint, i));
					break;
				case TypePlateforme::finTableau:
					//return Collision::fin;
					m_collisions.push_back(InfosCollision(*plateforme, PositionCollision::haut, Collision::fin, i));
					break;
				default:
					break;
				}
			}
	}
	//return Collision::aucune;
}

void MoteurPhysique::desactiverSaut()
{
	sf::Clock cycle{};
	while (m_peutDeplacer && m_threadsActifs)
	{
		if (m_tempsDixiemeSeconde < finTempsSaut) //Équivalent de 3 secondes
		{
			++m_tempsDixiemeSeconde;
			//if (m_moteur.niveau == 2) PLOGD << m_tempsDixiemeSeconde << L" dixièmes de secondes";
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

bool MoteurPhysique::piedSurPlateforme()
{
	for (auto& collision : m_collisions)
	{
		if (collision.positionCollision() == PositionCollision::bas && collision.collision() == Collision::normale)
			return true;
	}
	return false;
}

void MoteurPhysique::animationJoueur(const sf::Vector2f& mouvementsJoueur, long pFrameAnimation)
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
		else if (mouvementsJoueur.y >= 0 && !piedSurPlateforme())
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

void MoteurPhysique::animationCheckpoint(int indexCheckpoint, long pFrameAnimation, const int& subTexture)
{
	//À chaque 1/6 d'image, on change de frame
	static int frame{ 0 };
	if (pFrameAnimation % 15 == 0)
		if (m_moteur.checkpoint.checkpointActif())
		{
			m_sprites.avantPlan.changeTextureRect(subTexture + 1 + frame % 2, indexCheckpoint);
			++frame;
		}
}

void MoteurPhysique::peutSauter()
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

int MoteurPhysique::indexCheckpoint()
{
	//Il regarde quel objet a le comportement Checkpoint. Puis, une fois qu'il l'a trouvé, il vérifie quel est l'index de la texture correspondante au checkpoint
	for (int i{ 0 }; i < m_sprites.avantPlan.size(); ++i)
	{
		PlateformeOptimisee* const plateforme{ m_sprites.avantPlan.derivedPointer<PlateformeOptimisee>(i) };
		if (plateforme)
			if (plateforme->comportement() == TypePlateforme::checkPoint)
			{
				return i;
			}
	}
	return -1;
}

bool MoteurPhysique::personnagePeutSauter()
{
	return m_autorisationsSaut.test(3) && ((m_autorisationsSaut.test(0) || m_autorisationsSaut.test(1)));
}

void MoteurPhysique::mort(std::unique_ptr<std::thread>& sautEffectif)
{
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
	m_autorisationsSaut.reset();
}

bool MoteurPhysique::gestionCollisions(int pIndexCheckpoint, std::unique_ptr<std::thread>& pSautEffectif, sf::Vector2f& pDeplacements)
{
	for (auto& collision : m_collisions)
	{
		PlateformeOptimisee* const plateforme = m_sprites.avantPlan.derivedPointer<PlateformeOptimisee>(collision.indexPlateforme());
		switch (collision.collision())
		{
		case Collision::pics:
			mort(pSautEffectif);
			return true;
			break;
		case Collision::checkpoint:
			m_moteur.checkpoint.miseAJourCheckpoint(m_sprites.camera.getCenter(),
				m_sprites.joueur, m_sprites.arrierePlan,
				*plateforme);
			break;
		case Collision::objet:
			if (plateforme->getColour() != sf::Color(0x0))
			{
				m_autorisationsSaut.set(3);
				m_autorisationsSaut.set(1);
				m_DixiemeSecondePeutSauter = 0;
				m_sprites.avantPlan.changeColour(sf::Color(0x0), collision.indexPlateforme());
			}
			break;
		case Collision::normale:
			switch (collision.positionCollision())
			{
			case PositionCollision::droite:
				pDeplacements.x -= utilitaire::deplacement;
				break;
			case PositionCollision::gauche:
				pDeplacements.x += utilitaire::deplacement;
				break;
			case PositionCollision::haut:
				pDeplacements.y += utilitaire::deplacement;
				m_tempsDixiemeSeconde = finTempsSaut;
				m_autorisationsSaut.reset(2);
				break;
			case PositionCollision::bas:
				pDeplacements.y -= utilitaire::deplacement;

				m_autorisationsSaut.set(0);
				m_autorisationsSaut.set(1);
				m_autorisationsSaut.reset(2);
				break;
			}
			break;
		case Collision::fin:
			++m_moteur.niveau;
			m_peutDeplacer = !m_peutDeplacer;
			m_autorisationsSaut.reset();
			pSautEffectif.release();
			m_moteur.checkpoint.reinitialiser();
			m_menus.ecranChargement();
			return true;
			break;
		}
	}
	return false;
}

MoteurPhysique::MoteurPhysique(const touchesActives& pTouchesActionnees, ObjetADessiner& pSprites,
	bool& pPeutDeplacer, const bool& pThreadsActifs, Moteur& pMoteur,
	std::bitset<3>& m_touchesNonRepetables, MoteurMenu& pMenus, std::binary_semaphore& semaphore) :
	m_touchesActionnees{ pTouchesActionnees }, m_sprites{ pSprites },
	m_peutDeplacer{ pPeutDeplacer }, m_threadsActifs{ pThreadsActifs },
	m_moteur{ pMoteur }, m_touchesNonRepetables{ m_touchesNonRepetables },
	m_menus{ pMenus }, m_tempsDixiemeSeconde{ 0 }, m_semaphore{ semaphore }
{

}


void MoteurPhysique::deplacement()
{
	m_tempsDixiemeSeconde = 0;
	m_collisions.reserve(m_sprites.avantPlan.size());
	int positionTableauCheckpoint{ indexCheckpoint() };
	sf::Vector2f deplacementVectoriel{ 0.f, 0.f };
	const int nbVieDebut{ m_moteur.nbVie };
	std::unique_ptr<std::thread> sautEffectif{ new std::thread{ [&]() { desactiverSaut(); } } };
	std::vector<std::thread> minuterieObjetsTouches;
	//std::unique_ptr<std::thread> animationDrapeau{ new std::thread {[&]() {animationCheckpoint(m_sprites.avantPlan[indexCheckpoint()].sprite); }} };
	//std::unique_ptr<std::thread> animerJoueur{ new std::thread{ [&]() {animationJoueur(deplacementVectoriel); }} };
	std::unique_ptr<std::thread> joueurPeutSauter{ new std::thread{ [&]() {peutSauter(); } } };
	sf::Clock debutCycle;
	long frameAnimation{ 0 };
	int checkpointSubTexture{ (positionTableauCheckpoint < 0) ? -1 : m_sprites.avantPlan[positionTableauCheckpoint].subTextureIndex() };
	if (m_sprites.positionDansJeu == PositionJeu::fin)
	{
		//m_sprites.camera.zoom(3.f);
		float zoom{ 1.0038f };
		int tempsZoomOut{ 0 };
		while (m_peutDeplacer && m_threadsActifs)
		{
			//m_semaphore.acquire();
			deplacementVectoriel.x = utilitaire::deplacement;
			m_collisions.resize(1);
			if (frameAnimation < 50l)
			{
				if (m_collisions.size() > 0) m_collisions.pop_back();
				deplacementVectoriel.y = 0.f;
				PlateformeOptimisee* const plateforme{ m_sprites.avantPlan.derivedPointer<PlateformeOptimisee>(0) };
				m_collisions.push_back(InfosCollision(*plateforme, PositionCollision::bas, Collision::normale, 0));
				m_semaphore.acquire();
				m_sprites.joueur.move(deplacementVectoriel.x, 0.f);
				m_sprites.camera.move(deplacementVectoriel.x, 0.f);
				m_semaphore.release();
			}
			else if (frameAnimation < 170l)
			{
				deplacementVectoriel.y = -utilitaire::deplacement;
				m_semaphore.acquire();
				m_sprites.joueur.move(deplacementVectoriel);
				m_sprites.camera.move(deplacementVectoriel);
				m_semaphore.release();
			}
			else
			{
				if (tempsZoomOut >= 170)
					zoom = 1.f;
				m_semaphore.acquire();
				m_sprites.camera.zoom(zoom);
				++tempsZoomOut;
				deplacementVectoriel.y = -utilitaire::deplacement;
				m_sprites.joueur.move(deplacementVectoriel.x, 0.f);
				m_sprites.camera.move(deplacementVectoriel.x, 0.f);
				m_semaphore.release();
			}
			animationJoueur(deplacementVectoriel, frameAnimation);
			++frameAnimation;
			if (m_sprites.camera.getCenter().x - m_sprites.camera.getSize().x / 2.f >= m_sprites.avantPlan.getSubTextureSize(0).x)
			{
				m_semaphore.acquire();
				m_sprites.camera.move(-m_sprites.avantPlan.getSubTextureSize(0).x, 0.f);
				m_sprites.joueur.move(-m_sprites.avantPlan.getSubTextureSize(0).x, 0.f);
				m_semaphore.release();
			}
			if (Clv::isKeyPressed(Clv::Enter))
			{
				m_peutDeplacer = !m_peutDeplacer;
				m_touchesNonRepetables[0] = true;
				m_sprites.positionDansJeu = PositionJeu::accueil;
				m_sprites.couleur = sf::Color(0x808080FF);
				sautEffectif.release();
				joueurPeutSauter.release();
				m_semaphore.acquire();
				m_sprites.camera.setSize(1280.f, 720.f);
				m_sprites.camera.setCenter(m_sprites.camera.getSize() / 2.f);
				//reglerVisible.release();
				m_sprites.hud.resize(4);
				m_sprites.arrierePlan.resize(0);
				m_sprites.avantPlan.resetTiles();
				m_semaphore.release();
				//m_semaphore.release();
				return;
			}
			//m_semaphore.release();
			std::this_thread::sleep_for(std::chrono::microseconds(tempsParImage - debutCycle.restart().asMicroseconds()));
		}
		//reglerVisible.release();
		sautEffectif.release();
		joueurPeutSauter.release();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		//m_semaphore.release();
		return;
	}
	else
	{
		for (int i{ 0 }; i < m_sprites.avantPlan.size(); ++i)
		{
			PlateformeOptimisee* const plateforme{ m_sprites.avantPlan.derivedPointer<PlateformeOptimisee>(i) };
			if (plateforme && plateforme->comportement() == TypePlateforme::objet)
					minuterieObjetsTouches.push_back(std::thread([=]() mutable {rendreObjetVisible(i); }));
		}
	}
	while (m_peutDeplacer)
	{
		//m_semaphore.acquire();
		m_collisions.resize(0);
		deplacementVectoriel.x = 0.f;
		deplacementVectoriel.y = 0.f;
		if ((m_autorisationsSaut.test(2)) && (m_sprites.joueur.getPosition().y > m_moteur.minCameraY - m_sprites.camera.getSize().y / 2))
		{
			touchePlateformeHaut();
			deplacementVectoriel.y -= utilitaire::deplacement;
		}
		if (!m_autorisationsSaut.test(2))
		{
			touchePlateformeBas();
			deplacementVectoriel.y += utilitaire::deplacement;
		}
		if ((m_touchesActionnees[0] && m_sprites.joueur.getPosition().x > m_moteur.minCameraX))
		{
			touchePlateformeGauche();
			deplacementVectoriel.x -= utilitaire::deplacement;
		}
		if (m_touchesActionnees[1] && m_sprites.joueur.getPosition().x + getWidth(m_sprites.joueur) < m_moteur.maxCameraX)
		{
			touchePlateformeDroite();
			deplacementVectoriel.x += utilitaire::deplacement;
		}
		if (gestionCollisions(positionTableauCheckpoint, sautEffectif, deplacementVectoriel))
		{
			//m_semaphore.release();
			return;
		}
		if (m_touchesActionnees[5] && !m_touchesNonRepetables.test(2) && personnagePeutSauter())
		{
			m_touchesNonRepetables.set(2);
			(m_autorisationsSaut.test(0)) ? m_autorisationsSaut.reset(0) : m_autorisationsSaut.reset(1);
			m_autorisationsSaut.set(2);
			m_tempsDixiemeSeconde = 0;
		}
		if (m_touchesActionnees[6] && !m_touchesNonRepetables.test(1))
		{
			//m_sprites.ecranNoir.setFillColor(sf::Color(0, 0, 0, 127));
			m_peutDeplacer = !m_peutDeplacer;
			m_semaphore.acquire();
			m_sprites.positionDansJeu = PositionJeu::pause;
			m_sprites.avantPlan.changeColour(sf::Color(0x00000080), m_sprites.avantPlan.size() - 1);
			m_sprites.avantPlan.setPosition(m_sprites.camera.getCenter() - m_sprites.camera.getSize() / 2.f, m_sprites.avantPlan.size() - 1);
			m_sprites.hud.resize(2);
			m_semaphore.release();
			std::this_thread::sleep_for(std::chrono::microseconds(tempsParImage * 10));
			sautEffectif.release();
			m_menus.pause();
			m_semaphore.release();
			return;
		}
		if (deplacementVectoriel.x > vecteurNul)
		{
			m_gauche = false;
			if (cameraPeutContinuerDroite(m_sprites.joueur, m_sprites.camera, m_moteur))
			{
				m_semaphore.acquire();
				m_sprites.camera.move(deplacementVectoriel.x, 0.f);
				for (int i{ 0 }; i < m_sprites.arrierePlan.size(); ++i)
				{
					m_sprites.arrierePlan[i].move(deplacementVectoriel.x * .75f, 0);
				}
				m_semaphore.release();
			}
		}
		else if (deplacementVectoriel.x < vecteurNul)
		{
			m_gauche = true;
			if (cameraPeutContinuerGauche(m_sprites.joueur, m_sprites.camera, m_moteur))
			{
				m_semaphore.acquire();
				m_sprites.camera.move(deplacementVectoriel.x, 0);
				for (auto& arrierePlan : m_sprites.arrierePlan)
				{
					arrierePlan.move(deplacementVectoriel.x * .75f, 0);
				}
				m_semaphore.release();
			}
		}
		if (deplacementVectoriel.y < vecteurNul)
		{
			if (cameraPeutContinuerHaut(m_sprites.joueur, m_sprites.camera, m_moteur))
			{
				m_semaphore.acquire();
				m_sprites.camera.move(0, deplacementVectoriel.y);
				for (auto& arrierePlan : m_sprites.arrierePlan)
				{
					arrierePlan.move(0, deplacementVectoriel.y * .75f);
				}
				m_semaphore.release();
			}
		}
		else if (deplacementVectoriel.y > vecteurNul)
		{
			if (cameraPeutContinuerBas(m_sprites.joueur, m_sprites.camera, m_moteur))
			{
				m_semaphore.acquire();
				m_sprites.camera.move(0, deplacementVectoriel.y);
				for (auto& arrierePlan : m_sprites.arrierePlan)
				{
					arrierePlan.move(0, deplacementVectoriel.y * .75f);
				}
				m_semaphore.release();
			}
			else if (m_sprites.joueur.getPosition().y > m_moteur.maxCameraY)
			{
				mort(sautEffectif);
				//m_semaphore.release();
				return;
			}
		}
		m_semaphore.acquire();
		m_sprites.joueur.move(deplacementVectoriel);
		PLOGD << L"Position x: " << m_sprites.joueur.getPosition().x;
		//m_sprites.ecranNoir.setPosition(m_sprites.camera.getCenter() - m_sprites.camera.getSize() / 2.f);
		if (positionTableauCheckpoint != -1)
			animationCheckpoint(positionTableauCheckpoint, frameAnimation, checkpointSubTexture);
		animationJoueur(deplacementVectoriel, frameAnimation);
		++frameAnimation;
		m_semaphore.release();
		std::this_thread::sleep_for(std::chrono::microseconds(tempsParImage - debutCycle.restart().asMicroseconds()));
	}
	//std::this_thread::sleep_for(std::chrono::seconds(1));
	m_tempsDixiemeSeconde = 31;
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	//tempsDixiemeSeconde.release();
	//reglerVisible.release();
	for (auto& minuterie : minuterieObjetsTouches)
		minuterie.join();
	sautEffectif.release();
	if (joueurPeutSauter && joueurPeutSauter->joinable()) joueurPeutSauter->join();
	if (sautEffectif && sautEffectif->joinable()) sautEffectif->join();
	m_sprites.avantPlan.resetTiles();
}


void deplacementAChoisir(bool& peutDeplacer, bool& threadsActifs, MoteurPhysique& moteurJeu, MoteurMenu& moteurMenu)
{
	while (threadsActifs)
	{
		if (peutDeplacer)
		{
			moteurJeu.deplacement();
		}
		else
		{
			moteurMenu.deplacementMenus();
		}
	}
}

