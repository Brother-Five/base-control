#ifndef _LOCATION_H_
#define _LOCATION_H_
#include "bsp.h"

typedef struct MOTOR_STRUCT
{
	struct
	{
		int32_t position;
		short   speed;
	}motor1;
	struct
	{
		int32_t position;
		short   speed;
	}motor2;
	struct
	{
		int32_t position;
		short   speed;
	}motor3;
	struct
	{
		int32_t position;
		short   speed;
	}motor4;
}MOTORSTRUCT;

typedef struct LOCATION_STRUCT
{
	float gobal_X,gobal_Y,gobal_Z;
}LOCATIONSTRUCT;

void Location(MOTORSTRUCT *_pMotorMassage);
#endif
