#include "ressources.h"

void verifFichierExiste(const std::string& chemin)
{
	std::filesystem::directory_entry fichierNiveau{ chemin };
	if (!fichierNiveau.exists())
		PLOGE << chemin << "is not an existing file";
	assert(fichierNiveau.exists(), "Le fichier " && cheminNiveau && " n'existe pas");
	//Ça fonctionne très bien!
}

void chargementTexturesNiveau(std::vector<sf::Texture>& textures, const int& niveau, const std::string& langue)
{
	//if (!textures.at(0).loadFromFile("resources/texts/" + langue + "/keyboards/" + std::to_string(niveau) + ".png"))
	//	PLOGE << "Unable to load resources/texts/" << langue << "/keyboards/" << std::to_string(niveau) << ".png";
	if (!textures.at(1).loadFromFile("resources/sprites/player.png"))
		PLOGE << "Unable to load resources/sprites/player.png";
	//à l'index 0 et 1 sont réservés pour l'écran de chargement
	switch (niveau)
	{
	case 1:
		//avant-plan
		if (!textures[2].loadFromFile("resources/textures/dirt.png"))
			PLOGE << "Unable to load resources/textures/dirt.png";
		if (!textures[3].loadFromFile("resources/textures/grass.png"))
			PLOGE << "Unable to load resources/textures/grass.png";
		if (!textures[4].loadFromFile("resources/textures/spikes.png"))
			PLOGE << "Unable to load resources/textures/spikes.png";
		if (!textures[5].loadFromFile("resources/textures/caveEntry.png"))
			PLOGE << "Unable to load resources/textures/caveEntry.png";
		if (!textures[6].loadFromFile("resources/sprites/checkpoint.png"))
			PLOGE << "Unable to load resources/sprites/checkpoint.png";
		if (!textures[7].loadFromFile("resources/sprites/powerUp.png"))
			PLOGE << "Unable to load resources/sprites/powerUp.png";
		if (!textures[8].loadFromFile("resources/sprites/caveEntry.png"))
			PLOGE << "Unable to load resources/sprites/caveEntry.png";

		textures[2].setRepeated(true);
		textures[3].setRepeated(true);
		textures[4].setRepeated(true);
		//arrière-plan
		if (!textures[9].loadFromFile("resources/textures/sky.png"))
			PLOGE << "Unable to load resources/textures/sky.png";
		if (!textures[10].loadFromFile("resources/textures/mountain.png"))
			PLOGE << "Unable to load resources/textures/mountain.png";
	default:
		break;
	}
}

void chargementNiveau(ObjetADessiner& spritesEtFond, const int& niveau)
{
	spritesEtFond.hud.resize(1);
	std::string cheminNiveau{ "resources/texts/" + Jeu::symboleLangue(spritesEtFond.langue) + "/loading.txt" };
	verifFichierExiste(cheminNiveau);
	std::wfstream nomNiveau{ cheminNiveau }; //StreamReader nomNiveau = new StreamReader;
	std::wstring contenuNomNiveau{};
	std::wstring nomFinalNiveau{};

	while (nomNiveau)
	{
		std::getline(nomNiveau, contenuNomNiveau);
		auto indexNiveau{ contenuNomNiveau.find(L"- " + std::to_wstring(niveau)) };
		if (indexNiveau != std::wstring::npos)
		{
			nomFinalNiveau += contenuNomNiveau.substr(0, indexNiveau - 1) + L'\n';
		}
	}
	spritesEtFond.hud[0].setCharacterSize(45);
	spritesEtFond.hud[0].setString(nomFinalNiveau);

	switch (niveau)
	{
	case 1:
		spritesEtFond.couleur = sf::Color(119, 181, 254);
		spritesEtFond.textures.resize(11);
		spritesEtFond.avantPlan.resize(13);
		spritesEtFond.arrierePlan.resize(2);
		chargementTexturesNiveau(spritesEtFond.textures, niveau, Jeu::symboleLangue(spritesEtFond.langue));

		spritesEtFond.avantPlan[0].sprite.setTexture(spritesEtFond.textures[3]);
		spritesEtFond.avantPlan[0].sprite.setScale(0.75f, 0.75f);
		spritesEtFond.avantPlan[0].sprite.setPosition(0, 720 - spritesEtFond.textures[3].getSize().y);
		spritesEtFond.avantPlan[0].sprite.setTextureRect(sf::IntRect(0, 0, 3000, spritesEtFond.textures[3].getSize().y));

		spritesEtFond.avantPlan[1].sprite.setTexture(spritesEtFond.textures[2]);
		spritesEtFond.avantPlan[1].sprite.setTextureRect(sf::IntRect(0, 0, getWidth(spritesEtFond.textures[2]), getHeight(spritesEtFond.textures[2]) * 2.5));
		spritesEtFond.avantPlan[1].sprite.setScale(0.75f, 0.75f);
		spritesEtFond.avantPlan[1].sprite.setPosition(3 * getWidth(spritesEtFond.textures[3]),
			spritesEtFond.avantPlan[0].sprite.getPosition().y + getHeight(spritesEtFond.avantPlan[0].sprite) - getHeight(spritesEtFond.avantPlan[1].sprite));

		spritesEtFond.avantPlan[2].sprite.setTexture(spritesEtFond.textures[4]);
		spritesEtFond.avantPlan[2].sprite.setTextureRect(sf::IntRect(0, 0, getWidth(spritesEtFond.textures[4]) * 4, getHeight(spritesEtFond.textures[4])));
		spritesEtFond.avantPlan[2].sprite.setPosition(spritesEtFond.avantPlan[1].sprite.getPosition().x + getWidth(spritesEtFond.avantPlan[1].sprite),
			spritesEtFond.avantPlan[0].sprite.getPosition().y - getHeight(spritesEtFond.avantPlan[2].sprite));
		spritesEtFond.avantPlan[2].comportement = TypePlateforme::pics;

		spritesEtFond.avantPlan[3].sprite.setTexture(spritesEtFond.textures[2]);
		spritesEtFond.avantPlan[3].sprite.setScale(0.75f, .75f);
		spritesEtFond.avantPlan[3].sprite.setTextureRect(sf::IntRect(0, 0, spritesEtFond.avantPlan[0].sprite.getTextureRect().width, getHeight(spritesEtFond.textures[2]) * 3));
		spritesEtFond.avantPlan[3].sprite.setPosition(spritesEtFond.avantPlan[0].sprite.getPosition().x,
			spritesEtFond.avantPlan[0].sprite.getPosition().y + getHeight(spritesEtFond.avantPlan[0].sprite));

		spritesEtFond.avantPlan[4].sprite.setTexture(spritesEtFond.textures[3]);
		spritesEtFond.avantPlan[4].sprite.setTextureRect(sf::IntRect(0, 0, 3000, getHeight(spritesEtFond.textures[3])));
		spritesEtFond.avantPlan[4].sprite.setPosition(0,
			spritesEtFond.avantPlan[3].sprite.getPosition().y + getHeight(spritesEtFond.avantPlan[3].sprite) + 500);

		spritesEtFond.avantPlan[5].sprite.setTexture(spritesEtFond.textures[4]);
		spritesEtFond.avantPlan[5].sprite.setTextureRect(sf::IntRect(0, 0, spritesEtFond.textures[4].getSize().x, spritesEtFond.textures[4].getSize().y));
		spritesEtFond.avantPlan[5].sprite.setPosition(1500,
			spritesEtFond.avantPlan[4].sprite.getPosition().y - getHeight(spritesEtFond.avantPlan[5].sprite));
		spritesEtFond.avantPlan[5].comportement = TypePlateforme::pics;

		spritesEtFond.avantPlan[6].sprite.setTexture(spritesEtFond.textures[4]);
		spritesEtFond.avantPlan[6].sprite.setTextureRect(spritesEtFond.avantPlan[5].sprite.getTextureRect());
		spritesEtFond.avantPlan[6].sprite.setPosition(spritesEtFond.avantPlan[5].sprite.getPosition().x - 750,
			spritesEtFond.avantPlan[5].sprite.getPosition().y);
		spritesEtFond.avantPlan[6].comportement = TypePlateforme::pics;

		spritesEtFond.avantPlan[7].sprite.setTexture(spritesEtFond.textures[6]);
		spritesEtFond.avantPlan[7].sprite.setTextureRect(sf::IntRect(0, 0, getWidth(spritesEtFond.textures[6]) / 3, getHeight(spritesEtFond.textures[6])));
		spritesEtFond.avantPlan[7].sprite.setScale(2, 2);
		spritesEtFond.avantPlan[7].sprite.setPosition(spritesEtFond.avantPlan[5].sprite.getPosition().x + 500,
			spritesEtFond.avantPlan[4].sprite.getPosition().y - getHeight(spritesEtFond.avantPlan[7].sprite));
		spritesEtFond.avantPlan[7].comportement = TypePlateforme::checkPoint;

		spritesEtFond.avantPlan[8].sprite.setTexture(spritesEtFond.textures[7]);
		spritesEtFond.avantPlan[8].sprite.setPosition(spritesEtFond.avantPlan[1].sprite.getPosition().x - 100 - getWidth(spritesEtFond.avantPlan[8].sprite),
			spritesEtFond.avantPlan[0].sprite.getPosition().y - getHeight(spritesEtFond.avantPlan[8].sprite));
		spritesEtFond.avantPlan[8].comportement = TypePlateforme::objet;

		spritesEtFond.avantPlan[9].sprite.setTexture(spritesEtFond.textures[7]);
		spritesEtFond.avantPlan[9].sprite.setPosition(spritesEtFond.avantPlan[7].sprite.getPosition().x - getWidth(spritesEtFond.avantPlan[9].sprite) - 40,
			spritesEtFond.avantPlan[4].sprite.getPosition().y - getHeight(spritesEtFond.avantPlan[9].sprite));
		spritesEtFond.avantPlan[9].comportement = TypePlateforme::objet;

		spritesEtFond.joueur.setTexture(spritesEtFond.textures[1]);
		spritesEtFond.joueur.setTextureRect(sf::IntRect(0,
			marche_droite * getHeight(spritesEtFond.textures[1]) / max,
			getWidth(spritesEtFond.textures[1]) / 3,
			getHeight(spritesEtFond.textures[1]) / max));
		spritesEtFond.joueur.setScale(3, 3);
		spritesEtFond.joueur.setPosition(spritesEtFond.avantPlan[0].sprite.getPosition().x,
			spritesEtFond.avantPlan[0].sprite.getPosition().y - getHeight(spritesEtFond.joueur));

		spritesEtFond.arrierePlan[0].setTexture(spritesEtFond.textures[9]);
		spritesEtFond.arrierePlan[0].setScale(7, 7);
		spritesEtFond.arrierePlan[0].setPosition(0, -200);

		spritesEtFond.arrierePlan[1].setTexture(spritesEtFond.textures[10]);
		spritesEtFond.arrierePlan[1].setScale(7, 7);
		spritesEtFond.arrierePlan[1].setPosition(0, -150);

		break;
	default:
		break;
	}
}

void ecranChargement(ObjetADessiner& spritesEtFond, Moteur& moteur, bool& peutDeplacer)
{
	PLOGI << "Launching level " << moteur.niveau;
	spritesEtFond.positionDansJeu = PositionJeu::chargement;
	spritesEtFond.ecranNoir.setFillColor(sf::Color(0, 0, 0, 255));
	placementObjets(spritesEtFond);
	chargementNiveau(spritesEtFond, moteur.niveau);
	creationLimiteCamera(moteur);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	spritesEtFond.ecranNoir.setFillColor(sf::Color(0, 0, 0, 0));
	peutDeplacer = true;
	spritesEtFond.hud.resize(0);

}

void creationLimiteCamera(Moteur& moteur)
{
	switch (moteur.niveau)
	{
	case 1:
		moteur.minCameraY = -100;
		moteur.maxCameraX = 3000;
		moteur.maxCameraY = 2000;
		break;
	default:
		break;
	}
}

//Grandeur du texte du HUD
//int grandeurImage(const PositionJeu& menu)
//{
//	switch (menu)
//	{
//	case PositionJeu::accueil:
//		return 70;
//	case PositionJeu::options:
//		return 50;
//	default:
//		return 0;
//	}
//}

Clv::Key tradToucheCode(Clv::Key pTouche)
{
	switch (pTouche)
	{
	case sf::Keyboard::A:
	case sf::Keyboard::B:
	case sf::Keyboard::C:
	case sf::Keyboard::D:
	case sf::Keyboard::E:
	case sf::Keyboard::F:
	case sf::Keyboard::G:
	case sf::Keyboard::H:
	case sf::Keyboard::I:
	case sf::Keyboard::J:
	case sf::Keyboard::K:
	case sf::Keyboard::L:
	case sf::Keyboard::M:
	case sf::Keyboard::N:
	case sf::Keyboard::O:
	case sf::Keyboard::P:
	case sf::Keyboard::Q:
	case sf::Keyboard::R:
	case sf::Keyboard::S:
	case sf::Keyboard::T:
	case sf::Keyboard::U:
	case sf::Keyboard::V:
	case sf::Keyboard::W:
	case sf::Keyboard::X:
	case sf::Keyboard::Y:
	case sf::Keyboard::Z:
		return static_cast<Clv::Key>(pTouche + 97);
	case sf::Keyboard::Num0:
	case sf::Keyboard::Num1:
	case sf::Keyboard::Num2:
	case sf::Keyboard::Num3:
	case sf::Keyboard::Num4:
	case sf::Keyboard::Num5:
	case sf::Keyboard::Num6:
	case sf::Keyboard::Num7:
	case sf::Keyboard::Num8:
	case sf::Keyboard::Num9:
		return static_cast<Clv::Key>(pTouche + 22);
	case sf::Keyboard::Escape:
		break;
	case sf::Keyboard::LControl:
		break;
	case sf::Keyboard::LShift:
		break;
	case sf::Keyboard::LAlt:
		break;
	case sf::Keyboard::LSystem:
		break;
	case sf::Keyboard::RControl:
		break;
	case sf::Keyboard::RShift:
		break;
	case sf::Keyboard::RAlt:
		break;
	case sf::Keyboard::RSystem:
		break;
	case sf::Keyboard::Menu:
		break;
	case sf::Keyboard::LBracket:
		break;
	case sf::Keyboard::RBracket:
		break;
	case sf::Keyboard::Semicolon:
		break;
	case sf::Keyboard::Comma:
		break;
	case sf::Keyboard::Period:
		break;
	case sf::Keyboard::Quote:
		break;
	case sf::Keyboard::Slash:
		break;
	case sf::Keyboard::Backslash:
		break;
	case sf::Keyboard::Tilde:
		break;
	case sf::Keyboard::Equal:
		return static_cast<Clv::Key>(pTouche + 6);
		break;
	case sf::Keyboard::Hyphen:
		return static_cast<Clv::Key>(pTouche - 11);
		break;
	case sf::Keyboard::Space:
		break;
	case sf::Keyboard::Enter:
		return static_cast<Clv::Key>(pTouche - 45);
		break;
	case sf::Keyboard::Backspace:
		break;
	case sf::Keyboard::Tab:
		break;
	case sf::Keyboard::PageUp:
		break;
	case sf::Keyboard::PageDown:
		break;
	case sf::Keyboard::End:
		break;
	case sf::Keyboard::Home:
		break;
	case sf::Keyboard::Insert:
		break;
	case sf::Keyboard::Delete:
		break;
	case sf::Keyboard::Add:
		break;
	case sf::Keyboard::Subtract:
		break;
	case sf::Keyboard::Multiply:
		break;
	case sf::Keyboard::Divide:
		break;
	case sf::Keyboard::Left:
		break;
	case sf::Keyboard::Right:
		break;
	case sf::Keyboard::Up:
		break;
	case sf::Keyboard::Down:
		break;
	case sf::Keyboard::Numpad0:
		break;
	case sf::Keyboard::Numpad1:
		break;
	case sf::Keyboard::Numpad2:
		break;
	case sf::Keyboard::Numpad3:
		break;
	case sf::Keyboard::Numpad4:
		break;
	case sf::Keyboard::Numpad5:
		break;
	case sf::Keyboard::Numpad6:
		break;
	case sf::Keyboard::Numpad7:
		break;
	case sf::Keyboard::Numpad8:
		break;
	case sf::Keyboard::Numpad9:
		break;
	case sf::Keyboard::F1:
	case sf::Keyboard::F2:
	case sf::Keyboard::F3:
	case sf::Keyboard::F4:
	case sf::Keyboard::F5:
	case sf::Keyboard::F6:
	case sf::Keyboard::F7:
	case sf::Keyboard::F8:
	case sf::Keyboard::F9:
	case sf::Keyboard::F10:
	case sf::Keyboard::F11:
	case sf::Keyboard::F12:
	case sf::Keyboard::F13:
	case sf::Keyboard::F14:
	case sf::Keyboard::F15:
		return pTouche;
	case sf::Keyboard::Pause:
		break;
	default:
		break;
	}
}

bool verifTouches(const ensembleTouches& pTouches)
{
	for (int i{ 0 }; i < pTouches.size(); ++i)
	{
		for (int j{ i + 1 }; j < pTouches.size(); ++j)
		{
			if (pTouches[i] == pTouches[j])
			{
				return true;
			}
		}
	}
	return false;
}

// Placement des objets du HUD en dehors du moteur de jeu
void placementObjets(ObjetADessiner& spritesEtTextures)
{
	spritesEtTextures.camera.setCenter(spritesEtTextures.camera.getSize().x / 2, spritesEtTextures.camera.getSize().y / 2);
	switch (spritesEtTextures.positionDansJeu)
	{
	case PositionJeu::accueil:
		for (int i{ 0 }; i < spritesEtTextures.hud.size(); ++i)
		{
			spritesEtTextures.hud[i].setPosition(800, 100 + i * 100);
		}
		break;
	case PositionJeu::options:
		for (int i{ 0 }; i < spritesEtTextures.hud.size(); i += 2)
		{
			spritesEtTextures.hud[i].setPosition(sf::Vector2f(250, 50 + 40 * i));
		}
		for (int i{ 1 }; i < spritesEtTextures.hud.size(); i += 2)
		{
			spritesEtTextures.hud[i].setPosition(sf::Vector2f(900, spritesEtTextures.hud[i - 1].getPosition().y));
		}
		break;
	case PositionJeu::remmapage:
		spritesEtTextures.hud[0].setFillColor(sf::Color());
		spritesEtTextures.hud[0].setPosition(400, 300);
		//if (spritesEtTextures.hud.size() >= 2) spritesEtTextures.hud[1].setPosition(850, 580);
		break;
	case PositionJeu::chargement:
		spritesEtTextures.hud[0].setPosition(400, 400);
		//spritesEtTextures.hud[1].setPosition(600, 400);
		break;
	case PositionJeu::credits:
		spritesEtTextures.hud[0].setPosition(280, 200);
		break;
	}
}

void resetTouches(ensembleTouches& pTouches)
{
	pTouches[0] = Clv::A;
	pTouches[1] = Clv::D;
	pTouches[2] = Clv::W;
	pTouches[3] = Clv::S;
	pTouches[4] = Clv::Enter;
	pTouches[5] = Clv::Space;
	pTouches[6] = Clv::Escape;
	PLOGI << "Controls reset";
}

sf::Keyboard::Key assignationTouche(const sf::Event& pEvenement, const sf::Keyboard::Key& toucheExclue, sf::Clock& cycle)
{
	while (true)
	{
		//PLOGD << pEvenement.key.code;
		if ((pEvenement.key.code != toucheExclue) && (pEvenement.key.code != -858993460) && (pEvenement.key.code != tradToucheCode(toucheExclue)))
		{
			switch (pEvenement.key.code)
			{
				//case 0:
				//	break;
			case 8:
				return static_cast<Clv::Key>(pEvenement.key.code + 51);
			case 13: //Touche entrée
			case 14:
			case 15:
				return static_cast<sf::Keyboard::Key>(pEvenement.key.code + 45);
			case 27:
				//Touche Échap
				return static_cast<Clv::Key>(pEvenement.key.code + 9);
				//case 42:
			case 44:
				return static_cast<Clv::Key>(pEvenement.key.code + 5);
			case 45:
				//La touche -
				return static_cast<sf::Keyboard::Key>(pEvenement.key.code + 11);
			case 46:
				return static_cast<Clv::Key>(pEvenement.key.code + 4);
				//Les touches numériques de base
			case 48:
			case 49:
			case 50:
			case 51:
			case 52:
			case 53:
			case 54:
			case 55:
			case 56:
			case 57:
			case 58:
				return static_cast<Clv::Key>(pEvenement.key.code - 22);
			case 59:
				//La touche ;
				return static_cast<Clv::Key>(pEvenement.key.code - 11);
			case 61:
				//La touche =
				return static_cast<Clv::Key>(pEvenement.key.code - 6);
				//Lettres du clavier
			case 97:
			case 98:
			case 99:
			case 100:
			case 101:
			case 102:
			case 103:
			case 104:
			case 105:
			case 106:
			case 107:
			case 108:
			case 109:
			case 110:
			case 111:
			case 112:
			case 113:
			case 114:
			case 115:
			case 116:
			case 117:
			case 118:
			case 119:
			case 120:
			case 121:
			case 122:
			case 123:
				return static_cast<sf::Keyboard::Key>(pEvenement.key.code - 97);
			case 233:
				//La touche É ou /
				return static_cast<Clv::Key>(pEvenement.key.code - 181);
			default:
				//Touches mapables à trier...
				for (int i{ 0 }; i < sf::Keyboard::KeyCount; ++i)
				{
					if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i)))
						PLOGD << "Touche : " << i << ", key.code de " << pEvenement.key.code; //52
					//break;
				}

				return pEvenement.key.code;

				break;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(50 - cycle.restart().asMilliseconds()));
	}
}

bool confirmerReiniTouches(sf::Event& pEvenement, sf::Clock& cycle)
{
	while (true)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
			return false; //Correspond à la touche N sur le clavier
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
			return true; //Correspond à la touche Y sur le clavier
		std::this_thread::sleep_for(std::chrono::milliseconds(50 - cycle.restart().asMilliseconds()));
	}
}

void deplacementAChoisir(touchesActives& touchesActionnees, int& index, int& indexMax, bool& peutDeplacer, ObjetADessiner& spritesEtFond, fonctionsRessources& ptrFcnFichier, ensembleTouches& pTouches, bool& threadsActifs, sf::Event& pEvenement, Moteur& moteur, std::bitset<3>& touchesNonRepetables)
{
	while (threadsActifs)
	{
		if (peutDeplacer)
		{
			deplacement(touchesActionnees, spritesEtFond, peutDeplacer, threadsActifs, moteur, touchesNonRepetables);
		}
		else
		{
			deplacementMenus(touchesActionnees, index, indexMax, peutDeplacer, spritesEtFond, ptrFcnFichier, pTouches, threadsActifs, pEvenement, moteur, touchesNonRepetables);
		}
	}
}


void deplacementMenus(touchesActives& touchesActionnees, int& index, int& indexMax, bool& peutDeplacer, ObjetADessiner& spritesEtFond, fonctionsRessources& ptrFcnFichier, ensembleTouches& pTouches, bool& threadsActifs, sf::Event& pEvenement, Moteur& moteur, std::bitset<3>& toucheNonRepetables)
{
	std::vector<std::wstring> texteHUD{};
	texteHUD.resize(4);
	chargementTexteHUD(texteHUD, spritesEtFond, pTouches, index);

	while (!peutDeplacer)
	{
		sf::Clock debutCycle{}; //TRÈS IMPORTANT! NE PAS DÉPLACER
		//static bool imageClavier{ false };
		if (touchesActionnees[2])
			--index;
		if (touchesActionnees[3])
			++index;
		if (touchesActionnees[4] && !toucheNonRepetables.test(0))
		{
			toucheNonRepetables.set(0);
			switch (spritesEtFond.positionDansJeu)
			{
			case PositionJeu::accueil:

				switch (index)
				{
				case 0: //Entrer dans le jeu
					//verifMenu(spritesEtFond, index, ptrFcnFichier, pTouches);
					moteur.nbVie = 3;
					spritesEtFond.positionDansJeu = PositionJeu::chargement;
					//peutDeplacer = true;
					moteur.niveau = 1;
					ecranChargement(spritesEtFond, moteur, peutDeplacer);
					chargementTexteHUD(texteHUD, spritesEtFond, pTouches, index);
					return;
					break;
				case 1: //Entrer dans les options
					//verifMenu(spritesEtFond, index, ptrFcnFichier, pTouches);
					spritesEtFond.ecranNoir.setFillColor(sf::Color::Color(0, 0, 0, 128));
					spritesEtFond.hud.resize(15);
					texteHUD.resize(15);
					spritesEtFond.textures.resize(9);
					spritesEtFond.positionDansJeu = PositionJeu::options;
					index = 0;
					indexMax = 7;
					chargementTexteHUD(texteHUD, spritesEtFond, pTouches, index);
					PLOGI << "Exiting main menu";
					PLOGI << "Entering in settings region";
					break;
				case 2: //Quitter le logiciel
					Jeu::preparerQuitter(threadsActifs, touchesActionnees, peutDeplacer);
					//threadsActifs = false;
					//std::this_thread::sleep_for(100ms);
					//touchesActionnees.set(7);
					return;
					break;
				case 3: //Entrer dans les crédits
					//verifMenu(spritesEtFond, index, ptrFcnFichier, pTouches);
					spritesEtFond.ecranNoir.setFillColor(sf::Color(0, 0, 0, 128));
					spritesEtFond.positionDansJeu = PositionJeu::credits;
					texteHUD.resize(1);
					spritesEtFond.hud.resize(1);
					spritesEtFond.hud[0].setPosition(0, 0);
					//spritesEtFond.textures.resize(1);
					chargementTexteHUD(texteHUD, spritesEtFond, pTouches, index);
					PLOGI << "Exiting main menu";
					PLOGI << "Entering credits";
					break;
				}
				break;
			case PositionJeu::options:
				PLOGI << "Remapping touch number " << index;
				spritesEtFond.positionDansJeu = PositionJeu::remmapage;
				spritesEtFond.hud.resize(1);
				texteHUD.resize(1);
				chargementTexteHUD(texteHUD, spritesEtFond, pTouches, index);
				//std::this_thread::sleep_for(200ms);
				if (index < 7)
				{
					Clv::Key toucheTemp{ assignationTouche(pEvenement, pTouches[4], debutCycle) };
					pTouches[index] = toucheTemp;
					if (verifTouches(pTouches))
						resetTouches(pTouches);
				}
				else
				{
					if (confirmerReiniTouches(pEvenement, debutCycle)) resetTouches(pTouches);
				}
				spritesEtFond.positionDansJeu = PositionJeu::options;
				texteHUD.resize(15);
				spritesEtFond.hud.resize(15);
				chargementTexteHUD(texteHUD, spritesEtFond, pTouches, index);
				//spritesEtFond.textures.resize(9);
			}
		}
		if (touchesActionnees[6] && !toucheNonRepetables.test(1))
		{
			toucheNonRepetables.set(1);
			switch (spritesEtFond.positionDansJeu)
			{
			case PositionJeu::options:
				//verifMenu(spritesEtFond, index, ptrFcnFichier, pTouches);
				spritesEtFond.ecranNoir.setFillColor(sf::Color(0, 0, 0, 0));
				spritesEtFond.hud.resize(4);
				texteHUD.resize(4);
				//spritesEtFond.textures.resize(2);
				spritesEtFond.positionDansJeu = PositionJeu::accueil;
				index = 0;
				indexMax = 3;
				chargementTexteHUD(texteHUD, spritesEtFond, pTouches, index);
				PLOGI << "Exiting settings region";
				PLOGI << "Entering main menu";
				break;
			case PositionJeu::credits:
				//verifMenu(spritesEtFond, index, ptrFcnFichier, pTouches);
				spritesEtFond.ecranNoir.setFillColor(sf::Color(0, 0, 0, 0));
				spritesEtFond.hud.resize(4);
				texteHUD.resize(4);
				//spritesEtFond.textures.resize(2);
				spritesEtFond.positionDansJeu = PositionJeu::accueil;
				index = 0;
				indexMax = 3;
				chargementTexteHUD(texteHUD, spritesEtFond, pTouches, index);
				PLOGI << "Exiting credits";
				PLOGI << "Entering main menu";
				break;
			}
		}

		//chargementTextures(spritesEtFond.textures, spritesEtFond.langue, spritesEtFond.positionDansJeu);
		if (index > indexMax)
			index = indexMax;
		//0 ici représente le minimum de l'index
		else if (index < 0)
			index = 0;
		verifMenu(spritesEtFond, index, texteHUD, pTouches);

		std::this_thread::sleep_for(std::chrono::milliseconds(100 - debutCycle.restart().asMilliseconds()));
	}
}

void detectionEvenement(sf::Event& evenementJeu, bool& threadsActifs, bool& peutDeplacer, touchesActives& touchesActionnees, const ensembleTouches& pTouches, sf::RenderWindow& pFenetre, std::bitset<3>& touchesNonRepetables)
{
	if (pFenetre.pollEvent(evenementJeu))
	{
		switch (evenementJeu.type)
		{
		case sf::Event::KeyPressed:
			for (int i{ 0 }; i < pTouches.size(); ++i)
			{
				touchesActionnees[i] = Clv::isKeyPressed(pTouches[i]);
			}
			break;
		case sf::Event::KeyReleased:
			for (int i{ 0 }; i < pTouches.size(); ++i)
			{
				touchesActionnees[i] = Clv::isKeyPressed(pTouches[i]);
			}
			if (!touchesActionnees[4])
				touchesNonRepetables.reset(0);
			if (!touchesActionnees[6])
				touchesNonRepetables.reset(1);
			if (!touchesActionnees[5])
				touchesNonRepetables.reset(2);
			break;
		case sf::Event::Closed:
			Jeu::preparerQuitter(threadsActifs, touchesActionnees, peutDeplacer);
			return;
		default:
			break;
		}
	}
}

void chargementTexteHUD(std::vector<std::wstring>& textesHUD, ObjetADessiner& ensemble, const ensembleTouches& pTouches, const int index)
{
	std::string chemin{ chargementTextures(Jeu::symboleLangue(ensemble.langue), ensemble.positionDansJeu) };
	if (ensemble.positionDansJeu != PositionJeu::chargement) verifFichierExiste(chemin);
	std::wfstream fichier{ chemin };
	std::wstring texteActuel;

	switch (ensemble.positionDansJeu)
	{
	case PositionJeu::accueil:
		for (int i{ 0 }; i < textesHUD.size() && fichier; ++i)
		{
			std::wstring ligneTemp;
			fichier >> ligneTemp;
			textesHUD[i] = ligneTemp;
			ensemble.hud[i].setString(textesHUD[i]);
		}
		break;
	case PositionJeu::options:
		for (int i{ 0 }; i < textesHUD.size() && fichier; i += 2)
		{
			std::wstring ligneTemp;
			fichier >> ligneTemp;
			textesHUD[i] = ligneTemp;
			ensemble.hud[i].setString(textesHUD[i]);
		}
		for (int i{ 1 }; i < textesHUD.size(); i += 2)
		{
			std::wstring ligneTemp{ nomFichierImageTouches(pTouches[i / 2], ensemble.langue) };
			textesHUD[i] = ligneTemp;
			ensemble.hud[i].setString(textesHUD[i]);
		}
		break;
	case PositionJeu::credits:
		while (fichier)
		{
			std::wstring ligneTemp;
			std::getline(fichier, ligneTemp);
			texteActuel += ligneTemp + L'\n';
		}
		if (textesHUD.size() > 0)
		{
			textesHUD[0] = texteActuel;
			ensemble.hud[0].setString(textesHUD[0]);
		}
		break;
	case PositionJeu::remmapage:
		while (fichier)
		{
			std::wstring ligneTemp;
			std::getline(fichier, ligneTemp);
			auto balise{ (index == pTouches.size()) ? ligneTemp.find(L"- 7") : ligneTemp.find(L"- a") };
			if (balise != ligneTemp.npos)
			{
				texteActuel += ligneTemp.substr(0, balise - 1);
				balise = texteActuel.find(L"La touche actuelle est");
				if (balise != texteActuel.npos)
					texteActuel += L' ' + nomFichierImageTouches(pTouches[index], ensemble.langue);
			}
			texteActuel += L'\n';
		}
		if (textesHUD.size() > 0)
		{
			textesHUD[0] = texteActuel;
			ensemble.hud[0].setString(textesHUD[0]);
		}
	default:
		break;
	}
}

/// <summary>
/// Permet d'afficher le menu présentement sélectionné.
/// </summary>
/// <param name="ensemble">Ensemble des sprites du HUD et des textures</param>
/// <param name="textesHUD">Ensemble des textes du HUD</param>
/// <param name="index">Indique quel texture doit être celle sélectionnée</param>
/// <param name="menu">Indique dans quelle section du jeu on se trouve. Pourrait être le menu principal, les options, le menu de pause, etc.</param>
void verifMenu(ObjetADessiner& ensemble, int& index, std::vector<std::wstring>& textesHUD, const ensembleTouches& pTouches)
{
	//===================================
	//
	// Très important! L'index pour le texte affiché lorsque le menu est "options" est pair tandis que pour l'image de touche
	// correspondante, elle est impaire.
	//
	//===================================

	bool toucheImage{ false };
	//std::wfstream chemin{ chargementTextures(Jeu::symboleLangue(ensemble.langue), ensemble.positionDansJeu) };
	std::wstring ligneFinale{ L"" };

	for (int i{ 0 }; i < ensemble.hud.size() && i < textesHUD.size(); ++i)
	{
		ensemble.hud[i].setFont(ensemble.police);
		switch (ensemble.positionDansJeu)
		{
		case PositionJeu::accueil:
			ensemble.hud[i].setString(static_cast<std::wstring>(textesHUD[i]));
			ensemble.hud[i].setCharacterSize(80);
			if (index == i)
			{
				ensemble.hud[i].setCharacterSize(ensemble.hud[i].getCharacterSize() * 1.15);
				ensemble.hud[i].setFillColor(sf::Color(222, 215, 43));
				ensemble.hud[i].setStyle(sf::Text::Style::Bold);
			}
			else
			{
				ensemble.hud[i].setFillColor(sf::Color(210, 210, 210));
				ensemble.hud[i].setStyle(sf::Text::Style::Regular);
			}
			break;
		case PositionJeu::options:
			ensemble.hud[i].setCharacterSize(30);
			if (index * 2 == i)
			{
				ensemble.hud[i].setCharacterSize(ensemble.hud[i].getCharacterSize() * 1.15);
				ensemble.hud[i].setFillColor(sf::Color(222, 215, 43));
				ensemble.hud[i].setStyle(sf::Text::Style::Bold);
			}
			else
			{
				ensemble.hud[i].setFillColor(sf::Color(210, 210, 210));
				ensemble.hud[i].setStyle(sf::Text::Style::Regular);
			}
			break;
		case PositionJeu::credits:
			ensemble.hud[i].setCharacterSize(45u);
			break;
		case PositionJeu::remmapage:
			ensemble.hud[i].setFillColor(sf::Color(210, 210, 210));
			ensemble.hud[i].setCharacterSize(45u);
			break;
		}
	}
	placementObjets(ensemble);
}
