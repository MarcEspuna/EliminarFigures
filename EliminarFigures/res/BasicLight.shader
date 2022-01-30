#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	gl_Position = u_Proj * u_View * u_Model * vec4(position,1.0);


	FragPos = (u_Model * vec4(position, 1.0)).xyz;    //the a_vertex is in local position, v_vertex global pos

// STEP 3: Light 
// rotating normals
	mat3 normal_rotate = mat3(1.0f);
	normal_rotate[0] = vec3(u_Model[0][0], u_Model[0][1], u_Model[0][2]);
	normal_rotate[1] = vec3(u_Model[1][0], u_Model[1][1], u_Model[1][2]);
	normal_rotate[2] = vec3(u_Model[2][0], u_Model[2][1], u_Model[2][2]);
	normal_rotate = transpose(inverse(normal_rotate));

	Normal = normal_rotate * aNormal;

};



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 Normal;
in vec3 FragPos;

//Phong
uniform vec3 u_ambient;
uniform vec3 u_diffuse;
uniform vec3 u_specular;
uniform float u_shininess;

uniform vec4 u_Color;
uniform vec3 u_LightPos;


void main()
{

	vec3 N = normalize(Normal);

	vec3 L = normalize(-FragPos);
	vec3 E = normalize(u_LightPos - FragPos);
	vec3 H = normalize(L + E);	//H for Blinn-phong shading 

	float NdotL = max(dot(N, L), 0.0);
	float NdotH = max(dot(N, H), 0.0);

	vec3 result = u_Color.xyz * u_ambient +
		u_Color.xyz * u_diffuse * NdotL +
		u_Color.xyz * pow(NdotH, u_shininess); ;

	color = vec4(result, 1.0);
}
