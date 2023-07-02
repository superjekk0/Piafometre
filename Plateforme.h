#pragma once
#ifndef PLATEFORME_H
#define PLATEFORME_H

#include "ressources.h"
#include "Plateformes.h"

/// TODO : Changer le nom de PlateformeOptimisee � Plateforme
class PlateformeOptimisee : public Tile {
private:
	TypePlateforme m_comportement;
public:
	
	/// <summary>
	/// Ne devrait �tre utilis� que pour faire un objet vide
	/// </summary>
	PlateformeOptimisee();

	/// <summary>
	/// Constructeur d'une tuile mettant par d�faut le ratio de la texture � 1:1 (m�me si la sous-texture n'est pas carr�e)
	/// </summary>
	/// <param name="texture">R�f�rence de la texture utilis�e (provenant souvent du niveau)</param>
	/// <param name="noTuileDebutTexture">Num�ro de sous-texture utilis�</param>
	/// <param name="desiredSize">Taille de la tuile</param>
	/// <param name="position">Position au coin sup�rieur gauche de la texture</param>
	/// <param name="subTextureCount">Nombre de sous-textures</param>
	/// <param name="textureRule">R�gle appliqu�e � la texture</param>
	/// <param name="subTextures">Les positions et tailles des sous-textures</param>
	PlateformeOptimisee(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize, const sf::Vector2f& position, const int& subTextureCount, TextureRule textureRule, const std::vector<sf::FloatRect>& subTextures);

	/// <summary>
	/// Initialise une plateforme en incorporant un comportement � ladite plateforme
	/// </summary>
	/// <param name="texture">R�f�rence de la texture utilis�e (provenant probablement du niveau)</param>
	/// <param name="noTuileDebutTexture">Num�ro de sous-texture utilis�</param>
	/// <param name="desiredSize">Taille de la tuile</param>
	/// <param name="position">Position au coin sup�rieur gauche de la texture</param>
	/// <param name="subTextureCount">Nombre de sous-textures</param>
	/// <param name="textureRule">R�gle appliqu�e � la texture</param>
	/// <param name="subTextures">Les positions et tailles des sous-textures</param>
	/// <param name="comportement">Comportement de la plateforme</param>
	PlateformeOptimisee(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize, const sf::Vector2f& position, const int& subTextureCount, TextureRule textureRule, const std::vector<sf::FloatRect>& subTextures, TypePlateforme comportement);

	/// <summary>
	/// Initialise une plateforme en incorporant un comportement � la plateforme et en mettant une �chelle autre que 1:1
	/// </summary>
	/// <param name="texture">R�f�rence de la texture utilis�e (provenant probablement du niveau)</param>
	/// <param name="noTuileDebutTexture">Num�ro de sous-texture utilis�</param>
	/// <param name="desiredSize">Taille de la tuile</param>
	/// <param name="position">Position au coin sup�rieur gauche de la texture</param>
	/// <param name="subTextureCount">Nombre de sous-textures</param>
	/// <param name="textureRule">R�gle appliqu�e � la texture</param>
	/// <param name="subTextures">Les positions et tailles des sous-textures</param>
	/// <param name="scale">�chelle de la texture</param>
	/// <param name="comportement">Comportement de la plateforme</param>
	PlateformeOptimisee(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize, const sf::Vector2f& position, const int& subTextureCount, TextureRule textureRule, const std::vector<sf::FloatRect>& subTextures, const sf::Vector2f& scale, TypePlateforme comportement);

	/// <summary>
	/// Retourne le comportement de la plateforme
	/// </summary>
	TypePlateforme comportement() const;

	/// <summary>
	/// Change le comportement de la plateforme
	/// </summary>
	/// <param name="nouvComportement">Nouveau comportement donn� � la plateforme</param>
	void comportement(TypePlateforme nouvComportement);
};

inline PlateformeOptimisee::PlateformeOptimisee() : Tile(), m_comportement{ TypePlateforme::solide }
{

}

inline PlateformeOptimisee::PlateformeOptimisee(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize, const sf::Vector2f& position, const int& subTextureCount, TextureRule textureRule, const std::vector<sf::FloatRect>& subTextures)
	: Tile(texture, noTuileDebutTexture,desiredSize, position, subTextureCount, textureRule, subTextures), m_comportement{TypePlateforme::solide}
{

}

inline PlateformeOptimisee::PlateformeOptimisee(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize, const sf::Vector2f& position, const int& subTextureCount, TextureRule textureRule, const std::vector<sf::FloatRect>& subTextures, TypePlateforme comportement)
	: Tile(texture, noTuileDebutTexture, desiredSize, position, subTextureCount, textureRule, subTextures), m_comportement{ comportement }
{

}

inline PlateformeOptimisee::PlateformeOptimisee(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize, const sf::Vector2f& position, const int& subTextureCount, TextureRule textureRule, const std::vector<sf::FloatRect>& subTextures, const sf::Vector2f& scale, TypePlateforme comportement)
	: Tile(texture, noTuileDebutTexture, desiredSize, position, subTextureCount, textureRule, scale, subTextures), m_comportement{comportement}
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
	//std::vector <Plateforme> avantPlan;
	Niveau avantPlan;
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