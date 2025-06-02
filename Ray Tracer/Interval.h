#ifndef INTERVAL_H
#define INTERVAL_H

class Interval
{
public:
	double m_min;
	double m_max;

	Interval();
	Interval(double min, double max);
	Interval(const Interval& a, const Interval& b);
	double m_size() const;
	bool m_contains(double x) const;
	bool m_surrounds(double x) const;
	double m_clamp(double x) const;
	Interval m_expand(double delta) const;

	static const Interval s_empty, s_universe;
};

#endif