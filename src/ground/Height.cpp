#include "../../include/ground/Height.hpp"
#include "../../include/ground/Dvector.hpp"
#include <cstdlib>


Height::Height(const int nx, const int ny, const double Lx, const double Ly, const double init) {
    this->heightMatrix = std::vector<Dvector>(nx, Dvector(ny, init));
    this->Lx = Lx;
    this->Ly = Ly;
}
Height::Height() {
    this->heightMatrix = std::vector<Dvector>();
    this->Lx = 0;
    this->Ly = 0;

}
Height::Height(const Height & hgt) {
    this->heightMatrix = std::vector<Dvector>(hgt.heightMatrix);
    this->Lx = hgt.getLx();
    this->Ly = hgt.getLy();

}
Height::~Height() {
}

void Height::display(std::ostream& str) {
    for (uint i = 0; i < this->heightMatrix.size(); i++) {
        this->heightMatrix.at(i).display(str);
        str << "\n";
    }
}

/*void Height::displayFile(std::ostream& str) {
    str << "# (X,   Y,   Z)\n";
    for (uint x = 0; x < this->sizeNx(); x++) {
        for (uint y = 0; y < this->sizeNy(); y++) {
            str << "(" << x*(this->Lx/this->sizeNx()) << ", " << y*(this->Ly/this->sizeNy()) << ", " << (*this)(x, y, 0) << ")" << endl;
            str << endl;
        }
    }
}*/

double Height::operator()(const int x, const int y) const{
    return this->heightMatrix.at(x)(y);
}

uint Height::sizeNx() const {
    return this->heightMatrix.size();
}
uint Height::sizeNy() const {
    if (this->sizeNx() == 0) {
        return 0;
    }
    return this->heightMatrix.at(0).size();
}

double Height::getLx() const {
    return this->Lx;
}
double Height::getLy() const {
    return this->Ly;
}


void Height::setHeight(const int x, const int y, double h)
{
	heightMatrix.at(x)(y) = h;
}
