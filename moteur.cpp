#include "ressources.h"
#include "moteur.h"
//#include "MoteurMenu.h"
//#include "Checkpoint.h"

void deplacementAChoisir(touchesActives& touchesActionnees, int& index, int& indexMax, bool& peutDeplacer, ObjetADessiner& spritesEtFond, ensembleTouches& pTouches, bool& threadsActifs, sf::Event& pEvenement, Moteur& moteur, std::bitset<3>& touchesNonRepetables)
{
	std::unique_ptr<MoteurMenu> moteurMenu{ new (std::nothrow) MoteurMenu{touchesActionnees,index,indexMax,peutDeplacer,spritesEtFond,pTouches,moteur,touchesNonRepetables,threadsActifs,pEvenement} };
	std::unique_ptr<MoteurPhysique> moteurJeu{ new (std::nothrow) MoteurPhysique{touchesActionnees,spritesEtFond,peutDeplacer,threadsActifs,moteur,touchesNonRepetables,*moteurMenu} };
	if (!moteurMenu)
	{
		PLOGE << "User interface object is null";
		std::abort();
	}
	if (!moteurJeu)
	{
		PLOGE << "Graphic engine object is null";
		std::abort();
	}
	while (threadsActifs)
	{
		if (peutDeplacer)
		{
			moteurJeu->deplacement();
		}
		else
		{
			moteurMenu->deplacementMenus();
			//deplacementMenus(touchesActionnees, index, indexMax, peutDeplacer, spritesEtFond, ptrFcnFichier, pTouches, threadsActifs, pEvenement, moteur, touchesNonRepetables);
		}
	}
}

