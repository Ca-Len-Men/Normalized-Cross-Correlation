uniform vec2 resolution;
uniform vec4 pcolor;

void main(void)
{
	vec2 uv = gl_FragCoord.xy / resolution.xy;
	vec2 middle = vec2(0.5, 0.5);

	float alpha = mix(1, 0.0, length(uv - middle) * 2);
	gl_FragColor = vec4(pcolor.rgb, alpha);
}