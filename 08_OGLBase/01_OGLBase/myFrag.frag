#version 330 core

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

out vec4 fs_out_col;

// irány fényforrás: fény iránya
uniform vec3 light_dir;
uniform vec3 light_pos;

// fénytulajdonságok: ambiens, diffúz, ...
uniform vec3 La = vec3(0.4, 0.4, 0.4);
uniform vec3 Ld = vec3(0.6, 0.6, 0.6);

uniform sampler2D texImage;

void main()
{

	vec3 ambient = La;

	vec3 normal = normalize(vs_out_norm);
	vec3 to_light = normalize(light_pos - vs_out_pos);
	
	float cosa = max(dot(normal, to_light), 0);

	float spot_strength = max(dot(light_dir, - to_light) - 0.96, 0) * 30;

	vec3 diffuse = spot_strength * cosa * Ld;
	
	fs_out_col = vec4(ambient + diffuse, 1) * texture(texImage, vs_out_tex);
}