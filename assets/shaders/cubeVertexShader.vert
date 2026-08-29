#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (std140) uniform CameraData {
    mat4 projection; 
    mat4 view;      
};

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;


void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0f));

	mat3 normalMatrix = mat3(transpose(inverse(model)));
	Normal = normalMatrix * aNormal;
};