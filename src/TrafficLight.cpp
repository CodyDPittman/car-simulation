#include "TrafficLight.h"


TrafficLight::TrafficLight(void)
{
}


TrafficLight::~TrafficLight(void)
{
}

void TrafficLight::setMaterials()
{
	// In this function, you are supposed to assign values to the variables redOn, redOff,
	// yellowOn, yellowOff, greenOn, greenOff.

	// Disabled light values
	yellowOff.Kd[0] = 0.2;
	greenOff.Kd[0] = 0.2;
	redOff.Kd[0] = 0.2;

	// Enabled light values
	redOn = materials["_Red_"];
	yellowOn = materials["_Yellow_"];
	greenOn = materials["_Green_"];

}

void TrafficLight::setSignal(Signal signal)
{
	

	// You are supposed to assign the materials used in the ObjModel class based on
	// values of the input signal.
	switch (signal)
	{
	case Green:
		materials["_Green_"] = greenOn;
		materials["_Red_"] = redOff;
		materials["_Yellow_"] = yellowOff;
		break;
	case Yellow:
		materials["_Yellow_"] = yellowOn;
		materials["_Green_"] = greenOff;
		materials["_Red_"] = redOff;
		break;
	case Red:
		materials["_Red_"] = redOn;
		materials["_Yellow_"] = yellowOff;
		materials["_Green_"] = greenOff;
		break;
	}

}

void TrafficLight::ReadFile(string fileName)
{
	ObjModel::ReadFile(fileName);
	setMaterials();
}