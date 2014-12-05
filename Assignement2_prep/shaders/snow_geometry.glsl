#version 400
layout(points) in;
layout(triangle_strip, max_vertices = 12) out;

in float particleSize[];

// Interpolated values from the vertex shaders

void main()
{

	// Top Ray
	gl_Position = gl_in[0].gl_Position;
	gl_Position.x = gl_Position.x+particleSize[0];
	gl_Position.y = gl_Position.y+particleSize[0];
	EmitVertex();

	gl_Position = gl_in[0].gl_Position;
	gl_Position.x = gl_Position.x-particleSize[0];
	gl_Position.y = gl_Position.y+particleSize[0];
	EmitVertex();

	gl_Position = gl_in[0].gl_Position;
	gl_Position.y = gl_Position.y+particleSize[0]*2;
	EmitVertex();


	EndPrimitive();





	// Bottom ray
	gl_Position = gl_in[0].gl_Position;
	gl_Position.x = gl_Position.x+particleSize[0];
	gl_Position.y = gl_Position.y-particleSize[0];
	EmitVertex();

	gl_Position = gl_in[0].gl_Position;
	gl_Position.x = gl_Position.x-particleSize[0];
	gl_Position.y = gl_Position.y-particleSize[0];
	EmitVertex();

	gl_Position = gl_in[0].gl_Position;
	gl_Position.y = gl_Position.y-particleSize[0]*2;
	EmitVertex();


	EndPrimitive();


	// Left ray
	gl_Position = gl_in[0].gl_Position;
	gl_Position.x = gl_Position.x-particleSize[0];
	gl_Position.y = gl_Position.y-particleSize[0];
	EmitVertex();

	
	gl_Position = gl_in[0].gl_Position;
	gl_Position.x = gl_Position.x-particleSize[0]*2;
	EmitVertex();

	gl_Position = gl_in[0].gl_Position;
	gl_Position.x = gl_Position.x-particleSize[0];
	gl_Position.y = gl_Position.y+particleSize[0];
	EmitVertex();

	EndPrimitive();



	// Right ray
	gl_Position = gl_in[0].gl_Position;
	gl_Position.x = gl_Position.x+particleSize[0];
	gl_Position.y = gl_Position.y-particleSize[0];
	EmitVertex();

	
	gl_Position = gl_in[0].gl_Position;
	gl_Position.x = gl_Position.x+particleSize[0]*2;
	EmitVertex();

	gl_Position = gl_in[0].gl_Position;
	gl_Position.x = gl_Position.x+particleSize[0];
	gl_Position.y = gl_Position.y+particleSize[0];
	EmitVertex();


	EndPrimitive();

}