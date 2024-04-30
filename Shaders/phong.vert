#version 460
/// vert info IN LOCAL
in layout(location = 0) vec3 i_pos;
in layout(location = 1) vec3 i_normal;
in layout(location = 2) vec2 i_coord;

/// frag info IN WORLD
layout(location = 0) out vec3 o_position;
layout(location = 1) out vec3 o_normal;
layout(location = 2) out vec2 o_texCoord;

uniform mat4 u_worldTransform;
uniform mat4 u_viewProj;


void main()
{
    vec4 pos = vec4(i_pos,1.f);
    
    pos = pos * u_worldTransform;
    // vert pos in camera
    gl_Position = pos * u_viewProj;

    o_position = pos.xyz;
    // normal don't need translation
    o_normal =  (vec4(i_normal,0.f)*u_worldTransform).xyz ;
    o_texCoord = i_coord;
}
