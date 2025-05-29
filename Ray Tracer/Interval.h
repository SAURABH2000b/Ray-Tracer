#ifndef INTERVAL_H
#define INTERVAL_H

class Interval
{
public:
	double m_min;
	double m_max;

	Interval();
	Interval(double min, double max);
	double m_size() const;
	bool m_contains(double x) const;
	bool m_surrounds(double x) const;
	double m_clamp(double x) const;

	static const Interval s_empty, s_universe;
};

#endif