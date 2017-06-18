#include "../../include/ground/Dvector.hpp"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include "string.h"

using namespace std;

Dvector::Dvector(int d, double init)
{
 //   cout << "Appel du constructeur\n";
    dim = d;
    comp = new double[dim];
    for (int i=0; i<dim; i++) {
        comp[i] = init;
    }
}

Dvector::Dvector()
{
   // cout << "Appel du constructeur par défaut\n";
    dim = 0;
    comp = NULL;
}

Dvector::Dvector(const Dvector & vect)
{
   // cout << "Appel du constructeur par recopie\n";
    dim = vect.dim;
    if (vect.dim == 0) {
        comp = NULL;
        return;
    }
    comp = new double[dim];
    for (int i=0; i<dim; i++) {
        comp[i] = vect.comp[i];
    }
}


Dvector::Dvector(string filename)
{
  ifstream file(filename.c_str());  // On ouvre le fichier en lecture
  ifstream fileBis(filename.c_str());
  // Test de l'ouverture
  if(file && fileBis) {
    // comptage nombre de composantes
    string charNumber;
    int nbLines = 0;
    while (getline(file, charNumber)) {
      nbLines++;
    }
    // On remonte en haut du fichier
    //file.seekg(0, ios::beg);

    // On créé le vecteur
    dim = nbLines;
    comp = new double[dim];

    int i = 0;
    while(getline(fileBis, charNumber)) {
      comp[i] = atof(charNumber.c_str());
      i++;
    }

    file.close();  // On ferme le fichier
    fileBis.close();
  } else {
    cerr << "Impossible d'ouvrir le fichier !" << endl;
  }
}

Dvector::~Dvector()
{
    //cout << "Appel du destructeur\n";
	if (this->size() != 0){
		delete [] comp;
	}
    dim = 0;
}

void Dvector::display(ostream& str)
{
  for (int i = 0; i<dim; i++) {
    str << comp[i] << "\n";
  }
}

int Dvector::size() const
{
  return dim;
}

void Dvector::fillRandomly()
{
  srand(time(NULL));
  for (int i = 0; i<dim; i++) {
    comp[i] = rand()/(double)RAND_MAX;
  }
}
Dvector & Dvector::resize(const int newsize, const double elem){
  if (newsize != this->size()){
    double * tab= new double[newsize];
    if (newsize  < this->size()){
      for (int i=0; i<newsize;i++){
        tab[i]=this->comp[i];
      }
    }else if(newsize >this->size()){
      for (int i=0; i<this->size();i++){
        tab[i]=this->comp[i];
      }
      for (int i=this->size(); i<newsize;i++){
        tab[i]=elem;
      }
    }
    delete [] this->comp;
    this->dim=newsize;
    this->comp= tab;
  }
  return *this;
}

// Opérateurs
//Internes
double & Dvector::operator ()(int i){
  if (i>=0 && i<=(this->dim)-1){
    return comp[i];
  }else{
    throw string("Indice incorrect !");
  }
}

double  Dvector::operator ()(int i) const {
  if (i>=0 && i<=(this->dim)-1){
    return comp[i];
  }else{
    throw string("Indice incorrect !");
  }
}
double Dvector::prodscal (Dvector vect){
  if (this->size()== vect.size()){
    double result=0;
    for(int i=0;i<vect.size();i++){
      result+=vect(i)*(*this)(i);
    }
    return result;
  }
  throw "vecteur de dimension incorecte";
}

Dvector & Dvector::operator += (const Dvector & vect) {
  if (this->dim != vect.dim) {
    exit(-1); // Diimensions incompatibles
  }
  Dvector & res = *this;
  for (int i = 0; i < vect.dim; i++) {
    res.comp[i] += vect(i);
  }
  return res;
}

Dvector & Dvector::operator -= (const Dvector & vect) {
  if (this->dim != vect.dim) {
    exit(-1); // Diimensions incompatibles
  }
  Dvector & res = *this;
  for (int i = 0; i < vect.dim; i++) {
    res.comp[i] -= vect(i);
  }
  return res;
}

Dvector & Dvector::operator *= (const Dvector & vect) {
  if (this->dim != vect.dim) {
    exit(-1); // Dimensions incompatibles
  }
  Dvector & res = *this;
  for (int i = 0; i < vect.dim; i++) {
    res.comp[i] *= vect(i);
  }
  return res;
}

Dvector & Dvector::operator += (const double operand) {
    Dvector & res = *this;
    for (int i = 0; i < res.dim; i++) {
        res.comp[i] += operand;
    }
    return res;
}

Dvector & Dvector::operator -= (const double operand) {
    Dvector & res = *this;
    for (int i = 0; i < res.dim; i++) {
        res.comp[i] -= operand;
    }
    return res;
}

Dvector & Dvector::operator *= (const double operand) {
    Dvector & res = *this;
    for (int i = 0; i < res.dim; i++) {
        res.comp[i] *= operand;
    }
    return res;
}

Dvector & Dvector::operator /= (const double operand) {
    Dvector & res = *this;
    for (int i = 0; i < res.dim; i++) {
        res.comp[i] /= operand;
    }
    return res;
}

//Implem memcpy
Dvector & Dvector::operator = (const Dvector & vect) {
    if (this->dim == 0) {
        this->dim = vect.dim;
        this->comp = new double[this->dim];
    } else if (this->dim != vect.dim){
        delete [] this->comp;
        this->dim = vect.dim;
        this->comp = new double[this->dim];
    }
    memcpy(this->comp, vect.comp, this->dim*sizeof(double));
    return *this;
}


//Implem for
// Dvector & Dvector::operator = (const Dvector & vect) {
//     if (this->dim == 0) {
//         this->dim = vect.dim;
//         this->comp = new double[this->dim];
//     } else if (this->dim != vect.dim){
//         delete this->comp;
//         this->dim = vect.dim;
//         this->comp = new double[this->dim];
//     }
//     for (int i=0; i < this->dim; i++) {
//         this->comp[i] = vect(i);
//     }
//     return *this;
// }

bool Dvector::operator == (const Dvector &vect) {
    if (this->dim != vect.dim) {
        return false;
    }
    for (int i=0; i<this->dim; i++) {
        if (this->comp[i] != vect(i)) {
            return false;
        }
    }
    return true;
}

//Externes
Dvector operator - (const Dvector & vect1, const Dvector & vect2) {  //vect1-vect2
	if (vect1.size() == vect2.size()){
		Dvector result=Dvector(vect1);
		result-=vect2;
		return result;
	}
	exit(-1); // dimensions incompatibles
}

Dvector operator + (const Dvector & vect1, const Dvector & vect2) {
	if (vect1.size() == vect2.size()){
		Dvector result=Dvector(vect1);
		result+=vect2;
		return result;
	}
	exit(-1); // dimensions incompatibles
}

Dvector operator + (const Dvector & vect, const double operand) {
    Dvector res = Dvector(vect);
    return res+=operand;
}

Dvector operator + (const double operand, const Dvector & vect) {
    return vect + operand;
}

Dvector operator - (const Dvector & vect, const double operand) {
    Dvector res = Dvector(vect);
    return res-=operand;
}

Dvector operator * (const double operand, const Dvector & vect) {
    Dvector res = Dvector(vect);
    return res*=operand;
}

Dvector operator * (const Dvector & vect, const double operand) {
    return operand*vect;
}

Dvector operator / (const Dvector & vect, const double operand) {
    Dvector res = Dvector(vect);
    return res/=operand;
}

Dvector operator - (const Dvector & vect) {
  Dvector res=Dvector(vect.size());
  for (int i = 0; i<vect.size(); i++) {
    res(i) = - vect(i);
  }
  return res;
}
ostream & operator <<(ostream & out, const Dvector &  v){
  for (int i=0; i<v.size();i++){
    out << v(i)<< " ";
  }
  return out;
}
istream & operator >>(istream & in, Dvector &  v){
  for (int i=0; i<v.size();i++){
    in >> v(i);
  }
  return in;
}
