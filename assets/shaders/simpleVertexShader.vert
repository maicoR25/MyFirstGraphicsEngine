#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec3 position;
out vec2 TexCoord;

uniform float horizontalOffset;
//uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	ourColor = aColor;
	position = vec3(aPos.x + horizontalOffset, aPos.y, aPos.z);
	TexCoord = aTexCoord;
};