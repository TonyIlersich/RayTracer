#ifndef INTERVAL_H
#define INTERVAL_H

template <typename T>
struct Interval
{
	inline Interval(const T& minimum, const T& maximum, bool openMin, bool openMax):
		minimum(minimum), maximum(maximum), openMin(openMin), openMax(openMax)
	{}
	inline Interval(const T& minimum, const T& maximum):
		minimum(minimum), maximum(maximum), openMin(false), openMax(false)
	{}

	inline bool contains(const T& value) const
	{
		return (openMin ? (minimum < value) : (minimum <= value)) && (openMax ? (maximum > value) : (maximum >= value));
	}

	inline T normalize(const T& value) const
	{
		return (value - minimum) / (maximum - minimum);
	}

	inline T lerp(const T& t) const
	{
		return (T(1) - t) * minimum + t * maximum;
	}

	T minimum, maximum;
	bool openMin, openMax;
};

#endif