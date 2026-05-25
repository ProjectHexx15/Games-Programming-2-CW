#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in float visibility;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 fogColor;

uniform sampler2D modelTexture;

uniform float ambientStrength;
uniform float specularStrength;
uniform float shininess;
uniform float edgeFogStrength;

uniform vec3 cameraPos;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    vec3 ambient = ambientStrength * lightColor;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 lighting = ambient + diffuse + specular;

    vec4 tex = texture(modelTexture, TexCoord);
    vec4 litColor = vec4(lighting, 1.0) * tex;

    float finalVisibility = visibility * (1.0 - edgeFogStrength);

    FragColor = mix(vec4(fogColor, 1.0), litColor, finalVisibility);
}
