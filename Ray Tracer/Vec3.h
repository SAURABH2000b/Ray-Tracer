#ifndef VEC3_H
#define VEC3_H

class Vec3
{
public:
	double m_elements[3];

	Vec3() : m_elements{0,0,0} {}
	Vec3(double e0, double e1, double e2): m_elements{e0, e1, e2} {}

	double m_x() const { return m_elements[0]; }
	double m_y() const { return m_elements[1]; }
	double m_z() const { return m_elements[2]; }

	Vec3 operator-() const { return Vec3(-m_elements[0], -m_elements[1], -m_elements[2]); }
	double operator[](int i) const { return m_elements[i]; } //to read. ex. double a = obj[1]; const is added to
											         //allow operator overloading with return type itself.
	double& operator[](int i) { return m_elements[i]; } //to modify. ex. obj[1] = 2.3;

	Vec3& operator+=(const Vec3& v);
	Vec3& operator*=(double t);
	Vec3& operator/=(double t);
	double m_length() const;
	double m_lengthSquared() const;
	bool m_nearZero() const;

	static Vec3 s_random();
	static Vec3 s_random(double min, double max);
};

//point3 is just an alias for Vec3, but useful for geometric clarity in the code.
using Point3 = Vec3;

//Vector utility functions:
extern inline std::ostream& operator<<(std::ostream& out, const Vec3& v);
extern inline Vec3 operator+(const Vec3& u, const Vec3& v);
extern inline Vec3 operator-(const Vec3& u, const Vec3& v);
extern inline Vec3 operator*(const Vec3& u, const Vec3& v);
extern inline Vec3 operator*(double t, const Vec3& v);
extern inline Vec3 operator*(const Vec3& v, double t);
extern inline Vec3 operator/(const Vec3& v, double t);
extern inline double g_dot(const Vec3& u, const Vec3& v);
extern inline Vec3 g_cross(const Vec3& u, const Vec3& v);
extern inline Vec3 g_unitVector(const Vec3& v);
extern inline Vec3 g_randomUnitVector();
extern inline Vec3 g_randomOnHemisphere(const Vec3& normal);
extern inline Vec3 g_randomInUnitDisk();
extern inline Vec3 g_reflect(const Vec3& v, const Vec3& n);
extern inline Vec3 g_refract(const Vec3& uv, const Vec3& n, double etaiOverEtat);

#endif