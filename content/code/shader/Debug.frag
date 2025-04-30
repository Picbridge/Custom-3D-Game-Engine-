#version 400
out vec4 FragColor;
in float posColor;
uniform vec3 color;
void main()
{
    FragColor = vec4(color, 1.0); // Red color
}