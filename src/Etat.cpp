#include "Automate.h"

/*
* Constructeur par defaut
*/
Etat::Etat()
{
}

/*
* Destructeur
*/
Etat::~Etat()
{
	destroy(this);
}

/*
* La destruction de l'etat et tous les etats qui lui derivent
* @param automate l'etat a detruire
*/
void Etat::destroy(Etat * automate)
{
	for (unsigned int i = 0; i < automate->_nextState.size(); i++) {
		destroy(automate->_nextState.at(i));
	}
	automate = nullptr;
	delete automate;
}

/*
* Constructeur par parametres
* @param value la valeur de l'etat
*/
Etat::Etat(char value) : _value(value)
{ 
}

/*
* Ajouter un etat qui derive de l'etat present
* @param automate etat a ajouter
*/
void Etat::addChild(Etat * automate)
{
	_nextState.push_back(automate);
}

/*
* Indiquer si l'etat present possede des etats qui lui derivent ou pas
* @return boolean
*/
bool Etat::isEmpty()
{
	return _nextState.empty();
}

/*
* Modifier la valeur de l'etat present
* @param value la valeur de l'etat present
*/
void Etat::setValue(char value)
{
	_value = value;
}

/*
* Retourner la valeur de l'etat present
* @return la valeur de l'etat present
*/
char Etat::getValue()
{
	return _value;
}

/*
* Retourner l'etat de l'arbre qui a la valeur donnee
* @param value la valeur recherchee
* @return l'etat qui a la valeur recherchee
*/
Etat * Etat::contains(char value)
{
	for (unsigned int i = 0; i < _nextState.size(); i++) {
		if (_nextState.at(i)->getValue() == value)
		{
			return _nextState.at(i);
		}
	}
	return nullptr;
}

/*
* Indiquer s'il existe un etat de l'arbre qui possede le zipcode donne
* @param zip le zipcode recherche
* @return boolean
*/
bool Etat::search(Zipcode zip)
{
	if (zip.getZipCodeNameAt(0) != _value)
		return false;
	Etat* current = this;
	for (unsigned int i = 1; i < 6; i++) {
		char c = zip.getZipCodeNameAt(i);
		current = current->contains(c);
		if (current == nullptr)
			return false;
	}
	return true;
}

Zipcode Etat::generAdresse()
{
	char zip[6];
	Etat* current = this;
	zip[0] = current->_value;
	for (unsigned int i = 1; i < 6; i++) {
		current = current->_nextState.at(0);
		zip[i] = current->_value;
	}
	return Zipcode(zip);
}

