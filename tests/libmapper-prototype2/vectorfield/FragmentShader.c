
uniform sampler2D field;

void main()
{
  	gl_FragColor = vec4(gl_FragCoord.x/640, gl_FragCoord.y/480, 0, 1);
}
