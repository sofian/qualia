
uniform sampler2D field;

const float kernelR[] = float[9](0,1,1,
                                 0,0,1,
                                 0,1,1);

const float kernelG[] = float[9](0,0,0,
                                 1,0,1,
                                 1,1,1);

const float kernelB[] = float[9](1,1,0,
                                 1,0,0,
                                 1,1,0);

const float kernelA[] = float[9](1,1,1,
                                 1,0,1,
                                 0,0,0);

const float gain = 0.97;

void main()
{
    int i,j,k;
    vec4 t,m,a=vec4(0,0,0,0);
    for (k=0; k<9; k++) {
        i = k%3;
        j = k/3;
        t = texture(field,
                    vec2((gl_FragCoord.x+i-1)/640,
                         (gl_FragCoord.y+j-1)/480));
        m = vec4(t.r * kernelR[i+3*j],
                 t.g * kernelG[i+3*j],
                 t.b * kernelB[i+3*j],
                 t.a * kernelA[i+3*j]) * gain;
        a = vec4(max(m.r, a.r),
                 max(m.g, a.g),
                 max(m.b, a.b),
                 max(m.a, a.a));
    }

    gl_FragColor = a;
}
