#version 330 core

// VBO-b�l �rkez� v�ltoz�k
in vec3 vs_in_pos;
in vec3 vs_in_norm;
in vec2 vs_in_tex;
in int vs_in_atl_index;

// a pipeline-ban tov�bb adand� �rt�kek
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;

// shader k�ls� param�terei
uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;
uniform int atlas_row_count;


void main()
{
	gl_Position = MVP * vec4( vs_in_pos, 1 );
	
	vs_out_pos = (world * vec4(vs_in_pos, 1)).xyz;
	vs_out_norm = (worldIT * vec4(vs_in_norm, 0)).xyz;
	
	
	vec2 offset = vec2(vs_in_atl_index % atlas_row_count, floor(vs_in_atl_index / atlas_row_count));
	offset.x = offset.x / atlas_row_count;
	offset.y = offset.y / atlas_row_count;

	vs_out_tex  = (vs_in_tex / atlas_row_count) + offset;
}