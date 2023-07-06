#pragma once
#ifndef PLATEFORMES_H
#define PLATEFORMES_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cassert>
#include <fstream>
#include <string>
#include <sstream>

template <typename T>
inline T parse(const std::string& line)
{
	T valeur;
	std::stringstream conversion{line};
	if (!(conversion >> valeur))
		throw "Unable to parse the line";
	return valeur;
}

/// <summary>
/// Retourne une liste de chaînes de caractères après une filtration
/// </summary>
/// <param name="str">Chaîne de caractères de base</param>
/// <param name="separator">Caractère servant à séparer</param>
inline std::vector<std::string> splitString(std::string str, const char separator)
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

enum class TextureRule {
	repeat_texture,		// La texture sera répétée dans la tuile. Augmenter l'échelle augmentera simplement la taille de la tuile, sans augmenter la taille des textures
	keep_height,		// La texture sera transformée en mettant la hauteur en priorité. Ex: l'échelle de la tuile au début est de 1:1, puis devient 2:1, ça va être 1:1, puisque la hauteur a priorité. Le rectangle de texture n'est pas affecté (si une texture se répétait 2 fois, elle restera répétée 2 fois)
	keep_width,			// La texture sera transformée en mettant la largeur en priorité. Ex: l'échelle de la tuile au début est de 1:1 (100x100), puis devient 2:1. La tuile aura l'échelle 2:2 (200x200), puisque la largeur a priorité. Le rectangle de texture n'est pas affecté (si une texture se répète 2 fois, elle restera répétée 2 fois)
	keep_size,			// La texture sera plus grosse, mais l'objet gardera la même taille
	adjustable_size,	// Aucune tentative n'est faite pour garder la même taille que celle passée auparavant, tout sera transformée selon ce qui est donné, sans garder les proportions
	fill_space			// La texture prend l'entièreté de l'espace sans se répéter. La taille de la case a donc priorité sur son échelle
};

class Tile {
protected:
	const sf::Texture* const m_texture;						// La texture héritée de la classe contenant la tuile
	//sf::Vector2f m_textureSize;							// Indique la taille de la sous-texture utilisée
	//sf::Vector2f m_texturePosition;						// Indique la position de départ de la sous-texture
	sf::Vector2f m_position;								// Donne la position de la case au coin supérieur gauche
	sf::Vector2f m_tileSize;								// Donne la taille de la case
	std::vector<sf::Vertex> m_vertexes;						// L'ensemble des points qui composent l'objet dessinable
	TextureRule m_textureRule;								// Règle régissant le comportement d'une texture lorsque la taille ou l'échelle est changée
	sf::Vector2f m_scale{ 1.f, 1.f };						// Indique le rapport entre la texture et taille demandée (lorsque l'objet est instancié, il équivaut à 1:1 par défaut)
	//const int* const m_textureCount;						// Indique le nombre de textures dans la texture globale
	int m_numberSubTexture;									// Indique le numéro de sous-texture (utile lorsque la texture est changée)
	const std::vector<sf::FloatRect>* const m_subTextures;	// Indique les rectangles de sous-textures (peuvent être asymétriques
	sf::Color m_color;										// Indique la couleur appliquée aux sommets

	void intializeVertexes();
public:

	/// <summary>
	/// Ce constructeur ne devrait jamais être utilisé autrement que pour changer la taille du std::vector
	/// </summary>
	Tile();

	/// <summary>
	/// Constructeur d'une tuile mettant par défaut le ratio de la texture à 1:1 (même si la sous-texture n'est pas carrée)
	/// </summary>
	/// <param name="texture">Référence de la texture utilisée (provenant souvent du niveau)</param>
	/// <param name="noTuileDebutTexture">Numéro de sous-texture utilisé</param>
	/// <param name="desiredSize">Taille de la tuile</param>
	/// <param name="position">Position au coin supérieur gauche de la texture</param>
	/// <param name="subTextureCount">Nombre de sous-textures</param>
	/// <param name="textureRule">Règle appliquée à la texture</param>
	/// <param name="subTextures">Les positions et tailles des sous-textures</param>
	Tile(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize, const sf::Vector2f& position, TextureRule textureRule, const std::vector<sf::FloatRect>& subTextures);

	/// <summary>
	/// Constructeur d'une tuile mettant le ratio de la texture par rapport à la texture à l'échelle demandée
	/// </summary>
	/// <param name="texture">Référence de la texture utilisée (provenant probablement du niveau)</param>
	/// <param name="noTuileDebutTexture">Numéro de sous-texture utilisé</param>
	/// <param name="desiredSize">Taille de la tuile</param>
	/// <param name="position">Position au coin supérieur gauche de la texture</param>
	/// <param name="subTextureCount">Nombre de sous-textures</param>
	/// <param name="textureRule">Règle appliquée à la sous-texture</param>
	/// <param name="scale">Échelle de la sous-texture</param>
	/// <param name="subTextures">Les positions et tailles des sous-textures</param>
	Tile(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize, const sf::Vector2f& position, TextureRule textureRule, const sf::Vector2f& scale, const std::vector<sf::FloatRect>& subTextures);

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
	/// Retourne les coordonnées du coin supérieur gauche de la tuile
	/// </summary>
	sf::Vector2f getPosition() const;

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
	/// Met la tuile au coin supérieur gauche depuis la position rentrée en paramètre
	/// </summary>
	/// <param name="x">Nouvelle position horizontale</param>
	/// <param name="y">Nouvelle position verticale</param>
	void setPosition(float x, float y);

	/// <summary>
	/// Recharge les sommets pour que les tuiles suivent bien la nouvelle texture. Si le numéro de texture dépasse la nouvelle valeur maximale (contenue dans la classe Niveau), la sous-texture correspondra à la dernière disponible
	/// </summary>
	void reloadTexture();

	/// <summary>
	/// Permet d'obtenir l'adresse de l'objet
	/// </summary>
	virtual Tile* getThis();

	/// <summary>
	/// Crée un clone de l'objet pouvant contenir tous les champs de classe dérivée quelconque. Pour pouvoir permettre le clonage de classe dérivée, il faut surcharger cette méthode
	/// </summary>
	virtual std::unique_ptr<Tile> clone() const;

	/// <summary>
	/// Donne la hauteur de la sous-texture (utile si on veut limiter une tuile à une fois sa hauteur)
	/// </summary>
	float subTextureHeight() const;

	/// <summary>
	/// Donne la largeur de la sous-texture (utile si on veut répéter une texture un certain nombre de fois)
	/// </summary>
	float subTextureWidth() const;

	/// <summary>
	/// Donne les dimensions de la sous-texture
	/// </summary>
	sf::Vector2f subTextureSize() const;

	/// <summary>
	/// Change la couleur de la tuile
	/// </summary>
	/// <param name="color">Nouvelle couleur à appliquer</param>
	void changeColor(const sf::Color& color);

	/// <summary>
	/// Réinitialise la couleur de la tuile (met la couleur à blanc)
	/// </summary>
	void resetColor();

	/// <summary>
	/// Retourne la couleur de la tuile
	/// </summary>
	sf::Color getColor() const;

	/// <summary>
	/// Retourne la taille de la tuile
	/// </summary>
	sf::Vector2f getSize() const;

	/// <summary>
	/// Retourne l'index de la sous-texture
	/// </summary>
	int subTextureIndex() const;
};

inline void Tile::intializeVertexes()
{
	sf::Vector2f coinGaucheSommet;
	m_vertexes.resize(6);
	std::size_t indexSommet{ 0 };
	sf::Vector2f textureSize{m_subTextures->at(m_numberSubTexture).width, m_subTextures->at(m_numberSubTexture).height};
	sf::Vector2f texturePosition{m_subTextures->at(m_numberSubTexture).left, m_subTextures->at(m_numberSubTexture).top};

	if (m_textureRule == TextureRule::fill_space)
	{
		m_scale.x = m_tileSize.x / textureSize.x;
		m_scale.y = m_tileSize.y / textureSize.y;
	}

	while (coinGaucheSommet.y < m_tileSize.y)
	{
		coinGaucheSommet.x = 0.f;
		for (; coinGaucheSommet.x < m_tileSize.x; coinGaucheSommet.x += textureSize.x * m_scale.x,
			indexSommet = m_vertexes.size(), m_vertexes.resize(m_vertexes.size() + 6))
		{
			m_vertexes[indexSommet].position = coinGaucheSommet;
			m_vertexes[indexSommet].texCoords = sf::Vector2f(texturePosition.x, 0.f);

			if (coinGaucheSommet.y + textureSize.y * m_scale.y > m_tileSize.y)
			{
				m_vertexes[indexSommet + 1].position = coinGaucheSommet + sf::Vector2f(0.f, m_tileSize.y - coinGaucheSommet.y);
				m_vertexes[indexSommet + 1].texCoords = sf::Vector2f(texturePosition.x, m_tileSize.y - coinGaucheSommet.y);
			}
			else
			{
				m_vertexes[indexSommet + 1].position = coinGaucheSommet + sf::Vector2f(0.f, textureSize.y * m_scale.y);
				m_vertexes[indexSommet + 1].texCoords = sf::Vector2f(texturePosition.x, textureSize.y);
			}

			if (coinGaucheSommet.x + textureSize.x * m_scale.x > m_tileSize.x)
			{
				m_vertexes[indexSommet + 2].position = coinGaucheSommet + sf::Vector2f(m_tileSize.x - coinGaucheSommet.x, 0.f);
				m_vertexes[indexSommet + 2].texCoords = sf::Vector2f(texturePosition.x + (m_tileSize.x - coinGaucheSommet.x), 0.f);
			}
			else
			{
				m_vertexes[indexSommet + 2].position = coinGaucheSommet + sf::Vector2f(textureSize.x * m_scale.x, 0.f);
				m_vertexes[indexSommet + 2].texCoords = sf::Vector2f(texturePosition.x + textureSize.x - 1.f, 0.f);
			}

			m_vertexes[indexSommet + 3] = m_vertexes[indexSommet + 1];
			m_vertexes[indexSommet + 4] = m_vertexes[indexSommet + 2];

			m_vertexes[indexSommet + 5].position = sf::Vector2f(m_vertexes[indexSommet + 4].position.x, m_vertexes[indexSommet + 3].position.y);
			m_vertexes[indexSommet + 5].texCoords = sf::Vector2f(m_vertexes[indexSommet + 4].texCoords.x, m_vertexes[indexSommet + 3].texCoords.y);

		}
		coinGaucheSommet.y += textureSize.y * m_scale.y;
	}
	m_vertexes.resize(m_vertexes.size() - 6);
	for (sf::Vertex& point : m_vertexes)
	{
		point.position += m_position;
		point.color = sf::Color::White;
	}
}

inline Tile::Tile() : m_texture{ nullptr }, m_numberSubTexture{ 0 }, m_textureRule{ TextureRule::repeat_texture }, m_subTextures{ nullptr }
{}

inline Tile::Tile(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize,
	const sf::Vector2f& position, TextureRule textureRule, const sf::Vector2f& scale, const	std::vector<sf::FloatRect>& subTextures) :
	m_texture{ &texture }, m_numberSubTexture{ noTuileDebutTexture }, //m_texturePosition{ texture.getSize().x / static_cast<float>(subTextureCount) * noTuileDebutTexture ,0.f },
	//m_textureSize{ static_cast<float>(texture.getSize().x / subTextureCount) , static_cast<float>(texture.getSize().y) },
	m_textureRule{ textureRule }, m_scale{ scale }, m_tileSize{ desiredSize }, m_subTextures{ &subTextures }, m_position{ position }
{
	intializeVertexes();
}


inline Tile::Tile(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize,
	const sf::Vector2f& position, TextureRule textureRule, const std::vector<sf::FloatRect>& subTextures) :
	m_texture{ &texture }, m_numberSubTexture{ noTuileDebutTexture }, //m_texturePosition{ texture.getSize().x / static_cast<float>(subTextureCount) * noTuileDebutTexture , 0.f },
	//m_textureSize{ static_cast<float>(texture.getSize().x / subTextureCount) , static_cast<float>(texture.getSize().y) },
	m_textureRule{ textureRule }, m_scale{ 1.f, 1.f }, m_tileSize{ desiredSize }, m_subTextures{ &subTextures }, m_position{ position }
{
	intializeVertexes();
}

inline float Tile::height() const
{
	return m_tileSize.y;
}

inline float Tile::width() const
{
	return m_tileSize.x;
}

inline const std::vector<sf::Vertex>& Tile::vertexes() const
{
	return m_vertexes;
}

inline sf::Vector2f Tile::topLeftCorner() const
{
	return m_position;
}

inline sf::Vector2f Tile::topRightCorner() const
{
	return m_position + sf::Vector2f(this->width(), 0.f);
}

inline sf::Vector2f Tile::bottomLeftCorner() const
{
	return m_position + sf::Vector2f(0.f, this->height());
}

inline sf::Vector2f Tile::bottomRightCorner() const
{
	return m_position + this->m_tileSize;
}

inline sf::Vector2f Tile::getPosition() const
{
	return topLeftCorner();
}

inline void Tile::setScale(const sf::Vector2f& scale)
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

inline void Tile::setScale(float scale)
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

inline void Tile::setScale(float x, float y)
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

inline void Tile::setScale(const sf::Vector2f& scale, TextureRule textureRule)
{
	m_textureRule = textureRule;
	setScale(scale);
}

inline void Tile::setScale(float scale, TextureRule textureRule)
{
	m_textureRule = textureRule;
	setScale(scale);
}

inline void Tile::setScale(float x, float y, TextureRule textureRule)
{
	m_textureRule = textureRule;
	setScale(x, y);
}

inline void Tile::resize(const sf::Vector2f& size)
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

inline void Tile::resize(float x, float y)
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
		scale = x / m_tileSize.x;
		m_tileSize *= scale;
		m_scale *= scale;
		break;
	case TextureRule::keep_size:
		break;
	}
	intializeVertexes();
}

inline void Tile::resize(const sf::Vector2f& size, TextureRule textureRule)
{
	m_textureRule = textureRule;
	resize(size);
}

inline void Tile::resize(float x, float y, TextureRule textureRule)
{
	m_textureRule = textureRule;
	resize(x, y);
}

inline void Tile::changeTextureRect(int numberSubTexture)
{
	if (numberSubTexture >= 0 && numberSubTexture < m_subTextures->size())
	{
		m_numberSubTexture = numberSubTexture;
		//m_texturePosition = sf::Vector2f(m_textureSize.x / *m_textureCount * m_numberSubTexture, 0.f);
		intializeVertexes();
	}
}

inline void Tile::setTextureRule(TextureRule textureRule)
{
	m_textureRule = textureRule;
}

inline TextureRule Tile::getTextureRule()
{
	return m_textureRule;
}

inline void Tile::move(const sf::Vector2f& offset)
{
	for (auto& sommet : m_vertexes)
		sommet.position += offset;
}

inline void Tile::move(float offsetX, float offsetY)
{
	sf::Vector2f offset{ offsetX, offsetY };
	for (auto& sommet : m_vertexes)
		sommet.position += offset;
}

inline void Tile::setPosition(const sf::Vector2f& position)
{
	//m_position = position;
	// intializeVertexes();
	sf::Vector2f deplacement{position - m_position};
	m_position = position;
	for (auto& sommet : m_vertexes)
		sommet.position += deplacement;
}

inline void Tile::setPosition(float x, float y)
{
	sf::Vector2f deplacement{sf::Vector2f(x, y) - m_position};
	m_position = sf::Vector2f(x, y);
	for (auto& sommet : m_vertexes)
		sommet.position += deplacement;
	//m_position = sf::Vector2f(x, y);
	//intializeVertexes();
}

inline void Tile::reloadTexture()
{
	if (m_numberSubTexture >= m_subTextures->size())
		m_numberSubTexture = m_subTextures->size() - 1;
	changeTextureRect(m_numberSubTexture);
}

inline Tile* Tile::getThis()
{
	return this;
}

inline std::unique_ptr<Tile> Tile::clone() const
{
	return std::make_unique<Tile>(*this);
}

inline float Tile::subTextureHeight() const
{
	return m_subTextures->at(m_numberSubTexture).height;
}

inline float Tile::subTextureWidth() const
{
	return m_subTextures->at(m_numberSubTexture).width;
}

inline sf::Vector2f Tile::subTextureSize() const
{
	return sf::Vector2f(m_subTextures->at(m_numberSubTexture).width, m_subTextures->at(m_numberSubTexture).height);
}

inline void Tile::changeColor(const sf::Color& color)
{
	m_color = color;
	for (auto& sommet : m_vertexes)
		sommet.color = m_color;
}

inline void Tile::resetColor()
{
	m_color = sf::Color(0xFFFFFFFF);
	for (auto& sommet : m_vertexes)
		sommet.color = m_color;
}

inline sf::Color Tile::getColor() const
{
	return m_color;
}

inline sf::Vector2f Tile::getSize() const
{
	return m_tileSize;
}

inline int Tile::subTextureIndex() const
{
	return m_numberSubTexture;
}

class Niveau : public sf::Drawable {
private:
	std::vector<std::unique_ptr<Tile>> m_tiles;
	sf::Texture m_texture;						// Texture utilisée pour toutes les cases
	//std::size_t m_nbTexture;					// Indique le nombre de sous-textures dans le fichier
	std::vector<std::size_t> m_beginTileIndex;	// Indique l'index de commencement des sommets de chaque tuile
	std::vector<sf::Vertex> m_vertexes;			// Ensemble des sommets copiés par valeur des tuiles. À n'utiliser que pour la méthode draw et ce qui aide à faire le rendu
	std::vector<sf::FloatRect> m_subTextures;	// Indique les sous-textures possibles

	/// <summary>
	/// Indique si on continue de mettre à jour les sommets
	/// </summary>
	/// <param name="index">Index de la tuile</param>
	/// <param name="itterator">Index dans la liste générique de sommets</param>
	bool continueUpdate(std::size_t index, std::size_t itterator);

public:

	/// <summary>
	/// Recharge la liste générique de sommets et l'index de départ des tuiles par rapport aux sommets. Utile lorsque plusieurs tuiles ont été modifiées directement
	/// </summary>
	void reloadVertexes();

	/// <summary>
	/// Charge en mémoire la texture désirée et met le compteur de cases à 0
	/// </summary>
	/// <param name="pPathTexture"></param>
	Niveau(const std::string& pPathTexture, std::size_t pNbTextures);

	/// <summary>
	/// Constructeur par défaut. Utile lors de la création d'une structure contenant un niveau
	/// </summary>
	Niveau();

	/// <summary>
	/// Retourne une référence de la case à l'index spécifié
	/// </summary>
	/// <param name="index">Index de la tuile</param>
	auto& operator[](int index);

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
	/// Recharge la texture globale au chemin indiqué, redéfinis le nombre de textures et les rectangles de sous-textures
	/// </summary>
	/// <param name="path">Chemin de la texture</param>
	/// <param name="subTextureCount">Nombre de sous-textures</param>
	/// <param name="subTextures">Rectangles définissant les sous-textures</param>
	bool loadTexture(const std::string& path, std::vector<sf::FloatRect>& subTextures);

	/// <summary>
	/// Recharge la texture globale au chemin indiqué
	/// </summary>
	/// <param name="path">Chemin de la texture</param>
	/// <param name="subTexturePath">Chemin de la règle régissant les sous-textures. Le fichier doit être ordonné de même : left, top, width, height</param>
	bool loadTexture(const std::string& path, const std::string& subTexturePath);

	/// <summary>
	/// Recharge la texture gloable à la texture indiquée et redéfinis le nombre de sous-textures
	/// </summary>
	/// <param name="texture">Nouvelle texture globale</param>
	/// <param name="subTextureCount">Nombre de sous-textures dans la nouvelle texture globale</param>
	/// <returns>Indique si la texture a pu être chargée</returns>
	void loadTexture(const sf::Texture& texture, int subTextureCount);

	/// <summary>
	/// Recharge la texture globale à la texture indiquée, redéfinis le nombre de sous-textures et les rectangles de sous-textures
	/// </summary>
	/// <param name="texture">Nouvelle texture globale</param>
	/// <param name="subTextureCount">Nombre de sous textures dans la nouvelle texture globale</param>
	/// <param name="subTextures">Rectangles définissant les sous-textures</param>
	void loadTexture(const sf::Texture& texture, std::vector<sf::FloatRect>& subTextures);

	/// <summary>
	/// Recharge la texture depuis une texture déjà en mémoire et lit les dimensions de chaque sous-texture
	/// </summary>
	/// <param name="texture">Référence de texture utilisée</param>
	/// <param name="subTexturePath">Chemin du fichier régissant les sous-textures</param>
	void loadTexture(const sf::Texture& texture, const std::string& subTexturePath);

	/// <summary>
	/// Réinitialise la liste générique de sommets (pour le rendu) et la liste générique de tuiles
	/// </summary>
	void resetTiles();

	/// <summary>
	/// Rajoute une tuile à la liste générique de tuiles. Permet de rajouter dans la liste générique une classe dérivée de Tile
	/// </summary>
	/// <param name="tile">Tuile à rajouter</param>
	void add(const Tile& tile);

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

	/// <summary>
	/// Retourne un pointeur d'un objet d'une classe spécifiée
	/// </summary>
	/// <typeparam name="T">Type de l'objet dérivé</typeparam>
	/// <param name="index">Index de l'objet à vérifier</param>
	/// <returns>Si null, l'objet à l'index spécifié n'est pas de ce type</returns>
	template <class T>
	T* derivedPointer(int index);

	/// <summary>
	/// Retourne une référence constante de la texture utilisée dans le niveau
	/// </summary>
	const sf::Texture& getTexture() const;

	/// <summary>
	/// Retourne une référence de la liste générique. Ne devrait être utilisé que pour faire des objets dérivés de Tile
	/// </summary>
	/// <returns></returns>
	const std::vector<sf::FloatRect>& getSubTextures() const;

	/// <summary>
	/// Change le rectangle de texture (par exemple, pour faire de l'animation) et met à jour les sommets. Il est possible que la méthode ne soit pas optimisée
	/// </summary>
	/// <param name="numberTexture">Numéro de la nouvelle sous-texture</param>
	/// <param name="index">Index de la tuile</param>
	void changeTextureRect(int numberTexture, int index);

	/// <summary>
	/// Change la couleur des points d'une tuile
	/// </summary>
	/// <param name="color">Nouvelle couleur de la tuile (pour appliquer un effet de couleur)</param>
	/// <param name="index">Index de la tuile</param>
	void changeColor(const sf::Color& color, int index);

	/// <summary>
	/// Réinitialise la couleur des points d'une tuile
	/// </summary>
	/// <param name="index">Index de la tuile</param>
	void resetColor(int index);

	/// <summary>
	/// Retourne la couleur d'une tuile
	/// </summary>
	/// <param name="index">Index de la tuile</param>
	sf::Color getColor(int index) const;

	/// <summary>
	/// Retourne le rectangle de sous-texture à l'index indiqué
	/// </summary>
	/// <param name="index">Index de la sous-texture</param>
	sf::FloatRect getSubTexture(int index) const;

	/// <summary>
	/// Retourne la taille de la sous-texture à l'index indiqué
	/// </summary>
	/// <param name="index">Index de la sous-texture</param>
	sf::Vector2f getSubTextureSize(int index) const;

	/// <summary>
	/// Retourne le nombre de tuiles dans le niveau
	/// </summary>
	/// <returns></returns>
	std::size_t size() const;
};

inline void Niveau::reloadVertexes()
{
	m_vertexes.resize(0);
	m_beginTileIndex.resize(m_tiles.size());
	for (int i{ 0 }; i < m_tiles.size(); ++i)
	{
		m_beginTileIndex[i] = m_vertexes.size();
		auto& sommets{ m_tiles[i]->vertexes() };
		for (auto& sommet : sommets)
		{
			m_vertexes.push_back(sommet);
		}
	}
}

inline bool Niveau::continueUpdate(std::size_t index, std::size_t itterator)
{
	if (index >= m_beginTileIndex.size() - 1)
		return itterator < m_vertexes.size();
	else
		return itterator < m_beginTileIndex[index + 1];
}

inline Niveau::Niveau()// : m_nbTexture{ 0 }
{

}

inline Niveau::Niveau(const std::string& pPathTexture, std::size_t pNbTextures) //:
//m_nbTexture{ pNbTextures }
{
	if (!m_texture.loadFromFile(pPathTexture))
		PLOGE << "Unable to load the following texture file: " << pPathTexture;
	m_tiles.resize(0);
	m_subTextures.resize(pNbTextures);
	for (int i{ 0 }; i < m_subTextures.size(); ++i)
	{
		m_subTextures[i].height = m_texture.getSize().y;
		m_subTextures[i].width = m_texture.getSize().x / pNbTextures;
		m_subTextures[i].top = 0.f;
		m_subTextures[i].left = m_subTextures[i].width * i;
	}
}

inline auto& Niveau::operator[](int index)
{
	return m_tiles[index];
}

inline void Niveau::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = &m_texture;

	//states.transform = m_transformations;

	if (m_vertexes.size() > 0)
		target.draw(&m_vertexes[0], m_vertexes.size(), sf::Triangles, states);
}

inline void Niveau::move(float offsetX, float offsetY, std::size_t index)
{
	m_tiles[index]->move(offsetX, offsetY);
	for (std::size_t i{ m_beginTileIndex[index] }; continueUpdate(index, i); ++i)
	{
		m_vertexes[i].position += sf::Vector2f(offsetX, offsetY);
	}
}

inline void Niveau::move(const sf::Vector2f& offset, std::size_t index)
{
	m_tiles[index]->move(offset);
	for (std::size_t i{ m_beginTileIndex[index] }; continueUpdate(index, i); ++i)
	{
		m_vertexes[i].position += offset;
	}
}

inline void Niveau::resize(float x, float y, std::size_t index)
{
	m_tiles[index]->resize(x, y);
	reloadVertexes();
}

inline void Niveau::resize(const sf::Vector2f& size, std::size_t index)
{
	m_tiles[index]->resize(size);
	reloadVertexes();
}

inline void Niveau::resize(float x, float y, TextureRule textureRule, std::size_t index)
{
	m_tiles[index]->resize(x, y, textureRule);
	reloadVertexes();
}

inline void Niveau::resize(const sf::Vector2f& size, TextureRule textureRule, std::size_t index)
{
	m_tiles[index]->resize(size, textureRule);
	reloadVertexes();
}

inline bool Niveau::loadTexture(const std::string& path, int subTextureCount)
{
	if (!m_texture.loadFromFile(path))
		return false;
	//m_nbTexture = subTextureCount;
	m_subTextures.resize(subTextureCount);
	for (int i{ 0 }; i < m_subTextures.size(); ++i)
	{
		m_subTextures[i].height = m_texture.getSize().y;
		m_subTextures[i].width = m_texture.getSize().x / subTextureCount;
		m_subTextures[i].left = m_texture.getSize().x / subTextureCount * i;
		m_subTextures[i].top = m_texture.getSize().y;
	}

	for (auto& tuile : m_tiles)
	{
		tuile->reloadTexture();
	}
	reloadVertexes();
	return true;
}

inline bool Niveau::loadTexture(const std::string& path, std::vector<sf::FloatRect>& subTextures)
{
	if (!m_texture.loadFromFile(path))
		return false;
	//m_nbTexture = subTextures.size();
	/*m_subTextures.resize(subTextureCount);
	for (int i{ 0 }; i < m_subTextures.size(); ++i)
	{
		m_subTextures[i].height = m_texture.getSize().y;
		m_subTextures[i].width = m_texture.getSize().x / subTextureCount;
		m_subTextures[i].left = m_texture.getSize().x / subTextureCount * i;
		m_subTextures[i].top = m_texture.getSize().y;
	}*/

	m_subTextures = subTextures;
	for (auto& tuile : m_tiles)
	{
		tuile->reloadTexture();
	}
	reloadVertexes();
	return true;
}

inline bool Niveau::loadTexture(const std::string& path, const std::string& subTexturePath)
{
	if (!m_texture.loadFromFile(path))
		return false;
	std::fstream fichier{subTexturePath};
	std::vector<std::string> lignes;
	for (int i{ 0 }; fichier; ++i)
	{
		lignes.push_back("");
		std::getline(fichier, lignes[i]);
	}
	if (lignes.empty())
		throw "No data was within the file";
	m_subTextures.resize(lignes.size());
	for (int i{ 0 }; i < m_subTextures.size(); ++i)
	{
		std::vector<std::string> donnees{splitString(lignes[i], ',') };
		m_subTextures[i].left = parse<float>(donnees[0]);
		m_subTextures[i].top = parse<float>(donnees[1]);
		m_subTextures[i].width = parse<float>(donnees[2]);
		m_subTextures[i].height = parse<float>(donnees[3]);
	}
	return true;
}

inline void Niveau::loadTexture(const sf::Texture& texture, int subTextureCount)
{
	m_texture = texture;
	//m_nbTexture = subTextureCount;
	m_subTextures.resize(subTextureCount);
	for (int i{ 0 }; i < m_subTextures.size(); ++i)
	{
		m_subTextures[i].height = m_texture.getSize().y;
		m_subTextures[i].width = m_texture.getSize().x / subTextureCount;
		m_subTextures[i].left = m_texture.getSize().x / subTextureCount * i;
		m_subTextures[i].top = m_texture.getSize().y;
	}
	for (auto& tuile : m_tiles)
	{
		tuile->reloadTexture();
	}
	reloadVertexes();
}

inline void Niveau::loadTexture(const sf::Texture& texture, std::vector<sf::FloatRect>& subTextures)
{
	m_texture = texture;
	//m_nbTexture = subTextures.size();
	m_subTextures = subTextures;
}

inline void Niveau::loadTexture(const sf::Texture& texture, const std::string& subTexturePath)
{
	m_texture = texture;
	std::fstream fichier{subTexturePath};
	std::vector<std::string> lignes;
	for (int i{ 0 }; fichier; ++i)
	{
		lignes.push_back("");
		std::getline(fichier, lignes[i]);
	}
	if (lignes[lignes.size() - 1] == "")
		lignes.pop_back();
	if (lignes.empty())
		throw "No data was within the file";
	m_subTextures.resize(lignes.size());
	for (int i{ 0 }; i < m_subTextures.size(); ++i)
	{
		std::vector<std::string> donnees{splitString(lignes[i], ',') };
		m_subTextures[i].left = parse<float>(donnees[0]);
		m_subTextures[i].top = parse<float>(donnees[1]);
		m_subTextures[i].width = parse<float>(donnees[2]);
		m_subTextures[i].height = parse<float>(donnees[3]);
	}
	//m_nbTexture = m_subTextures.size();
}

inline void Niveau::resetTiles()
{
	m_tiles.resize(0);
	m_vertexes.resize(0);
}

inline void Niveau::add(const Tile& tile)
{
	m_tiles.push_back(tile.clone());
	m_beginTileIndex.push_back(m_vertexes.size());
	for (const sf::Vertex& sommet : tile.vertexes())
		m_vertexes.push_back(sommet);
}

inline void Niveau::add(const sf::Vector2f& size, const sf::Vector2f& position, int numberSubTexture, TextureRule textureRule)
{
	Tile tuile{ Tile(m_texture, numberSubTexture, size, position, textureRule, m_subTextures) };
	m_tiles.push_back(std::make_unique<Tile>(Tile(tuile)));
	m_beginTileIndex.push_back(m_vertexes.size());
	for (const sf::Vertex& sommet : tuile.vertexes())
		m_vertexes.push_back(sommet);
}

inline void Niveau::add(const sf::Vector2f& size, const sf::Vector2f& position, int numberSubTexture, TextureRule textureRule, const sf::Vector2f& scale)
{
	Tile tuile{ Tile(m_texture, numberSubTexture, size, position, textureRule, scale, m_subTextures) };
	m_tiles.push_back(std::make_unique<Tile>(Tile(tuile)));
	m_beginTileIndex.push_back(m_vertexes.size());
	for (const sf::Vertex& sommet : tuile.vertexes())
		m_vertexes.push_back(sommet);
}

template <class T>
inline T* Niveau::derivedPointer(int index)
{
	return dynamic_cast<T*>(m_tiles[index].get());
}

inline const sf::Texture& Niveau::getTexture() const
{
	return m_texture;
}

//inline const int& Niveau::getSubTextureCount() const
//{
//	return m_nbTexture;
//}

inline const std::vector<sf::FloatRect>& Niveau::getSubTextures() const
{
	return m_subTextures;
}

inline void Niveau::changeTextureRect(int numberTexture, int index)
{
	m_tiles[index]->changeTextureRect(numberTexture);
	reloadVertexes();
}

inline void Niveau::changeColor(const sf::Color& color, int index)
{
	m_tiles[index]->changeColor(color);
	for (std::size_t i{ m_beginTileIndex[index] }; continueUpdate(static_cast<std::size_t>(index), i); ++i)
		m_vertexes[i].color = color;
}

inline void Niveau::resetColor(int index)
{
	m_tiles[index]->resetColor();
	for (std::size_t i{ m_beginTileIndex[index] }; continueUpdate(static_cast<std::size_t>(index), i); ++i)
		m_vertexes[i].color = sf::Color(0xFFFFFFFF);
}

inline sf::Color Niveau::getColor(int index) const
{
	return m_tiles[index]->getColor();
}

inline sf::FloatRect Niveau::getSubTexture(int index) const
{
	return m_subTextures[index];
}

inline sf::Vector2f Niveau::getSubTextureSize(int index) const
{
	return sf::Vector2f(m_subTextures[index].width, m_subTextures[index].height);
}

inline std::size_t Niveau::size() const
{
	return m_tiles.size();
}
#endif 
