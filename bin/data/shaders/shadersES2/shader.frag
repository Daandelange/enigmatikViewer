#version 150

// this is how we receive the texture
uniform sampler2DRect tex0;
uniform sampler2DRect nextSlide1;
uniform sampler2DRect glitchData2;

uniform float solved2;

//layout(pixel_center_integer​) in vec4 gl_FragCoord;
//in vec4 gl_FragCoord; // frag position (pixel x,y)  (already defined)
//in vec2 gl_PointCoord;// relative position to the vertex point this pixel was created/instanciated from.  (already defined)
// gl_FragColor (& out?)

// custom variables
in vec2 texCoordVarying;

// custom variable suggestions:
// uniform vec4 myColor;
// uniform float myRange, myPosition;
// uniform vec2 myScreenPosition;
// uniform vec4 myPosition;
// uniform sampler2DRect myMask;

out vec4 outputColor; // final pixel color
 
void main()
{
	// currentSlide0*glitchData2.r
	//outputColor = vec4( texture(tex0, texCoordVarying).rgb, texture(glitchData2, texCoordVarying).r );
	outputColor = vec4( texture(glitchData2, texCoordVarying ).r, 0, 0, 1 );
	//if( texture(glitchData2, gl_PointCoord.xy ).r != 0 ) outputColor = vec4( texture(glitchData2, texCoordVarying ).r ,0,0,1);//texture(tex0, texCoordVarying) * (1-solved2) + texture(nextSlide1, texCoordVarying) * solved2;

	//else outputColor = vec4(255,0,0,1);//texture(tex0, texCoordVarying);
}