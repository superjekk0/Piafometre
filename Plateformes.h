#ifndef PLATEFORMES_H
#define PLATEFORMES_H

#include "Jeu.h"

/// TODO : Ajouter l'h�ritage des classe SFML de base
class Niveau {
private:
/// TODO : Membres priv�s � rajouter. Inclue le std::vector de CaseOptimisee et la texture globale


	public:
	/// TODO : Constructeur � un param�tre prenant un chemin de fichier

	/// TODO : Surcharger la m�thode draw avec la classe Niveau comme param�tre
	/// TODO : Ajouter la surcharge de l'op�rateur [] pour acc�der � une case

};

/// TODO : Ajouter l'h�ritage des classe SFML de base
class CaseOptimisee {
private:
	///TODO : Inclure les membres priv�s tels que la texture globale, le carr� de texture, les transformations appliqu�es � la texture, la taille d�sir�e
public:
	/// TODO : Constructeur � trois param�tres. La texture, puis le rectangle de texture, puis les coordonn�es au coin sup�rieur gauche de la case.
};

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

#endif 
