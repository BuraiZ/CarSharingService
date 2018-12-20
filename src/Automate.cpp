#include "Automate.h"

/*
* Constructeur par defaut
*/
Automate::Automate()
{
}

/*
* Constructeur par parametres
* @param automate l'etat racine de l'automate
*/
Automate::Automate(Etat * automate):_racine(automate)
{
}

/*
* Destructeur
*/
Automate::~Automate()
{
	delete _racine;
}

/*
* Modifier le vecteur de vehicule
* @param vec le vecteur de vehicule
*/
void Automate::setVecVehicule(vector<Vehicule*> vec)
{
	_vecVehicule = vec;
}

/*
* Ajouter un vehicule dans le vecteur
* @param vehicule le vehicule a ajouter
*/
void Automate::addVehicule(Vehicule * vehicule)
{
	_vecVehicule.push_back(vehicule);
}

/*
* Enlever un vehicule du vecteur (de la zone)
* @param vehicule le vehicule a enlever
*/
void Automate::eraseVehicule(Vehicule * vehicule)
{
	for (unsigned int i = 0; i < _vecVehicule.size(); i++) {
		if (_vecVehicule.at(i) == vehicule) {
			_vecVehicule.erase(_vecVehicule.begin() + i);
			return;
		}
	}
}

/*
* Retourner le nombre de vehicule du vecteur
* @return le nombre de vehicule
*/
unsigned int Automate::nbVehicule()
{
	return _vecVehicule.size();
}

/*
* Indiquer s'il y a au moins un vehicule de disponible pour servir le client
* @param destination la destination du client
* @return boolean
*/
bool Automate::deplacerUtilisateurMemeQuartier(Zipcode depart, Zipcode destination)
{
	for (unsigned int i = 0; i < _vecVehicule.size(); i++) {
		if (_vecVehicule.at(i)->getPositionActuelle().compare(depart) && _vecVehicule.at(i)->getDisponibilite()) {
			_vecVehicule.at(i)->incrementTrajetClient();
			_vecVehicule.at(i)->setDisponibilite(false);
			_vecVehicule.at(i)->setPositionFinale(destination);
			return true;
		}
	}
	return false;
}

bool Automate::deplacerVehiculeMemeZone(Zipcode depart)
{
	for (unsigned int i = 0; i < _vecVehicule.size(); i++) {
		if (_vecVehicule.at(i)->getDisponibilite()) {
			_vecVehicule.at(i)->incrementTrajetVide();
			_vecVehicule.at(i)->setPositionActuelle(depart);
			_vecVehicule.at(i)->setDisponibilite(true);
			return true;
		}
	}
	return false;
}



/*
* Gestion a la fin du deplacement. Changer les etats et les positions des vehicules.
* @param automate le vecteur des automates
*/
void Automate::finDeplacement(vector<Automate*> automate)
{
	for (unsigned int i = 0; i < _vecVehicule.size();) {
		if (!_vecVehicule.at(i)->getDisponibilite()) {
			Automate* automateGenerale = findAutomateGenerale(automate, _vecVehicule.at(i)->getPositionFinale());
			_vecVehicule.at(i)->setDisponibilite(true);
			_vecVehicule.at(i)->setPositionActuelle(_vecVehicule.at(i)->getPositionFinale());
			automateGenerale->addVehicule(_vecVehicule.at(i));
			eraseVehicule(_vecVehicule.at(i));
		}
		else i++;
	}
}

/*
* Retourner la racine de l'automate
* @return la racine
*/
Etat * Automate::getRacine()
{
	return _racine;
}

bool Automate::contientVehicule()
{
	return !_vecVehicule.empty();
}

void Automate::nbDeplacementClient()
{
	for (unsigned int i = 0; i < _vecVehicule.size(); i++) {
		cout << "id:" << _vecVehicule.at(i)->getId() << "\t\t\t" << _vecVehicule.at(i)->getNbTrajetVide() << "\t\t" << _vecVehicule.at(i)->getNbTrajetClient() << endl;
	}
}

void Automate::transferer(Automate * automate, unsigned int nbVec)
{
	for (unsigned int i = 0; i < nbVec; i++) {		
		_vecVehicule.at(0)->setPositionActuelle(automate->generAdresse());
		_vecVehicule.at(0)->setPositionFinale(automate->generAdresse());
		automate->addVehicule(_vecVehicule.at(0));
		_vecVehicule.erase(_vecVehicule.begin() + 0);
	}
}

Zipcode Automate::generAdresse()
{
	return _racine->generAdresse();
}

void Automate::clearVehicules()
{
	_vecVehicule.clear();
}

/*
* Trouver l'automate par le zipcode
* @param automate le vecteur des automates
* @param destination le zipcode de la destination du vehicule
* @return l'automate qui possede le zipcode
*/
Automate* Automate::findAutomateGenerale(vector<Automate*> automate, Zipcode destination)
{
	for (unsigned int i = 0; i < automate.size(); i++) {
		if (automate.at(i)->_racine->search(destination)) {
			return automate.at(i);
		}
	}
	return nullptr;
}



