precision highp float;

// this is how we receive the texture
uniform sampler2D tex0;
uniform sampler2D nextSlide1;
uniform sampler2D glitchData2;

uniform float solved2;

//layout(pixel_center_integer​) in vec4 gl_FragCoord;
//attribute vec4 gl_FragCoord; // frag position (pixel x,y)  (already defined)
//in vec2 gl_PointCoord;// relative position to the vertex point this pixel was created/instanciated from.  (already defined)
//attribute vec4 gl_FragColor;// (& out?)

// custom variables
varying vec2 texCoordVarying;

// custom variable suggestions:
// uniform vec4 myColor;
// uniform float myRange, myPosition;
// uniform vec2 myScreenPosition;
// uniform vec4 myPosition;
// uniform sampler2DRect myMask;

vec4 outputColor; // final pixel color

//varying float intensity; 

void main()
{
	float intensity;
	intensity = texture2D(glitchData2, gl_FragCoord.xy ).b;

	// currentSlide0*glitchData2.r
	//outputColor = vec4( texture(tex0, texCoordVarying).rgb, texture(glitchData2, texCoordVarying).r );
	//outputColor = vec4( texture(glitchData2, texCoordVarying ).r, 0, 0, 1 );
	
	if( intensity > solved2 ) outputColor = vec4( texture2D(tex0, texCoordVarying ).rgb, 1);//vec4( texture(glitchData2, texCoordVarying ).r ,0,0,1);//texture(tex0, texCoordVarying) * (1-solved2) + texture(nextSlide1, texCoordVarying) * solved2;
	else outputColor = vec4(0,0,0,0);//texture(tex0, texCoordVarying);
	outputColor = vec4( texture(glitchData2, gl_FragCoord.xy ).rgb, 1); // dunno why, but r and b are reversed in glitchData2

	// sin((ceil(i/numChannels)*.0f)/100)+sin((ceil(i/numChannels)*.0f)/100) > 1.0f)
	gl_FragColor = outputColor;
}
