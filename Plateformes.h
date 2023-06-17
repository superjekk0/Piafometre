#ifndef PLATEFORMES_H
#define PLATEFORMES_H

#include "Jeu.h"

enum class TextureRule {
	repeat_texture,		// La texture sera r�p�t�e dans la tuile. Augmenter l'�chelle augmentera simplement la taille de la tuile, sans augmenter la taille des textures
	keep_height,		// La texture sera transform�e en mettant la hauteur en priorit�. Ex: l'�chelle de la tuile au d�but est de 1:1, puis devient 2:1, �a va �tre 1:1, puisque la hauteur a priorit�. Le rectangle de texture n'est pas affect� (si une texture se r�p�tait 2 fois, elle restera r�p�t�e 2 fois)
	keep_width,			// La texture sera transform�e en mettant la largeur en priorit�. Ex: l'�chelle de la tuile au d�but est de 1:1 (100x100), puis devient 2:1. La tuile aura l'�chelle 2:2 (200x200), puisque la largeur a priorit�. Le rectangle de texture n'est pas affect� (si une texture se r�p�te 2 fois, elle restera r�p�t�e 2 fois)
	keep_size,			// La texture sera plus grosse, mais l'objet gardera la m�me taille
	adjustable_size,	// Aucune tentative n'est faite pour garder la m�me taille que celle pass�e auparavant, tout sera transform�e selon ce qui est donn�, sans garder les proportions
	fill_space			// La texture prend l'enti�ret� de l'espace sans se r�p�ter. La taille de la case a donc priorit� sur son �chelle

};

class Tile : sf::Transformable {
private:
	const sf::Texture* const m_texture;		// La texture h�rit�e de la classe contenant la tuile
	sf::Vector2f m_textureSize;				// Indique la taille de la sous-texture utilis�e
	sf::Vector2f m_texturePosition;			// Indique la position de d�part de la sous-texture
	sf::Vector2f m_position;				// Donne la position de la case au coin sup�rieur gauche
	sf::Vector2f m_tileSize;				// Donne la taille de la case
	std::vector<sf::Vertex> m_vertexes;		// L'ensemble des points qui composent l'objet dessinable
	TextureRule m_textureRule;				// R�gle r�gissant le comportement d'une texture lorsque la taille ou l'�chelle est chang�e
	sf::Vector2f m_scale{ 1.f, 1.f };		// Indique le rapport entre la texture et taille demand�e (lorsque l'objet est instanci�, il �quivaut � 1:1 par d�faut)
	
	void intializeVertexes();
public:

	/// <summary>
	/// Ce constructeur ne devrait jamais �tre utilis� autrement que pour changer la taille du std::vector
	/// </summary>
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

	/// <summary>
	/// Mets la tuile � l'�chelle sp�cifi�e selon la r�gle de textures. Attention! Pour que le changement soit perceptible par l'utilisateur, l'ensemble des sommets doit �tre recopi� dans une liste g�n�rique.
	/// </summary>
	/// <param name="scale">Multiplicateur de taille</param>
	void setScale(const sf::Vector2f& scale);

	/// <summary>
	/// Mets la tuile � l'�chelle sp�cifi�e selon la r�gle de textures. Attention! Pour que le changement soit perceptible par l'utilisateur, il faut recopier l'enti�ret� des sommets dans une liste g�n�rique.
	/// </summary>
	/// <param name="scale">�chelle de texture appliqu�e aux ordonn�es et aux abcisses</param>
	void setScale(float scale);

	/// <summary>
	/// Si la r�gle le permet, la taille de la tuile sera mise � jour selon la nouvelle taille (voir la documentation des r�gles de textures pour plus de d�tails)
	/// </summary>
	/// <param name="size">Nouvelle taille demand�e</param>
	void resize(const sf::Vector2f& size);

	/// <summary>
	/// Si la r�gle le permet, la taille de la tuile sera mise � jour selon la nouvelle taille (voir la documentation des r�gles de textures pour plus de d�tails)
	/// </summary>
	/// <param name="x">Nouvelle taille horizontale demand�e</param>
	/// <param name="y">Nouvelle taille verticale demand�e</param>
	void resize(float x, float y);
};

void Tile::intializeVertexes()
{
	if (m_textureRule == TextureRule::fill_space)
	{
		m_scale.x = m_tileSize.x / m_textureSize.x;
		m_scale.y = m_tileSize.y / m_textureSize.y;
	}

	sf::Vector2f coinGaucheSommet;
	m_vertexes.resize(6);
	std::size_t indexSommet{ 0 };

	while (coinGaucheSommet.y < m_tileSize.y)
	{
		coinGaucheSommet.x = 0.f;
		for (; coinGaucheSommet.x < m_tileSize.x; coinGaucheSommet.x += m_textureSize.x * m_scale.x,
			indexSommet = m_vertexes.size(), m_vertexes.resize(m_vertexes.size() + 6))
		{
			m_vertexes[indexSommet].position = coinGaucheSommet;
			m_vertexes[indexSommet].texCoords = sf::Vector2f(m_texturePosition.x, 0.f);

			if (coinGaucheSommet.y + m_textureSize.y * m_scale.y > m_tileSize.y)
			{
				m_vertexes[indexSommet + 1].position = coinGaucheSommet + sf::Vector2f(0.f, m_tileSize.y - coinGaucheSommet.y);
				m_vertexes[indexSommet + 1].texCoords = sf::Vector2f(m_texturePosition.x, m_tileSize.y - coinGaucheSommet.y);
			}
			else
			{
				m_vertexes[indexSommet + 1].position = coinGaucheSommet + sf::Vector2f(0.f, m_textureSize.y * m_scale.y);
				m_vertexes[indexSommet + 1].texCoords = sf::Vector2f(m_texturePosition.x, m_textureSize.y);
			}

			if (coinGaucheSommet.x + m_textureSize.x * m_scale.x > m_tileSize.y)
			{
				m_vertexes[indexSommet + 2].position = coinGaucheSommet + sf::Vector2f(m_tileSize.x - coinGaucheSommet.x, 0.f);
				m_vertexes[indexSommet + 2].texCoords = sf::Vector2f(m_texturePosition.x + (m_tileSize.x - coinGaucheSommet.x), 0.f);
			}
			else
			{
				m_vertexes[indexSommet + 2].position = coinGaucheSommet + sf::Vector2f(m_textureSize.x * m_scale.x, 0.f);
				m_vertexes[indexSommet + 2].texCoords = sf::Vector2f(m_texturePosition.x + m_textureSize.x, 0.f);
			}

			m_vertexes[indexSommet + 3] = m_vertexes[indexSommet + 1];
			m_vertexes[indexSommet + 4] = m_vertexes[indexSommet + 2];

			m_vertexes[indexSommet + 5].position = sf::Vector2f(m_vertexes[indexSommet + 4].position.x, m_vertexes[indexSommet + 3].position.y);
			m_vertexes[indexSommet + 5].texCoords = sf::Vector2f(m_vertexes[indexSommet + 4].texCoords.x, m_vertexes[indexSommet + 3].texCoords.y);

		}
		coinGaucheSommet.y += m_textureSize.y * m_scale.y;
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
	m_textureRule{ textureRule }, m_scale{ m_scale }, m_tileSize{desiredSize}
{
	intializeVertexes();
}


Tile::Tile(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize,
	const sf::Vector2f& position, int subTextureCount, TextureRule textureRule) :
	m_texture{ &texture }, m_texturePosition{ texture.getSize().x / static_cast<float>(subTextureCount) * noTuileDebutTexture , 0.f },
	m_textureSize{ static_cast<float>(texture.getSize().x / subTextureCount) , static_cast<float>(texture.getSize().y) },
	m_textureRule{ textureRule }, m_scale{ 1.f, 1.f }, m_tileSize{desiredSize}
{
	intializeVertexes();
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

void Tile::setScale(const sf::Vector2f& scale)
{
	switch (m_textureRule)
	{
	case TextureRule::repeat_texture:
	case TextureRule::fill_space:
		m_tileSize.x *= scale.x;
		m_tileSize.y *= scale.y;
		break;
	case TextureRule::keep_height:
		m_scale.x *= scale.y;
		m_scale.y *= scale.y;
		m_tileSize.x *= scale.y;
		m_tileSize.y *= scale.y;
		break;
	case TextureRule::keep_width:
		m_scale.x *= scale.x;
		m_scale.y *= scale.x;
		m_tileSize.x *= scale.x;
		m_tileSize.y *= scale.x;
		break;
	case TextureRule::keep_size:
		m_scale.x *= scale.x;
		m_scale.y *= scale.y;
		break;
	case TextureRule::adjustable_size:
		m_scale.x *= scale.x;
		m_scale.y *= scale.y;
		m_tileSize.x *= scale.x;
		m_tileSize.y *= scale.y;
		break;
	}
	intializeVertexes();
}

void Tile::setScale(float scale)
{
	switch (m_textureRule)
	{
	case TextureRule::keep_height:
	case TextureRule::keep_width:
	case TextureRule::adjustable_size:
		m_tileSize *= scale;
		m_scale *= scale;
		break;
	case TextureRule::repeat_texture:
	case TextureRule::fill_space:
		m_tileSize *= scale;
		break;
	case TextureRule::keep_size:
		m_scale *= scale;
		break;
	}
	intializeVertexes();
}

void Tile::resize(const sf::Vector2f& size)
{
	float scale{};
	switch (m_textureRule)
	{
	case TextureRule::repeat_texture:
	case TextureRule::adjustable_size:
	case TextureRule::fill_space:
		m_tileSize = size;
		break;
	case TextureRule::keep_height:
		scale = size.y / m_tileSize.y;
		m_tileSize *= scale;
		m_scale *= scale;
		break;
	case TextureRule::keep_width:
		scale = size.x / m_tileSize.y;
		m_tileSize *= scale;
		m_scale *= scale;
		break;
	case TextureRule::keep_size:
		break;
	}
	intializeVertexes();
}

void Tile::resize(float x, float y)
{
	float scale{};
	switch (m_textureRule)
	{
	case TextureRule::repeat_texture:
	case TextureRule::adjustable_size:
	case TextureRule::fill_space:
		m_tileSize = sf::Vector2f(x, y);
		break;
	case TextureRule::keep_height:
		scale = y / m_tileSize.y;
		m_tileSize *= scale;
		m_scale *= scale;
		break;
	case TextureRule::keep_width:
		scale = x / m_textureSize.x;
		m_tileSize *= scale;
		m_scale *= scale;
		break;
	case TextureRule::keep_size:
		break;
	}
	intializeVertexes();
}

void Tile::resize(const sf::Vector2f& size)
{
	float scale{};
	switch (m_textureRule)
	{
	case TextureRule::repeat_texture:
	case TextureRule::adjustable_size:
	case TextureRule::fill_space:
		m_tileSize = size;
		break;
	case TextureRule::keep_height:
		scale = size.y / m_tileSize.y;
		m_tileSize *= scale;
		m_scale *= scale;
		break;
	case TextureRule::keep_width:
		scale = size.x / m_tileSize.y;
		m_tileSize *= scale;
		m_scale *= scale;
		break;
	case TextureRule::keep_size:
		break;
	}
}

void Tile::resize(float x, float y)
{
	float scale{};
	switch (m_textureRule)
	{
	case TextureRule::repeat_texture:
	case TextureRule::adjustable_size:
	case TextureRule::fill_space:
		m_tileSize = sf::Vector2f(x, y);
		break;
	case TextureRule::keep_height:
		scale = y / m_tileSize.y;
		m_tileSize *= scale;
		m_scale *= scale;
		break;
	case TextureRule::keep_width:
		scale = x / m_textureSize.x;
		m_tileSize *= scale;
		m_scale *= scale;
		break;
	case TextureRule::keep_size:
		break;
	}
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
