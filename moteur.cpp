#include "ressources.h"

using namespace std::chrono_literals;

constexpr int finTempsSaut{ 30 }; //30 dixièmes de secondes (3 secondes)

enum class Collision {
	aucune,
	pics,
	checkpoint,
	objet,
	normale,
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
Collision touchePlateformeBas(sf::Sprite& joueur, std::vector<Plateforme>& plateformes, Moteur& moteur, const sf::View& camera)
{
	for (auto& plateforme : plateformes)
	{

		if (collisionBas(joueur, plateforme))
		{
			switch (plateforme.comportement)
			{
			case TypePlateforme::solide:
			case TypePlateforme::semiSolide:
				if (procheSol(joueur, plateforme.sprite))
					joueur.setPosition(joueur.getPosition().x, plateforme.sprite.getPosition().y - getHeight(joueur));
				return Collision::normale;
				break;
			case TypePlateforme::pics:
				--moteur.nbVie;
				return Collision::pics;
				break;
			case TypePlateforme::checkPoint:
				//if (!moteur.checkpoint.checkpointActif())
				//	moteur.checkpoint.miseAJourCheckpoint(camera.getCenter(),
				//		sf::Vector2f(plateformes[i].sprite.getPosition().x, plateformes[i].sprite.getPosition().y + getHeight(plateformes[i].sprite) - getHeight(joueur)));
				return Collision::checkpoint;
				break;
			case TypePlateforme::objet:
				if (!plateforme.touche) plateforme.touche = true;
				break;
			default:
				break;
			}

		}
		//if (plateformes[i].comportement != TypePlateforme::objet)
	}
	return Collision::aucune;
}
Collision touchePlateformeGauche(sf::Sprite& joueur, std::vector<Plateforme>& plateformes, Moteur& moteur, const sf::View& camera)
{
	for (auto& plateforme : plateformes)
	{
		if (collisionGauche(joueur, plateforme))
		{
			switch (plateforme.comportement)
			{
			case TypePlateforme::solide:
			case TypePlateforme::pics:
				if (procheBordGauche(joueur, plateforme.sprite))
					joueur.setPosition(plateforme.sprite.getPosition().x + getWidth(plateforme.sprite), joueur.getPosition().y);
				return Collision::normale;
				break;
			case TypePlateforme::checkPoint:
				return Collision::checkpoint;
				break;
			case TypePlateforme::objet:
				if (!plateforme.touche) plateforme.touche = true;
				break;
			default:
				break;
			}

		}
	}
	return Collision::aucune;
}

Collision touchePlateformeDroite(sf::Sprite& joueur, std::vector<Plateforme>& plateformes, Moteur& moteur, const sf::View& camera)
{
	for (auto& plateforme : plateformes)
	{
		if (collisionDroite(joueur, plateforme))
		{
			switch (plateforme.comportement)
			{
			case TypePlateforme::solide:
			case TypePlateforme::pics:
				if (procheBordDroite(joueur, plateforme.sprite))
					joueur.setPosition(plateforme.sprite.getPosition().x - getWidth(joueur),
						joueur.getPosition().y);
				return Collision::normale;
			case TypePlateforme::checkPoint:
				return Collision::checkpoint;
			case TypePlateforme::objet:
				if (!plateforme.touche) plateforme.touche = true;
				break;
			default:
				break;
			}
		}
	}
	return Collision::aucune;
}

Collision touchePlateformeHaut(sf::Sprite& joueur, std::vector<Plateforme>& plateformes, int& tempsDixiemeSeconde, const sf::View& camera)
{
	for (auto& plateforme : plateformes)
	{
		if (collisionHaut(joueur, plateforme))
		{
			switch (plateforme.comportement)
			{
			case TypePlateforme::solide:
				tempsDixiemeSeconde = finTempsSaut;
				if (prochePlafond(joueur, plateforme.sprite))
					joueur.setPosition(joueur.getPosition().x, plateforme.sprite.getPosition().y + getHeight(plateforme.sprite));
				return Collision::normale;
			case TypePlateforme::pics:
				return Collision::pics;
			case TypePlateforme::objet:
				if (!plateforme.touche) plateforme.touche = true;
				break;
			case TypePlateforme::checkPoint:
				return Collision::checkpoint;
			default:
				break;
			}
		}
	}
	return Collision::aucune;
}
void desactiverSaut(std::bitset<3>& autorisationsSaut, std::unique_ptr<int>& tempsDixiemeSeconde, const bool& peutDeplacer, const bool& threadsActifs)
{
	sf::Clock cycle{};
	while (peutDeplacer && threadsActifs && tempsDixiemeSeconde)
	{
		if (*tempsDixiemeSeconde < finTempsSaut) //Équivalent de 3 secondes
		{
			++(*tempsDixiemeSeconde);
		}
		else
		{
			//if (*tempsDixiemeSeconde == finTempsSaut) PLOGD << L"A arrêté de sauter";
			*tempsDixiemeSeconde = 0;
			autorisationsSaut.reset(2);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100 - cycle.restart().asMilliseconds()));
	}
}

bool dansEcran(const sf::View& camera, const sf::Sprite& plateforme)
{
	static constexpr int longueurSecurite{ 50 }; //Je charge tout objet qui est dans une portée de 50 pixels en plus de la zone de caméra
	return ((plateforme.getPosition().x < camera.getCenter().x + camera.getSize().x / 2 + longueurSecurite)
		&& (plateforme.getPosition().x + getWidth(plateforme) > camera.getCenter().x - camera.getSize().x / 2 - longueurSecurite)
		&& (plateforme.getPosition().y < camera.getCenter().y + camera.getSize().y / 2 + longueurSecurite)
		&& (plateforme.getPosition().y + getHeight(plateforme) > camera.getCenter().y - camera.getSize().y / 2 - longueurSecurite));
}

void doitAfficher(const sf::View& camera, std::vector<Plateforme>& plateformes, const bool& threadsActifs)
{
	sf::Clock cycle{};
	while (threadsActifs)
	{
		for (auto& plateforme : plateformes)
		{
			plateforme.visible = (plateforme.comportement == TypePlateforme::objet) ? dansEcran(camera, plateforme.sprite) && !plateforme.touche : dansEcran(camera, plateforme.sprite);
		}
		std::this_thread::sleep_for(std::chrono::microseconds(tempsParImage - cycle.restart().asMicroseconds()));
	}
}

void rendreObjetVisible(Plateforme& plateforme, const bool& threadsActifs)
{
	sf::Clock minuterie;
	while (threadsActifs)
	{
		if (plateforme.touche)
		{
			std::this_thread::sleep_for(std::chrono::seconds(3));
			plateforme.touche = false;
		}
		std::this_thread::sleep_for(std::chrono::microseconds(tempsParImage));
	}
}

void animationCheckpoint(const Checkpoint& checkpoint, const bool& threadsActifs, sf::Sprite& spriteCheckpoint)
{
	sf::Clock minuterie;
	int frame{ 1 };
	while (threadsActifs)
	{
		if (checkpoint.checkpointActif())
		{
			spriteCheckpoint.setTextureRect(sf::IntRect(getWidth(*spriteCheckpoint.getTexture()) / 3 + getWidth(*spriteCheckpoint.getTexture()) / 3 * (frame % 2), 0, getWidth(*spriteCheckpoint.getTexture()) / 3, getHeight(*spriteCheckpoint.getTexture())));
			++frame;
		}
		std::this_thread::sleep_for(std::chrono::microseconds(tempsParImage * 10 
			//- minuterie.restart().asMicroseconds()
		));
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="joueur">Sprite du joueur</param>
/// <param name="threadsActifs">Sémaphore pour le fil d'exécution</param>
/// <param name="mouvementsJoueur">Mouvements vectoriels du joueur</param>
/// <param name="gauche">Indique si la direction de l'oiseau est à gauche</param>
void animationJoueur(sf::Sprite& joueur, const bool& threadsActifs, const sf::Vector2f& mouvementsJoueur, const bool& gauche)
{
	sf::Clock minuterie;
	long frameAnimation{ 0 };
	while (threadsActifs)
	{
		if (mouvementsJoueur.y < 0)
		{
			//Pourquoi je divise la taille de la texture par 3? Il y a 3 frames d'animation
			joueur.setTextureRect(sf::IntRect(frameAnimation % 3 * getWidth(*joueur.getTexture()) / 3,
				((gauche) ? Oiseau::vole_gauche : Oiseau::vole_droite) * getHeight(*joueur.getTexture()) / Oiseau::max,
				getWidth(*joueur.getTexture()) / 3, getHeight(*joueur.getTexture()) / Oiseau::max));
			++frameAnimation;
		}
		else if (mouvementsJoueur.y > 0)
		{
			//Puisque l'image de l'oiseau où il plane est à la position 1, il est inutile de faire le modulo
			joueur.setTextureRect(sf::IntRect(getWidth(*joueur.getTexture()) / 3,
				((gauche) ? Oiseau::vole_gauche : Oiseau::vole_droite) * getHeight(*joueur.getTexture()) / Oiseau::max,
				getWidth(*joueur.getTexture()) / 3, getHeight(*joueur.getTexture()) / Oiseau::max));
			//++frameAnimation;
		}
		else if (mouvementsJoueur.x != vecteurNul)
		{
			joueur.setTextureRect(sf::IntRect(frameAnimation % 3 * getWidth(*joueur.getTexture()) / 3,
				((gauche) ? Oiseau::marche_gauche : Oiseau::marche_droite) * getHeight(*joueur.getTexture()) / Oiseau::max,
				getWidth(*joueur.getTexture()) / 3, getHeight(*joueur.getTexture()) / Oiseau::max));
			++frameAnimation;
		}
		std::this_thread::sleep_for(std::chrono::microseconds(tempsParImage * 5 - minuterie.restart().asMicroseconds()));
	}
}

int indexCheckpoint(const int niveau)
{
	switch (niveau)
	{
	case 1:
		return 7;
	default:
		break;
	}
}

void deplacement(const touchesActives& touchesActionnees, ObjetADessiner& spritesEtFond, bool& peutDeplacer, const bool& threadActifs, Moteur& moteur, std::bitset<3>& touchesNonRepetables)
{
	sf::Vector2f deplacementVectoriel{ 0.f, 0.f };
	//Le bit 0 correspond à si le joueur peut sauter
	//Le bit 1 correspond à si le joueur peut faire un autre saut
	//Le bit 2 correspond à si un saut est en cours.
	std::bitset<3> autorisationsSaut{ 0b000 };
	const int nbVieDebut{ moteur.nbVie };
	std::unique_ptr<int> tempsDixiemeSeconde{ new (std::nothrow) int{ 0 } };
	std::unique_ptr<std::thread> sautEffectif{ new (std::nothrow) std::thread{ desactiverSaut, std::ref(autorisationsSaut), std::ref(tempsDixiemeSeconde), std::ref(peutDeplacer), std::ref(threadActifs) } };
	std::unique_ptr<std::thread> reglerVisible{ new (std::nothrow) std::thread {doitAfficher, std::ref(spritesEtFond.camera), std::ref(spritesEtFond.avantPlan), std::ref(threadActifs) } };
	unsigned long long frameAnimation{ 0 };
	bool gauche{ false }; //Le joueur regarde à gauche (Si aucune touche n'est appuyée, garde la direction actuelle)
	std::vector<std::thread> minuterieObjetsTouches;
	std::thread animationDrapeau{animationCheckpoint, std::ref(moteur.checkpoint), std::ref(threadActifs), std::ref(spritesEtFond.avantPlan[indexCheckpoint(moteur.niveau)].sprite)};
	std::thread animerJoueur{ animationJoueur, std::ref(spritesEtFond.joueur), std::ref(threadActifs), std::ref(deplacementVectoriel), std::ref(gauche)};
	//for (int i{ 0 }; i < spritesEtFond.avantPlan.size(); ++i)
	for (auto& plateforme : spritesEtFond.avantPlan)
		if (plateforme.comportement == TypePlateforme::objet)
			minuterieObjetsTouches.push_back(std::thread(rendreObjetVisible, std::ref(plateforme), std::ref(threadActifs)));

	for (auto& minuterie : minuterieObjetsTouches)
		minuterie.detach();
	
	reglerVisible->detach();
	animationDrapeau.detach();
	animerJoueur.detach();
	sf::Clock debutCycle{};
	while (peutDeplacer)
	{
		deplacementVectoriel.x = 0;
		deplacementVectoriel.y = 0;
		if ((touchesActionnees[0] && spritesEtFond.joueur.getPosition().x > moteur.minCameraX))
		{
			switch (touchePlateformeGauche(spritesEtFond.joueur, spritesEtFond.avantPlan, moteur, spritesEtFond.camera))
			{
			case Collision::aucune:
				deplacementVectoriel.x -= utilitaire::deplacement;
				break;
			case Collision::checkpoint:
				deplacementVectoriel.x -= utilitaire::deplacement;
				moteur.checkpoint.miseAJourCheckpoint(spritesEtFond.camera.getCenter(),
					spritesEtFond.joueur.getPosition(),
					spritesEtFond.arrierePlan);
				break;
			default:
				break;
			}
		}
		if (touchesActionnees[1] && spritesEtFond.joueur.getPosition().x + getWidth(spritesEtFond.joueur) < moteur.maxCameraX)
		{
			switch (touchePlateformeDroite(spritesEtFond.joueur, spritesEtFond.avantPlan, moteur, spritesEtFond.camera))
			{
			case Collision::aucune:
				deplacementVectoriel.x += utilitaire::deplacement;
				break;
			case Collision::checkpoint:
				deplacementVectoriel.x += utilitaire::deplacement;
				if (!moteur.checkpoint.checkpointActif()) moteur.checkpoint.miseAJourCheckpoint(spritesEtFond.camera.getCenter(), spritesEtFond.joueur.getPosition(), spritesEtFond.arrierePlan);
				break;
			case Collision::objet:
				deplacementVectoriel.x += utilitaire::deplacement;
				break;
			default:
				break;
			}
		}
		if ((autorisationsSaut.test(2)) && (spritesEtFond.joueur.getPosition().y > moteur.minCameraY - spritesEtFond.camera.getSize().y / 2))
		{
			switch (touchePlateformeHaut(spritesEtFond.joueur, spritesEtFond.avantPlan, *tempsDixiemeSeconde, spritesEtFond.camera))
			{
			case Collision::aucune:
				deplacementVectoriel.y -= utilitaire::deplacement;
				break;
			case Collision::checkpoint:
				deplacementVectoriel.y -= utilitaire::deplacement;
				moteur.checkpoint.miseAJourCheckpoint(spritesEtFond.camera.getCenter(), spritesEtFond.joueur.getPosition(), spritesEtFond.arrierePlan);
				break;
			default:
				break;
			}
		}
		if (!autorisationsSaut.test(2))
		{
			switch (touchePlateformeBas(spritesEtFond.joueur, spritesEtFond.avantPlan, moteur, spritesEtFond.camera))
			{
			case Collision::aucune:
				deplacementVectoriel.y += utilitaire::deplacement;
				break;
			case Collision::pics:
				spritesEtFond.hud.resize(2);
				*tempsDixiemeSeconde = finTempsSaut;
				peutDeplacer = !peutDeplacer;
				ecranChargement(spritesEtFond, moteur, peutDeplacer);
				if (moteur.checkpoint.checkpointActif())
				{
					spritesEtFond.joueur.setPosition(moteur.checkpoint.coordonneesJoueur());
					spritesEtFond.camera.setCenter(moteur.checkpoint.coordonneesCamera());

					for (int i{ 0 }; i < spritesEtFond.arrierePlan.size(); ++i)
					{
						spritesEtFond.arrierePlan[i].setPosition(moteur.checkpoint.coordonneesArrierePlan()[i]);
					}
				}
				spritesEtFond.hud.resize(0);
				return;
				break;
			case Collision::checkpoint:
				deplacementVectoriel.y += utilitaire::deplacement;
				moteur.checkpoint.miseAJourCheckpoint(spritesEtFond.camera.getCenter(), spritesEtFond.joueur.getPosition(), spritesEtFond.arrierePlan);
				break;
			default:
				break;
			}
			//deplacementVectoriel.y += utilitaire::deplacement;
		}
		if (touchesActionnees[5] && !touchesNonRepetables.test(2))
		{
			//PLOGD << L"A commencé à sauter";
			touchesNonRepetables.set(2);
			autorisationsSaut.set(2);
			if (sautEffectif->joinable())
				sautEffectif->detach();
			else
				*tempsDixiemeSeconde = 0;
		}
		if (deplacementVectoriel.x > vecteurNul)
		{
			gauche = false;
			if (cameraPeutContinuerDroite(spritesEtFond.joueur, spritesEtFond.camera, moteur))
			{
				spritesEtFond.camera.move(deplacementVectoriel.x, 0);
				for (int i{ 0 }; i < spritesEtFond.arrierePlan.size(); ++i)
				{
					spritesEtFond.arrierePlan[i].move(deplacementVectoriel.x * .75, 0);
				}

			}
		}
		else if (deplacementVectoriel.x < vecteurNul)
		{
			gauche = true;
			if (cameraPeutContinuerGauche(spritesEtFond.joueur, spritesEtFond.camera, moteur))
			{
				spritesEtFond.camera.move(deplacementVectoriel.x, 0);
				for (auto& arrierePlan : spritesEtFond.arrierePlan)
				{
					arrierePlan.move(deplacementVectoriel.x * .75, 0);
				}
			}
		}
		//PLOGD << L"Déplacement vertical: " << deplacementVectoriel.y;
		if (deplacementVectoriel.y < vecteurNul)
		{
			if (cameraPeutContinuerHaut(spritesEtFond.joueur, spritesEtFond.camera, moteur))
			{
				spritesEtFond.camera.move(0, deplacementVectoriel.y);
				for (auto& arrierePlan : spritesEtFond.arrierePlan)
				{
					arrierePlan.move(0, deplacementVectoriel.y * .75);
				}
			}
		}
		else if (deplacementVectoriel.y > vecteurNul)
		{
			if (cameraPeutContinuerBas(spritesEtFond.joueur, spritesEtFond.camera, moteur))
			{
				spritesEtFond.camera.move(0, deplacementVectoriel.y);
				for (auto& arrierePlan : spritesEtFond.arrierePlan)
				{
					arrierePlan.move(0, deplacementVectoriel.y * .75);
				}
			}
		}
		spritesEtFond.joueur.move(deplacementVectoriel);

		std::this_thread::sleep_for(std::chrono::microseconds(tempsParImage - debutCycle.restart().asMicroseconds()));
	}
	//std::this_thread::sleep_for(std::chrono::seconds(1));
	*tempsDixiemeSeconde = 31;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//tempsDixiemeSeconde.release();
	reglerVisible.release();
	sautEffectif.release();
}
