#include "ressources.h"

std::wstring toucheMultiNoms(const Clv::Key pTouche, const Langue langue)
{
	switch (langue)
	{
	case Langue::fr:
		switch (pTouche)
		{
		case Clv::Down:
			return L"Bas";
		case Clv::Escape:
			return L"Echap";
		case Clv::Delete:
			return L"Suppr";
		case Clv::End:
			return L"Fin";
		case Clv::Enter:
			return L"Entree";
		case Clv::Home:
			return L"Home";
		case Clv::Insert:
			return L"Inser";
		case Clv::LControl:
			return L"Ctrl(G)";
		case Clv::LAlt:
			return L"Alt(G)";
		case Clv::Left:
			return L"Gauche";
		case Clv::LShift:
			return L"Maj(G)";
		case Clv::LSystem:
			return L"Win(G)";
		case Clv::PageDown:
			return L"PageBas";
		case Clv::PageUp:
			return L"PageHaut";
		case Clv::RAlt:
			return L"Alt(D)";
		case Clv::RControl:
			return L"Ctrl(D)";
		case Clv::Right:
			return L"Droite";
		case Clv::RShift:
			return L"Maj(D)";
		case Clv::RSystem:
			return L"Win(D)";
		case Clv::Space:
			return L"Espace";
		case Clv::Up:
			return L"Haut";
		default:
			return L"";
		}
		break;
	case Langue::en:
		switch (pTouche)
		{
		case Clv::Down:
			return L"Down";
		case Clv::Escape:
			return L"Escape";
		case Clv::Delete:
			return L"Del";
		case Clv::End:
			return L"End";
		case Clv::Enter:
			return L"Enter";
		case Clv::Home:
			return L"Home";
		case Clv::Insert:
			return L"Inser";
		case Clv::LControl:
			return L"Ctrl(L)";
		case Clv::LAlt:
			return L"Alt(L)";
		case Clv::Left:
			return L"Left";
		case Clv::LShift:
			return L"Shift(L)";
		case Clv::LSystem:
			return L"Win(L)";
		case Clv::PageDown:
			return L"PageDown";
		case Clv::PageUp:
			return L"PageUp";
		case Clv::RAlt:
			return L"Alt(R)";
		case Clv::RControl:
			return L"Ctrl(R)";
		case Clv::Right:
			return L"Right";
		case Clv::RShift:
			return L"Shift(R)";
		case Clv::RSystem:
			return L"Win(R)";
		case Clv::Space:
			return L"Space";
		case Clv::Up:
			return L"Up";
		default:
			return L"";
		}
		break;
	default:
		assert(langue != Langue::max && "Langue non disponible ou inexistante");
	}
}

/// <summary>
///	Retourne le nom de fichier de la touche (situé dans resources\texts\"langue"\keyboard\)
/// </summary>
/// <param name="pTouche">Sert pour retourner le bon nom</param>
/// <returns></returns>
std::wstring nomFichierImageTouches(const Clv::Key pTouche, const Langue langue)
{
	//switch (langue)
	//{
	//default:
	//	PLOGE << "Unknown language (" << langue << ")";
	//	break;
	//}
	switch (pTouche)
	{
	case Clv::A:
		return L"A";
	case Clv::B:
		return L"B";
	case Clv::C:
		return L"C";
	case Clv::D:
		return L"D";
	case Clv::E:
		return L"E";
	case Clv::F:
		return L"F";
	case Clv::G:
		return L"G";
	case Clv::H:
		return L"H";
	case Clv::I:
		return L"I";
	case Clv::J:
		return L"J";
	case Clv::K:
		return L"K";
	case Clv::L:
		return L"L";
	case Clv::M:
		return L"M";
	case Clv::N:
		return L"N";
	case Clv::O:
		return L"O";
	case Clv::P:
		return L"P";
	case Clv::Q:
		return L"Q";
	case Clv::R:
		return L"R";
	case Clv::S:
		return L"S";
	case Clv::T:
		return L"T";
	case Clv::U:
		return L"U";
	case Clv::V:
		return L"V";
	case Clv::W:
		return L"W";
	case Clv::X:
		return L"X";
	case Clv::Y:
		return L"Y";
	case Clv::Z:
		return L"Z";
	case Clv::Comma:
		return L",";
	case Clv::Semicolon:
		return L";";
	case Clv::Num0:
		return L"0";
	case Clv::Num1:
		return L"1";
	case Clv::Num2:
		return L"2";
	case Clv::Num3:
		return L"3";
	case Clv::Num4:
		return L"4";
	case Clv::Num5:
		return L"5";
	case Clv::Num6:
		return L"6";
	case Clv::Num7:
		return L"7";
	case Clv::Num8:
		return L"8";
	case Clv::Num9:
		return L"9";
	case Clv::Numpad0:
		return L"Num0";
	case Clv::Numpad1:
		return L"Num1";
	case Clv::Numpad2:
		return L"Num2";
	case Clv::Numpad3:
		return L"Num3";
	case Clv::Numpad4:
		return L"Num4";
	case Clv::Numpad5:
		return L"Num5";
	case Clv::Numpad6:
		return L"Num6";
	case Clv::Numpad7:
		return L"Num7";
	case Clv::Numpad8:
		return L"Num8";
	case Clv::Numpad9:
		return L"Num9";
	case Clv::F1:
		return L"F1";
	case Clv::F2:
		return L"F2";
	case Clv::F3:
		return L"F3";
	case Clv::F4:
		return L"F4";
	case Clv::F5:
		return L"F5";
	case Clv::F6:
		return L"F6";
	case Clv::F7:
		return L"F7";
	case Clv::F8:
		return L"F8";
	case Clv::F9:
		return L"F9";
	case Clv::F10:
		return L"F10";
	case Clv::F11:
		return L"F11";
	case Clv::F12:
		return L"F12";
	case Clv::F13:
		return L"F13";
	case Clv::F14:
		return L"F14";
	case Clv::F15:
		return L"F15";
	case Clv::Equal:
		return L"=";
	case Clv::Period:
		return L".";
	case Clv::Tab:
		return L"Tab";
	case Clv::RAlt:
	case Clv::LControl:
	case Clv::LAlt:
	case Clv::LSystem:
	case Clv::RSystem:
	case Clv::RControl:
	case Clv::RShift:
	case Clv::Delete:
	case Clv::Escape:
	case Clv::Insert:
	case Clv::Enter:
	case Clv::Space:
	case Clv::LShift:
	case Clv::Right:
	case Clv::Left:
	case Clv::Up:
	case Clv::Down:
	case Clv::PageUp:
	case Clv::PageDown:
	case Clv::Backspace:
		return toucheMultiNoms(pTouche, langue);
	case Clv::Hyphen:
		return L"-";
	case Clv::Slash:
		return L"/";
	case Clv::Backslash:
		return L"\\";
	case Clv::Add:
		return L"Num+";
	case Clv::Divide:
		return L"Num/";
	case Clv::Multiply:
		return L"Num*";
	case Clv::Quote:
		return L"'";
	case Clv::Subtract:
		return L"Num-";
	case Clv::Tilde:
		return L"~";
	default:
		return L"";
	}
}

std::string chargementTextures(const std::string& langue, const PositionJeu position)
{
	switch (position)
	{
	case PositionJeu::accueil:
		return "resources/texts/" + langue + "/home.txt";
		break;
	case PositionJeu::options:
		return "resources/texts/" + langue + "/settings.txt";
		break;
	case PositionJeu::remmapage:
		return "resources/texts/" + langue + "/remapping.txt";
		break;
	case PositionJeu::credits:
		return "resources/texts/" + langue + "/credits.txt";
		break;
	case PositionJeu::chargement:
		return "resources/texts/" + langue + "/loading.txt";
		break;
	case PositionJeu::pause:
		return "resources/texts/" + langue + "/pause.txt";
		break;
	case PositionJeu::quitter_pause:
		return "resources/texts/" + langue + "/leave.txt";
	default:
		return "";
		break;
	}
}