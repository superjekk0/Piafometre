#ifndef PLATEFORMES_H
#define PLATEFORMES_H

#include "Jeu.h"

enum class TextureRule {
	repeat_texture,		// La texture sera répétée dans la tuile. Augmenter l'échelle augmentera simplement la taille de la tuile, sans augmenter la taille des textures
	keep_height,		// La texture sera transformée en mettant la hauteur en priorité. Ex: l'échelle de la tuile au début est de 1:1, puis devient 2:1, ça va être 1:1, puisque la hauteur a priorité. Le rectangle de texture n'est pas affecté (si une texture se répétait 2 fois, elle restera répétée 2 fois)
	keep_width,			// La texture sera transformée en mettant la largeur en priorité. Ex: l'échelle de la tuile au début est de 1:1 (100x100), puis devient 2:1. La tuile aura l'échelle 2:2 (200x200), puisque la largeur a priorité. Le rectangle de texture n'est pas affecté (si une texture se répète 2 fois, elle restera répétée 2 fois)
	keep_size,			// La texture sera plus grosse, mais l'objet gardera la même taille
	adjustable_size,	// Aucune tentative n'est faite pour garder la même taille que celle passée auparavant, tout sera transformée selon ce qui est donné, sans garder les proportions
	fill_space			// La texture prend l'entièreté de l'espace sans se répéter. La taille de la case a donc priorité sur son échelle

};

class Tile {
private:
	const sf::Texture* const m_texture;		// La texture héritée de la classe contenant la tuile
	sf::Vector2f m_textureSize;				// Indique la taille de la sous-texture utilisée
	sf::Vector2f m_texturePosition;			// Indique la position de départ de la sous-texture
	sf::Vector2f m_position;				// Donne la position de la case au coin supérieur gauche
	sf::Vector2f m_tileSize;				// Donne la taille de la case
	std::vector<sf::Vertex> m_vertexes;		// L'ensemble des points qui composent l'objet dessinable
	TextureRule m_textureRule;				// Règle régissant le comportement d'une texture lorsque la taille ou l'échelle est changée
	sf::Vector2f m_scale{ 1.f, 1.f };		// Indique le rapport entre la texture et taille demandée (lorsque l'objet est instancié, il équivaut à 1:1 par défaut)
	int m_textureCount{0};

	void intializeVertexes();
public:

	/// <summary>
	/// Ce constructeur ne devrait jamais être utilisé autrement que pour changer la taille du std::vector
	/// </summary>
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

	/// <summary>
	/// Mets la tuile à l'échelle spécifiée selon la règle de textures (voir la documentation pour plus de détails). Attention! Pour que le changement soit perceptible par l'utilisateur, l'ensemble des sommets doit être recopié dans une liste générique.
	/// </summary>
	/// <param name="scale">Facteur d'agrandissement en deux dimensions</param>
	void setScale(const sf::Vector2f& scale);

	/// <summary>
	/// Mets la tuile à l'échelle spécifiée selon la règle de textures (voir la documentation pour plus de détails). Attention! Pour que le changement soit perceptible par l'utilisateur, il faut recopier l'entièreté des sommets dans une liste générique.
	/// </summary>
	/// <param name="scale">Facteur d'agrandissement appliqué en x et en y</param>
	void setScale(float scale);

	/// <summary>
	/// Mets la tuile à l'échelle spécifiée selon la règle de textures (voir la documentation pour plus de détails). Attention! Pour que le changement soit perceptible par l'utilisateur, il faut recopier l'entièreté des sommets dans une liste générique.
	/// </summary>
	/// <param name="x">Facteur d'agrandissement horizontal</param>
	/// <param name="y">Facteur d'agrandissement vertical</param>
	void setScale(float x, float y);

	/// <summary>
	/// Met l'échelle de la texture à l'échelle spécifée selon la règle de texture ou change la taille autrement (voir la documentation pour plus de détails)
	/// </summary>
	/// <param name="scale">Facteur d'agrandissement de la texture</param>
	/// <param name="textureRule">Nouvelle règle de texture</param>
	void setScale(const sf::Vector2f& scale, TextureRule textureRule);

	/// <summary>
	/// Met l'échelle de la texture à l'échelle spécifiée dans les deux axes selon la règle de texture (ou change la taille autrement). Voir la documentation pour plus de détails.
	/// </summary>
	/// <param name="scale">Facteur d'agrandissement de la texture à l'horizontal et à la verticale</param>
	/// <param name="textureRule">Nouvelle règle de texture</param>
	void setScale(float scale, TextureRule textureRule);

	/// <summary>
	/// Met l'échelle de la texture à l'échelle spécifié à l'axe spécifié en paramètre selon la règle de texture (ou change la taille, autrement). Voir la documentation pour plus de détails.
	/// </summary>
	/// <param name="x">Facteur d'agrandissement horizontal</param>
	/// <param name="y">Facteur d'agrandissement vertical</param>
	/// <param name="textureRule">Nouvelle règle de texture</param>
	void setScale(float x, float y, TextureRule textureRule);

	/// <summary>
	/// Si la règle le permet, la taille de la tuile sera mise à jour selon la nouvelle taille. Voir la documentation des règles de textures pour plus de détails.
	/// </summary>
	/// <param name="size">Nouvelle taille demandée</param>
	void resize(const sf::Vector2f& size);

	/// <summary>
	/// Si la règle le permet, la taille de la tuile sera mise à jour selon la nouvelle taille. Voir la documentation des règles de textures pour plus de détails.
	/// </summary>
	/// <param name="x">Nouvelle taille horizontale demandée</param>
	/// <param name="y">Nouvelle taille verticale demandée</param>
	void resize(float x, float y);

	/// <summary>
	/// Si la règle le permet, la taille de la tuile sera mise à jour selon la nouvelle taille. Voir la documentation des règles de textures pour plus de détails.
	/// </summary>
	/// <param name="size">Nouvelle taille demandée</param>
	/// <param name="textureRule">Nouvelle règle de texture</param>
	void resize(const sf::Vector2f& size, TextureRule textureRule);

	/// <summary>
	/// Si la règle le permet, la taille de la tuile sera mise à jour selon la nouvelle taille. Voir la documentation pour plus de détails.
	/// </summary>
	/// <param name="x">Nouvelle taille horizontale demandée</param>
	/// <param name="y">Nouvelle taille verticale demandée</param>
	/// <param name="textureRule">Nouvelle règle de texture</param>
	void resize(float x, float y, TextureRule textureRule);

	/// <summary>
	/// Change la sous-texture de la tuile
	/// </summary>
	/// <param name="numberSubTexture">Correspond au numéro de la texture. Le premier numéro possible est 0 et va jusqu'au nombre de texture - 1. Si le numéro est non valide, le rectangle de texture reste inchangé.</param>
	void changeTextureRect(int numberSubTexture);

	/// <summary>
	/// Sert à uniquement changer la règle de texture
	/// </summary>
	/// <param name="textureRule">Nouvelle règle de texture</param>
	void setTextureRule(TextureRule textureRule);

	/// <summary>
	/// Sert à obtenir la règle de texture depuis l'extérieur de la tuile
	/// </summary>
	TextureRule getTextureRule();

	/// <summary>
	/// Bouge la tuile du vecteur rentré en paramètre de manière plus efficace que si rentré séparément
	/// </summary>
	/// <param name="offset">Déplacement à effectuer</param>
	void move(const sf::Vector2f& offset);

	/// <summary>
	/// Bouge la tuile du vecteur horizontal et vertical rentrés en paramètres de manière plus efficace que si rentré séparément
	/// </summary>
	/// <param name="offsetX">Déplacement horizontal à effectuer</param>
	/// <param name="offsetY">Déplacement vertical à effectuer</param>
	void move(float offsetX, float offsetY);

	/// <summary>
	/// Met la tuile au coin supérieur gauche à la position rentrée en paramètre
	/// </summary>
	/// <param name="position">Nouvelle position de la tuile</param>
	void setPosition(const sf::Vector2f& position);

	/// <summary>
	/// Met la tuile au coin supérieur gauche à la position rentrée en paramètre
	/// </summary>
	/// <param name="x">Nouvelle position horizontale</param>
	/// <param name="y">Nouvelle position verticale</param>
	void setPosition(float x, float y);
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
		point.position += m_position;
		point.color = sf::Color::White;
	}
}

Tile::Tile() : m_texture{nullptr}
{}

Tile::Tile(const sf::Texture& texture, int noTuileDebutTexture,
	const sf::Vector2f& desiredSize, const sf::Vector2f& position, int subTextureCount,
	TextureRule textureRule, const sf::Vector2f& scale) : m_texture{ &texture }, m_textureCount{subTextureCount},
	m_texturePosition{ texture.getSize().x / static_cast<float>(subTextureCount) * noTuileDebutTexture ,0.f },
	m_textureSize{ static_cast<float>(texture.getSize().x / subTextureCount) , static_cast<float>(texture.getSize().y) },
	m_textureRule{ textureRule }, m_scale{ m_scale }, m_tileSize{desiredSize}
{
	intializeVertexes();
}


Tile::Tile(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize,
	const sf::Vector2f& position, int subTextureCount, TextureRule textureRule) : m_textureCount{subTextureCount},
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

void Tile::setScale(float x, float y)
{
	switch (m_textureRule)
	{
	case TextureRule::repeat_texture:
	case TextureRule::fill_space:
		m_tileSize.x *= x;
		m_tileSize.y *= y;
		break;
	case TextureRule::adjustable_size:
		m_tileSize.x *= x;
		m_tileSize.y *= y;
		m_scale.x *= x;
		m_scale.y *= y;
		break;
	case TextureRule::keep_height:
		m_tileSize.x *= y;
		m_tileSize.y *= y;
		m_scale.x *= y;
		m_scale.y *= y;
		break;
	case TextureRule::keep_width:
		m_tileSize.x *= x;
		m_tileSize.y *= x;
		m_scale.x *= x;
		m_scale.y *= x;
		break;
	case TextureRule::keep_size:
		m_scale.x *= x;
		m_scale.y *= y;
		break;
	}
	intializeVertexes();
}

void Tile::setScale(const sf::Vector2f& scale, TextureRule textureRule)
{
	m_textureRule = textureRule;
	setScale(scale);
}

void Tile::setScale(float scale, TextureRule textureRule)
{
	m_textureRule = textureRule;
	setScale(scale);
}

void Tile::setScale(float x, float y, TextureRule textureRule)
{
	m_textureRule = textureRule;
	setScale(x, y);
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

void Tile::resize(const sf::Vector2f& size, TextureRule textureRule)
{
	m_textureRule = textureRule;
	resize(size);
}

void Tile::resize(float x, float y, TextureRule textureRule)
{
	m_textureRule = textureRule;
	resize(x, y);
}

void Tile::changeTextureRect(int numberSubTexture)
{
	if (numberSubTexture >= 0 && numberSubTexture < m_textureCount)
	{
		m_texturePosition = sf::Vector2f(m_textureSize.x / m_textureCount * numberSubTexture, 0.f);
		intializeVertexes();
	}
}

void Tile::setTextureRule(TextureRule textureRule)
{
	m_textureRule = textureRule;
}

TextureRule Tile::getTextureRule()
{
	return m_textureRule;
}

void Tile::move(const sf::Vector2f& offset)
{
	for (auto& sommet : m_vertexes)
		sommet.position += offset;	
}

void Tile::move(float offsetX, float offsetY)
{
	sf::Vector2f offset{ offsetX, offsetY };
	for (auto& sommet : m_vertexes)
		sommet.position += offset;
}

void Tile::setPosition(const sf::Vector2f& position)
{
	m_position = position;
	intializeVertexes();
}

void Tile::setPosition(float x, float y)
{
	m_position = sf::Vector2f(x, y);
	intializeVertexes();
}
/// TODO : Changer le nom de PlateformeOptimisee à Plateforme
/// TODO : Hériter de CaseOptimisee
class PlateformeOptimisee {
private:
	/// TODO : Membres privés à rajouter. Le comportement
public:
	/// TODO : Constructeur à quatre paramètres. D'abord la texture, puis le rectangle de texture, puis les coordonnées au coin supérieur gauche de la plateforme et finalement, le comportement de la plateforme (si non donné, la plateforme a le comportement solide). Ça appellera la classe de base

	/// TODO : Méthode pour changer le comportement de la plateforme
};

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
