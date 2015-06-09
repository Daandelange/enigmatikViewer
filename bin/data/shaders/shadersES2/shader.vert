
attribute vec4 position;        // set automatically by OF 
attribute vec4 color;           // set automatically by OF 
attribute vec4 normal;          // set automatically by OF 
attribute vec2 texcoord;        // set automatically by OF 

uniform mat4 modelViewMatrix;   // set automatically by OF 
uniform mat4 projectionMatrix;  // set automatically by OF
uniform mat4 modelViewProjectionMatrix;

varying vec2 vertexTexCoord; // modified vertex coord send to frag shader

// variables coming from enigmatikSlideShow
uniform sampler2D glitchData2;
uniform float param2Solved;
uniform float param3Solved;
uniform float param4Solved;
uniform float param6Solved;

void main()
{
    vec4 pos = projectionMatrix * modelViewMatrix * position;
    vertexTexCoord = texcoord;

    // Absolute window position: gl_FragCoord.x / windowWidth
    //gl_Position.xy;// 
    //vertexTexCoord = position.xy;
    //gl_Position = modelViewProjectionMatrix * position;

    // apply effect 2
    // here we simply move the texture coordinates
    vertexTexCoord = vec2(texcoord.x + (1.-param2Solved)*0.1*cos(position.x+(param2Solved-1.)*0.1 ) , texcoord.y + (1.-param2Solved)*0.2*sin(position.y+(param2Solved-1.)*0.2 ) );// nice
    //vertexTexCoord = vec2(texcoord.x, texcoord.y );
    

    // effect 3 doesn't apply on vertex shader

    // apply effect 4
    vertexTexCoord += vec2( (1.-param6Solved)*0.15*sin( position.x+position.y/10. ), (1.-param6Solved)*0.1*cos( position.y+position.y/0.05 ) );

    //
    //texCoordVarying = vec2(texcoord.x + (1-param2Solved)*50, texcoord.y + 50*(1-param2Solved) );
    //texCoordVarying = position.xy + vec2( sin(gl_Position.y)*50, cos(gl_Position.x) *50);
    //gl_Position = projectionMatrix * modelViewMatrix * position;

    //copy variables to GL ES 2 build-in variables
    gl_Position = pos;
}