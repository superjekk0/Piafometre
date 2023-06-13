#include "ressources.h"
#include "Jeu.h"

void verifFichierExiste(const std::string& chemin)
{
	std::filesystem::directory_entry fichierNiveau{ chemin };
	if (!fichierNiveau.exists())
		PLOGE << chemin << "is not an existing file";
	assert(fichierNiveau.exists() && "Le fichier specifie n'existe pas");
	//Ça fonctionne très bien!
}

/// <summary>
/// Retourne une liste de chaînes de caractères après une filtration
/// </summary>
/// <param name="str">Chaîne de caractères de base</param>
/// <param name="separator">Caractère servant à séparer</param>
std::vector<std::string> splitString(std::string str, const char separator)
{
	std::vector<std::string> listStrings{};
	listStrings.reserve(4);
	for (size_t indexSeparateur{ str.find(separator) }; indexSeparateur != str.npos; indexSeparateur = str.find(separator))
	{
		listStrings.push_back(str.substr(0, indexSeparateur));
		str = str.substr(indexSeparateur + 1, str.size() - indexSeparateur);
	}
	if (str != "")
		listStrings.push_back(str);
	return listStrings;
}


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
				//Les touches num�riques de base
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
				//La touche � ou /
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




