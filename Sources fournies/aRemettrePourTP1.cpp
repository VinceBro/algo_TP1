//
// Created by Mario Marchand on 16-12-29.
#include "DonneesGTFS.h"
#include <fstream>
#include <exception>
#include <sstream>
#include <vector>
#include <string>
#include <tuple>

using namespace std;


//! \brief ajoute les lignes dans l'objet GTFS
//! \param[in] p_nomFichier: le nom du fichier contenant les lignes
//! \throws logic_error si un problème survient avec la lecture du fichier
void DonneesGTFS::ajouterLignes(const std::string &p_nomFichier)
{
    unsigned int counter = 0;
    string s;
    vector<string> vec;
    stringstream ss;
    ifstream ifs;
    ifs.open("../" +  p_nomFichier);
    if (!ifs) throw logic_error("Impossible d'ouvrir le fichier");
    while (getline(ifs, s))
    {
        if (counter == 0) {
            counter++;
            continue;
        }
        else vec = string_to_vector(s, *",");

        string short_name = vec[2];
        short_name.erase(0, 1);
        short_name.erase(short_name.size() - 1);

        string description = vec[4];
        description.erase(0, 1);
        description.erase(description.size() - 1);

        CategorieBus categorie= Ligne::couleurToCategorie(vec[7]);

        unsigned int route_id = stoi(vec[0]);
        Ligne l(route_id, short_name, description,categorie );
        m_lignes.insert({route_id, l});
        m_lignes_par_numero.insert({short_name, l});

        vec.clear();
        counter++;

    }
    ifs.close();
}

//! \brief ajoute les stations dans l'objet GTFS
//! \param[in] p_nomFichier: le nom du fichier contenant les station
//! \throws logic_error si un problème survient avec la lecture du fichier
void DonneesGTFS::ajouterStations(const std::string &p_nomFichier)
{
    unsigned int counter = 0;
    vector<string> vec;
    string s;
    stringstream ss;
    ifstream ifs;
    ifs.open("../" + p_nomFichier);
    if (!ifs) throw logic_error("Impossible d'ouvrir le fichier");
    while (getline(ifs, s)){
        if (counter == 0) {
            counter++;
            continue;
        }else vec = string_to_vector(s, *",");

        double latitude = stod(vec[3]);
        double longitude = stod(vec[4]);
        unsigned int  stop_id = stoi(vec[0]);
        string description = vec[2];
        description.erase(0, 1);
        description.erase(description.size() - 1);

        string short_name = vec[1];
        short_name.erase(0, 1);
        short_name.erase(short_name.size() - 1);

        Station stat(stop_id, short_name, description, Coordonnees(latitude, longitude));
        m_stations.insert({stop_id, stat});
        vec.clear();
        counter++;
    }
    ifs.close();
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
    unsigned int counter = 0;
    vector<string> vec;
    string s;
    stringstream ss;
    ifstream ifs;
    ifs.open("../" + p_nomFichier);
    if (!ifs) throw logic_error("Impossible d'ouvrir le fichier");
    while (getline(ifs, s)){
        if (counter == 0) {
            counter++;
            continue;
        } else vec = string_to_vector(s, *",");
        if (m_stations.count(stoi(vec[0])) and m_stations.count(stoi(vec[1]))){
            unsigned int from_stop_id = stoi(vec[0]);
            unsigned int to_stop_id = stoi(vec[1]);
            unsigned int transfer_time = stoi(vec[3]);

            //Un temps de transfert est impossible, minimum une seconde
            if (transfer_time == 0) transfer_time = 1;
            auto t = make_tuple(from_stop_id, to_stop_id, transfer_time);

            m_transferts.push_back(t);
            m_stationsDeTransfert.insert(from_stop_id);

        }
        }

}


//! \brief ajoute les services de la date du GTFS (m_date)
//! \param[in] p_nomFichier: le nom du fichier contenant les services
//! \throws logic_error si un problème survient avec la lecture du fichier
void DonneesGTFS::ajouterServices(const std::string &p_nomFichier)
{
    unsigned int counter = 0;
    vector<string> vec;
    string s;
    string date;
    stringstream ss;
    ifstream ifs;
    ss << m_date;
    date = ss.str();
    // référence pour la prochaine ligne
    // https://stackoverflow.com/questions/20326356/how-to-remove-all-the-occurrences-of-a-char-in-c-string
    date.erase(remove(date.begin(), date.end(), '-'), date.end());
    ifs.open("../" + p_nomFichier);
    if (!ifs) throw logic_error("Impossible d'ouvrir le fichier");
    while (getline(ifs, s)){
        if (counter == 0) {
            counter++;
            continue;
        }
        else vec = string_to_vector(s, *",");

        string date_fich = vec[1];
        string service_id = vec[0];

        if (date_fich == date) m_services.insert(service_id);
        vec.clear();
        counter++;
    }
    ifs.close();
//écrire votre code ici

}

//! \brief ajoute les voyages de la date
//! \brief seuls les voyages dont le service est présent dans l'objet GTFS sont ajoutés
//! \param[in] p_nomFichier: le nom du fichier contenant les voyages
//! \throws logic_error si un problème survient avec la lecture du fichier
void DonneesGTFS::ajouterVoyagesDeLaDate(const std::string &p_nomFichier)
{

    unsigned int counter = 0;
    vector<string> vec;
    string s;
    stringstream ss;
    ifstream ifs;
    ifs.open("../" + p_nomFichier);
    if (!ifs) throw logic_error("Impossible d'ouvrir le fichier");
    while (getline(ifs, s)){
        if (counter == 0) {
            counter++;
            continue;
        }
        else vec = string_to_vector(s, *",");


        string service_id = vec[1];
        unsigned int route_id = stoi(vec[0]);
        string trip_id = vec[2];
        for(string c : m_services){
            if (vec[1] == c){
                string headsign = vec[3];
                headsign.erase(0, 1);
                headsign.erase(headsign.size() - 1);

                m_voyages.insert({trip_id, Voyage(trip_id, route_id, service_id, headsign)});
            }
        }
        vec.clear();
        counter++;
    }
    ifs.close();
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

    unsigned int counter = 0;
    vector<string> vec;
    vector<string> v_now1;
    vector<string> v_now2;
    set<unsigned int> allstations;
    string s;
    stringstream ss;
    ifstream ifs;
    ifs.open("../" + p_nomFichier);
    if (!ifs) throw logic_error("Impossible d'ouvrir le fichier");
    while (getline(ifs, s)){
        if (counter == 0) {
            counter++;
            continue;
        }
        else vec = string_to_vector(s, *",");

        if (m_voyages.count(vec[0]) ) {
            // initialiser les dates pour les comparer avec les deux m_now
            v_now1 = string_to_vector(vec[1], *":");
            Heure p_now1(stoul(v_now1[0]), stoul(v_now1[1]), stoul(v_now1[2]));
            v_now2 = string_to_vector(vec[2], *":");
            Heure p_now2(stoul(v_now2[0]), stoul(v_now2[1]), stoul(v_now2[2]));

            if (p_now1 >= m_now1 and p_now2 <  m_now2) {
                string voyage_id = vec[0];
                unsigned int numero_sequence = stoi(vec[4]);
                unsigned int station_id = stoi(vec[3]);


                Arret::Ptr a = make_shared<Arret>(station_id, p_now2, p_now1, numero_sequence, voyage_id);
                m_voyages[voyage_id].ajouterArret(a);
                m_stations[station_id].addArret(a);
                m_nbArrets++;
            }
        }
        vec.clear();
        counter++;
    }
    ifs.close();

    std::map<std::string, Voyage> voyages_copie(m_voyages);
    for (auto un_voyage : voyages_copie){
        if (un_voyage.second.getNbArrets() == 0){
            m_voyages.erase(un_voyage.first);
        }
    }

    std::map<unsigned int, Station> stations_copie(m_stations);
    for (auto une_station :stations_copie){
        if (une_station.second.getNbArrets() == 0){
            m_stations.erase(une_station.first);
        }
    }
//écrire votre code ici

}




