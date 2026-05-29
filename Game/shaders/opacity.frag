uniform sampler2D texture;
uniform float alpha;

void main()
{
    vec4 color = gl_Color;
    color.a = alpha;
    gl_FragColor = color;
}