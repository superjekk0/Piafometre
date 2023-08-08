#pragma once
#ifndef PLATEFORME_H
#define PLATEFORME_H

#include <Level.h>
#include <Tile.h>
#include "ressources.h"

/// TODO : Changer le nom de PlateformeOptimisee à Plateforme
class PlateformeOptimisee : public opt::Tile {
private:
	TypePlateforme m_comportement;
public:
	
	/// <summary>
	/// Ne devrait être utilisé que pour faire un objet vide
	/// </summary>
	PlateformeOptimisee();

	/// <summary>
	/// Initialise une plateforme en incorporant un comportement à la plateforme et en mettant une échelle autre que 1:1
	/// </summary>
	/// <param name="texture">Référence de la texture utilisée (provenant probablement du niveau)</param>
	/// <param name="noTuileDebutTexture">Numéro de sous-texture utilisé</param>
	/// <param name="desiredSize">Taille de la tuile</param>
	/// <param name="position">Position au coin supérieur gauche de la texture</param>
	/// <param name="subTextureCount">Nombre de sous-textures</param>
	/// <param name="textureRule">Règle appliquée à la texture</param>
	/// <param name="subTextures">Les positions et tailles des sous-textures</param>
	/// <param name="scale">Échelle de la texture</param>
	/// <param name="comportement">Comportement de la plateforme</param>
	PlateformeOptimisee(int noTuileDebutTexture, const sf::FloatRect& tileRect, opt::TextureRule textureRule, const std::vector<sf::FloatRect>& subTextures, std::vector<std::size_t>& beginTiles, std::vector<sf::Vertex>& vertices, sf::Vector2f scale = sf::Vector2f(1.f, 1.f), TypePlateforme comportement = TypePlateforme::solide);

	/// <summary>
	/// Retourne le comportement de la plateforme
	/// </summary>
	TypePlateforme comportement() const;

	/// <summary>
	/// Change le comportement de la plateforme
	/// </summary>
	/// <param name="nouvComportement">Nouveau comportement donné à la plateforme</param>
	void comportement(TypePlateforme nouvComportement);

	/// <summary>
	/// Permet d'accéder aux objets polymorphiques PlateformeOptimisee depuis le niveau
	/// </summary>
	std::unique_ptr<opt::Tile> clone() const override;
};

inline PlateformeOptimisee::PlateformeOptimisee() : opt::Tile(), m_comportement{ TypePlateforme::solide }
{

}

inline PlateformeOptimisee::PlateformeOptimisee(int noTuileDebutTexture,
	const sf::FloatRect& tileRect, opt::TextureRule textureRule, const std::vector<sf::FloatRect>& subTextures,
	std::vector<std::size_t>& beginTiles, std::vector<sf::Vertex>& vertices, sf::Vector2f scale, TypePlateforme comportement)
	: opt::Tile{noTuileDebutTexture, tileRect, textureRule, subTextures, beginTiles, vertices, scale}, m_comportement{comportement}
{

}

inline TypePlateforme PlateformeOptimisee::comportement() const
{
	return m_comportement;
}

inline void PlateformeOptimisee::comportement(TypePlateforme nouvComportement)
{
	m_comportement = nouvComportement;
}

inline std::unique_ptr<opt::Tile> PlateformeOptimisee::clone() const
{
	return std::make_unique<PlateformeOptimisee>(*this);
}

class Plateforme {
public:
	bool touche{ false };
	bool visible{ true };
	sf::Sprite sprite{};
	TypePlateforme comportement{};

	Plateforme() = default;

	sf::Vector2f positionSprite() const
	{
		return sprite.getPosition();
	}

	sf::Vector2f coinSpriteDroitHaut() const
	{
		return sprite.getPosition() + sf::Vector2f(getWidth(sprite), 0.f);
	}

	sf::Vector2f coinSpriteGaucheBas() const
	{
		return sprite.getPosition() + sf::Vector2f(0.f, getHeight(sprite));
	}

	sf::Vector2f coinSpriteGaucheHaut() const 
	{
		return sprite.getPosition();
	}

	sf::Vector2f coinSpriteDroitBas() const
	{
		return sprite.getPosition() + sf::Vector2f(getWidth(sprite), getHeight(sprite));
	}
};

struct ObjetADessiner {
	std::vector <sf::Sprite> arrierePlan;
	opt::Level avantPlan;
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