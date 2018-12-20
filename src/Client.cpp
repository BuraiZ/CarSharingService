#include "Client.h"

/*
* Constructeur par defaut
*/
Client::Client()
{
}

/*
* Constructeur par parametres
* @param pointDepart le zipcode de la position de depart
* @param destination le zipcode de la position de destination
*/
Client::Client(const Zipcode & pointDepart, const Zipcode & destination)
	:_pointDepart(pointDepart), _destination(destination)
{
}

/*
* Afficher les informations du client
*/
void Client::afficherInformations() const
{
	cout << "Voici le point de depart du client: " << endl << endl;
	cout << getPointDepart().getZipCodeName() << " ";
	

	cout << endl << endl;

	cout << "Voici la destination du client: " << endl << endl;

	cout << getDestination().getZipCodeName() << " ";
	

	cout << endl << endl;



}

/*
* Retourner le zipcode de la position de depart du client
* @return le caractere a la position donnee
*/
Zipcode Client::getPointDepart() const
{
	return _pointDepart;
}

/*
* Retourner le zipcode de la position de destination du client
* @return le caractere a la position donnee
*/
Zipcode Client::getDestination() const
{
	return _destination;
}
