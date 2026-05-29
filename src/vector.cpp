#include "libvrf/vector.hpp"

#include <sstream>


// DVec3
namespace kcei {

    DVec3::DVec3() : DtVector64() {}

    DVec3::DVec3(double x, double y, double z) : DtVector64(x, y, z) {}

    DVec3::DVec3(const DtVector32& vec) : DtVector64(DtVector32To64(vec)) {}

    DVec3::DVec3(const DtVector64& vec) : DtVector64(vec) {}

    DVec3 DVec3::operator+(const DVec3& rhs) const {
        DVec3 out;
        DtVecAdd(*this, rhs, out);
        return out;
    }

    DVec3 DVec3::operator-(const DVec3& rhs) const {
        DVec3 out;
        DtVecSub(*this, rhs, out);
        return out;
    }

    DVec3 DVec3::operator*(double rhs) const {
        DVec3 out;
        DtVecScale(*this, rhs, out);
        return out;
    }

    DVec3 DVec3::operator/(double rhs) const {
        DVec3 out;
        DtVecScale(*this, 1.0 / rhs, out);
        return out;
    }

    double DVec3::len_sqr() const { return this->dot(*this); }

    double DVec3::len() const { return std::sqrt(this->len_sqr()); }

    DVec3 DVec3::normalize() const {
        auto out = *this;
        out.DtVector64::normalize();
        return out;
    }

    double DVec3::dot(const DVec3& rhs) const { return this->dotProduct(rhs); }

    DVec3 DVec3::cross(const DVec3& rhs) const {
        return DVec3{ this->crossProduct(rhs) };
    }

    std::string DVec3::to_str() const { return this->string().stdString(); }

}  // namespace kcei
