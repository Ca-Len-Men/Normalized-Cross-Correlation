#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;

void main()
{
    vec2 uv = gl_FragCoord.xy/u_resolution.xy;
    vec4 color = vec4(0.0);

    vec2 bl = smoothstep(vec2(0.0), vec2(0.1), uv);
    vec2 tr = smoothstep(vec2(0.0), vec2(0.1), 1.0 - uv);
    float alpha = bl.x * bl.y * tr.x * tr.y;

    color = vec4(0, 0, 0, alpha);
    gl_FragColor = color;
}