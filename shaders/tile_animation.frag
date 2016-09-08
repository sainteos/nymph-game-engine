#version 330 core
precision highp float;

in vec2 uv;
flat in uint unit;
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

vec4 selectTexel(uint unit) {
  if(unit == 0u)
    return texture(tileset0, uv);
  if(unit == 1u)
    return texture(tileset1, uv);
  if(unit == 2u)
    return texture(tileset2, uv);
  if(unit == 3u)
    return texture(tileset3, uv);
  if(unit == 4u)
    return texture(tileset4, uv);
  if(unit == 5u)
    return texture(tileset5, uv);
  if(unit == 6u)
    return texture(tileset6, uv);
  if(unit == 7u)
    return texture(tileset7, uv);
  if(unit == 8u)
    return texture(tileset8, uv);
  if(unit == 9u)
    return texture(tileset9, uv);
  if(unit == 10u)
    return texture(tileset10, uv);
  if(unit == 11u)
    return texture(tileset11, uv);
}

void main()
{
  vec4 texel = selectTexel(unit); 
  if(texel.a < 0.1)
    discard;
  fragColor = texel;
}