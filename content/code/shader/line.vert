#version 400 core
layout(location = 0) in vec3 position;
uniform mat4 WorldProjection, WorldView;
out float posColor;
void main()
{
    gl_Position = WorldProjection * WorldView * vec4(position, 1.0);
    posColor = gl_Position.y;
}