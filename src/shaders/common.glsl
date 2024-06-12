R""(
#version 330 core

const uint max32 = 0xffffffffu;

float h2f(uint h)
{
	return float(h) / float(max32);
}

// taken from https://nullprogram.com/blog/2018/07/31/
uint hash(uint x)
{
	x ^= x >> 16;
	x *= 0x7feb352dU;
	x ^= x >> 15;
	x *= 0x846ca68bU;
	x ^= x >> 16;
	return x;
}
uint hash(float x)
{
	return hash(floatBitsToUint(x));
}
uint hash(vec2 v)
{
	return hash(floatBitsToUint(v.x))
		^ hash(floatBitsToUint(v.y) ^ 0x1u);
}
uint hash(vec3 v)
{
	return hash(floatBitsToUint(v.x))
		^ hash(floatBitsToUint(v.y) ^ 0x1u)
		^ hash(floatBitsToUint(v.z) ^ 0x2u);
}
uint hash(vec4 v)
{
	return hash(floatBitsToUint(v.x))
		^ hash(floatBitsToUint(v.y) ^ 0x1u)
		^ hash(floatBitsToUint(v.z) ^ 0x2u)
		^ hash(floatBitsToUint(v.w) ^ 0x4u);
}

// 1D unoise
float unoise1(uint  seed) { return h2f(hash(seed)); }
float unoise1(float seed) { return h2f(hash(seed)); }
float unoise1(vec2  seed) { return h2f(hash(seed)); }
float unoise1(vec3  seed) { return h2f(hash(seed)); }
float unoise1(vec4  seed) { return h2f(hash(seed)); }

// 2D unoise
vec2 unoise2(uint seed)
{
	uint h = hash(seed);
	return vec2(h2f(h), h2f(hash(h)));
}
vec2 unoise2(float seed)
{
	uint h = hash(seed);
	return vec2(h2f(h), h2f(hash(h)));
}
vec2 unoise2(vec2 seed)
{
	uint h = hash(seed);
	return vec2(h2f(h), h2f(hash(h)));
}
vec2 unoise2(vec3 seed)
{
	uint h = hash(seed);
	return vec2(h2f(h), h2f(hash(h)));
}
vec2 unoise2(vec4 seed)
{
	uint h = hash(seed);
	return vec2(h2f(h), h2f(hash(h)));
}

// 3D unoise
vec3 unoise3(uint seed)
{
	uint h1 = hash(seed);
	uint h2 = hash(h1);
	return vec3(h2f(h1), h2f(h2), h2f(hash(h2)));
}
vec3 unoise3(float seed)
{
	uint h1 = hash(seed);
	uint h2 = hash(h1);
	return vec3(h2f(h1), h2f(h2), h2f(hash(h2)));
}
vec3 unoise3(vec2 seed)
{
	uint h1 = hash(seed);
	uint h2 = hash(h1);
	return vec3(h2f(h1), h2f(h2), h2f(hash(h2)));
}
vec3 unoise3(vec3 seed)
{
	uint h1 = hash(seed);
	uint h2 = hash(h1);
	return vec3(h2f(h1), h2f(h2), h2f(hash(h2)));
}
vec3 unoise3(vec4 seed)
{
	uint h1 = hash(seed);
	uint h2 = hash(h1);
	return vec3(h2f(h1), h2f(h2), h2f(hash(h2)));
}

// 4D unoise
vec4 unoise4(uint seed)
{
	uint h1 = hash(seed);
	uint h2 = hash(h1);
	uint h3 = hash(h2);
	return vec4(h2f(h1), h2f(h2), h2f(h3), h2f(hash(h3)));
}
vec4 unoise4(float seed)
{
	uint h1 = hash(seed);
	uint h2 = hash(h1);
	uint h3 = hash(h2);
	return vec4(h2f(h1), h2f(h2), h2f(h3), h2f(hash(h3)));
}
vec4 unoise4(vec2 seed)
{
	uint h1 = hash(seed);
	uint h2 = hash(h1);
	uint h3 = hash(h2);
	return vec4(h2f(h1), h2f(h2), h2f(h3), h2f(hash(h3)));
}
vec4 unoise4(vec3 seed)
{
	uint h1 = hash(seed);
	uint h2 = hash(h1);
	uint h3 = hash(h2);
	return vec4(h2f(h1), h2f(h2), h2f(h3), h2f(hash(h3)));
}
vec4 unoise4(vec4 seed)
{
	uint h1 = hash(seed);
	uint h2 = hash(h1);
	uint h3 = hash(h2);
	return vec4(h2f(h1), h2f(h2), h2f(h3), h2f(hash(h3)));
}
)""