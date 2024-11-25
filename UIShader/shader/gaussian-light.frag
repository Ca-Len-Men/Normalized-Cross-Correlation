uniform vec2 resolution;
uniform vec4 pcolor;

float gaussian_method(float _sigma, float x)
{
	return exp(- (x * x) / (2 * _sigma * _sigma));
}

void main(void)
{
	vec2 uv = gl_FragCoord.xy / resolution.xy;
	vec2 middle = vec2(0.5, 0.5);

	float pi = 3.14159265359;
	float e = 2.71828182846;
	// float sigma = 1.0 / sqrt(2 * pi);
	float sigma = pi / 10;

	float d = length(uv - middle);
	float px = 2 * d;
	float alpha = gaussian_method(sigma, px);

	gl_FragColor = vec4(pcolor.rgb, alpha);
}