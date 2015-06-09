
#ifdef GL_ES
precision highp float;
#endif

// this is how we receive the texture
uniform sampler2D tex0; // video layer
//uniform sampler2D nextSlide1;
//uniform sampler2D glitchData2; // contains regions to apply the effect to.

// to enable
uniform float param2Solved;
uniform float param3Solved;
uniform float param6Solved;

uniform vec2 resolution;
uniform vec2 textureResolution;
uniform float textureScale;

// custom variable suggestions:
// uniform vec4 myColor;
// uniform float myRange, myPosition;
// uniform vec2 myScreenPosition;
// uniform vec4 myPosition;
// uniform sampler2DRect myMask;

varying vec2 vertexTexCoord;
uniform vec4 color;
//varying vec4 gl_FragCoord;

//out vec4 outputColor;

//float mod(float _a1, float _a2){
//	return _a1 % _a2;
//}

float getBrightness(vec3 _color){
	return (_color.r+_color.g+_color.b)/3.;
}


void main()
{
	vec4 outputColor;
	//outputColor = vec4( texture(tex0, gl_FragCoord.xy/resolution.xy ).rgb, 1);
	vec2 texturePixelPosition = mod( (vertexTexCoord)/resolution*textureResolution, textureResolution);
	vec4 imageColor = vec4( texture2D(tex0, texturePixelPosition ).rgb, 1);

	// apply glitch effect 1
	// mod(vec2,1) is like vec2%1
	outputColor = imageColor;

	// apply glitch effect 2
	//outputColor = mod( outputColor*(1-param3Solved), 1 );
	outputColor = vec4(
		mod( outputColor.r*( pow( (param3Solved*param3Solved), param3Solved )/param3Solved), 1.0 ),
		mod( outputColor.g*( ( (param3Solved+6.1) * param3Solved )-6.1), 1.0 ),
		mod( outputColor.b*( ( (param3Solved-1.3) * param3Solved )+1.3), 1.0 )
		,1);
	// fix for effect 3 if color = 1; (1%1=0 unstead of 1)
	if(param3Solved == 1.0){
		if(outputColor.r==0.0) outputColor.r=1.0;
		if(outputColor.g==0.0) outputColor.g=1.0;
		if(outputColor.b==0.0) outputColor.b=1.0;
	}// */

	// apply glitch effect 3
	float pixelBrightness;
	pixelBrightness = getBrightness( texture2D(tex0, texturePixelPosition ).rgb );
	float effectIntensity = getBrightness( texture2D(tex0, texturePixelPosition ).rgb);

	if( pixelBrightness > param2Solved ){
		//outputColor += texture(tex0, vertexTexCoord) * (1-param2Solved) + texture(nextSlide1, vertexTexCoord) * param2Solved;
		//float effect2_intensity = 
		vec2 posOffset = vec2( mod(effectIntensity*0.4*(pixelBrightness-param2Solved)+texturePixelPosition.x, 0.35), mod(effectIntensity*0.23+texturePixelPosition.x+param2Solved, 0.23) );
		vec4 newColor = vec4( texture2D(tex0, texturePixelPosition+posOffset ).rgb, 1);
		vec4 effect2 = vec4( texture2D(tex0, texturePixelPosition ).rgb * vec3(effectIntensity,effectIntensity,effectIntensity), 1);
		outputColor = newColor;
	}

	/*float intensity;
	//intensity = texture2D(glitchData2, gl_FragCoord.xy ).b;
	
	// currentSlide0*glitchData2.r
	//outputColor = vec4( texture(tex0, texCoordVarying).rgb, texture(glitchData2, texCoordVarying).r );
	//outputColor = vec4( texture(glitchData2, texCoordVarying ).r, 0, 0, 1 );
	
	//if( intensity > param2Solved ) outputColor = vec4( texture2D(tex0, texCoordVarying ).rgb, 1);//vec4( texture(glitchData2, texCoordVarying ).r ,0,0,1);//texture(tex0, texCoordVarying) * (1-param2Solved) + texture(nextSlide1, texCoordVarying) * param2Solved;
	//else outputColor = vec4(0,0,0,0);//texture(tex0, texCoordVarying);
	outputColor = vec4( texture(glitchData2, gl_FragCoord.xy ).rgb, 1); // dunno why, but r and b are reversed in glitchData2

	// tmp
	// sin((ceil(i/numChannels)*.0f)/100)+sin((ceil(i/numChannels)*.0f)/100) > 1.0f) // */

	// solved indicator (for debugging)
	if(param2Solved == 1. && param3Solved ==1. ) outputColor.g += 0.15;

	gl_FragColor = outputColor;
}
