R""(
#version 330 core

const uint max32 = 0xffffffffu;

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

// 1D unoise
float unoise1(uint seed)
{
	return float(hash(seed)) / float(max32);
}
float unoise1(float seed)
{
	return unoise1(floatBitsToUint(seed));
}
float unoise1(vec2 seed)
{
	return unoise1(floatBitsToUint(seed.x) ^ floatBitsToUint(seed.y));
}
float unoise1(vec3 seed)
{
	return unoise1(floatBitsToUint(seed.x) ^ floatBitsToUint(seed.y) ^ floatBitsToUint(seed.z));
}
float unoise1(vec4 seed)
{
	return unoise1(floatBitsToUint(seed.x) ^ floatBitsToUint(seed.y) ^ floatBitsToUint(seed.z) ^ floatBitsToUint(seed.w));
}

// 2D unoise
vec2 unoise2(uint seed)
{
	return vec2(unoise1(seed), unoise1(seed ^ 0x1u));
}
vec2 unoise2(float seed)
{
	return unoise2(floatBitsToUint(seed));
}
vec2 unoise2(vec2 seed)
{
	return unoise2(floatBitsToUint(seed.x) ^ floatBitsToUint(seed.y));
}
vec2 unoise2(vec3 seed)
{
	return unoise2(floatBitsToUint(seed.x) ^ floatBitsToUint(seed.y) ^ floatBitsToUint(seed.z));
}
vec2 unoise2(vec4 seed)
{
	return unoise2(floatBitsToUint(seed.x) ^ floatBitsToUint(seed.y) ^ floatBitsToUint(seed.z) ^ floatBitsToUint(seed.w));
}

// 3D unoise
vec3 unoise3(uint seed)
{
	return vec3(unoise1(seed), unoise1(seed ^ 0x1u), unoise1(seed ^ 0x2u));
}
vec3 unoise3(float seed)
{
	return unoise3(floatBitsToUint(seed));
}
vec3 unoise3(vec2 seed)
{
	return unoise3(floatBitsToUint(seed.x) ^ floatBitsToUint(seed.y));
}
vec3 unoise3(vec3 seed)
{
	return unoise3(floatBitsToUint(seed.x) ^ floatBitsToUint(seed.y) ^ floatBitsToUint(seed.z));
}
vec3 unoise3(vec4 seed)
{
	return unoise3(floatBitsToUint(seed.x) ^ floatBitsToUint(seed.y) ^ floatBitsToUint(seed.z) ^ floatBitsToUint(seed.w));
}

// 4D unoise
vec4 unoise4(uint seed)
{
	return vec4(unoise1(seed), unoise1(seed ^ 0x1u), unoise1(seed ^ 0x2u), unoise1(seed ^ 0x3u));
}
vec4 unoise4(float seed)
{
	return unoise4(floatBitsToUint(seed));
}
vec4 unoise4(vec2 seed)
{
	return unoise4(floatBitsToUint(seed.x) ^ floatBitsToUint(seed.y));
}
vec4 unoise4(vec3 seed)
{
	return unoise4(floatBitsToUint(seed.x) ^ floatBitsToUint(seed.y) ^ floatBitsToUint(seed.z));
}
vec4 unoise4(vec4 seed)
{
	return unoise4(floatBitsToUint(seed.x) ^ floatBitsToUint(seed.y) ^ floatBitsToUint(seed.z) ^ floatBitsToUint(seed.w));
}
)""