#version 330 core

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

uniform sampler2D NormalTex;

in vec3 exLightDistance;
in vec3 exEyeDistance;
in vec2 exTex;
out vec4 fragColor;

vec3 unpackNormal(in vec2 coord)
{
	vec3 tex = texture(NormalTex, coord).rgb;
	vec3 res = tex * 2.0 - 1.0;
	return res;
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

void main(void)
{
	vec3 eyeDistance = exEyeDistance;
	vec3 eyeDir = normalize(eyeDistance);
	vec3 lightDistance = exLightDistance;
	float lightDistanceLength = length(lightDistance);
	vec3 lightDir = normalize(lightDistance);
	vec3 normal = normalize(unpackNormal(exTex));
	
	vec3 emition = MaterialEmit;
	vec3 ambient = computeAmbient();
	vec3 diffuse = computeDiffuse(normal, lightDir);
	vec3 specular = computeSpecular(normal, lightDir, eyeDir);
	float attenuation = computeAttenuation(lightDistanceLength);
	
	vec3 outColor = emition + ambient + (diffuse + specular) * attenuation;

	//fragColor = vec4(0.5,0.5,0.5,1.0);
	//fragColor = vec4(exViewNormal,1.0);
	fragColor = vec4(outColor,1.0);
	//fragColor = vec4(diffuse,1.0);
	//fragColor = vec4(specular,1.0);
	

}
