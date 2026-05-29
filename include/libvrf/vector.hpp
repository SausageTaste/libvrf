#pragma once

#include <matrix/vlVector.h>


namespace kcei {

    class DVec3 : public DtVector64 {

    public:
        DVec3();
        DVec3(double x, double y, double z);
        DVec3(const DtVector32& vec);
        DVec3(const DtVector64& vec);

        DVec3 operator+(const DVec3& rhs) const;
        DVec3 operator-(const DVec3& rhs) const;
        DVec3 operator*(double rhs) const;
        DVec3 operator/(double rhs) const;

        double len_sqr() const;
        double len() const;
        DVec3 normalize() const;

        double dot(const DVec3& rhs) const;
        DVec3 cross(const DVec3& rhs) const;

        std::string to_str() const;
    };

}  // namespace kcei
