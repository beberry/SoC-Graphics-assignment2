// The main fragment shader for the second assignment.
// Written by Jekabs Stikans
// Version: 2.0v, 02/12/2014

#version 400

// Define the vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 textcoord;

// Uniforms
uniform mat4 model, view, projection;
uniform mat3 normalmatrix;
uniform vec4 lightPos;
uniform uint emitmode, textureMode, specularMode, terrainMode, cloudMode;

// My out definitions
out VS_OUT
{
	vec3 N, L;
	vec4 colour, diffusecolour, ambientcolour, P, world_coord;
	float attenuation;
	vec2 texcoord;
	float u1, u2;
} vs_out;

void main()
{
	if(textureMode == 1)
	{
		// Used for texturing without vertex seams.
		// Taken from http://vcg.isti.cnr.it/~tarini/no-seams/jgt_tarini.pdf 
		vs_out.u1 = fract(textcoord.x);
		vs_out.u2 = fract(textcoord.x+0.5)-0.5;

		vs_out.texcoord = textcoord.xy;
	}

	vec4 position_h     = vec4(position, 1.0);		// Convert the (x,y,z) position to homogeneous coords (x,y,z,w)
	vec4 diffuse_albedo = vec4(1.0, 1.0, 1.0, 1.0);	// This is the vertex colour, used to handle the colourmode change
	vec3 light_pos3		= lightPos.xyz;			


	// Define our vectors to calculate diffuse and specular lighting
	mat4 mv_matrix = view * model;				// Calculate the model-view transformation
	vec4 P = mv_matrix * position_h;			// Modify the vertex position (x, y, z, w) by the model-view transformation
	vec3 N = normalize(normalmatrix * normal);	// Modify the normals by the normal-matrix (i.e. to model-view (or eye) coordinates )
	vec3 L = light_pos3 - P.xyz;				// Vector from the light position to the vertex in eye coordinates.
	float distanceToLight = length(L);			// For attenuation
	L = normalize(L);							// Normalise our light vector

	// Calculate the attenuation factor;
	float attenuation_k = 0.01;
    float attenuation = 1.0 / (1.0 + attenuation_k * pow(distanceToLight, 2));

	// Set Out variables
	vs_out.N = N;
	vs_out.P = P;
	vs_out.L = L;

	vs_out.colour	   = diffuse_albedo;
	vs_out.attenuation = attenuation;
	vs_out.world_coord = model*position_h;

	// Colour based on height values.
	if(vs_out.world_coord.y < -0.5 && terrainMode == 1)
	{
		vs_out.colour = vec4(108.f/255.f, 169.f/255.f, 248.f/255.f, 0.99);
	}
	else if(vs_out.world_coord.y < -0.48 && terrainMode == 1)
	{
		vs_out.colour = vec4(181.f/255.f, 116.f/255.f, 42.f/255.f, 1.0);
	}
	else if(vs_out.world_coord.y < 2 && terrainMode == 1)
	{
		vs_out.colour = vec4(87.f/255.f, 149.f/255.f, 64.f/255.f, 1.0);
	}

	// Even out the water surface
	if(vs_out.world_coord.y < -0.5 && terrainMode == 1)
	{
		// Calculate the difference from the sea level
		float diff = -0.5 - vs_out.world_coord.y;

		float newDiff = diff*0.95;
		vs_out.world_coord.y = vs_out.world_coord.y+newDiff;
	}


	gl_Position = (projection * view * model) * position_h;
}