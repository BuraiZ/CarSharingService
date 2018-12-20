#include "Vehicule.h"

/*
* Constructeur par defaut
*/
Vehicule::Vehicule()
{
	_disponible = true;
}

/*
* Constructeur par parametres
* @param positionActuelle le zipcode de la position actuelle
* @ param positionFinale le zipcode de la position de destination
*/
Vehicule::Vehicule(Zipcode positionActuelle, Zipcode positionFinale, int id):_positionActuelle(positionActuelle), _positionFinale(positionFinale), _id(id)
{
	_disponible = true;
}

/*
* Retourner la disponibilite du vehicule
* @return la disponibilite du vehicule
*/
bool Vehicule::getDisponibilite() const
{
	return _disponible;
}

/*
* Modifier la disponibilite du vehicule
* @param disponible la disponibilite du vehicule
*/
void Vehicule::setDisponibilite(const bool & disponible)
{
	_disponible = disponible;
}

/*
* Retourner la position actuelle du vehicule
* @return la position actuelle du vehicule
*/
Zipcode Vehicule::getPositionActuelle() const
{
	return _positionActuelle;
}

/*
* Retourner la position finale du vehicule
* @return la position finale du vehicule
*/
Zipcode Vehicule::getPositionFinale() const
{
	return _positionFinale;
}

/*
* MNodifier la position finale du vehicule
* @param positionFinale la position finale du vehicule
*/
void Vehicule::setPositionFinale(const Zipcode & positionFinale)
{
	_positionFinale = positionFinale;
}

/*
* Modifier la position actuelle du vehicule
* @param positionActuelle la position actuelle du vehicule
*/
void Vehicule::setPositionActuelle(const Zipcode & positionActuelle)
{
	_positionActuelle = positionActuelle;
}

void Vehicule::incrementTrajetClient()
{
	_nbTrajetClient++;
}

void Vehicule::incrementTrajetVide()
{
	_nbTrajetVide++;
}

int Vehicule::getNbTrajetClient()
{
	return _nbTrajetClient;
}

int Vehicule::getNbTrajetVide()
{
	return _nbTrajetVide;
}

int Vehicule::getId()
{
	return _id;
}
