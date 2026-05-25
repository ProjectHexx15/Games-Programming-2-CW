#version 330 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
out float visibility;

const float fogDensity = 0.01;
const float fogGradient = 2;

void main()
{
	vec4 worldPos = model * vec4(VertexPosition, 1.0);
	FragPos = worldPos.xyz;

	Normal = mat3(transpose(inverse(model))) * VertexNormal;
	TexCoord = VertexTexCoord;

	float distance = length(cameraPos - FragPos);
	visibility = exp(-pow((distance * fogDensity), fogGradient));
	visibility = clamp(visibility, 0.0, 1.0);

	gl_Position = projection * view * worldPos;
}