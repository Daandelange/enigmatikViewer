#version 330

// todo:
// // randomly generated sampler2dRect per slide // to use for effect variations over time/usage (or other)

// this is how we receive the texture
uniform sampler2DRect tex0; // video layer
uniform sampler2DRect nextSlide1;
uniform sampler2DRect glitchData2; // contains regions to apply the effect to.

// to enable
uniform float param2Solved;
uniform float param3Solved;
uniform vec2 resolution;
uniform vec2 textureResolution;
uniform float textureScale;

// custom variable suggestions:
// uniform vec4 myColor;
// uniform float myRange, myPosition;
// uniform vec2 myScreenPosition;
// uniform vec4 myPosition;
// uniform sampler2DRect myMask;

in vec2 vertexTexCoord;
in vec4 gl_FragCoord;

out vec4 outputColor;

// functions

float getBrightness(vec3 _color){
	return (_color.r+_color.g+_color.b)/3;
}

// rgb2hsl & hsl2rgb functions: http://gamedev.stackexchange.com/a/59808

void main()
{
	//outputColor = vec4( texture(tex0, gl_FragCoord.xy/resolution.xy ).rgb, 1);
	
	// apply glitch effect 1
	// mod(vec2,1) is like vec2%1
	vec2 texturePixelPosition = mod( (vertexTexCoord)/resolution*textureResolution, textureResolution);
	vec4 imageColor = vec4( texture(tex0, texturePixelPosition ).rgb, 1);
	outputColor = imageColor;
	
	// apply glitch effect 2
	//outputColor = mod( outputColor*(1-param3Solved), 1 );
	outputColor = vec4(
		mod( outputColor.r*( pow( (param3Solved*param3Solved), param3Solved )/param3Solved), 1 ),
		mod( outputColor.g*( ( (param3Solved+6.1f) * param3Solved )-6.1f), 1 ),
		mod( outputColor.b*( ( (param3Solved-1.3f) * param3Solved )+1.3f), 1 )
		,1);
	/*/ fix for effect 3 if color = 1; (1%1=0 unstead of 1)
	if(param3Solved == 1){
		if(outputColor.r==0) outputColor.r=1;
		if(outputColor.g==0) outputColor.g=1;
		if(outputColor.b==0) outputColor.b=1;
	}// */

	// apply glitch effect 3

	float pixelBrightness;
	pixelBrightness = getBrightness( texture(tex0, texturePixelPosition ).rgb );
	float effectIntensity = getBrightness( texture(glitchData2, vertexTexCoord ).rgb);

	// currentSlide0*glitchData2.r
	//outputColor = vec4( texture(tex0, texCoordVarying).rgb, texture(glitchData2, texCoordVarying).r );
	//outputColor = vec4( texture(glitchData2, texCoordVarying ).r, 0, 0, 1 );
	
	if( pixelBrightness > param2Solved ){
		//outputColor += texture(tex0, vertexTexCoord) * (1-param2Solved) + texture(nextSlide1, vertexTexCoord) * param2Solved;
		//float effect2_intensity = 
		vec2 posOffset = vec2( mod(effectIntensity*50*(pixelBrightness-param2Solved)+texturePixelPosition.x, 50), mod(effectIntensity*40+texturePixelPosition.x+param2Solved, 40) );
		vec4 newColor = vec4( texture(tex0, texturePixelPosition+posOffset ).rgb, 1);
		vec4 effect2 = vec4( texture(tex0, texturePixelPosition ).rgb * vec3(effectIntensity,effectIntensity,effectIntensity), 1);
		outputColor = newColor;
	}
	//else outputColor += vec4(0,0,0,0);//texture(tex0, texCoordVarying);
	//outputColor = vec4( texture(glitchData2, gl_FragCoord.xy ).bgr, 1); // dunno why, but r and b are reversed in glitchData2
	//outputColor = vec4(0,0,0,1);//effectIntensity;
	// tmp
	// sin((ceil(i/numChannels)*.0f)/100)+sin((ceil(i/numChannels)*.0f)/100) > 1.0f)
	//gl_FragColor = outputColor; // */

	// solved indicator (for debugging)
	if(param2Solved == 1 && param3Solved ==1 ) outputColor.g += 0.15f;
}

