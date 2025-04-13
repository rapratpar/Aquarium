#version 330

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

in vec4 vertex;
in vec4 normal;
in vec2 texCoord;


out vec3 iFragPos;
out vec3 iNormal;
out vec2 iTexCoords;


void main()
{
    iFragPos = vec3(M * vertex);
    iNormal = mat3(transpose(inverse(M))) * vec3(normal);  
    iTexCoords = texCoord;
    
    gl_Position = P * V * vec4(iFragPos, 1.0);
}