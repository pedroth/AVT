#version 330 core

uniform vec3 LightPosition;

uniform mat4 ModelMatrix;
layout(std140) uniform SharedMatrices {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};


in vec3 inPosition;
in vec3 inNormal;
in vec3 inTangent;
in vec2 inTex;

//out vec3 exPosition;
//out vec3 exNormal; //0 0 1 in tangent space
out vec3 exLightDistance;
out vec3 exEyeDistance;
out vec2 exTex;


void main(void) {

	mat4 ModelView = ViewMatrix * ModelMatrix;

	vec3 N = normalize((ModelView * vec4(inNormal,0.0)).xyz);//TODO use NormalMatrix
	vec3 T = normalize((ModelView * vec4(inTangent,0.0)).xyz);
	vec3 B = cross(N,T);
	mat3 TBN = mat3(
		T.x,B.x,N.x,
		T.y,B.y,N.y,
		T.z,B.z,N.z
	);

	vec3 viewPos = (ModelView * vec4(inPosition,1.0)).xyz;
	vec3 viewLpos = (ViewMatrix * vec4(LightPosition, 1.0)).xyz;
	
	vec3 viewLDist = viewLpos - viewPos;
	vec3 viewEyeDist = vec3(0.0) - viewPos;

	vec3 tanLDist = TBN * viewLDist;
	vec3 tanEyeDist = TBN * viewEyeDist;

	exLightDistance = tanLDist;
	exEyeDistance = tanEyeDist;
	exTex = inTex;

	
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition,1.0f);
}
