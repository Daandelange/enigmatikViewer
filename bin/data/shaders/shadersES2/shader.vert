#version 150

// these are for the programmable pipeline system and are passed in
// by default from OpenFrameworks
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2 texcoord;
// in vec4 normal
// in vec4 color

// already defined
//in int gl_VertexID; // vertex ID
//in int gl_InstanceID; // likeyly param

// this is something we're creating for this shader (transmitted to fragment shader and used between both shaders)
out vec2 texCoordVarying;
//out vec4 gl_Position; // (un)altered position (already defined)
//out float gl_Pointsize; // not always used (already defined)
//out vec4 gl_FrontColor; // final pixel color  (already defined)

// this is coming from our C++ code
//uniform float mouseX;

void main()
{
	#ifdef INTEL_CARD
    color = vec4(1.0); // for intel HD cards
    normal = vec4(1.0); // for intel HD cards
    #endif

    // here we move the texture coordinates
    texCoordVarying = vec2(texcoord.x, texcoord.y);//position.x, position.y);

    // send the vertices to the fragment shader
	gl_Position = modelViewProjectionMatrix * position;

	// simply get color from other pixels
	
}