#ifndef PLATEFORMES_H
#define PLATEFORMES_H

#include "Jeu.h"

/// TODO : Ajouter l'héritage des classe SFML de base
class Niveau {
private:
/// TODO : Membres privés à rajouter. Inclue le std::vector de CaseOptimisee et la texture globale


	public:
	/// TODO : Constructeur à un paramètre prenant un chemin de fichier

	/// TODO : Surcharger la méthode draw avec la classe Niveau comme paramètre
	/// TODO : Ajouter la surcharge de l'opérateur [] pour accéder à une case

};

/// TODO : Ajouter l'héritage des classe SFML de base
class CaseOptimisee {
private:
	///TODO : Inclure les membres privés tels que la texture globale, le carré de texture, les transformations appliquées à la texture, la taille désirée
public:
	/// TODO : Constructeur à trois paramètres. La texture, puis le rectangle de texture, puis les coordonnées au coin supérieur gauche de la case.
};

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

#endif 
