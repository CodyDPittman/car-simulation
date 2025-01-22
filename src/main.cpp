/// \file
/// This is the main file for the project Traffic Simulation.
/// \author Yuzhong Shen
/// \version 1.0
/// \date 12/10/2017
///

#include <iostream>
#include <sstream>
#include <string>

#include <gl/glew.h>
#include <gl/glut.h>

#include "ObjModel.h"
#include "TrafficLight.h"
#include "utility.h"
#include "BillBoard.h"

using namespace std;

int counter = 0; ///< Counter for the traffic signal.  Only one is needed.
int updateInterval = 20; ///< Update interval for the update function in miliseconds.
int carSpeed = 0;

ObjModel car;
ObjModel surveillanceCamera;
TrafficLight trafficLight;
Billboard billboard1, billboard2; ///<Objects for the billboards
PPMImage i;

int carID; ///< Display List ID for car
int surveillanceCameraID; ///< Display list ID for surveillance camera
int terrainID; ///< Display list ID for terrain

Signal NS_Signal = Green;  ///< North-South signal.
Signal WE_Signal = Red;  ///< West-East signal.

Vector3 carPosition = { 3, 0, 45 }; ///< Car position with initial value.
Vector3 localCarSpeed = { 0, 0, 0 }; ///< Car speed in car's local space.
Vector3 worldCarSpeed; ///< Car speed in world space.

float carDirection = 180;  ///< Car direction
string carHeading = "N"; ///< String for car heading

Vector3	localCameraOffset = { 0, 0, -6 };  ///< Third person camera offset in the car's local space.
Vector3 worldCameraOffset; // by calling computeRotateVector input i localcarspeed and angle is carDirction for localCarSpeed and  worldCarSpeed from compute the output is worldcarspeed ///< Third person camera offset in world space.
Vector3 billboardLocation;

int winWidth; ///< Width of OpenGL window
int winHeight; ///< Height of OpenGL window
int sWidth = winWidth/2; ///< Width of the small viewport
int sHeight = winHeight/4; ///< Height of the small viewport

/// Update the small viewports' size automatically.
/// \param w Width of the OpenGL window
/// \param h Height of the OpenGL window
void reshape(int w, int h)
{
	winWidth = w, winHeight = h;

	// Update sWidth and sHeight here.
	sWidth = winWidth / 2;
	sHeight = winHeight / 4;
}

/// Callback function for special keys.
/// \param key ASCII code of the key pressed.
/// \param x X coordinate of the mouse cursor when the key is pressed.
/// \param y Y coordinate of the mouse cursor when the key is pressed.
void specialKey(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)
	{
		// Update car direction
		carDirection += 1.00;
		
		// Compute the car heading.
		if (carDirection == 180) {
			carHeading = "N";
		}
		else if (carDirection == 0) {
			carHeading = "S";
		}
		else if (carDirection == 90) {
			carHeading = "E";
		}
		else if (carDirection == 270) {
			carHeading = "W";
		}
		else if (carDirection > 360 || carDirection < 0) {
			carDirection = 0;
		}

	
		// Update the third person camera offset in the world frame.
		worldCameraOffset = computeRotatedVector(localCameraOffset,  carDirection);
		worldCarSpeed = computeRotatedVector(localCarSpeed, carDirection);
	
		
	}
	if (key == GLUT_KEY_RIGHT)
	{
		// Handle the right turns.
		carDirection -= 1.0;

		// Compute the car heading.
		if (carDirection == 180) {
			carHeading = "N";
		}
		else if (carDirection == 0) {
			carHeading = "S";
		}
		else if (carDirection == 90) {
			carHeading = "E";
		}
		else if (carDirection == 270) {
			carHeading = "W";
		}
		else if (carDirection > 360 || carDirection < 0) {
			carDirection = 360;
		}

		
		worldCameraOffset = computeRotatedVector(localCameraOffset, carDirection);
		worldCarSpeed = computeRotatedVector(localCarSpeed, carDirection);
		
	}
	if (key == GLUT_KEY_UP)
	{
		// acceleration
		localCarSpeed.z = .75;
		worldCarSpeed = computeRotatedVector(localCarSpeed, carDirection);
	}

	if (key == GLUT_KEY_DOWN)
	{
		// deceleration
		localCarSpeed.z = -.75;
		worldCarSpeed = computeRotatedVector(localCarSpeed, carDirection);	
	}

	
	cout << carDirection << "\n";

	glutPostRedisplay();
}

/// Function to draw the entire scene.
void drawScene()
{
	// Draw terrain
	glCallList(terrainID);

	glEnable(GL_LIGHTING);

	// North-East (NS_Signal)
	glPushMatrix();
	glTranslatef(10, 0, -10.5); // to origin
	glScalef(1/3.28/12, 1/3.28/12, 1/3.28/12); // scale
	trafficLight.setSignal(NS_Signal);
	trafficLight.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 0, -10);
	glRotatef(-45, 0, 1, 0);
	glCallList(surveillanceCameraID);
	glPopMatrix();

	// South-West (NS_Signal)
	glPushMatrix();
	glTranslatef(-10, 0, 10.5);
	glRotatef(180, 0, 1, 0);
	glScalef(1 / 3.28 / 12, 1 / 3.28 / 12, 1 / 3.28 / 12);
	trafficLight.setSignal(NS_Signal);
	trafficLight.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10, 0, 10);
	glRotatef(135, 0, 1, 0);
	glCallList(surveillanceCameraID);
	glPopMatrix();

	// South-East (WE_Signal)
	glPushMatrix();
	glTranslatef(10, 0, 10.5);
	glRotatef(-90, 0, 1, 0);
	glScalef(1 / 3.28 / 12, 1 / 3.28 / 12, 1 / 3.28 / 12);
	trafficLight.setSignal(WE_Signal);
	trafficLight.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 0, 10);
	glRotatef(225, 0, 1, 0);
	glCallList(surveillanceCameraID);
	glPopMatrix();

	// North-West (WE_Signal)
	glPushMatrix();
	glTranslatef(-10, 0, -10.5);
	glRotatef(90, 0, 1, 0);
	glScalef(1 / 3.28 / 12, 1 / 3.28 / 12, 1 / 3.28 / 12);
	trafficLight.setSignal(WE_Signal);
	trafficLight.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10, 0, -10);
	glRotatef(45, 0, 1, 0);
	glCallList(surveillanceCameraID);
	glPopMatrix();

	// Draw the car.
	glPushMatrix();
	glTranslatef(carPosition.x, carPosition.y, carPosition.z);
	glRotatef(carDirection, 0, 1, 0);
	//glScalef(1 / 3.78 / 12, 1 / 3.78 / 12, 1 / 3.78 / 12);
	glCallList(carID);
	glPopMatrix();

	
	
	/// Drawing the Google billboard.
	glPushMatrix();
	billboardLocation = { 20, 3, -15 };
	billboard1.SetLocation(billboardLocation);
	billboard1.SetOrientation(315);
	billboard1.SetSize(15, 8);
	billboard1.Draw();
	glPopMatrix();

	/// Drawing the McDonald's billboard.
	glPushMatrix();
	billboardLocation = { -20, 5, -25 };
	billboard2.SetLocation(billboardLocation);
	billboard2.SetOrientation(30);
	billboard2.SetSize(10, 10);
	billboard2.Draw();
	glPopMatrix();

	glEnable(GL_LIGHTING);
}

/// Initialization.
/// Set up lighting, generate display lists for the surveillance camera, 
/// car, and terrain.
void init()
{
	glClearColor(0.5, 0.5, 1.0, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Note that the light is defined in the eye or camera frame.
	GLfloat light_position[] = {0.0, 0.0, 0.0, 1.0};

	GLfloat ambient[] = {0.3, 0.3, 0.3, 1};
	GLfloat diffuse[] = {1.0, 1.0, 1.0, 1};
	GLfloat specular[] = {1.0, 1.0, 1.0, 1};

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glEnable(GL_LIGHTING);	
	glEnable(GL_LIGHT0);

	// Generate display list for the surveillance camera.
	surveillanceCameraID = glGenLists(1);
	
	glNewList(surveillanceCameraID, GL_COMPILE);
	surveillanceCamera.Draw();
	glEndList();

	// Generate display list for the car.
	carID = glGenLists(1);

	glNewList(carID, GL_COMPILE);
	car.Draw();
	glEndList();


	// Generate the display list for terrain, including road and grass.
	terrainID = glGenLists(1);
	glNewList(terrainID, GL_COMPILE);
	glDisable(GL_LIGHTING);

	// Grass
	glColor3f(0, 0.7, 0);
	glBegin(GL_QUADS);
		glVertex3f(-1000, 0, 1000);
		glVertex3f(-10, 0, 1000);
		glVertex3f(-10, 0, 10);
		glVertex3f(-1000, 0, 10);

		glVertex3f(10, 0, 1000);
		glVertex3f(1000, 0, 1000);
		glVertex3f(1000, 0, 10);
		glVertex3f(10, 0, 10);

		glVertex3f(10, 0, -10);
		glVertex3f(1000, 0, -10);
		glVertex3f(1000, 0, -1000);
		glVertex3f(10, 0, -1000);

		glVertex3f(-1000, 0, -10);
		glVertex3f(-10, 0, -10);
		glVertex3f(-10, 0, -1000);
		glVertex3f(-1000, 0, -1000);
	glEnd();

	// Roads
	glBegin(GL_QUADS);
		glColor3f(0.2, 0.2, 0.2);

		glVertex3f(-10, 0, 1000);
		glVertex3f(10, 0, 1000);
		glVertex3f(10, 0, -1000);
		glVertex3f(-10, 0, -1000);

		glVertex3f(-1000, 0, 10);
		glVertex3f(1000, 0, 10);
		glVertex3f(1000, 0, -10);
		glVertex3f(-1000, 0, -10);
	glEnd();

	// South Yellow line
	glBegin(GL_POLYGON);
		glColor3f(1, 1, 0);
		glVertex3f(-0.1, 0.05, 1000);
		glVertex3f(0.1, 0.05, 1000);
		glVertex3f(0.1, 0.0, 10);
		glVertex3f(-0.1, 0.0, 10);
	glEnd();

	// North Yellow line
	glBegin(GL_POLYGON);
	glVertex3f(-0.1, 0.05, -1000);
	glVertex3f(0.1, 0.05, -1000);
	glVertex3f(0.1, 0.05, -10);
	glVertex3f(-0.1, 0.05, -10);
	glEnd();

	// East Yellow line
	glBegin(GL_POLYGON);
	glVertex3f(-10, -.05, -.1);
	glVertex3f(-10, .05, .1);
	glVertex3f(-1000, .05, -.1);
	glVertex3f(-1000, -.05, .1);
	glEnd();

	// West Yellow line
	glBegin(GL_POLYGON);
	glVertex3f(10, -.05, -.1);
	glVertex3f(10, .05, .1);
	glVertex3f(1000, .05, -.1);
	glVertex3f(1000, -.05, .1);
	glEnd();

	glEndList();
}

/// Display callback.
/// Displays 4 viewports.  For for each viewport, set up position and size, projection, 
/// and camera (ModelView matrix).
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Generate head-up display (HUD)
	glColor3f(1, 1, 0);
	stringstream ss;
	glWindowPos2d((winWidth / 2) - 115, winHeight - 220);
	printLargeString("Heading:");
	//glWindowPos2d((winWidth / 2) - 120, winHeight - 220);
	printLargeString(carHeading);
	glWindowPos2d((winWidth / 2) + 10, winHeight - 220);
	printLargeString("Speed:");
	printLargeString(to_string(carSpeed));
	printLargeString(" mph");


	// Setup viewport, projection, and camera for the main view.
	glViewport(0, 0, winWidth, winHeight - sHeight - 50);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (float) winWidth / (winHeight - sHeight - 50), 1, 1000); // these are in meters
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Update the code here for the correct third person camera that moves with the car.
	gluLookAt(carPosition.x + worldCameraOffset.x, carPosition.y + 2 + worldCameraOffset.y, carPosition.z + worldCameraOffset.z, carPosition.x, carPosition.y + 1.5,
		carPosition.z, 0, 1, 0);
	drawScene();

	////// update worldCameraOffset as the car turns
	worldCameraOffset = computeRotatedVector(localCameraOffset, carDirection);
	worldCarSpeed = computeRotatedVector(localCarSpeed, carDirection);
	
	// Setup viewport, projection, and camera for the South-East camera and draw the scene again.
	glViewport(1, winHeight - 200, sWidth - 230, sHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (float)(sWidth-230)/sHeight, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt(10, 3, 10, 4, 3, 0, 0, 1, 0);
	drawScene();

	// Setup the viewport, projection, camera for the top view and draw the scene again.
	glViewport((winWidth/2) - 215, winHeight - 200, sWidth - 230, sHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)(sWidth - 230) / sHeight, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0, 30, 0, 0, 0, 0, 0, 0, -1);
	drawScene();

	// Setup viewport, projection, camera for the South-West camera and draw the scene again.
	glViewport((winWidth/2) + 220, winHeight - 200, sWidth - 230, sHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (float)(sWidth - 230) / sHeight, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(-10, 3, 10, 4, 3, 0, 0, 1, 0);
	drawScene();

	glutSwapBuffers();
	glFlush();
}

/// Keyboard callback
/// Handle regular key presses, and for P2, "r" for reset, "b" for break, "s" for 
/// a screenshot and escape for quit.
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'r':
		// Add code for reset
		carPosition.x = 3;
		carPosition.y = 0;
		carPosition.z = 45;
		carDirection = 180;
		carHeading = "N";
		//localCarSpeed.x = 0;
		//localCarSpeed.y = 0;
		//localCarSpeed.z = 0;
		localCarSpeed = { 0, 0, 0 };
		//worldCarSpeed = { 0, 0, 0 };
		break;
	case 'b':
		// Add code for breaking.
		//localCarSpeed.x = 0;
		//localCarSpeed.y = 0;
		//localCarSpeed.z = 0;
		localCarSpeed = { 0, 0, 0 };
		//worldCarSpeed = { 0, 0, 0 };
		break;
	case 's':
		// Code for taking a screenshot of the display window
		i.AllocateMemory(winWidth, winHeight);
		glReadPixels(0, 0, winWidth, winHeight, GL_RGB, GL_UNSIGNED_BYTE, i.image);
		i.VerticalFlip();
		i.WriteFile("screenshot.ppm", "P6");
		break;
	case 27:
		exit(0);
		break;
	}

	glutPostRedisplay();
}



/// Updates the dynamic objects.
/// Update the car position and traffic signals.
void update()
{
	counter++; // 1 second = when counter is 50

	float lastX, lastZ;

	lastX = carPosition.x;
	lastZ = carPosition.z;

	// Update car position.
	carPosition.x += worldCarSpeed.x;
	carPosition.y += worldCarSpeed.y;
	carPosition.z += worldCarSpeed.z;

	// Calculate the distance to find total speed in MPH.
	float d = sqrt((pow(carPosition.x - lastX, 2)) + ((pow(carPosition.z - lastZ, 2))));

	// Meters per second = Distance in meters traveled / Time(seconds) , where Seconds = counter/20
	float speed_meterspersecond = d / .05;
	carSpeed = speed_meterspersecond * 2.2369;


	// State machine for the traffic signals using three variables: NS_Signal, WE_Signal, and counter.
	// Green for 5 seconds
	if (counter < 250)
	{
		NS_Signal = Green;
		WE_Signal = Red;
	}
	
	// Yellow for 1 second
	if (counter > 250 && counter < 300)
	{
		NS_Signal = Yellow;
	}

	// Red for 6 seconds
	if (counter > 300 && counter < 600)
	{
		WE_Signal = Green;
		NS_Signal = Red;
	}

	if (counter > 600)
		counter = 0;

}

/// Set the interval between updates.
/// \param miliseconds is the number of miliseconds passed before this function is called.  It is the third
/// parameter of glutTimerFunc().
void timer(int miliseconds)
{
	update();
	glutTimerFunc(updateInterval, timer, updateInterval);	
	glutPostRedisplay();
}

/// Main function
/// GLUT initialization, load 3D models, and register GLUT callbacks.
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	winWidth = 1300, winHeight = 800;
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Traffic Simulation");
	glewInit();

	// Load the 3D models.
	trafficLight.ReadFile("Models/TrafficLight.obj");
	car.ReadFile("Models/taxi.obj");
	surveillanceCamera.ReadFile("Models/camera.obj");

	// Load the billboard texture images.
	billboard1.ReadFile("Models/google.ppm");
	billboard2.ReadFile("Models/mcdonalds.ppm");

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKey);
	glutTimerFunc(0, timer, updateInterval);
	glutMainLoop();

	system("pause");
}