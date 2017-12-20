#version 410

out vec4 fragment_colour; // final colour of surface

in vec4 position;
in vec2 uv;

const float offset = 1.0 / 128.0;

uniform sampler2D uTexColor;
uniform sampler2D uTexNormals;

vec3 sampleNrm(sampler2D tex, vec2 texcoords) {
	return texture(tex,texcoords).xyz;
}

void main () {
	
	vec3 center = sampleNrm( uTexNormals, vec2(0.0, 0.0) );

 	 vec3 top = sampleNrm( uTexNormals, vec2(0.0, offset) );
 	 vec3 topRight = sampleNrm( uTexNormals, vec2(offset, offset) );
 	 vec3 right = sampleNrm( uTexNormals, vec2(offset, 0.0) );

 	 // the rest is pretty arbitrary, but seemed to give me the
 	 // best-looking results for whatever reason.

 	 vec3 t = center - top;
 	 vec3 r = center - right;
 	 vec3 tr = center - topRight;

 	 t = abs( t );
 	 r = abs( r );
 	 tr = abs( tr );

 	 float n;
 	 n = max( n, t.x );
 	 n = max( n, t.y );
 	 n = max( n, t.z );
 	 n = max( n, r.x );
 	 n = max( n, r.y );
 	 n = max( n, r.z );
 	 n = max( n, tr.x );
 	 n = max( n, tr.y );
 	 n = max( n, tr.z );

  	// threshold and scale.
 	 n = 1.0 - clamp( clamp((n * 2.0) - 0.8, 0.0, 1.0) * 1.5, 0.0, 1.0 );

  	fragment_colour.rgb = texture(uTexColor, uv).rgb * (0.1 + 0.9*n);
}
