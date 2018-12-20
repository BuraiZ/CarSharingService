#include "Zipcode.h"

/*
* Constructeur par defaut
*/
Zipcode::Zipcode()
{
	for (unsigned int i = 0; i < 6; i++)
	{
		_zipCodeName[i] = ' ';
		
	}
}

/*
* Constructeur par parametres
* @param zipCodeString le zipcode sous forme de tableau de caractere
*/
Zipcode::Zipcode(char zipCodeString[])
{
	for (unsigned int i = 0; i < 6; i++)
	{
		_zipCodeName[i] = zipCodeString[i];
	}
}

/*
* Constructeur par parametres
* @param zipCode le zipcode
*/
Zipcode::Zipcode(const Zipcode & zipCode)
{
	for (unsigned int i = 0; i < 6; i++)
	{
		_zipCodeName[i] = zipCode.getZipCodeNameAt(i);
	}
}

/*
* Retourner le caractere du zipcode a une position donnnee
* @param index la position du caractere
* @return le caractere a la position donnee
*/
char Zipcode::getZipCodeNameAt(const unsigned int& index) const
{
	return _zipCodeName[index];
}

/*
* Retourner le nom du zipcode
* @return le nom du zipcode
*/
string Zipcode::getZipCodeName() const
{
	string name;
	for (unsigned int i = 0; i < 6; i++)
	{
		name += _zipCodeName[i];
	}
	return name;
}

bool Zipcode::compare(Zipcode zip)
{
	for (unsigned int i = 0; i < 6; i++) 
	{
		if (_zipCodeName[i] != zip._zipCodeName[i])
			return false;
	}
	return true;
}

