#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 a_LocalPosition;

out vec2 v_LocalPosition;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	gl_Position = u_Proj * u_View * u_Model * vec4(position,1.0);
	v_LocalPosition = a_LocalPosition;

};



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_LocalPosition;

//Phong
uniform vec4 u_Color;
uniform float thickness;

void main()
{
    if (abs(v_LocalPosition.x) < thickness && abs(v_LocalPosition.y) < thickness)
        discard;

    if (any(greaterThan(v_LocalPosition, vec2(1.0))) || (any(lessThan(v_LocalPosition, vec2(-1.0)))))
        discard;

    // Set output color
    color = u_Color;

}