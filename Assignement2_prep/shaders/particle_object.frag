#version 400


// Ouput data
out vec4 color;

uniform sampler2D myTextureSampler;

void main(){
	// Output color = color of the texture at the specified UV
	color = vec4(1.0, 1.0, 1.0, 1.0);
}