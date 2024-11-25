uniform sampler2D image;
uniform vec2 image_size;

void main(void)
{
    vec2 uv = gl_FragCoord.xy / image_size;
    vec4 image_pixel = texture(image, uv);
    
    // Chuyển đổi sang giá trị ảnh xám (Gray)
    // Công thức trọng số: Gray = 0.299*R + 0.587*G + 0.114*B
    float gray = dot(image_pixel.rgb, vec3(0.299, 0.587, 0.114));
    
    // Gán giá trị ảnh xám vào output
    gl_FragColor = vec4(vec3(gray), image_pixel.a);
}