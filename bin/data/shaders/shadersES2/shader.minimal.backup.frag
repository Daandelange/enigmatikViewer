
#ifdef GL_ES
precision highp float;
#endif

// this is how we receive the texture
//uniform sampler2D tex0; // video layer
//uniform sampler2D nextSlide1;
//uniform sampler2D glitchData2; // contains regions to apply the effect to.

// to enable
//uniform float param2Solved;
//uniform float param3Solved;
//uniform vec2 resolution;
//uniform vec2 textureResolution;
//uniform float textureScale;

// custom variable suggestions:
// uniform vec4 myColor;
// uniform float myRange, myPosition;
// uniform vec2 myScreenPosition;
// uniform vec4 myPosition;
// uniform sampler2DRect myMask;

//varying vec2 vertexTexCoord;

void main()
{
	gl_FragColor = vec4(1.0,0.0,0.0,1.0);
}
