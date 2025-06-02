#include "Interval.h"
#include "Utility.h"

//Default interval is empty
Interval::Interval()
{
	m_min = +infinity;
	m_max = -infinity;
}

Interval::Interval(double min, double max)
{
	m_min = min;
	m_max = max;
}

Interval::Interval(const Interval& a, const Interval& b)
{
	//Create the interval tightly enclosing the two input intervals.
	m_min = (a.m_min <= b.m_min) ? a.m_min : b.m_min;
	m_max = (a.m_max >= b.m_max) ? a.m_max : b.m_max;
}

double Interval::m_size() const
{
	return m_max - m_min;
}

bool Interval::m_contains(double x) const
{
	return m_min <= x && x <= m_max;
}

bool Interval::m_surrounds(double x) const
{
	return m_min < x && x < m_max;
}

double Interval::m_clamp(double x) const
{
	if (x < m_min) return m_min;
	if (x > m_max)return m_max;
}

Interval Interval::m_expand(double delta) const
{
	auto padding = delta / 2;
	return Interval(m_min - padding, m_max + padding);
}

const Interval Interval::s_empty = Interval(+infinity, -infinity);
const Interval Interval::s_universe = Interval(-infinity, +infinity);