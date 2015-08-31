#version 330 core
precision highp float;

in vec2 uv;
layout(location = 0)out vec4 fragColor;

uniform sampler2D tileset;

void main()
{
  vec4 texel = texture(tileset, uv); 
  if(texel.a < 0.1)
    discard;
  fragColor = texel;
}