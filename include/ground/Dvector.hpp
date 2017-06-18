#ifndef DVECTORH
#define DVECTORH
#include <ostream>
#include <string>

using namespace std;

class Dvector
{
    private :
    int dim;
    double * comp;

    public :
    Dvector(const int d, const double init=0);
    Dvector();
    Dvector(const Dvector & vect);
    Dvector(string filename);
    ~Dvector();
    void display(std::ostream& str);
    int size() const;
    void fillRandomly();
    Dvector & resize(int newsize, double elem=0.0);
    double & operator () (int i); //acces par alias (modif possible !!)
    double operator () (int i) const;
    double prodscal (Dvector vect);
    // surcharge en interne : opérations sur l'objet lui meme.
    Dvector & operator += (const Dvector &);
    Dvector & operator -= (const Dvector &);
    Dvector & operator *= (const Dvector &);
    //
    Dvector & operator += (const double operand);
    Dvector & operator -= (const double operand);
    Dvector & operator *= (const double operand);
    Dvector & operator /= (const double operand);

    Dvector & operator = (const Dvector & vect);

    bool operator == (const Dvector &);
};
// Surcharge en externe: Permet de garder la symetrie (on crée un nouveau objet pour stocker le resultat)
Dvector operator - (const Dvector & vect1, const Dvector & vect2);
Dvector operator + (const Dvector & vect1, const Dvector & vect2);

Dvector operator + (const double operand, const Dvector & vect);
Dvector operator + (const Dvector & vect, const double operand);
Dvector operator - (const Dvector & vect, const double operand);
Dvector operator * (const double operand, const Dvector & vect);
Dvector operator * (const Dvector & vect, const double operand);
Dvector operator / (const Dvector & vect, const double operand);

Dvector operator - (const Dvector & vect);
ostream & operator <<(ostream & out, const Dvector & v);
istream & operator >>(istream & in, Dvector &  v);

#endif
