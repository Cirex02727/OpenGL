#shader vertex
#version 460 core

layout(location = 0) in uint data;

uniform mat4 u_MVP;

flat out ivec4 o_fragData;
out vec2 o_uvs;

void main()
{
	vec2 UVS[4] = vec2[](vec2(0.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 0.0), vec2(1.0, 1.0));

	int x    = int((data >> 00u) & (0x1Fu));
	int y    = int((data >> 05u) & (0xFFu));
	int z    = int((data >> 13u) & (0x1Fu));
	int side = int((data >> 18u) & (0x07u));
	int uv   = int((data >> 21u) & (0x03u));

	o_fragData = ivec4(x, y, z, side);
	o_uvs = UVS[uv];

	vec3 coord = vec3(x, y, z) - vec3(0.5);
	gl_Position = u_MVP * vec4(coord, 1.0);
}

#shader fragment
#version 460 core

layout(location = 0) out vec4 color;

uniform sampler2D inTexture;

flat in ivec4 o_fragData;
in vec2 o_uvs;

void main()
{
	vec3 SIDE[6] = vec3[](vec3(0.0, 0.0, 1.0), vec3(0.0, 0.0, -1.0), vec3(1.0, 0.0, 0.0), vec3(-1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, -1.0, 0.0));
	
	vec3 lightPos = vec3(o_fragData.x + 0.7, o_fragData.y + 3, o_fragData.z + 0.7);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	vec3 norm = normalize(SIDE[o_fragData.w]);
	vec3 lightDir = normalize(lightPos - o_fragData.xyz);
	
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	//vec3 result = (ambient + diffuse) * vec3(0.234, 0.156, 0.05);
	color = texture(inTexture, o_uvs) * vec4((ambient + diffuse), 1.0);
}