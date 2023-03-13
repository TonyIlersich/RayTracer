R""(
#version 330 core

in vec3 rayOrigin;
in vec3 rayDirection;

out vec4 FragColor;

const vec3 up = vec3(0.0, 1.0, 0.0);

const float infinity = 1e+38 * 10.0;

const uint SURFACE_TYPE_CUBE     = 0u;
const uint SURFACE_TYPE_CYLINDER = SURFACE_TYPE_CUBE + 1u;
const uint SURFACE_TYPE_DISC     = SURFACE_TYPE_CYLINDER + 1u;
const uint SURFACE_TYPE_PLANE    = SURFACE_TYPE_DISC + 1u;
const uint SURFACE_TYPE_QUAD     = SURFACE_TYPE_PLANE + 1u;
const uint SURFACE_TYPE_SPHERE   = SURFACE_TYPE_QUAD + 1u;

const float RAY_EPSILON = 1e-4;
const uint MAX_RAY_BOUNCES = 4u;
const vec3 MIN_RAY_SIGNIFICANCE = vec3(0.001);
// const uint SAMPLES_PER_PIXEL = 1u;

const uint MAX_SURFACES = 64u;
const uint MAX_MATERIALS = 16u;

uniform float time;

// NOTE: vec3 is same size as vec4

// size = 4096 + 256 + 256 + 256 + 256 + 64 + 4 = 5188 < 16384 = minimum guaranteed
layout(packed) uniform SceneBlock
{
	mat4  worldToLocal[MAX_SURFACES];        // size = sizeof(mat4) * MAX_SURFACES  = 64 * 64 = 4096
	uint  surfaceType[MAX_SURFACES];         // size = sizeof(uint) * MAX_SURFACES  =  4 * 64 = 256
	uint  surfaceMaterialId[MAX_SURFACES];   // size = sizeof(uint) * MAX_SURFACES  =  4 * 64 = 256
	vec3  materialEmission[MAX_MATERIALS];   // size = sizeof(vec3) * MAX_MATERIALS = 16 * 16 = 256
	vec4  materialAlbedo[MAX_MATERIALS];     // size = sizeof(vec4) * MAX_MATERIALS = 16 * 16 = 256
	float materialGlossiness[MAX_MATERIALS]; // size = sizeof(float) * MAX_MATERIALS = 4 * 16 = 64
	uint  surfaceCount;                      // size = sizeof(uint)                           = 4
};

// Forward declare functions from common.glsl
uint hash(uint seed);
float unoise1(uint seed);
float unoise1(float seed);
float unoise1(vec2 seed);
float unoise1(vec3 seed);
float unoise1(vec4 seed);
vec2 unoise2(uint seed);
vec2 unoise2(float seed);
vec2 unoise2(vec2 seed);
vec2 unoise2(vec3 seed);
vec2 unoise2(vec4 seed);
vec3 unoise3(uint seed);
vec3 unoise3(float seed);
vec3 unoise3(vec2 seed);
vec3 unoise3(vec3 seed);
vec3 unoise3(vec4 seed);
vec4 unoise4(uint seed);
vec4 unoise4(float seed);
vec4 unoise4(vec2 seed);
vec4 unoise4(vec3 seed);
vec4 unoise4(vec4 seed);

// Returns the least number that is above threshold, or if there no such number, a number that is below or equal to the threshold
float minThreshold(float a, float b, float threshold)
{
	return a > threshold ? (b > threshold ? min(a, b) : a) : b;
}

void doBounce(
	in vec3 point, in vec3 normal, in uint materialId,
	inout vec3 colorSoFar, inout vec3 rayDirection, inout vec3 bounceSignificance)
{
	vec3 apparentNormal = normalize(normal * -sign(dot(rayDirection, normal)));

	// Calculate the color of this bounce (local sources only)
	vec3 bounceColor = materialEmission[materialId] * max(0.0, -dot(normalize(rayDirection), normalize(normal)));

	// Add contribution from this bounce to ray color
	colorSoFar += bounceColor * bounceSignificance;

	// Calculate the significance of the next bounce
	bounceSignificance *= materialAlbedo[materialId].rgb;

	// Calculate the bounce direction
	// TODO: calculate eta and handle TIR
	vec3 refractDirection = refract(rayDirection, apparentNormal, 1.0 / (1.0 + 0.3 * float(normal == apparentNormal))); // NOTE: this eta calculation is temporary
	bool willRefract = refractDirection != vec3(0.0) && unoise1(vec4(point, time)) > materialAlbedo[materialId].a;
	rayDirection = willRefract ? refractDirection : reflect(rayDirection, apparentNormal);
	vec3 bounceNormal = willRefract ? -apparentNormal : apparentNormal;

	// Apply gloss
	vec3 gloss = unoise3(vec4(point, time));
	vec3 perp1 = normalize(cross(rayDirection, apparentNormal));
	vec3 perp2 = cross(perp1, apparentNormal); // already normalized
	vec3 glossDirection = mat3(perp1, perp2, bounceNormal) * vec3(gloss.xy * 2.0 - 1.0, gloss.z);
	float glossiness = materialGlossiness[materialId];
	rayDirection = normalize(glossDirection * glossiness + rayDirection * (1.0 - glossiness));
}

void intersectPlane(vec3 rayOrigin, vec3 rayDirection, inout float t, inout vec3 normal, vec2 clipBounds, float clipSqrRadius)
{
	float t1 = -rayOrigin.y / rayDirection.y;
	vec2 p = rayOrigin.xy + t1 * rayDirection.xy;
	bool didHit = (t1 > RAY_EPSILON && t1 < t) && all(lessThanEqual(abs(p), clipBounds)) && dot(p, p) <= clipSqrRadius;
	t = didHit ? t1 : t;
	normal = didHit ? up : normal;
}

void intersectSphere(vec3 rayOrigin, vec3 rayDirection, inout float t, inout vec3 normal)
{
	float d2 = dot(rayDirection, rayDirection);
	float od = dot(rayOrigin, rayDirection);
	float discriminant = od * od - d2 * (dot(rayOrigin, rayOrigin) - 1.f);
	if (discriminant < 0.0)
	{
		return;
	}
	float rootDisc = sqrt(discriminant);
	float t1 = minThreshold((-od + rootDisc) / d2, (-od - rootDisc) / d2, RAY_EPSILON);
	if (t1 > RAY_EPSILON && t1 < t)
	{
		t = t1;
		normal = rayOrigin + t * rayDirection;
	}
}

void intersect(
	uint surfaceId, vec3 rayOrigin, vec3 rayDirection,
	inout float t, inout vec3 normal, inout uint hitSurfaceId)
{
	uint type = surfaceType[surfaceId];
	vec3 localRayOrigin = vec3(worldToLocal[surfaceId] * vec4(rayOrigin, 1.0));
	vec3 localRayDirection = vec3(worldToLocal[surfaceId] * vec4(rayDirection, 0.0));
	float localT = infinity;
	vec3 localNormal;
	switch (type)
	{
		case SURFACE_TYPE_PLANE:
		case SURFACE_TYPE_DISC:
		case SURFACE_TYPE_QUAD:
			intersectPlane(localRayOrigin, localRayDirection, localT, localNormal,
				(type == SURFACE_TYPE_DISC) ? vec2(1.0) : vec2(infinity),
				(type == SURFACE_TYPE_DISC) ? 1.0 : infinity);
			break;
		case SURFACE_TYPE_SPHERE:
			intersectSphere(localRayOrigin, localRayDirection, localT, localNormal);
			break;
		case SURFACE_TYPE_CYLINDER:
		case SURFACE_TYPE_CUBE:
			// UNSUPPORTED
			break;
	}
	if (localT < t)
	{
		hitSurfaceId = surfaceId;
		mat4 localToWorld = inverse(worldToLocal[surfaceId]);
		// rayOrigin = vec3(localToWorld * vec4(localBounceDirection, 1.0));
		// rayDirection = vec3(localToWorld * vec4(localBounceDirection, 0.0));
		t = localT;
		normal = transpose(mat3(worldToLocal[surfaceId])) * localNormal;
	}
}

bool trace(inout vec3 rayOrigin, inout vec3 rayDirection, inout vec3 color, inout vec3 bounceSignificance)
{
	float t = infinity;
	vec3 normal;
	uint hitSurfaceId;
	for (uint i = 0u; i < surfaceCount; i++)
	{
		intersect(i, rayOrigin, rayDirection, t, normal, hitSurfaceId);
	}
	if (t != infinity)
	{
		rayOrigin = rayOrigin + rayDirection * t;
		doBounce(rayOrigin, normal, surfaceMaterialId[hitSurfaceId], color, rayDirection, bounceSignificance);
	}
	return t != infinity;
}

vec3 bleedChannels(vec3 color, float strength)
{
	vec3 excess = max(vec3(0.0), color - vec3(1.0));
	return clamp(color + strength * (excess.gbr + excess.brg), vec3(0.0), vec3(1.0));
}

void main()
{
	vec3 color = vec3(0.0);
	// for (uint s = 0u; s < SAMPLES_PER_PIXEL; s++)
	// {
		bool didHit = true;
		vec3 rayO = rayOrigin;
		vec3 rayD = rayDirection;
		vec3 bounceSignificance = vec3(1.0);
		for (uint i = 0u; i < MAX_RAY_BOUNCES && didHit && any(greaterThan(bounceSignificance, MIN_RAY_SIGNIFICANCE)); i++)
		{
			didHit = trace(rayO, rayD, color, bounceSignificance);
		}
	// }
	// FragColor = vec4(bleedChannels(color / float(SAMPLES_PER_PIXEL), 0.1), 1.0);
	FragColor = vec4(bleedChannels(color, 0.1), 1.0);
	// FragColor = vec4(color, 1.0);
}
)""