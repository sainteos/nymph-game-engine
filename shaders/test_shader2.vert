#version 330

layout(location = 0)in vec3 vert;

uniform mat4 transform;
void main()
{
    gl_Position = transform * vec4(vert.x, vert.y, 0.0, 1.0);
}