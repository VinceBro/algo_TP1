//
// Created by Mario Marchand on 16-12-29.
//

#include "DonneesGTFS.h"
#include <fstream>
#include <exception>
#include <sstream>
#include <vector>
//#include "ligne.h"
#include <string>

using namespace std;


//! \brief ajoute les lignes dans l'objet GTFS
//! \param[in] p_nomFichier: le nom du fichier contenant les lignes
//! \throws logic_error si un problème survient avec la lecture du fichier
void DonneesGTFS::ajouterLignes(const std::string &p_nomFichier)
{
    string s;
    vector<string> vec;
//    const char *sauce;
//    sauce = ",";
//    cout << int(*sauce) << endl;
//    throw exception();
    stringstream ss;
    ifstream ifs;
    // TEMP SECTION
    char temp;
    int tempint;
    ifs.open("../" + p_nomFichier);
    while (!ifs.eof())
    {
        ifs.ignore();
        tempint = 0;
        getline(ifs, s);
        for(char c : s + ","){
            if (int(c) == 44){
                vec.push_back(ss.str());
//                cout << ss.str() << endl;
                ss.str(string());
            } else ss << c;
        }
        cout << "p_id : " << vec[0] << "p_numero : " <<  vec[2] << "p_description : " << vec[4] << "p_categorie : " << vec[5] <<  endl;
        Ligne l(stoul(vec[0]), string(vec[2]), string(vec[4]), static_cast<CategorieBus >(stoi(vec[5])));
        cout << l << endl;
        throw exception();
        for (string lol : vec){


            cout << tempint << "for : " << lol <<  endl;
            tempint++;
        }
        vec.clear();
        cin >> temp;

    }
    ifs.close();

//écrire votre code ici

}

//! \brief ajoute les stations dans l'objet GTFS
//! \param[in] p_nomFichier: le nom du fichier contenant les station
//! \throws logic_error si un problème survient avec la lecture du fichier
void DonneesGTFS::ajouterStations(const std::string &p_nomFichier)
{

//écrire votre code ici

}

//! \brief ajoute les transferts dans l'objet GTFS
//! \breif Cette méthode doit âtre utilisée uniquement après que tous les arrêts ont été ajoutés
//! \brief les transferts (entre stations) ajoutés sont uniquement ceux pour lesquelles les stations sont prensentes dans l'objet GTFS
//! \brief les transferts sont ajoutés dans m_transferts
//! \brief les from_station_id des stations de transfert sont ajoutés dans m_stationsDeTransfert
//! \param[in] p_nomFichier: le nom du fichier contenant les station
//! \throws logic_error si un problème survient avec la lecture du fichier
//! \throws logic_error si tous les arrets de la date et de l'intervalle n'ont pas été ajoutés
void DonneesGTFS::ajouterTransferts(const std::string &p_nomFichier)
{

//écrire votre code ici

}


//! \brief ajoute les services de la date du GTFS (m_date)
//! \param[in] p_nomFichier: le nom du fichier contenant les services
//! \throws logic_error si un problème survient avec la lecture du fichier
void DonneesGTFS::ajouterServices(const std::string &p_nomFichier)
{

//écrire votre code ici

}

//! \brief ajoute les voyages de la date
//! \brief seuls les voyages dont le service est présent dans l'objet GTFS sont ajoutés
//! \param[in] p_nomFichier: le nom du fichier contenant les voyages
//! \throws logic_error si un problème survient avec la lecture du fichier
void DonneesGTFS::ajouterVoyagesDeLaDate(const std::string &p_nomFichier)
{

//écrire votre code ici

}

//! \brief ajoute les arrets aux voyages présents dans le GTFS si l'heure du voyage appartient à l'intervalle de temps du GTFS
//! \brief De plus, on enlève les voyages qui n'ont pas d'arrêts dans l'intervalle de temps du GTFS
//! \brief De plus, on enlève les stations qui n'ont pas d'arrets dans l'intervalle de temps du GTFS
//! \param[in] p_nomFichier: le nom du fichier contenant les arrets
//! \post assigne m_tousLesArretsPresents à true
//! \throws logic_error si un problème survient avec la lecture du fichier
void DonneesGTFS::ajouterArretsDesVoyagesDeLaDate(const std::string &p_nomFichier)
{

//écrire votre code ici

}



