#include "Utility.h"

Vec3& Vec3::operator+=(const Vec3& v)
{
    m_elements[0] += v.m_elements[0];
    m_elements[1] += v.m_elements[1];
    m_elements[2] += v.m_elements[2];
    return *this;
}

Vec3& Vec3::operator*=(double t)
{
    m_elements[0] *= t;
    m_elements[1] *= t;
    m_elements[2] *= t;
    return *this;
}

Vec3& Vec3::operator/=(double t)
{
    return *this *= (1 / t);
}

double Vec3::m_length() const
{
    return std::sqrt(m_lengthSquared());
}

double Vec3::m_lengthSquared() const
{
    return m_elements[0] * m_elements[0] + m_elements[1] * m_elements[1] + m_elements[2] * m_elements[2];
}

bool Vec3::m_nearZero() const
{
    //Return true if the vector is close to zero in all dimensions.
    auto s = 1e-8;
    return (std::fabs(m_elements[0]) < s && std::fabs(m_elements[1]) < s && std::fabs(m_elements[2]) < s);
}

Vec3 Vec3::s_random()
{
    return Vec3(g_randomDouble(), g_randomDouble(), g_randomDouble());
}

Vec3 Vec3::s_random(double min, double max)
{
    return Vec3(g_randomDouble(min, max), g_randomDouble(min, max), g_randomDouble(min, max));
}

//Vector utility functions:
inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
    return out << v.m_elements[0] << ' ' << v.m_elements[1] << ' ' << v.m_elements[2];
}

inline Vec3 operator+(const Vec3& u, const Vec3& v)
{
    return Vec3(u.m_elements[0] + v.m_elements[0],
        u.m_elements[1] + v.m_elements[1],
        u.m_elements[2] + v.m_elements[2] );
}

inline Vec3 operator-(const Vec3& u, const Vec3& v)
{
    return Vec3(u.m_elements[0] - v.m_elements[0],
        u.m_elements[1] - v.m_elements[1],
        u.m_elements[2] - v.m_elements[2]);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v)
{
    return Vec3(u.m_elements[0] * v.m_elements[0],
        u.m_elements[1] * v.m_elements[1],
        u.m_elements[2] * v.m_elements[2]);
}

inline Vec3 operator*(double t, const Vec3& v)
{
    return Vec3(t * v.m_elements[0],
        t * v.m_elements[1],
        t * v.m_elements[2]);
}

inline Vec3 operator*(const Vec3& v, double t)
{
    return t * v;
}

inline Vec3 operator/(const Vec3& v, double t)
{
    return (1/t) * v;
}

inline double g_dot(const Vec3& u, const Vec3& v)
{
    return u.m_elements[0] * v.m_elements[0] + 
        u.m_elements[1] * v.m_elements[1] + 
        u.m_elements[2] * v.m_elements[2];
}

inline Vec3 g_cross(const Vec3& u, const Vec3& v)
{
    return Vec3(u.m_elements[1] * v.m_elements[2] - u.m_elements[2] * v.m_elements[1],
        u.m_elements[2] * v.m_elements[0] - u.m_elements[0] * v.m_elements[2],
        u.m_elements[0] * v.m_elements[1] - u.m_elements[1] * v.m_elements[0]);
}

inline Vec3 g_unitVector(const Vec3& v)
{
    return v / v.m_length();
}

inline Vec3 g_randomUnitVector()
{
    while (true) {
        auto p = Vec3::s_random(-1, 1);
        auto lengthSquared = p.m_lengthSquared();
        if (1e-160 < lengthSquared <= 1)
            return p / sqrt(lengthSquared);
   }
}

inline Vec3 g_randomOnHemisphere(const Vec3& normal)
{
    Vec3 onUnitSphere = g_randomUnitVector();
    if (g_dot((onUnitSphere), normal) > 0.0) //In the same sphere as the normal
        return onUnitSphere;
    else
        return -onUnitSphere;
}

inline Vec3 g_randomInUnitDisk()
{
    while (true) {
        auto p = Vec3(g_randomDouble(-1, 1), g_randomDouble(-1, 1), 0.0);
        if (p.m_lengthSquared() < 1)
            return p;
   }
}

inline Vec3 g_reflect(const Vec3& v, const Vec3& n)
{
    return v - 2 * g_dot(v, n) * n;
}

inline Vec3 g_refract(const Vec3& uv, const Vec3& n, double etaiOverEtat)
{
    auto cosTheta = std::fmin(g_dot(-uv, n), 1.0);
    Vec3 rOutPerpendicular = etaiOverEtat * (uv + cosTheta * n);
    Vec3 rOutParallel = -std::sqrt(std::abs(1.0 - rOutPerpendicular.m_lengthSquared())) * n;
    return rOutPerpendicular + rOutParallel;
}
