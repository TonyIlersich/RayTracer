#ifndef UTILS_H
#define UTILS_H

#include "includes.h"

template<glm::length_t L, typename T, glm::qualifier Q>
inline std::ostream& operator<<(std::ostream& out, const glm::vec<L, T, Q>& v)
{
	out << '(' << v[0];
	for (glm::length_t i = 1; i < L; i++)
	{
		out << ", " << v[i];
	}
	return out << ')';
}

template<glm::length_t W, glm::length_t H, typename T, glm::qualifier Q>
inline std::ostream& operator<<(std::ostream& out, const glm::mat<W, H, T, Q>& m)
{
	for (glm::length_t i = 0; i < H; i++)
	{
		if (i)
		{
			out << '\n';
		}
		out << '[';
		for (glm::length_t j = 0; j < W; j++)
		{
			if (j)
			{
				out << "\t";
			}
			out << m[j][i];
		}
		out << ']';
	}
	return out;
}

template<glm::length_t L, typename T, glm::qualifier Q>
inline T maxComponent(const glm::vec<L, T, Q>& v)
{
	T result = v[0];
	for (glm::length_t i = 1; i < L; i++)
	{
		result = glm::max(result, v[i]);
	}
	return result;
}

template<glm::length_t L, typename T, glm::qualifier Q>
inline T minComponent(const glm::vec<L, T, Q>& v)
{
	T result = v[0];
	for (glm::length_t i = 1; i < L; i++)
	{
		result = glm::max(result, v[i]);
	}
	return result;
}

template<glm::length_t L, typename T, glm::qualifier Q>
inline T sumComponents(const glm::vec<L, T, Q>& v)
{
	return glm::dot(glm::vec<L, T, Q>(1), v);
}

template<glm::length_t L, typename T, glm::qualifier Q>
inline T avgComponent(const glm::vec<L, T, Q>& v)
{
	return sumComponents(v) / T(L);
}

#endif