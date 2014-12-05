// The vertex shader for the snow particle animation.
// Modified by Jekabs Stikans, taken from Ian Martins particle example.
// Version: 1.0v, 02/12/2014

#version 420

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec4 xyzs; 				// Position of the center of the particule and size of the square

// Values that stay constant for the whole mesh.
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP; // Model-View-Projection matrix, but without the Model (the position is in BillboardPos; the orientation depends on the camera)

// Out variables.
out float particleSize;

void main()
{
	particleSize = xyzs.w; // because we encoded it this way.
	vec3 particleCenter_wordspace = xyzs.xyz;
	
	vec3 point = particleCenter_wordspace + CameraRight_worldspace * particleSize + CameraUp_worldspace * particleSize;

	// The transformed position of this vertex.
	gl_Position = VP * vec4(point, 1.0f);
}

