
uniform mat4 modelViewProjectionMatrix; // This is provide by openFrameworks
uniform vec2 texcoord; // This is provided by openFrameworks
uniform vec4 position;
varying vec2 vertexTexCoord; // modified vertex coord

uniform sampler2DRect glitchData2;

// in vec4 normal;
// in vec4 color;
uniform float param2Solved;
uniform float param3Solved;
uniform float param4Solved;

void main()
{
    // todo: keep this ?
    #ifdef INTEL_CARD
    color = vec4(1.0); // for intel HD cards
    normal = vec4(1.0); // for intel HD cards
    #endif

    // Absolute window position: gl_FragCoord.x / windowWidth
    //gl_Position.xy;// 
    //vertexTexCoord = position.xy;
    //gl_Position = modelViewProjectionMatrix * position;

    // apply effect 2
    // here we move the texture coordinates
    //vertexTexCoord = vec2(texcoord.x + (1-param2Solved)*50*cos(position.x+(param2Solved-1)*30 ) , texcoord.y + (1-param2Solved)*50*sin(position.y+(param2Solved-1)*30 ) );// nice
    vertexTexCoord = vec2(texcoord.x, texcoord.y );
    gl_Position = modelViewProjectionMatrix * position;

    // effect 3 doesn't apply on vertex shader

    // apply effect 4
    //vertexCoord += vec2( (1-param4Solved)*30*sin( position.x+position.y/10 ), (1-param4Solved)*30*cos( position.y+position.y/10 ) );

    //
    //texCoordVarying = vec2(texcoord.x + (1-param2Solved)*50, texcoord.y + 50*(1-param2Solved) );
    //texCoordVarying = position.xy + vec2( sin(gl_Position.y)*50, cos(gl_Position.x) *50);
    //gl_Position = modelViewProjectionMatrix * position;
}