#include "Billboard.h"


Billboard::Billboard(void)
{
}

Billboard::~Billboard(void)
{
}

/// <summary>
/// Reads the texture file information for the billboard image.
/// </summary>
/// <param name="fileName"></param>
void Billboard::ReadFile(string fileName)
{
	// Read the texture file, generate the texture object, and configure
	// the texturing parameters.

	// Read the PPM texture file
	textureImage.ReadFile(fileName);
	cout << "Successfully read the texture file" << fileName << " " << endl;

	// Generate the texture object
	glGenTextures(1, &textureNumber);
	glBindTexture(GL_TEXTURE_2D, textureNumber);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureImage.width, textureImage.height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, textureImage.image);

	// Configure texturing parameters
	glBindTexture(GL_TEXTURE_2D, textureNumber);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

}

/// <summary>
/// Determines the size of the billboard.
/// </summary>
/// <param name="width"></param>
/// <param name="height"></param>
void Billboard::SetSize(float width, float height)
{
	// Your code
	glScalef(width, height, 1);
}
/// <summary>
/// Determines the location of the billboard.
/// </summary>
/// <param name="location"></param>
void Billboard::SetLocation(Vector3 location)
{
	// Your code
	glTranslatef(location.x, location.y, location.z);
}

/// <summary>
/// Determines the orientation of the billboard.
/// </summary>
/// <param name="orientation"></param>
void Billboard::SetOrientation(float orientation)
{
	// Your code
	glRotatef(orientation, 0, 1, 0);
}

void Billboard::Draw()
{	
	// Draw the board and pillar.  Use texture mapping for the board only.
	// The pillar is drawn just using simple geometry, i.e., a rectangle.
	// Use glEnable() and glDisable() to change OpenGL states
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureNumber);
	
	// Makes the billboards not transparent.
	glDisable(GL_BLEND);

	// Drawing the billboard.
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(-.5, 0);
	glTexCoord2f(1, 0); glVertex2f(.5, 0);
	glTexCoord2f(1, 1); glVertex2f(.5, 1);
	glTexCoord2f(0, 1); glVertex2f(-.5, 1);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	// Drawing the pillar.
	glColor3f(1, 1, 1);
	glRectf(location.x, -location.y, location.x + .05, -2);	
}
