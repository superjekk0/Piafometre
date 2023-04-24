#include "ressources.h"

#ifndef PLATEFORME_H
#define PLATEFORME_H

class Plateforme {
public:
	bool touche{ false };
	bool visible{ true };
	sf::Sprite sprite{};
	TypePlateforme comportement{};

	Plateforme() = default;

	sf::Vector2f positionSprite()
	{
		return sprite.getPosition();
	}

	sf::Vector2f coinSpriteDroitHaut()
	{
		return sprite.getPosition() + sf::Vector2f(getWidth(sprite), 0.f);
	}

	sf::Vector2f coinSpriteGaucheBas()
	{
		return sprite.getPosition() + sf::Vector2f(0.f, getHeight(sprite));
	}
};

struct ObjetADessiner {
	std::vector <sf::Sprite> arrierePlan;
	std::vector <Plateforme> avantPlan;
	sf::Sprite joueur;
	std::vector <sf::Text> hud;
	Langue langue;
	sf::Color couleur;
	sf::RectangleShape ecranNoir;
	std::vector<sf::Texture> textures;
	sf::View camera;
	sf::Font police;
	PositionJeu positionDansJeu;
};


#endif