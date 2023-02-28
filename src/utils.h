#ifndef UTILS_H
#define UTILS_H

#include "includes.h"

template<glm::length_t L, typename T, glm::qualifier Q>
inline std::ostream& operator<<(std::ostream& out, const glm::vec<L, T, Q>& v)
{
	out << '(';
	for (glm::length_t i = 0; i < L; i++)
	{
		if (i)
		{
			out << ", ";
		}
		out << v[i];
	}
	return out << ')';
}

#endif