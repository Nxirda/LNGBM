#ifndef ABR_H_
#define ABR_H_
 
#include <iostream>
 
using namespace std;
 
// Définition du noeud en tant que struct
template <typename T>
struct Noeud
{
     T donnees;
    int*Noeud::gauche;
    int*Noeud ::droit;
    char donnees;

} ;
 
// Définition de la classe Liste
template <class T>
class ABR
{
    private:
        Noeud <T>* racine;
        int compteur;
        Noeud <T>* CreerNoeud (const T& valeur);
        void detruire (Noeud <T>* ptr); // Fonction d'aide
        void inserer (const T& value, Noeud <T>*& ptr); // Fonction d'aide
        void infixe (Noeud <T>* ptr) const; // Fonction d'aide
        void prefixe (Noeud <T>* ptr) const; // Fonction d'aide
        void postfixe (Noeud <T>* ptr) const; // Fonction d'aide
        Noeud <T>* successeur (Noeud <T>* ptr, Noeud <T>*& parent) const; // Fonction d'aide
        Noeud <T>* predecesseur (Noeud <T>* ptr, Noeud <T>*& parent) const; // Fonction d'aide
        void supprimer (Noeud <T>* ptr, Noeud <T>* parent); // Fonction d'aide
        Noeud <T>* recherche (const T& value, Noeud <T>* ptr, Noeud <T>*& parent) const; // Fonction d'aide
     
    public:
        ABR ();
        ~ABR ();
        void inserer (const T& value);
        void detruire ();
        void supprimer (const T& value);
        Noeud <T>* recherche (const T& value) const;
        void infixe () const;
        void prefixe () const;
        void postfixe () const;
        int taille () const;
        bool estVide () const;
};
#endif
