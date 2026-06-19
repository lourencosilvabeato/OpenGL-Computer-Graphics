#version 330 core

// Vertex shader outputs
in vec3 n;        // normal
in vec3 t;        // tangente
in vec3 b;        // bitangente
in vec3 worldPos; // posição em world space

out vec4 color;

// Texture samplers
uniform sampler2D grassTexture;
uniform sampler2D rockTexture;
uniform sampler2D roadColorTexture;
uniform sampler2D roadNormalMap;
uniform sampler2D roadSpecularMap;
uniform sampler2D alphaMap;

// Camera
uniform vec3 cameraPos;

// Light
const vec3 dirToLight = normalize(vec3(1, 3, 1));

// Blinn-Phong lighting
vec4 calculateLighting(vec4 materialColor, float specularIntensity,
                       vec3 normalizedNormal, vec3 directionToViewer)
{
    vec4 col = materialColor;
    vec3 h = normalize(dirToLight + directionToViewer);
    col.xyz *= 0.9 * max(dot(normalizedNormal, dirToLight), 0.0) + 0.1;
    col.xyz += specularIntensity * pow(max(dot(h, normalizedNormal), 0.0), 50.0);
    return col;
}

void main()
{
    vec2 uvTerrain = worldPos.xz / 25.5; 
    vec2 uvRoad    = worldPos.xz / 5.0;  
    vec2 uvAlpha   = worldPos.xz / 255.0; 

    vec3 grassColor = texture(grassTexture, uvTerrain).rgb;
    vec3 rockColor  = texture(rockTexture,  uvTerrain).rgb;

    float slope = dot(normalize(n), vec3(0.0, 1.0, 0.0));
    float grassFactor = smoothstep(0.6, 0.9, slope);
    vec3 terrainColor = mix(rockColor, grassColor, grassFactor);

    vec3 roadColor    = texture(roadColorTexture, uvRoad).rgb;
    float roadSpecular= texture(roadSpecularMap, uvRoad).r;
    float alpha       = texture(alphaMap, uvAlpha).r;

    vec3 normalTS     = texture(roadNormalMap, uvRoad).rgb;
    normalTS = normalTS * 2.0 - 1.0; 
    normalTS.y *= -1.0;     

    mat3 TBN = mat3(normalize(t), normalize(b), normalize(n));
    vec3 normalMapped = normalize(TBN * normalTS);

    vec3 finalNormal = normalize(mix(normalize(n), normalMapped, alpha));

    vec3 finalColor = mix(terrainColor, roadColor, alpha);

    vec3 dirToViewer = normalize(cameraPos - worldPos);
    float specular = roadSpecular * alpha; // specular only for road
    color = calculateLighting(vec4(finalColor, 1.0), specular, finalNormal, dirToViewer);
}