/// \file
/// Define several utility functions.

#pragma once
#include <string>
#include <gl/glut.h>

#include "Vector3D.h"

using namespace std;

/// Display text at the current raster position using font GLUT_BITMAP_9_BY_15.
/// \param str String to be displayed.
void printString(string str);

/// Display text at the current raster position using font GLUT_BITMAP_HELVETICA_18.
/// \param str String to be displayed.
void printLargeString(string str);

/// Compute a Vector3 rotated around y axis based on an input Vector3 and an angle.
/// \param input Input Vector3
/// \param angle Angle of rotation
/// \return Resulting Vector3 generated by rotating input around y axis for the angle.
Vector3 computeRotatedVector(Vector3 input, float angle);