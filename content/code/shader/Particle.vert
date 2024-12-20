#version 400 core

layout (location = 0) in vec3 aPos;

uniform mat4 u_View;
uniform mat4 u_Proj;
uniform mat4 u_Model;

void main()
{
	gl_Position = u_Proj * u_View * u_Model * vec4(aPos, 1.0);
}