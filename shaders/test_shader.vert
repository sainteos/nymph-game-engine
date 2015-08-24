#version 330

layout(location = 0)in vec3 vert;
layout(location = 1)in vec3 vert2;
layout(location = 2)in vec3 vert3;
layout(location = 3)in vec3 vert4;
layout(location = 4)in vec3 vert5;
layout(location = 5)in vec3 vert6;
layout(location = 6)in vec3 vert7;
layout(location = 7)in vec3 vert8;
layout(location = 8)in vec3 vert9;
layout(location = 9)in vec3 vert10;

uniform mat4 transform;

void main()
{
  vec3 derp = vert2 * vert3 * vert4 * vert5 * vert6 * vert7 * vert8 * vert9;  
  gl_Position = transform * vec4(vert, 1.0);
}