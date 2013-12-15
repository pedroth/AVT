#version 330 core

layout(std140) uniform SharedMatrices {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};
uniform vec3 LightPosition;
uniform vec3 LightAmbient;
uniform vec3 LightDiffuse;
uniform vec3 LightSpecular;
uniform vec3 LightAttenuation;
uniform float LightRangeLimit;

uniform vec3 MaterialEmit;
uniform vec3 MaterialAmbient;
uniform vec3 MaterialDiffuse;
uniform vec3 MaterialSpecular;
uniform float MaterialShininess;

in vec3 exViewPosition;
in vec3 exViewNormal;
in vec3 exPosition;
out vec4 fragColor;

uniform sampler3D UTexture;

float noise(in vec3 position) 
{
	float noiseAux = texture(UTexture, normalize(position)*0.5 + 0.5).r;
	return noiseAux;
}

vec3 computeAmbient()
{
	vec3 ambient = LightAmbient * MaterialAmbient;
	return ambient;
}

vec3 computeDiffuse(in vec3 normal, in vec3 lightDir)
{
	float NdotL = dot(normal, lightDir);
	vec3 diff = max(NdotL,0.0) * LightDiffuse * MaterialDiffuse;
	return diff;
}

vec3 computeSpecular(in vec3 normal, in vec3 lightDir, in vec3 eyeDir)
{
	vec3 R = reflect(-lightDir, normal);
	float RdotE = max(dot(R,eyeDir),0.0);
	float specFactor = pow(RdotE,MaterialShininess);
	vec3 spec = specFactor * LightSpecular * MaterialSpecular;
	return spec;
}

float computeAttenuation(in float lightDistance)
{
	float denominator = LightAttenuation.x + 
				LightAttenuation.y * lightDistance +
				LightAttenuation.z * pow(lightDistance,2);
	float distanceFactor = 1 / denominator;
	
	if(LightRangeLimit < 0.0)
		return distanceFactor;
	
	if(LightRangeLimit < lightDistance)
		return 0.0;
		
	return distanceFactor;
}

float sinozoid(vec3 pos) {
	vec3 npos = normalize(pos);
	float ret = (sin(32 * noise(pos) *(npos.x + npos.y)));
	return ret;
}

void main(void)
{
	vec3 eyeDistance = vec3(0.0) - exViewPosition;
	vec3 eyeDir = normalize(eyeDistance);
	vec4 viewLightPos = ViewMatrix * vec4(LightPosition,1.0);
	vec3 lightDistance = viewLightPos.xyz - exViewPosition;
	float lightDistanceLength = length(lightDistance);
	vec3 lightDir = normalize(lightDistance);
	vec3 normal = normalize(exViewNormal);
	
	vec3 emition = MaterialEmit;
	vec3 ambient = computeAmbient();
	vec3 diffuse = computeDiffuse(normal, lightDir);
	vec3 specular = computeSpecular(normal, lightDir, eyeDir);
	float attenuation = computeAttenuation(lightDistanceLength);
	
	vec3 outColor = emition + ambient + (diffuse + specular) * attenuation;

	fragColor = vec4(sinozoid(exPosition) * outColor,1.0);
	

}
