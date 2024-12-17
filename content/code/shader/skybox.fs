#version 400


struct CubeMap 
{
  sampler2D Back;
  sampler2D Front;
  sampler2D Down;
  sampler2D Up;
  sampler2D Left;
  sampler2D Right;
};

in vec3 FragPos;

out vec4 frag_Color;

uniform CubeMap cubeMap;
vec4 Cubemap();

void main()
{
	vec4 result = vec4(0,0,0,0);
	result = Cubemap();
	frag_Color = result;
}

vec4 Cubemap()
{
    vec4 result;
    vec2 uvreturn;
    vec3 pos = FragPos;

    if(abs(pos.y) < abs(pos.x))
    {
        if(abs(pos.z) < abs(pos.x))
        {
            // -X (Right face)
            if(pos.x < 0.0)
            {
                uvreturn.x = -pos.z / abs(pos.x);  
                uvreturn.y = pos.y / abs(pos.x);
                uvreturn.x = (uvreturn.x + 1.0) / 2.0; 
                uvreturn.y = 1.0 - (uvreturn.y + 1.0) / 2.0;  // Flip vertically
                result = texture(cubeMap.Right, uvreturn);
            }
            // +X (Left face)
            else
            {
                uvreturn.x = pos.z / abs(pos.x);  
                uvreturn.y = pos.y / abs(pos.x);
                uvreturn.x = (uvreturn.x + 1.0) / 2.0; 
                uvreturn.y = 1.0 - (uvreturn.y + 1.0) / 2.0;  // Flip vertically
                result = texture(cubeMap.Left, uvreturn);
            }
        }
        else
        {
            // +Z (Front face)
            if(pos.z > 0.0)
            {
                uvreturn.x = -pos.x / abs(pos.z);
                uvreturn.y = pos.y / abs(pos.z);
                uvreturn.x = (uvreturn.x + 1.0) / 2.0; 
                uvreturn.y = 1.0 - (uvreturn.y + 1.0) / 2.0;  // Flip vertically
                result = texture(cubeMap.Front, uvreturn);
            }
            // -Z (Back face)
            else
            {
                uvreturn.x = pos.x / abs(pos.z);
                uvreturn.y = pos.y / abs(pos.z);
                uvreturn.x = (uvreturn.x + 1.0) / 2.0; 
                uvreturn.y = 1.0 - (uvreturn.y + 1.0) / 2.0;  // Flip vertically
                result = texture(cubeMap.Back, uvreturn);
            }
        }
    }
    else
    {
        if(abs(pos.z) < abs(pos.y))
        {
            // +Y (Up face) - Rotate 90 degrees clockwise
			if (pos.y > 0.0)
			{
				uvreturn.x = -pos.x / abs(pos.y);  
				uvreturn.y = -pos.z / abs(pos.y);  
				uvreturn.x = (uvreturn.x + 1.0) / 2.0; 
				uvreturn.y = 1.0 - (uvreturn.y + 1.0) / 2.0;  // Flip vertically
				result = texture(cubeMap.Up, uvreturn);
			}
            // -Y (Down face) - Counterclockwise 90 degree rotation
            else
            {
                uvreturn.x = -pos.x / abs(pos.y);
                uvreturn.y = pos.z / abs(pos.y);
                uvreturn.x = (uvreturn.x + 1.0) / 2.0; 
                uvreturn.y = 1.0 - (uvreturn.y + 1.0) / 2.0;  // Flip vertically
                result = texture(cubeMap.Down, uvreturn);
            }
        }
        else
        {
            // +Z (Front face)
            if(pos.z > 0.0)
            {
                uvreturn.x = -pos.x / abs(pos.z);
                uvreturn.y = pos.y / abs(pos.z);
                uvreturn.x = (uvreturn.x + 1.0) / 2.0; 
                uvreturn.y = 1.0 - (uvreturn.y + 1.0) / 2.0;  // Flip vertically
                result = texture(cubeMap.Front, uvreturn);
            }
            // -Z (Back face)
            else
            {
                uvreturn.x = pos.x / abs(pos.z);
                uvreturn.y = pos.y / abs(pos.z);
                uvreturn.x = (uvreturn.x + 1.0) / 2.0; 
                uvreturn.y = 1.0 - (uvreturn.y + 1.0) / 2.0;  // Flip vertically
                result = texture(cubeMap.Back, uvreturn);
            }
        }
    }

    return result;
}
