#version 330 core

in vec2 TexCoord;
in float visibility;

uniform sampler2D terrainTexture;
uniform sampler2D flowerTerrainTexture;
uniform sampler2D pathTexture;
uniform sampler2D brickTexture;
uniform sampler2D blendMapTexture;

uniform vec3 fogColor;
uniform float ambientStrength;
uniform float edgeFogStrength;

uniform float uvGrass;
uniform float uvFlower;
uniform float uvPath;
uniform float uvBrick;

out vec4 FragColor;

void main()
{
    vec4 blendMap = texture(blendMapTexture, TexCoord);

    float r = blendMap.r;
    float g = blendMap.g;
    float b = blendMap.b;
    float backgroundAmount = 1.0 - (r + g + b);

    vec4 backgroundTex = texture(terrainTexture, TexCoord * uvGrass);
    vec4 rTex = texture(pathTexture, TexCoord * uvPath);
    vec4 gTex = texture(flowerTerrainTexture, TexCoord * uvFlower);
    vec4 bTex = texture(brickTexture, TexCoord * uvBrick);

    vec4 splatColor =backgroundTex * backgroundAmount + rTex * r + gTex * g + bTex * b;

    vec4 litColor = splatColor * ambientStrength;

    float finalVisibility = visibility * (1.0 - edgeFogStrength);

    FragColor = mix(vec4(fogColor, 1.0), litColor, finalVisibility);
}
