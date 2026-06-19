#version 330 core
// This source code is property of the Computer Graphics and Visualization 
// chair of the TU Dresden. Do not distribute! 
// Copyright (C) CGV TU Dresden - All Rights Reserved

in vec4 position;
in vec2 offset;

out vec3 n;
out vec3 t;
out vec3 b;
out vec3 worldPos;
uniform mat4 mvp;

// Returns the height of the procedural terrain at a given xz position
float getTerrainHeight(vec2 p);

void main()
{
    float epsilon = 0.01;

    float hL = getTerrainHeight(position.xz - vec2(epsilon, 0.0));
    float hR = getTerrainHeight(position.xz + vec2(epsilon, 0.0));
    float hD = getTerrainHeight(position.xz - vec2(0.0, epsilon));
    float hU = getTerrainHeight(position.xz + vec2(0.0, epsilon));

    vec3 dx = vec3(2.0 * epsilon, hR - hL, 0.0);
    vec3 dz = vec3(0.0, hU - hD, 2.0 * epsilon);

    t = normalize(dx);
    b = normalize(dz);
    n = normalize(cross(b, t));

    float height = getTerrainHeight(position.xz);
    vec4 pos = position;
    pos.y = height;

    worldPos = pos.xyz + vec3(offset.x, 0.0, offset.y);

gl_Position = mvp * vec4(worldPos, 1.0);
}


//source: https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
float rand(vec2 c)
{
	return 2 * fract(sin(dot(c.xy ,vec2(12.9898,78.233))) * 43758.5453) - 1;
}

float perlinNoise(vec2 p )
{
	vec2 ij = floor(p);
	vec2 xy = p - ij;
	//xy = 3.*xy*xy-2.*xy*xy*xy;
	xy = .5*(1.-cos(3.1415926 * xy));
	float a = rand((ij+vec2(0.,0.)));
	float b = rand((ij+vec2(1.,0.)));
	float c = rand((ij+vec2(0.,1.)));
	float d = rand((ij+vec2(1.,1.)));
	float x1 = mix(a, b, xy.x);
	float x2 = mix(c, d, xy.x);
	return mix(x1, x2, xy.y);
}

//based on https://www.seedofandromeda.com/blogs/58-procedural-heightmap-terrain-generation
float getTerrainHeight(vec2 p)
{
	float total = 0.0;
	float maxAmplitude = 0.0;
	float amplitude = 1.0;
	float frequency = 0.02;
	for (int i = 0; i < 11; i++) 
	{
		total +=  ((1.0 - abs(perlinNoise(p * frequency))) * 2.0 - 1.0) * amplitude;
		frequency *= 2.0;
		maxAmplitude += amplitude;
		amplitude *= 0.45;
	}
	return 15 * total / maxAmplitude;
}
