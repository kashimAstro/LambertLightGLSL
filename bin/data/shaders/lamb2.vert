attribute vec2 position;
varying vec2 vTexCoord;

void main()
{

    gl_Position = gl_ModelViewProjectionMatrix * vec4(position, 0, 1);
    vTexCoord = position;//vec2(position * vec4(vTexCoord, 1, 1));
}
