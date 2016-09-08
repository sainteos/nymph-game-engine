#version 330 core
precision highp float;

in vec2 uv;
flat in int unit;
layout(location = 0)out vec4 fragColor;

uniform sampler2D tileset0;
uniform sampler2D tileset1;
uniform sampler2D tileset2;
uniform sampler2D tileset3;
uniform sampler2D tileset4;
uniform sampler2D tileset5;
uniform sampler2D tileset6;
uniform sampler2D tileset7;
uniform sampler2D tileset8;
uniform sampler2D tileset9;
uniform sampler2D tileset10;
uniform sampler2D tileset11;

vec4 selectTexel(int unit) {
  if(unit == 0)
    return texture(tileset0, uv);
  else if(unit == 1)
    return texture(tileset1, uv);
  else if(unit == 2)
    return texture(tileset2, uv);
  else if(unit == 3)
    return texture(tileset3, uv);
  else if(unit == 4)
    return texture(tileset4, uv);
  else if(unit == 5)
    return texture(tileset5, uv);
  else if(unit == 6)
    return texture(tileset6, uv);
  else if(unit == 7)
    return texture(tileset7, uv);
  else if(unit == 8)
    return texture(tileset8, uv);
  else if(unit == 9)
    return texture(tileset9, uv);
  else if(unit == 10)
    return texture(tileset10, uv);
  else if(unit == 11)
    return texture(tileset11, uv);
  else
    return vec4(1.0, 1.0, 1.0, 1.0);
}

void main()
{
  vec4 texel = selectTexel(unit); 
  if(texel.a < 0.1)
    discard;
  fragColor = texel;
}