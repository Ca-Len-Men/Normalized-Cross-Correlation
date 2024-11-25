uniform vec2 resolution;
uniform vec3 pcolor;
uniform int radius;
uniform int border;

#define sqrt_2 sqrt(2.0)

float alpha_corner(int rad, int x, int y)
{
	if(x == 0 || y == 0)
		return 1.0;

	int rad_squared = rad * rad;
	int tr_squared = (x + 1) * (x + 1) + (y + 1) * (y + 1);

	if(tr_squared <= rad_squared)
		return 1.0;

	int bl_squared = (x * x) + (y * y);
	if(bl_squared < rad_squared)
		return (rad - sqrt(float(bl_squared))) / sqrt_2;

	return 0.0;
}

float alpha_rect(vec2 size, int rad, int x, int y)
{
	// Top left
	if(x < rad && y < rad)
		return alpha_corner(rad, rad - x - 1, rad - y - 1);

	// Top right
	if(x >= (size.x - rad) && y < rad)
		return alpha_corner(rad, x - (int(size.x) - rad), rad - y - 1);

	// Bottom left
	if(x < rad && y >= (size.y - rad))
		return alpha_corner(rad, rad - x - 1, y - (int(size.y) - rad));

	// Bottom right
	if(x >= (size.x - rad) && y >= (size.y - rad))
		return alpha_corner(rad, x - (int(size.x) - rad), y - (int(size.y) - rad));

	return 1.0;
}

void main(void)
{
	int px = int(gl_FragCoord.x);
	int py = int(gl_FragCoord.y);
	float alpha = alpha_rect(resolution, radius, px, py);

	float alpha2 = 0.0;
	vec2 smaller_resolution = vec2(resolution.x - 2 * border, resolution.y - 2 * border);
	if(px >= border && py >= border && px < resolution.x - border && py < resolution.y - border)
		alpha2 = alpha_rect(smaller_resolution, radius - border, px - border, py - border);

	gl_FragColor = vec4(pcolor, alpha - alpha2);
}