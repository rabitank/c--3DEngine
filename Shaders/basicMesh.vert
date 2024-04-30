#version 460

in layout(location = 0) vec3 i_pos;
in layout(location = 1) vec3 i_normal;
in layout(location = 2) vec2 i_coord;

layout(location = 0) out vec2 o_fragCoord;

uniform mat4 u_worldTransform;
uniform mat4 u_viewProj;

void main()
{
    
    gl_Position = vec4(i_pos,1.f) * u_worldTransform * u_viewProj ;
    o_fragCoord = i_coord;
}
