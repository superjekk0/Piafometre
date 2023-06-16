#pragma once
#ifndef PLATEFORMES_H
#define PLATEFORMES_H

#include "Jeu.h"

enum class TextureRule {
	repeatTexture,		// La texture sera répétée dans la tuile. Augmenter l'échelle augmentera simplement la taille de la tuile, sans augmenter la taille des textures
	keep_height,		// La texture sera transformée en mettant la hauteur en priorité. Ex: l'échelle de la tuile au début est de 1:1, puis devient 2:1, ça va être 1:1, puisque la hauteur a priorité. Le rectangle de texture n'est pas affecté (si une texture se répétait 2 fois, elle restera répétée 2 fois)
	keep_width,			// La texture sera transformée en mettant la largeur en priorité. Ex: l'échelle de la tuile au début est de 1:1 (100x100), puis devient 2:1. La tuile aura l'échelle 2:2 (200x200), puisque la largeur a priorité. Le rectangle de texture n'est pas affecté (si une texture se répète 2 fois, elle restera répétée 2 fois)
	keep_size,			// La texture sera plus grosse, mais l'objet gardera la même taille
	adjustable_size		// Aucune tentative n'est faite pour garder la même taille que celle passée auparavant, tout sera transformée selon ce qui est donné, sans garder les proportions
};

class Tile : sf::Transformable {
private:
	const sf::Texture* const m_texture;			// La texture héritée de la classe contenant la tuile
	sf::Vector2f m_textureSize;				// Indique la taille de la sous-texture utilisée
	sf::Vector2f m_texturePosition;			// Indique la position de départ de la sous-texture
	sf::Vector2f m_position;				// Donne la position de la case au coin supérieur gauche
	sf::Vector2f m_tileSize;				// Donne la taille de la case
	std::vector<sf::Vertex> m_vertexes;		// L'ensemble des points qui composent l'objet dessinable
	TextureRule m_textureRule;				// Règle régissant le comportement d'une texture lorsque la taille ou l'échelle est changée
	sf::Vector2f m_scale{ 1.f, 1.f };			// Indique le rapport entre la texture et taille demandée (lorsque l'objet est instancié, il équivaut à 1:1 par défaut)
	///TODO : Inclure les membres privés tels que la texture globale, le carré de texture, les transformations appliquées à la texture, la taille désirée
	
	void intializeVertexes(const sf::Vector2f& position, const sf::Vector2f& desiredSize);
public:

	Tile();

	Tile(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize, const sf::Vector2f& position, int subTextureCount, TextureRule textureRule);

	Tile(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize, const sf::Vector2f& position, int subTextureCount, TextureRule textureRule, const sf::Vector2f& scale);
	/// <summary>
	/// Retourne une référence de la liste générique de sommets (pour pouvoir tout dessiner en un appel de la méthode draw)
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
	/// Retourne les coordonnées du coin supérieur gauche de la tuile
	/// </summary>
	sf::Vector2f topLeftCorner() const;

	/// <summary>
	/// Retourne les coordonnées du coin supérieur droit de la tuile
	/// </summary>
	sf::Vector2f topRightCorner() const;

	/// <summary>
	/// Retourne les coordonnées du coin inférieur gauche de la tuile
	/// </summary>
	sf::Vector2f bottomLeftCorner() const;

	/// <summary>
	/// Retourne les coordonnées du coin inférieur droit de la tuile
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

			//Le but est de toujours changer d'un sommet sans jamais faire de bond qui ne semble pas possible à faire avec un triangle
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
/// TODO : Changer le nom de PlateformeOptimisee à Plateforme
/// TODO : Hériter de CaseOptimisee
class PlateformeOptimisee {
private:
	/// TODO : Membres privés à rajouter. Le comportement
public:
	/// TODO : Constructeur à quatre paramètres. D'abord la texture, puis le rectangle de texture, puis les coordonnées au coin supérieur gauche de la plateforme et finalement, le comportement de la plateforme (si non donné, la plateforme a le comportement solide). Ça appellera la classe de base

	/// TODO : Méthode pour changer la taille du carré de texture
	/// TODO : Méthode pour changer la taille désirée de la plateforme
	/// TODO : Méthode pour changer le comportement de la plateforme
	/// TODO : Méthode pour obtenir les coordonnées du coin supérieur droit
	/// TODO : Méthode pour obtenir les coordonnées du coin supérieur gauche
	/// TODO : Méthode pour obtenir les coordonnées du coin inférieur gauche
	/// TODO : Méthode pour obtenir les coordonnées du coin inférieur droit
	/// TODO : Méthode pour obtenir la largeur de la plateforme
	/// TODO : Méthode pour obtenir la hauteur de la plateforme
};

/// TODO : Ajouter l'héritage des classe SFML de base
class Niveau : sf::Drawable {
private:
	/// TODO : Membres privés à rajouter. Inclue le std::vector de CaseOptimisee et la texture globale
	std::vector<Tile> m_tiles;
	sf::Texture m_texture;					// Texture utilisée pour toutes les cases
	std::size_t m_nbTexture;				// Indique le nombre de sous-textures dans le fichier

public:
	/// <summary>
	/// Charge en mémoire la texture désirée et met le compteur de cases à 0
	/// </summary>
	/// <param name="pPathTexture"></param>
	Niveau(const std::string& pPathTexture, std::size_t pNbTextures);
	/// TODO : Surcharger la méthode draw avec la classe Niveau comme paramètre

	/// <summary>
	/// Retourne une référence de la case à l'index spécifié
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
