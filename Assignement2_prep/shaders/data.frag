﻿// The main fragment shader for the second assignment.
// Written by Jekabs Stikans
// Version: 2.0v, 02/12/2014

#version 400

in VS_OUT
{
	vec3 N, L;
	vec4 colour, diffusecolour, ambientcolour, P, world_coord;
	float attenuation;

	vec2 texcoord;
	float u1, u2;
} vs_out;


vec3 global_ambient  = vec3(0.05, 0.05, 0.05);
vec3 specular_albedo = vec3(1.0, 0.8, 0.6);
vec4 fog_colour		 = vec4(0.1, 0.1, 0.1, 1.0);

vec4 sky   = vec4(90/255.0f, 166/255.0f, 196/255.0f, 1.0);
vec4 cloud = vec4(1.0, 1.0, 1.0, 1.0);

uniform uint emitmode, textureMode, specularMode, fogMode, terrainMode, cloudMode;
uniform sampler2D tex1;


out vec4 outputColor;

// Exponential fog function.
vec4 fog(vec4 c)
{
	// Taken from "OpenGL SuperBible Sixth Edition", page 543
	float z = length(vs_out.P.xyz);

	float de = 0.025 * smoothstep(0.0, 6.0, 10.0 - vs_out.world_coord.y);
	float di = 0.045 * smoothstep(0.0, 40.0, 20.0  - vs_out.world_coord.y);

	float extinction = exp(-z * de);
	float inscattering = exp(-z * di);

	return c * extinction + fog_colour * (1.0 - inscattering);
}

// Linear fog function.
vec4 fog_linear(vec4 c)
{
	// This example is taken from one of our lecture slides by Ian Martin.
	float fog_maxD = 17.0;
	float fog_minD = -2.0;

	float dist = length(vs_out.P.xyz);
	float fog_factor = (fog_maxD - dist) / (fog_maxD - fog_minD);
	fog_factor = clamp(fog_factor, 0.0, 1.0);

	return mix(fog_colour, c, fog_factor);
}

vec4 texturing()
{
	// Used for texturing without vertex seams.
	// Taken from http://vcg.isti.cnr.it/~tarini/no-seams/jgt_tarini.pdf 

	vec2 tmp1, tmp2, tmp3;
	tmp1.y = vs_out.texcoord.y;
	tmp2.y = vs_out.texcoord.y;
	tmp3.y = vs_out.texcoord.y;

	tmp1.x = vs_out.u1;
	tmp2.x = vs_out.u2;

	float a = fwidth(tmp1);
	float b = fwidth(tmp2);

	if(a <= b)
	{
		tmp3.x = tmp1.x;
	}
	else
	{
		tmp3.x = tmp2.x;
	}

	vec4 texcolour = texture(tex1, tmp3);
	
	return texcolour;
}

void main()
{
	vec3 emissive = vec3(0);

	// Ambient lighting calculation;
	vec3 ambient = vs_out.colour.xyz * 0.3;

	// Diffuse lighting calculation
	vec3 diffuse = max(dot(vs_out.N, vs_out.L), 0.0) * vs_out.colour.xyz;

	// Specular lighting caluclation
	vec3 V = normalize(-vs_out.P.xyz);	
	vec3 R = reflect(-vs_out.L, vs_out.N);
	vec3 specular = vec3(0.0, 0.0, 0.0);
	
	// Check whether specular mode is on and whether the fragment does not belong to the terrain & is not above sea level
	if((specularMode == 1 && terrainMode == 0) || (specularMode == 1 && terrainMode == 1 && vs_out.world_coord.y < -0.5))
	{
		specular = pow(max(dot(R, V), 0.0), 5.0) * specular_albedo;
	}

	if (emitmode == 1) 
	{
		emissive = vec3(245/255.0f, 226/255.0f, 100/255.0f);
	}

	outputColor = vec4(vs_out.attenuation*(ambient+diffuse+specular)+global_ambient+emissive, 1.0);

	if(textureMode == 1)
	{
		if(terrainMode == 0 || terrainMode == 1 &&  vs_out.world_coord.y >= -0.48)
		{
			// If texture mode is enabled, then get the fragment colour from the texture map.
			vec4 texcolour = texture(tex1, vs_out.texcoord.xy);

			outputColor = vec4(vs_out.attenuation*(texcolour.xyz*(ambient + diffuse+specular))+global_ambient+emissive, 1.0);

			if(cloudMode == 1)
			{
				float noise_intensity = (texcolour[0] + texcolour[1] + texcolour[2] + texcolour[3]) / 4.0;
				
				// If the fragment is on the horizon or below it, should fade out the clouds.
				noise_intensity = smoothstep(0.50+clamp((-0.6)*(vs_out.world_coord.y-0.8), -0.5, 0.3), 1.0, noise_intensity);

				outputColor = mix(sky, cloud, noise_intensity);
			}
		}
	}

	if(fogMode == 1)
	{
		// Apply linear fog
		outputColor = fog_linear(outputColor);
	}
	else if(fogMode == 2)
	{
		// Apply exponential Fog from the super bible
		outputColor = fog(outputColor);
	}
}