uniform sampler2D picture;
uniform vec2 picture_size;

void main(void)
{
    // Kernel 51x51
    vec4 color_medium = vec4(0.0);
    int count = 51 * 51;

    for (int x = -25; x <= 25; x++)
    {
        for (int y = -25; y <= 25; y++)
        {
            vec2 point = gl_FragCoord.xy + vec2(float(x), float(y));
            vec2 sampler_uv = point / picture_size;
            vec4 pic_color = texture(picture, sampler_uv);
            color_medium += pic_color;
        }
    }

    color_medium += vec4(117.0 / 255.0, 135.0 / 255.0, 24.0 / 255.0, 1.0) * 2 * count;
    color_medium /= (3 * float(count));
    
    gl_FragColor = color_medium;  
}