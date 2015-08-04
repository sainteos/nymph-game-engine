#version 330 core
precision highp float;

in vec2 uv;
layout(location = 0)out vec4 fragColor;

uniform sampler2D sample;

void main()
{
  fragColor = texture(sample, uv);
}