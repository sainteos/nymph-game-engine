#version 330 core
precision highp float;

struct Light
{
  vec3 position;
  vec3 color;
  float intensity;
  float linear_attenuation;
  float quadratic_attenuation;
  float cone_angle;
  vec3 cone_direction;
  float quantization_bandwidth;
  int number_quantized_bands;
};

const int MAX_LIGHTS = 8;

in vec2 uv;
in vec3 surface_pos;

layout(location = 0)out vec4 fragColor;

uniform sampler2D tileset;
uniform int num_lights;
uniform Light lights[MAX_LIGHTS];

uniform vec3 ambient_color;    //ambient RGB
uniform float ambient_intensity;

vec3 applyLight(Light light, vec3 surface_color, vec3 normal, vec3 surface_pos) {
  vec3 surface_to_light = normalize(vec3(light.position.xy - surface_pos.xy, light.position.z));
  float distance_to_light = length(vec3(light.position.xy - surface_pos.xy, light.position.z));

  float attenuation = 1.0 / ( 1.0 + (light.linear_attenuation * distance_to_light) + (light.quadratic_attenuation * distance_to_light * distance_to_light));

  //cone restrictions (affects attenuation)
  float light_to_surface_angle = degrees(acos(dot(-surface_to_light, normalize(light.cone_direction))));
  if(light_to_surface_angle > light.cone_angle){
    attenuation = 0.0;
  }

  //diffuse
  float diffuse_coefficient = max(0.0, dot(normal, surface_to_light));
  //Should we quantize?
  if(light.number_quantized_bands > 0 && diffuse_coefficient > 0.0) {
    diffuse_coefficient = floor(diffuse_coefficient * float(light.number_quantized_bands + 1.0)) / float(light.number_quantized_bands + 1.0);
  }

  vec3 diffuse = diffuse_coefficient * light.color * light.intensity;

  
  vec3 intensity = attenuation * diffuse;
  //linear color (color before gamma correction)
  return clamp(intensity * surface_color.rgb, vec3(0.0), vec3(1.0));
}

void main()
{
  vec4 texel = texture(tileset, uv);
  if(texel.a < 0.1)
    discard;

  vec3 normal = vec3(0.0, 0.0, 1.0);

  vec3 final_color = vec3(0.0, 0.0, 0.0);

  //ambient
  vec3 ambient = ambient_color.rgb * ambient_intensity * texel.rgb;


  for(int i = 0; i < num_lights; i++) {
    final_color += applyLight(lights[i], texel.rgb, normal, surface_pos);
  }

  fragColor = vec4(clamp(final_color + ambient, vec3(0.0), vec3(1.0)), texel.a);
}