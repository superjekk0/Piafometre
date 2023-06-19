#ifndef PLATEFORMES_H
#define PLATEFORMES_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cassert>

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
	const int* const m_textureCount;		// Indique le nombre de textures dans la texture globale
	int m_numberSubTexture;					// Indique le numéro de sous-texture (utile lorsque la texture est changée)

	void intializeVertexes();
public:

	/// <summary>
	/// Ce constructeur ne devrait jamais être utilisé autrement que pour changer la taille du std::vector
	/// </summary>
	Tile();

	Tile(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize, const sf::Vector2f& position, const int& subTextureCount, TextureRule textureRule);

	Tile(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize, const sf::Vector2f& position, const int& subTextureCount, TextureRule textureRule, const sf::Vector2f& scale);
	
	/// <summary>
	/// Retourne une r�f�rence de la liste générique de sommets (pour pouvoir tout dessiner en un appel de la méthode draw)
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
	/// Met l'échelle de la texture à l'échelle spécifiée selon la règle de texture ou change la taille autrement (voir la documentation pour plus de détails)
	/// </summary>
	/// <param name="scale">Facteur d'agrandissement de la texture</param>
	/// <param name="textureRule">Nouvelle règle de texture</param>
	void setScale(const sf::Vector2f& scale, TextureRule textureRule);

	/// <summary>
	/// Met l'échelle de la texture à l'échelle spécifiée dans les deux axes selon la règle de texture (ou change la taille autrement). Voir la documentation pour plus de détails.
	/// </summary>
	/// <param name="scale">Facteur d'agrandissement de la texture è l'horizontal et à la verticale</param>
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
	/// <param name="offsetX">D�placement horizontal à effectuer</param>
	/// <param name="offsetY">D�placement vertical à effectuer</param>
	void move(float offsetX, float offsetY);

	/// <summary>
	/// Met la tuile au coin supérieur gauche depuis la position rentrée en paramètre
	/// </summary>
	/// <param name="position">Nouvelle position de la tuile</param>
	void setPosition(const sf::Vector2f& position);

	/// <summary>
	/// Met la tuile au coin sup�rieur gauche depuis la position rentrée en param�tre
	/// </summary>
	/// <param name="x">Nouvelle position horizontale</param>
	/// <param name="y">Nouvelle position verticale</param>
	void setPosition(float x, float y);

	/// <summary>
	/// Recharge les sommets pour que les tuiles suivent bien la nouvelle texture. Si le numéro de texture dépasse la nouvelle valeur maximale (contenue dans la classe Niveau), la sous-texture correspondra à la dernière disponible
	/// </summary>
	void reloadTexture();
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

Tile::Tile() : m_texture{nullptr}, m_textureCount{nullptr}, m_numberSubTexture{0}, m_textureRule{TextureRule::repeat_texture}
{}

Tile::Tile(const sf::Texture& texture, int noTuileDebutTexture,
	const sf::Vector2f& desiredSize, const sf::Vector2f& position, const int& subTextureCount,
	TextureRule textureRule, const sf::Vector2f& scale) : m_texture{ &texture }, m_textureCount{&subTextureCount},
	m_texturePosition{ texture.getSize().x / static_cast<float>(subTextureCount) * noTuileDebutTexture ,0.f },
	m_textureSize{ static_cast<float>(texture.getSize().x / subTextureCount) , static_cast<float>(texture.getSize().y) },
	m_textureRule{ textureRule }, m_scale{ m_scale }, m_tileSize{desiredSize}
{
	intializeVertexes();
}


Tile::Tile(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize,
	const sf::Vector2f& position, const int& subTextureCount, TextureRule textureRule) : m_textureCount{&subTextureCount},
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
	if (numberSubTexture >= 0 && numberSubTexture < *m_textureCount)
	{
		m_numberSubTexture = numberSubTexture;
		m_texturePosition = sf::Vector2f(m_textureSize.x / *m_textureCount * m_numberSubTexture, 0.f);
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

void Tile::reloadTexture()
{
	if (m_numberSubTexture >= *m_textureCount)
		m_numberSubTexture = *m_textureCount - 1;
	changeTextureRect(m_numberSubTexture);
}

/// TODO : Changer le nom de PlateformeOptimisee à Plateforme
class PlateformeOptimisee : Tile {
private:
	/// TODO : Membres privés à rajouter. Le comportement
public:
	/// TODO : Constructeur à quatre paramètres. D'abord la texture, puis le rectangle de texture, puis les coordonnées au coin supérieur gauche de la plateforme et finalement, le comportement de la plateforme (si non donné, la plateforme a le comportement solide). Ça appellera la classe de base

	/// TODO : Méthode pour changer le comportement de la plateforme
};

class Niveau : sf::Drawable {
private:
	std::vector<Tile> m_tiles;
	sf::Texture m_texture;						// Texture utilisée pour toutes les cases
	std::size_t m_nbTexture;					// Indique le nombre de sous-textures dans le fichier
	std::vector<std::size_t> m_beginTileIndex;	// Indique l'index de commencement des sommets de chaque tuile
	std::vector<sf::Vertex> m_vertexes;			// Ensemble des sommets copiés par valeur des tuiles. À n'utiliser que pour la méthode draw et ce qui aide à faire le rendu

	/// <summary>
	/// Recharge la liste générique de sommets et l'index de départ des tuiles par rapport aux sommets
	/// </summary>
	void reloadVertexes();

	/// <summary>
	/// Indique si on continue de mettre à jour les sommets
	/// </summary>
	/// <param name="index">Index de la tuile</param>
	/// <param name="itterator">Index dans la liste générique de sommets</param>
	bool continueUpdate(std::size_t index, std::size_t itterator);
public:
	/// <summary>
	/// Charge en mémoire la texture désirée et met le compteur de cases à 0
	/// </summary>
	/// <param name="pPathTexture"></param>
	Niveau(const std::string& pPathTexture, std::size_t pNbTextures);

	/// <summary>
	/// Retourne une référence de la case à l'index spécifié
	/// </summary>
	/// <param name="index">Index de case</param>
	Tile& operator[](int index);

	/// <summary>
	/// Dessine le niveau sur l'élément SFML cible
	/// </summary>
	/// <param name="target">Élément SFML cible du rendu</param>
	/// <param name="states">États à ajouter aux vecteurs</param>
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	/// <summary>
	/// Bouge la tuile spécifiée à l'index au mouvement spécifié puis met à jour les sommets
	/// </summary>
	/// <param name="x">Mouvement horizontal</param>
	/// <param name="y">Mouvement vertical</param>
	/// <param name="index">Index de la tuile</param>
	void move(float offsetX, float offsetY, std::size_t index);

	/// <summary>
	/// Bouge la tuile spécifiée par l'index au mouvement indiqué en paramètre puis met à jour les sommets
	/// </summary>
	/// <param name="offset">Mouvement à effectuer</param>
	/// <param name="index">Index de la tuile</param>
	void move(const sf::Vector2f& offset, std::size_t index);

	/// <summary>
	/// Permet de redimensionner la tuile et met à jour les sommets pour faire le rendu
	/// </summary>
	/// <param name="x">Nouvelle taille horizontale</param>
	/// <param name="y">Nouvelle taille verticale</param>
	/// <param name="index">Index de la tuile</param>
	void resize(float x, float y, std::size_t index);

	/// <summary>
	/// Permet de redimensionner la tuile et met à jour les sommets pour faire le rendu
	/// </summary>
	/// <param name="size">Nouvelles dimensions de la tuile</param>
	/// <param name="index">Index de la tuile</param>
	void resize(const sf::Vector2f& size, std::size_t index);

	/// <summary>
	/// Permet de redimensionner la tuile, de changer la règle de texture et met à jour les sommets pour faire le rendu
	/// </summary>
	/// <param name="x">Nouvelle taille horizontale</param>
	/// <param name="y">Nouvelle taille verticale</param>
	/// <param name="textureRule">Nouvelle règle de texture de la tuile</param>
	/// <param name="index">Index de la tuile</param>
	void resize(float x, float y, TextureRule textureRule, std::size_t index);

	/// <summary>
	/// Permet de redimensionner la tuile, de changer la r�gle de texture et met à jour les sommets pour faire le rendu
	/// </summary>
	/// <param name="size">Nouvelles dimensions de la tuile</param>
	/// <param name="textureRule">Nouvelle règle de texture de la tuile</param>
	/// <param name="index">Index de la tuile</param>
	void resize(const sf::Vector2f& size, TextureRule textureRule, std::size_t index);

	/// <summary>
	/// Recharge la texture globale au chemin indiqué et redéfinis le nombre de sous-textures
	/// </summary>
	/// <param name="path">Chemin de la texture</param>
	/// <param name="subTextureCount">Nombre de sous-textures dans la texture globale</param>
	/// <returns>Indique si la texture a pu être chargée</returns>
	bool loadTexture(const std::string& path, int subTextureCount);

	/// <summary>
	/// Recharge la texture gloable à la texture indiquée et redéfinis le nombre de sous-textures
	/// </summary>
	/// <param name="texture">Nouvelle texture globale</param>
	/// <param name="subTextureCount">Nombre de sous-textures dans la nouvelle texture globale</param>
	/// <returns>Indique si la texture a pu être chargée</returns>
	void loadTexture(const sf::Texture& texture, int subTextureCount);

	/// <summary>
	/// Réinitialise la liste générique de sommets (pour le rendu) et la liste générique de tuiles
	/// </summary>
	void resetTiles();

	/// <summary>
	/// Rajoute une tuile à la liste générique de tuiles. Permet de rajouter dans la liste générique une classe dérivée de Tile
	/// </summary>
	/// <param name="tile">Tuile à rajouter</param>
	void add(Tile tile);

	/// <summary>
	/// Construit une nouvelle tuile à la liste générique
	/// </summary>
	/// <param name="size">Taille de la nouvelle tuile</param>
	/// <param name="position">Position de la nouvelle tuile</param>
	/// <param name="numberSubTexture">Numéro de sous-texture</param>
	/// <param name="textureRule">Règle appliquée à la sous-texture. Voir la documentation pour plus de détails</param>
	void add(const sf::Vector2f& size, const sf::Vector2f& position, int numberSubTexture, TextureRule textureRule);
	
	/// <summary>
	/// Construit une nouvelle tuile à la liste générique
	/// </summary>
	/// <param name="size">Taille de la nouvelle tuile</param>
	/// <param name="position">Position de la nouvelle tuile</param>
	/// <param name="numberSubTexture">Numéro de sous-texture</param>
	/// <param name="textureRule">Règle appliquée à la sous-texture. Voir la documentation pour plus de détails</param>
	/// <param name="scale">Échelle appliquée à la texture</param>
	void add(const sf::Vector2f& size, const sf::Vector2f& position, int numberSubTexture, TextureRule textureRule, const sf::Vector2f& scale);
};

void Niveau::reloadVertexes()
{
	m_vertexes.resize(0);
	m_beginTileIndex.resize(m_tiles.size());
	for (int i{0}; i < m_tiles.size(); ++i)
	{
		m_beginTileIndex[i] = m_vertexes.size();
		auto& sommets{ m_tiles[i].vertexes() };
		for (auto& sommet : sommets)
		{
			m_vertexes.push_back(sommet);
		}
	}
}

bool Niveau::continueUpdate(std::size_t index, std::size_t itterator)
{
	if (index >= m_beginTileIndex.size() - 1)
		return itterator < m_vertexes.size();
	else
		return itterator < m_beginTileIndex[index + 1];
}

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

void Niveau::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = &m_texture;

	//states.transform = m_transformations;

	target.draw(&m_vertexes[0], m_vertexes.size(), sf::Triangles, states);
}

void Niveau::move(float offsetX, float offsetY, std::size_t index)
{
	m_tiles[index].move(offsetX, offsetY);
	for (std::size_t i{ m_beginTileIndex[index] }; continueUpdate(index, i); ++i)
	{
		m_vertexes[i].position += sf::Vector2f(offsetX, offsetY);
	}
}

void Niveau::move(const sf::Vector2f& offset, std::size_t index)
{
	m_tiles[index].move(offset);
	for (std::size_t i{ m_beginTileIndex[index] }; continueUpdate(index, i); ++i)
	{
		m_vertexes[i].position += offset;
	}
}

void Niveau::resize(float x, float y, std::size_t index)
{
	m_tiles[index].resize(x, y);
	reloadVertexes();
}

void Niveau::resize(const sf::Vector2f& size, std::size_t index)
{
	m_tiles[index].resize(size);
	reloadVertexes();
}

void Niveau::resize(float x, float y, TextureRule textureRule, std::size_t index)
{
	m_tiles[index].resize(x, y, textureRule);
	reloadVertexes();
}

void Niveau::resize(const sf::Vector2f& size, TextureRule textureRule, std::size_t index)
{
	m_tiles[index].resize(size, textureRule);
	reloadVertexes();
}

bool Niveau::loadTexture(const std::string& path, int subTextureCount)
{
	if (!m_texture.loadFromFile(path))
		return false;
	m_nbTexture = subTextureCount;
	for (Tile& tuile : m_tiles)
	{
		tuile.reloadTexture();
	}
	reloadVertexes();
	return true;
}

void Niveau::loadTexture(const sf::Texture& texture, int subTextureCount)
{
	m_texture = texture;
	m_nbTexture = subTextureCount;
	for (Tile& tuile : m_tiles)
	{
		tuile.reloadTexture();
	}
	reloadVertexes();
}

void Niveau::resetTiles()
{
	m_tiles.resize(0);
	m_vertexes.resize(0);
}

void Niveau::add(Tile tile)
{
	m_tiles.push_back(tile);
	for (const sf::Vertex& sommet : tile.vertexes())
		m_vertexes.push_back(sommet);
}

void Niveau::add(const sf::Vector2f& size, const sf::Vector2f& position, int numberSubTexture, TextureRule textureRule)
{
	Tile tuile{ Tile(m_texture, numberSubTexture, size, position, m_nbTexture, textureRule) };
	m_tiles.push_back(tuile);
	for (const sf::Vertex& sommet : tuile.vertexes())
		m_vertexes.push_back(sommet);
}

void Niveau::add(const sf::Vector2f& size, const sf::Vector2f& position, int numberSubTexture, TextureRule textureRule, const sf::Vector2f& scale)
{
	Tile tuile{ Tile(m_texture, numberSubTexture, size, position, m_nbTexture,textureRule, scale) };
	m_tiles.push_back(tuile);
	for (const sf::Vertex& sommet : tuile.vertexes())
		m_vertexes.push_back(sommet);
}
#endif 
