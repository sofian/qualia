
uniform sampler2D field;

const float kernelR[] = float[25](0,0,0,0,1,
                                  0,0,1,1,1,
                                  0,0,0,1,1,
                                  0,0,1,1,1,
                                  0,0,0,0,1);

const float kernelG[] = float[25](0,0,0,0,0,
                                  0,0,0,0,0,
                                  0,1,0,1,0,
                                  0,1,1,1,0,
                                  1,1,1,1,1);

const float kernelB[] = float[25](1,0,0,0,0,
                                  1,1,1,0,0,
                                  1,1,0,0,0,
                                  1,1,1,0,0,
                                  1,0,0,0,0);

const float kernelA[] = float[25](1,1,1,1,1,
                                  0,1,1,1,0,
                                  0,1,0,1,0,
                                  0,0,0,0,0,
                                  0,0,0,0,0);

const float gain = 0.97;

void main()
{
    int i,j,k;
    vec4 t,m,a=vec4(0,0,0,0);
    for (k=0; k<25; k++) {
        i = k%5;
        j = k/5;
        t = texture(field,
                    vec2((gl_FragCoord.x+i-2)/640,
                         (gl_FragCoord.y+j-2)/480));
        m = vec4(t.r * kernelR[k],
                 t.g * kernelG[k],
                 t.b * kernelB[k],
                 t.a * kernelA[k]) * gain;
        a = vec4(max(m.r, a.r),
                 max(m.g, a.g),
                 max(m.b, a.b),
                 max(m.a, a.a));
    }

    gl_FragColor = a;
}
