#ifndef HEIGHTH
#define HEIGHTH
#include <vector>
#include "Dvector.hpp"

class Height
{
    private :
    double Lx, Ly;
    std::vector<Dvector> heightMatrix;

    public :
    Height(const int nx, const int ny, const double Lx, const double Ly, const double init=0);
    Height();
    Height(const Height & hgt);
    ~Height();
    void display(std::ostream& str);
    //void displayFile(std::ostream& str);
    double operator()(const int x, const int y) const;
	void setHeight(const int x, const int y, double h);


    uint sizeNx() const;
    uint sizeNy() const;
    double getLx() const;
    double getLy() const;
};

#endif
