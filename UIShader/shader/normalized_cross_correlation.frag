uniform sampler2D source;
uniform vec2 source_size;
uniform sampler2D templ;
uniform vec2 template_size;

vec4 packFloatToRGBA(float value)
{
    vec4 rgba;
    rgba.r = floor(value * 256.0) / 256.0;
    rgba.g = floor(fract(value * 256.0) * 256.0) / 256.0;
    rgba.b = floor(fract(value * 256.0 * 256.0) * 256.0) / 256.0;
    rgba.a = fract(value * 256.0 * 256.0 * 256.0);
    return rgba;
}

void main(void)
{
    int template_w = int(template_size.x);
    int template_h = int(template_size.y);

    // Normalized Cross-Correlation
    float dot_sum = 0.0;
    float source_squared_sum = 0.0;
    float templ_squared_sum = 0.0;

    for (int x = 0; x < template_w; ++x) {
        for (int y = 0; y < template_h; ++y) {
            vec2 xy = vec2(float(x), float(y));
            vec2 source_uv = (gl_FragCoord.xy + xy) / source_size;
            vec2 templ_uv = xy / template_size;

            float source_r = texture(source, source_uv).r * 255;
            float templ_r = texture(templ, templ_uv).r * 255;

            dot_sum += source_r * templ_r;
            source_squared_sum += source_r * source_r;
            templ_squared_sum += templ_r * templ_r;
        }
    }

    float normalized_value = dot_sum / (sqrt(source_squared_sum) * sqrt(templ_squared_sum));
    gl_FragColor = packFloatToRGBA(normalized_value);
}