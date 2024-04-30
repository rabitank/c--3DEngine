#version 460

layout(location = 0) in vec3 i_pos;
layout(location = 1) in vec3 i_normal;
layout(location = 2) in vec2 i_coord;

out vec4 o_color;

uniform sampler2D u_texture;

// scene's attribute: provide renderer/shader
uniform vec3  u_cameraPos ;
uniform vec3  u_ambientLight ;

struct DirctionalLight
{
    vec3 m_direction;
    vec3 m_diffuseColor;
    vec3 m_specColor;
};
uniform DirctionalLight u_dirctionLight ;

// surface's attribute: provider mesh
uniform float u_specPower; // defualt 100 -> none


void main()
{
    // normal
    vec3 N = normalize(i_normal);
    // surface to light
    vec3 L = normalize( -u_dirctionLight.m_direction );
    // surface to camera
    vec3 V = normalize(u_cameraPos - i_pos );
    // L 's reflection
    vec3 R = normalize(reflect(-L,N));
    // ambient;
    vec3 phong = u_ambientLight;
    float NdotL = dot(N,L);
    // means the positive surface ( face to light)
    if(NdotL >0)
    {
        // doffuse
        phong += NdotL * u_dirctionLight.m_diffuseColor;
        // specular : u_specPower control shape
        phong += pow(max(dot(R,V),0.0)  , u_specPower ) * u_dirctionLight.m_specColor;
    }
    o_color =  texture(u_texture , i_coord) * vec4( phong , 1.f);
    // o_color =  texture(u_texture , i_coord) * vec4( 1.f,1.f,1.f , 1.f);

}
