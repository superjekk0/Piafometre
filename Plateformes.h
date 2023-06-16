#pragma once
#ifndef PLATEFORMES_H
#define PLATEFORMES_H

#include "Jeu.h"

enum class TextureRule {
	repeatTexture,		// La texture sera r�p�t�e dans la tuile. Augmenter l'�chelle augmentera simplement la taille de la tuile, sans augmenter la taille des textures
	keep_height,		// La texture sera transform�e en mettant la hauteur en priorit�. Ex: l'�chelle de la tuile au d�but est de 1:1, puis devient 2:1, �a va �tre 1:1, puisque la hauteur a priorit�. Le rectangle de texture n'est pas affect� (si une texture se r�p�tait 2 fois, elle restera r�p�t�e 2 fois)
	keep_width,			// La texture sera transform�e en mettant la largeur en priorit�. Ex: l'�chelle de la tuile au d�but est de 1:1 (100x100), puis devient 2:1. La tuile aura l'�chelle 2:2 (200x200), puisque la largeur a priorit�. Le rectangle de texture n'est pas affect� (si une texture se r�p�te 2 fois, elle restera r�p�t�e 2 fois)
	keep_size,			// La texture sera plus grosse, mais l'objet gardera la m�me taille
	adjustable_size		// Aucune tentative n'est faite pour garder la m�me taille que celle pass�e auparavant, tout sera transform�e selon ce qui est donn�, sans garder les proportions
};

class Tile : sf::Transformable {
private:
	const sf::Texture* const m_texture;			// La texture h�rit�e de la classe contenant la tuile
	sf::Vector2f m_textureSize;				// Indique la taille de la sous-texture utilis�e
	sf::Vector2f m_texturePosition;			// Indique la position de d�part de la sous-texture
	sf::Vector2f m_position;				// Donne la position de la case au coin sup�rieur gauche
	sf::Vector2f m_tileSize;				// Donne la taille de la case
	std::vector<sf::Vertex> m_vertexes;		// L'ensemble des points qui composent l'objet dessinable
	TextureRule m_textureRule;				// R�gle r�gissant le comportement d'une texture lorsque la taille ou l'�chelle est chang�e
	sf::Vector2f m_scale{ 1.f, 1.f };			// Indique le rapport entre la texture et taille demand�e (lorsque l'objet est instanci�, il �quivaut � 1:1 par d�faut)
	///TODO : Inclure les membres priv�s tels que la texture globale, le carr� de texture, les transformations appliqu�es � la texture, la taille d�sir�e
	
	void intializeVertexes(const sf::Vector2f& position, const sf::Vector2f& desiredSize);
public:

	Tile();

	Tile(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize, const sf::Vector2f& position, int subTextureCount, TextureRule textureRule);

	Tile(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize, const sf::Vector2f& position, int subTextureCount, TextureRule textureRule, const sf::Vector2f& scale);
	/// <summary>
	/// Retourne une r�f�rence de la liste g�n�rique de sommets (pour pouvoir tout dessiner en un appel de la m�thode draw)
	/// </summary>
	const std::vector<sf::Vertex>& vertexes() const;

	/// <summary>
	/// Retourne la hauteur de la tuile
	/// </summary>
	float height() const;

	/// <summary>
	/// Retourne la largeur de la tuile
	/// </summary>
	float width() const;

	/// <summary>
	/// Retourne les coordonn�es du coin sup�rieur gauche de la tuile
	/// </summary>
	sf::Vector2f topLeftCorner() const;

	/// <summary>
	/// Retourne les coordonn�es du coin sup�rieur droit de la tuile
	/// </summary>
	sf::Vector2f topRightCorner() const;

	/// <summary>
	/// Retourne les coordonn�es du coin inf�rieur gauche de la tuile
	/// </summary>
	sf::Vector2f bottomLeftCorner() const;

	/// <summary>
	/// Retourne les coordonn�es du coin inf�rieur droit de la tuile
	/// </summary>
	sf::Vector2f bottomRightCorner() const;

};

void Tile::intializeVertexes(const sf::Vector2f& position, const sf::Vector2f& desiredSize)
{
	setPosition(position);
	sf::Vector2f coinGaucheSommet;
	m_vertexes.resize(6);
	std::size_t indexSommet{ 0 };

	while (coinGaucheSommet.y < desiredSize.y)
	{
		coinGaucheSommet.x = 0.f;
		for (; coinGaucheSommet.x < desiredSize.x; coinGaucheSommet.x += m_textureSize.x,
			indexSommet = m_vertexes.size(), m_vertexes.resize(m_vertexes.size() + 6))
		{
			m_vertexes[indexSommet].position = coinGaucheSommet;
			m_vertexes[indexSommet].texCoords = sf::Vector2f(m_texturePosition.x, 0.f);

			if (coinGaucheSommet.y + m_textureSize.y > desiredSize.y)
			{
				m_vertexes[indexSommet + 1].position = coinGaucheSommet + sf::Vector2f(0.f, desiredSize.y - coinGaucheSommet.y);
				m_vertexes[indexSommet + 1].texCoords = sf::Vector2f(m_texturePosition.x, desiredSize.y - coinGaucheSommet.y);
			}
			else
			{
				m_vertexes[indexSommet + 1].position = coinGaucheSommet + sf::Vector2f(0.f, m_textureSize.y);
				m_vertexes[indexSommet + 1].texCoords = sf::Vector2f(m_texturePosition.x, m_textureSize.y);
			}

			if (coinGaucheSommet.x + m_textureSize.x > desiredSize.y)
			{
				m_vertexes[indexSommet + 2].position = coinGaucheSommet + sf::Vector2f(desiredSize.x - coinGaucheSommet.x, 0.f);
				m_vertexes[indexSommet + 2].texCoords = sf::Vector2f(m_texturePosition.x + (desiredSize.x - coinGaucheSommet.x), 0.f);
			}
			else
			{
				m_vertexes[indexSommet + 2].position = coinGaucheSommet + sf::Vector2f(m_textureSize.x, 0.f);
				m_vertexes[indexSommet + 2].texCoords = sf::Vector2f(m_texturePosition.x + m_textureSize.x, 0.f);
			}

			m_vertexes[indexSommet + 3] = m_vertexes[indexSommet + 1];
			m_vertexes[indexSommet + 4] = m_vertexes[indexSommet + 2];

			m_vertexes[indexSommet + 5].position = sf::Vector2f(m_vertexes[indexSommet + 4].position.x, m_vertexes[indexSommet + 3].position.y);
			m_vertexes[indexSommet + 5].texCoords = sf::Vector2f(m_vertexes[indexSommet + 4].texCoords.x, m_vertexes[indexSommet + 3].texCoords.y);

			//Le but est de toujours changer d'un sommet sans jamais faire de bond qui ne semble pas possible � faire avec un triangle
			//indexSommet = m_vertexes.size();
			//m_vertexes.resize(m_vertexes.size() + 3);
			//coinGaucheSommet.x += m_taille;
		}
		coinGaucheSommet.y += m_textureSize.y;
	}
	for (sf::Vertex& point : m_vertexes)
	{
		point.position += getPosition();
		point.color = sf::Color::White;
	}
}

Tile::Tile() : m_texture{nullptr}
{}

Tile::Tile(const sf::Texture& texture, int noTuileDebutTexture,
	const sf::Vector2f& desiredSize, const sf::Vector2f& position, int subTextureCount,
	TextureRule textureRule, const sf::Vector2f& scale) : m_texture{ &texture },
	m_texturePosition{ texture.getSize().x / static_cast<float>(subTextureCount) * noTuileDebutTexture ,0.f },
	m_textureSize{ static_cast<float>(texture.getSize().x / subTextureCount) , static_cast<float>(texture.getSize().y) },
	m_textureRule{ textureRule }, m_scale{ m_scale }
{
	intializeVertexes(position, desiredSize);
}


Tile::Tile(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize,
	const sf::Vector2f& position, int subTextureCount, TextureRule textureRule) :
	m_texture{ &texture }, m_texturePosition{ texture.getSize().x / static_cast<float>(subTextureCount) * noTuileDebutTexture , 0.f },
	m_textureSize{ static_cast<float>(texture.getSize().x / subTextureCount) , static_cast<float>(texture.getSize().y) },
	m_textureRule{ textureRule }, m_scale{ 1.f, 1.f }
{
	intializeVertexes(position, desiredSize);
}

float Tile::height() const
{
	return m_tileSize.y;
}

float Tile::width() const
{
	return m_tileSize.x;
}

const std::vector<sf::Vertex>& Tile::vertexes() const
{
	return m_vertexes;
}

sf::Vector2f Tile::topLeftCorner() const
{
	return m_position;
}

sf::Vector2f Tile::topRightCorner() const
{
	return m_position + sf::Vector2f(this->width(), 0.f);
}

sf::Vector2f Tile::bottomLeftCorner() const
{
	return m_position + sf::Vector2f(0.f, this->height());
}

sf::Vector2f Tile::bottomRightCorner() const
{
	return m_position + this->m_tileSize;
}
/// TODO : Changer le nom de PlateformeOptimisee � Plateforme
/// TODO : H�riter de CaseOptimisee
class PlateformeOptimisee {
private:
	/// TODO : Membres priv�s � rajouter. Le comportement
public:
	/// TODO : Constructeur � quatre param�tres. D'abord la texture, puis le rectangle de texture, puis les coordonn�es au coin sup�rieur gauche de la plateforme et finalement, le comportement de la plateforme (si non donn�, la plateforme a le comportement solide). �a appellera la classe de base

	/// TODO : M�thode pour changer la taille du carr� de texture
	/// TODO : M�thode pour changer la taille d�sir�e de la plateforme
	/// TODO : M�thode pour changer le comportement de la plateforme
	/// TODO : M�thode pour obtenir les coordonn�es du coin sup�rieur droit
	/// TODO : M�thode pour obtenir les coordonn�es du coin sup�rieur gauche
	/// TODO : M�thode pour obtenir les coordonn�es du coin inf�rieur gauche
	/// TODO : M�thode pour obtenir les coordonn�es du coin inf�rieur droit
	/// TODO : M�thode pour obtenir la largeur de la plateforme
	/// TODO : M�thode pour obtenir la hauteur de la plateforme
};

/// TODO : Ajouter l'h�ritage des classe SFML de base
class Niveau : sf::Drawable {
private:
	/// TODO : Membres priv�s � rajouter. Inclue le std::vector de CaseOptimisee et la texture globale
	std::vector<Tile> m_tiles;
	sf::Texture m_texture;					// Texture utilis�e pour toutes les cases
	std::size_t m_nbTexture;				// Indique le nombre de sous-textures dans le fichier

public:
	/// <summary>
	/// Charge en m�moire la texture d�sir�e et met le compteur de cases � 0
	/// </summary>
	/// <param name="pPathTexture"></param>
	Niveau(const std::string& pPathTexture, std::size_t pNbTextures);
	/// TODO : Surcharger la m�thode draw avec la classe Niveau comme param�tre

	/// <summary>
	/// Retourne une r�f�rence de la case � l'index sp�cifi�
	/// </summary>
	/// <param name="index">Index de case</param>
	Tile& operator[](int index);
};

Niveau::Niveau(const std::string& pPathTexture, std::size_t pNbTextures) :
	m_nbTexture{ pNbTextures }
{
	bool loaded{ m_texture.loadFromFile(pPathTexture) };
	assert(loaded && "Unable to load the texture file");
	m_tiles.resize(0);
}

Tile& Niveau::operator[](int index)
{
	return m_tiles[index];
}
#endif 
