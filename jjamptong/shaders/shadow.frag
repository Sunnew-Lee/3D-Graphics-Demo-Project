#version 460 core

precision mediump float;

uniform vec3 fogColor = vec3(0.5, 0.5, 0.5);
uniform vec3 u_lightColor = vec3(1.0, 1.0, 1.0);
uniform float u_ambient = 0.5;
uniform vec4 color;

uniform sampler2DShadow ShadowMap;

in vec2 UV;
in vec3 NRM;
in vec3 frag_pos;
in vec4 ShadowCoord; 
in vec3 u_light;

out vec4 outColor;

vec3 diffAndSpec() {
    vec3 n = normalize(NRM);
    vec3 s = normalize(u_light - frag_pos);

    vec3 v = normalize(-frag_pos);
    vec3 r = reflect(-s,n);
    float sn=max(dot(s,n),0.);//brightness
    vec3 diffuse = u_lightColor * sn;

    vec3 spec = vec3(0.);

    if(sn >0.) {
        spec = u_lightColor * pow(max(dot(r,v),0.0),u_ambient);
    }

    return diffuse + spec;
} 

subroutine void RenderPassType(); 
subroutine uniform RenderPassType RenderPass; 

subroutine (RenderPassType) 
void shadeWithShadow() {
  vec3 ambient = vec3(u_ambient); // compute ambient component here 
  vec3 diffSpec = diffAndSpec(); 
 
  // Do the shadow-map look-up 
  float shadow = textureProj(ShadowMap, ShadowCoord); 
 
  // If the fragment is in shadow, use ambient light only. 
    outColor = vec4((diffSpec * shadow + ambient) * color.xyz , 1.0); 
    outColor = mix(vec4(fogColor,1.),outColor,1.0);   /* Use obj's color as usual */
} 
subroutine (RenderPassType) 
void recordDepth() {
    // Do nothing, depth will be written automatically 
} 
 
void main() { 
  // This will call either shadeWithShadow or recordDepth 
  RenderPass(); 
} 
