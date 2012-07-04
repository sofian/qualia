
uniform sampler2D field;
uniform float kernels[100];
uniform float gain;

void main()
{
    int i,j,k;
    vec4 t,m,a=vec4(0,0,0,0);
    for (k=0; k<25; k++) {
        i = int(mod(float(k), 5.0));
        j = k / 5;
        t = texture2D(field,
                      vec2((gl_FragCoord.x+float(i)-2.0)/640.0,
                           (gl_FragCoord.y+float(j)-2.0)/480.0));
        m = vec4(t.r * kernels[k + 0],
                 t.g * kernels[k + 25],
                 t.b * kernels[k + 50],
                 t.a * kernels[k + 75]) * gain;
        a = vec4(max(m.r, a.r),
                 max(m.g, a.g),
                 max(m.b, a.b),
                 max(m.a, a.a));
    }

    gl_FragColor = a;
}
