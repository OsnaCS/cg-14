#version 330 core
// FXAA shader, GLSL code adapted from:
// http://horde3d.org/wiki/index.php5?title=Shading_Technique_-_FXAA
// Whitepaper describing the technique:
// http://developer.download.nvidia.com/assets/gamedev/files/sdk/11/FXAA_WhitePaper.pdf

// fxaa works in 2 stages. one is to detect wheter it is part of a stage (then blurr it)
in VertexData {
	vec2 uv;
} inData;

uniform sampler2D R_filterTexture;
uniform vec2 R_inverseFilterTextureSize; // 1/texturesize -> move one pixel further

out vec3 o_color;

void main()
{
	float R_fxaaSpanMax = 8.0;
	float R_fxaaReduceMin = 1.0/128.0;
	float R_fxaaReduceMul = 1.0/8.0;
	vec2 texCoordOffset = R_inverseFilterTextureSize.xy;	// help-variable in order to move in one-pixel-steps
	// the human eye is easily  able to detect luminacity-changes in colors
	vec3 luma = vec3(0.299, 0.587, 0.114);	// doing dot product or mulitplying this in a color -> gives float of luminacity

	// we're gonna sample an "x-pattern"
	// by doing the dot product, we get the luminacity of the pixel:
	float lumaTL = dot(luma, texture(R_filterTexture, inData.uv.xy + (vec2(-1.0, -1.0) * texCoordOffset)).xyz);	// one pixel to the top-left side
	float lumaTR = dot(luma, texture(R_filterTexture, inData.uv.xy + (vec2(1.0, -1.0) * texCoordOffset)).xyz); 	// pixel top-right
	float lumaBL = dot(luma, texture(R_filterTexture, inData.uv.xy + (vec2(-1.0, 1.0) * texCoordOffset)).xyz); 	// pixel bottom-left
	float lumaBR = dot(luma, texture(R_filterTexture, inData.uv.xy + (vec2(1.0, 1.0) * texCoordOffset)).xyz); 	// pixel bottom-right
	float lumaM  = dot(luma, texture(R_filterTexture, inData.uv.xy).xyz); // take a tex2d of the filter texture & the rgb component (.xyz)


	//determine the blur direction and whether or not this is an edge pixel (edge detection)
	vec2 dir; //for the blur direction
	// is there a difference in x or y direction?
	// of this isn't an edge, it won't perform any blurring at all, because there is no difference determining a direction 
	// -> in this case, you stay in the same pixel 
	dir.x = -((lumaTL + lumaTR) - (lumaBL + lumaBR)); // determining the direction from the luminacity subtract top-bottom 
	dir.y = ((lumaTL + lumaBL) - (lumaTR + lumaBR)); 	// subtract left-right

	// we're now gonna skale the vector up, so that whatever the smallest vector is, it will get the length 1
	float dirReduce = max((lumaTL + lumaTR + lumaBL + lumaBR) * (R_fxaaReduceMul * 0.25), R_fxaaReduceMin); // we get an average .. (reduce direction)
	float inverseDirAdjustment = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce); // searches for the smallest vector
	
	dir = min(vec2(R_fxaaSpanMax, R_fxaaSpanMax), 
		max(vec2(-R_fxaaSpanMax, -R_fxaaSpanMax), dir * inverseDirAdjustment)) * texCoordOffset; // clamp, put dir into texturespace

	//do the blur
	//get 2 samples :
	vec3 result1 = (1.0/2.0) * (
		texture(R_filterTexture, inData.uv.xy + (dir * vec2(1.0/3.0 - 0.5))).xyz + // direction as the amount; skale down by a certain amount(backwards on the direction)
		texture(R_filterTexture, inData.uv.xy + (dir * vec2(2.0/3.0 - 0.5))).xyz);

	vec3 result2 = result1 * (1.0/2.0) + (1.0/4.0) * (
		texture(R_filterTexture, inData.uv.xy + (dir * vec2(0.0/3.0 - 0.5))).xyz +
		texture(R_filterTexture, inData.uv.xy + (dir * vec2(3.0/3.0 - 0.5))).xyz);

	float lumaMin = min(lumaM, min(min(lumaTL, lumaTR), min(lumaBL, lumaBR)));	// the smallest
	float lumaMax = max(lumaM, max(max(lumaTL, lumaTR), max(lumaBL, lumaBR)));	// the greatest
	float lumaResult2 = dot(luma, result2); // test if i have sampled too far
	
	if(lumaResult2 < lumaMin || lumaResult2 > lumaMax)	// wich result will be used
		o_color = vec3(result1);
	else
		o_color = vec3(result2);
}