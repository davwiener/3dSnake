#version 120
attribute vec3 weights;
attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;
attribute vec3 color;

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 color0;
varying vec3 weights0;
uniform mat4 MVP;
uniform mat4 Normal;
uniform int index;
uniform mat4[20] transMat; 

void main()
{
	vec4 tempPosition= vec4(0,0,0,1.0);
	gl_Position = MVP * vec4(position, 1.0);
	if(index <=0 || index > 3){
		gl_Position = MVP * vec4(position, 1.0);
	}
	else{
	        for (int i = 0 ; i < 3 ; i++)	{
    			gl_Position +=  weights[i]* transMat[index-1+i] * vec4(position, 1.0);; 		
    		}
    		//gl_Position =tempPosition;
	}
	color0 =color ;
	texCoord0 = texCoord;
	weights0 = weights;
	normal0 = (Normal * vec4(normal, 0.0)).xyz;
}
