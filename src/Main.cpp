#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm> // remove_if
#include <map>
#include <windows.h>
#include <chrono>
#include <thread>

#include"Etat.h"
#include"Automate.h"
#include "Vehicule.h"
#include "Zipcode.h"
#include "Client.h"
#include "Groupe.h"

using namespace std;

/*
* lire les fichiers textes correspondants aux zones des vehicules et creer l'automate.
* @param path la location du fichier
*/
Automate* creerLexique(string path) {
	Etat* racine = new Etat();
	ifstream infile(path);
	Etat* current = new Etat();
	string line;

	if (infile.is_open()) {
		while (getline(infile, line))
		{
			remove_if(line.begin(), line.end(), isspace);
			bool firstTime = true;

			for (char& c : line)
			{
				if (firstTime)
				{
					current = racine;
					current->setValue(c);
					firstTime = false;
				}
				else
				{
					if (current->isEmpty() || current->contains(c) == nullptr) {
						Etat* next = new Etat(c);
						current->addChild(next);
						current = next;
					}
					else
						current = current->contains(c);

				}
			}
		}
		infile.close();
		return new Automate(racine);
	}
	else {
		return nullptr;
	}
}

/*
* Equilibrer le nombre de vehicules pour chaque zone (automate)
*/
void equilibrerFlotte(vector<Automate*> automates) {
	unsigned int nbVehicules = 0;
	unsigned int nbAutomates = automates.size();

	for (unsigned int z = 0; z < automates.size(); z++) {
		nbVehicules += automates.at(z)->nbVehicule();
	}	

	unsigned int min = nbVehicules / nbAutomates;
	unsigned int max = min;

	if (nbVehicules % nbAutomates != 0)
	{
		max += 1;
	}

	
	unsigned int nbMax = nbVehicules - (min * nbAutomates);
	unsigned int nbMin = nbAutomates - nbMax;

	for (unsigned int i = 0; i < nbAutomates && !(nbMin == 0 && nbMax == 0); i++)
	{


		//Si lautomate en question ne contient pas assez de voitures
		//Il doit en voler a un autre automate
		if (automates.at(i)->nbVehicule() < min)
		{
			unsigned int toAdd = 0;
			unsigned int j = i + 1;

			while (automates.at(j)->nbVehicule() < max)
			{
				j++;
			}

			//Si lautomate ne contient pas assez de voitures, on doit
			//en emprunter le moins possible pour economiser.
			//Donc, il devient un min
			if (nbMin > 0)
			{
				toAdd = min - automates.at(i)->nbVehicule();
				nbMin--;
			}

			//Si on doit en emprunter mais quon a deja tous nos mins,
			//lautomate devient un max
			else
			{
				toAdd = max - automates.at(i)->nbVehicule();
				nbMax--;
			}

			automates.at(j)->transferer(automates.at(i), toAdd);
		}

		else if (automates.at(i)->nbVehicule() > max)
		{
			unsigned int toRemove = 0;
			unsigned int j = i + 1;

			//Si un automate contient trop de voitures, on doit
			//deplacer le moins de voitures possibles pour economiser.
			//Donc, on le transforme en max
			if (nbMax > 0)
			{
				toRemove = automates.at(i)->nbVehicule() - max;
				nbMax--;
			}

			//Si on a deja tous nos max, on le transforme en min
			else
			{
				toRemove = automates.at(i)->nbVehicule() - min;
				nbMin--;
			}

			unsigned int tempNbMax = nbMax;
			unsigned int tempNbMin = nbMin;
			while (toRemove > 0)
			{
				//On add juste aux etats qui nont pas assez de voitures
				if (automates.at(j)->nbVehicule() < min)
				{
					unsigned int toAdd = 0;


					//On transforme lautomate en min
					if (tempNbMin > 0)
					{
						toAdd = min - automates.at(j)->nbVehicule();
						tempNbMin--;
					}

					//On fait les max apres
					else
					{
						toAdd = max - automates.at(j)->nbVehicule();
						tempNbMax--;
					}

					if (toAdd > toRemove)
						toAdd = toRemove;

					toRemove -= toAdd;
					automates.at(i)->transferer(automates.at(j), toAdd);
				}else if (automates.at(j)->nbVehicule() == min)
				{
					//Si lautomate en question est un min et quon a deja le nombre
					//maximal de min, on le transforme en max. Il va donc
					//voler une voiture a lautomate qui les distribue
					if (tempNbMin == 0)
					{
						unsigned int toAdd = max - automates.at(j)->nbVehicule();

						if (toAdd > toRemove)
							toAdd = toRemove;

						toRemove -= toAdd;
						automates.at(i)->transferer(automates.at(j), toAdd);
						tempNbMax--;
					}else
						tempNbMin--;


				}
				j++;
			}

			//nbMin = tempNbMin;
			//nbMax = tempNbMax;
		}

		//Si lautomate en question contient un nombre min de voitures
		else if (automates.at(i)->nbVehicule() == min)
		{
			//Sil peut etre un min, on fait rien et nBMinMax diminue de 1
			if (nbMin > 0)
			{
				nbMin--;
			}

			//Sil ne peut pas etre un min, on le transforme
			//en max
			else
			{
				unsigned int toAdd = 0;
				unsigned int j = i + 1;

				while (automates.at(j)->nbVehicule() < max)
				{
					j++;
				}

				toAdd = max - automates.at(i)->nbVehicule();
				nbMax--;

				automates.at(j)->transferer(automates.at(i), toAdd);
			}
		}

		//Si lautomate est un max
		else
		{
			//Sil peut etre un max, on fait rien et nbMax--
			if (nbMax > 0)
			{
				nbMax--;
			}

			//Sil est un max et il ne peut pas en etre un, on le transforme
			//en min
			else
			{
				unsigned int toRemove = 0;
				unsigned int j = i + 1;

				while (automates.at(j)->nbVehicule() > min)
				{
					j++;
				}

				toRemove = automates.at(i)->nbVehicule() - min;
				nbMin--;
				automates.at(j)->transferer(automates.at(i), toRemove);
			}
		}
	}
}
/*
* Demarrer la simualation apres la creation des donnees necessaires
*/

/*
* Afficher l'interface du menu principal
*/
void afficherMenu()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << endl<< "----------------------Menu principal------------------------------" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout << "(a) Creer les zones." << endl;
	cout << "(b) Entrer les clients et les vehicules." << endl;
	cout << "(c) Demarrer la simulation." << endl;
	cout << "(d) Quitter." << endl << endl;
	cout << "Entrer votre choix : ";
}

/*
* Afficher l'interface du menu de creation de client ou vehicule
*/
void afficherMenuCreationClientVehicule()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << endl << "-------------------------Creation de client ou vehicule---------------------------" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout << "(a) Creer un client." << endl;
	cout << "(b) Creer un vehicule." << endl;
	cout << "(c) Retourner au menu principal." << endl;
	cout << "Entrer votre choix : ";
}

/*
* Creer un ou plusieurs client par la lecture d'un fichier
* @return une liste de client cree
*/
vector<Client> creerClientParFichier(string path) {
	ifstream myFile(path);
	string line;
	vector<Client> listeClient;

	string zipcodeDepStr, zipcodeDestStr;
	char zipcodeDep[6];
	char zipcodeDest[6];
	if (myFile.is_open()) {
		while (getline(myFile, line)) {			
			for (unsigned int i = 0; i < 6; i++) {
				zipcodeDep[i] = line.at(i);
			}
			for (unsigned int i = 0; i < 6; i++) {
				zipcodeDest[i] = line.at(i+7);
			}
			listeClient.push_back(Client(Zipcode(zipcodeDep), Zipcode(zipcodeDest)));
		}
	}
	myFile.close();
	return listeClient;
}

/*
* Creer une ou plusieurs vehicule par la lecture d'un fichier
* @return une liste de client cree
*/
void creerVehiculeParFichier(vector<Automate*> listeAutomate, string path, map<unsigned int,unsigned int>& map) {
	ifstream myFile(path);
	string line;
	string zipcodeDepStr;
	char zip[6];
	Automate* automateTarget;
	unsigned int id = 0;
	if (myFile.is_open()) {
		while (getline(myFile, line)) {
			for (unsigned int i = 0; i < 3; i++) {
				zip[i] = line.at(i);
			}
			for (unsigned int i = 3; i < 6; i++) {
				zip[i] = line.at(i+1);
			}
			Zipcode zipDepart = Zipcode(zip);
			Vehicule* vec = new Vehicule(zipDepart, zipDepart, id++);
			Automate* current = new Automate();
			for (unsigned int i = 0; i < listeAutomate.size(); i++) 
			{
				if (listeAutomate.at(i)->getRacine()->search(zipDepart)) {
					map[i] = map[i] + 1;
					current = listeAutomate.at(i);
					break;
				}
			}
			current->addVehicule(vec);

		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
		cout << "Les vehicules ont ete crees" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		cout << "Attention! La creation des vehicules a echoue!" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

}

/*
* Afficher le tableau de la carte
* @param initial la liste des positions initiales des vehicules
* @param final la liste d'automate final
*/
void afficherTableauMap(map<unsigned int,unsigned int> intial, vector<Automate*> final) {
	cout << endl<< "-----------Vehicule par zone au debut et a la fin----------------" << endl <<endl;
	cout << "zone" << "\t\t" << "debut" << "\t\t" << "fin" << endl;
	for (unsigned int i = 0; i < final.size(); i++) {
		cout << i + 1 << "-zone" << "\t\t" << intial[i] << "\t\t" << final.at(i)->nbVehicule() << endl;
	}
}

/*
* Afficher le tableau des deplacements des vehicules
* @param automate la liste d'automate
*/
void afficherTableauDeplacement(vector<Automate*> automate) {
	cout << endl << "----------------------Trajet vide et occupant---------------------" << endl <<endl;
	cout << "Vehicule" << "\t\t" << "vide" << "\t\t" << "occupant" << endl;
	for (unsigned int i = 0; i < automate.size(); i++) {
		automate.at(i)->nbDeplacementClient();
	}
}

/*
* Trouver l'automate de depart du client
* @param listAutomate la liste d'automate
* @param client le client cible
*/
Automate* findAutomateDepartArrivee(vector<Automate*> listAutomate, Client client) {
	Zipcode zipDepart = client.getPointDepart();
	Zipcode zipArrivee = client.getDestination();
	unsigned int x = 0;
	for (unsigned int i = 0; i < listAutomate.size(); i++) {
		if (listAutomate.at(i)->getRacine()->search(zipDepart)) {
			return listAutomate.at(i);
		}
	}
	return nullptr;
}

/*
* Lancer la simulation par vague
* @param groupe la vague de client
* @param listAutomate la liste d'automate
*/
void lancerSimulation(Groupe groupe, vector<Automate*> listAutomate) {
	bool fini = false;
	vector<Client> clientTotal = groupe.getClient();
	for (unsigned int i = 0; i < clientTotal.size(); i++)
	{
		bool reussi = false;
		Zipcode dep = clientTotal.at(i).getPointDepart();
		Zipcode arrivee = clientTotal.at(i).getDestination();
		Automate* automateDepart =  findAutomateDepartArrivee(listAutomate,clientTotal.at(i));

		if (!automateDepart->deplacerUtilisateurMemeQuartier(dep, arrivee)) {
			if(!automateDepart->deplacerVehiculeMemeZone(dep))
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
				cout << "La demande du Client " << i << " a ete refusee" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else {
				i--;
			}
		}		
	}

	for (unsigned int i = 0; i < listAutomate.size(); i++) 
	{
		listAutomate.at(i)->finDeplacement(listAutomate);
	}
	
}

/*
* Indiquer si le vehicule existe dans la list d'automate
* @param list la liste d'automate
* @return boolean
*/
bool vehiculeExistant(vector<Automate*> list) {

	for (unsigned int i = 0; i < list.size(); i++)
	{
		if (list.at(i)->contientVehicule())
			return true;
	}
	return false;
}

unsigned int main()
{
	//exemple de path lexique:       X:\\Bureau\\zones
	//exemple de path client:        X:\\Bureau\\clients\\client1.txt

	vector<Vehicule*> listeVehicule;				//liste des vehicules du service
	vector<Automate*> listeAutomate;				//liste des automates
	vector<Groupe> listGroupeClient;
	vector<Automate> listeAutomateInitial;
	map<unsigned int, unsigned int> nbVecIntial;

	string inputPrincipal = " ";					//input pour le menu principal
	string path;									//la location des fichiers de zone

	string pathToZone = "\\zones\\zone";
	string pathToGroupeClient = "\\groupes\\groupe";
	string pathToVehicule = "\\vehicules\\vehicules";
	string extension = ".txt";

	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	do {
		afficherMenu();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cin >> inputPrincipal;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

		if (inputPrincipal == "a") { ////////////////////////////////////////////////////////////////////////////////////////////////// A
			//Clear les ancienes zones si ils ont etat creer
			for (unsigned int i = 0; i < listeAutomate.size(); i++) {
				listeAutomate.at(i) = nullptr;
				delete listeAutomate.at(i);
			}
			listeAutomate.clear();
			unsigned int nbFichier;							//le nombre de fichier a lire
			cout << "------------------------Option pour creer les zones----------------------------" << endl;
			cout << "Entrez la location du dossier general des fichiers : ";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
			cin >> path;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			cout << "Combien de fichier voulez-vous ajouter? ";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
			cin >> nbFichier;
			while (cin.fail())
			{
				cin.clear();
				cin.ignore(INT_MAX, '\n'); 
				cout << "Entrez un nombre SVP! : ";
				cin >> nbFichier;
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);


			for (unsigned int i = 1; i <= nbFichier; i++) {
				Automate* temp = creerLexique(path + pathToZone + to_string(i) + extension);
				if (temp != nullptr) {
					listeAutomate.push_back(temp);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
					cout << "Le lexique pour la zone " << i << " a ete cree!" << endl;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				}
				else {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
					cout << "Attention! La creation du lexique pour la zone " << i << " a echoue!" << endl;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				}

			}
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		else if (inputPrincipal == "b") { ///////////////////////////////////////////////////////////////////////////////////////// B
		
			//Vérifier que l'utilisateur ait choisi l'option A en premier
			if (!listeAutomate.empty()) {
				string inputClientVehicule = " ";			//input pour le menu de creation de client ou vehicule

				do {
					afficherMenuCreationClientVehicule();	//debut menu creation client ou vehicule
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
					cin >> inputClientVehicule;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

					if (inputClientVehicule == "a") {		//debut creation client
						//Clear les anciens groupes si ils ont ete creer
						listGroupeClient.clear();
						unsigned int nbGroupe;
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
						cout << "-----------------------------Creation de groupe---------------------------------" << endl;
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
						cout << "Nombre de groupe a creer : ";
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
						cin >> nbGroupe;
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

						while (cin.fail())
						{
							cin.clear();
							cin.ignore(INT_MAX, '\n');
							cout << "Entrez un nombre SVP! : ";
							cin >> nbGroupe;
						}

						for (unsigned int i = 1; i <= nbGroupe; i++) {
							vector<Client> listeClientTemp = creerClientParFichier(path + pathToGroupeClient + to_string(i) + extension);
							if (!listeClientTemp.empty()) {
								Groupe groupe = Groupe();
								groupe.setVecClients(listeClientTemp);
								listGroupeClient.push_back(groupe);
								SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
								cout << "Le groupe  " << i << " a ete cree!" << endl;
								SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
							}
							else {
								SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
								cout << "Attention! La creation du groupe " << i << " a echoue!" << endl;
								SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
							}
						}
						std::this_thread::sleep_for(std::chrono::milliseconds(500));

					}	//fin creation client

					else if (inputClientVehicule == "b") {	//debut creation vehicule
						//Clear les anciens vehicule si ils ont etat creer
						for (unsigned int i = 0; i < listeAutomate.size(); i++) {
							listeAutomate.at(i)->clearVehicules();
						}
						creerVehiculeParFichier(listeAutomate, path + pathToVehicule + extension, nbVecIntial);

					}	//fin creation vehicule

					else if (inputClientVehicule != "c") {
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
						cout << "Entree invalide! Veuillez choisir un des options avec a, b ou c" << endl;
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
					}

				} while (inputClientVehicule != "c");
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
				cout << "Vous devez d'abord creer une premiere lexique! Veuillez choisir l'option A avant!" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}

		}	//fin menu creation client ou vehicule

		else if (inputPrincipal == "c") { ///////////////////////////////////////////////////////////////////////////////// C
			if (!listGroupeClient.empty() && vehiculeExistant(listeAutomate)) {
				for (Groupe groupe : listGroupeClient) {
					lancerSimulation(groupe, listeAutomate);
					afficherTableauDeplacement(listeAutomate);
					afficherTableauMap(nbVecIntial, listeAutomate);
					equilibrerFlotte(listeAutomate);
					std::this_thread::sleep_for(std::chrono::milliseconds(500));
				}
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
				cout << "Vous devez d'abord creer au moins un client et un vehicule! Veuillez choisir l'option B avant!" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			
		}
		else if (inputPrincipal != "d") { //////////////////////////////////////////////////////////////////////////////// Autre
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
			cout << "Entree invalide! Veuillez choisir un des options avec a, b, c ou d" << endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}

	} while (inputPrincipal != "d");


	
	for (unsigned int i = 0; i < listeAutomate.size(); i++) {
		delete listeAutomate.at(i);
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << endl << "Automate detruit pour l'espace memoire" << endl<< endl <<endl;
	return 0;
}