#include "location.h"
#include "math.h"
MOTORSTRUCT MotorMassage;
LOCATIONSTRUCT car_location;
#define Pi 3.141592f
#define D2R 0.0174532f
#define R2D 57.295779f
float body_X,body_Y,body_C;
void Location(MOTORSTRUCT *_pMotorMassage)
{
	float dx,dy;
	static uint32_t now_time,last_time;
	float dt;

	if(last_time == 0)
	{
		last_time = micros();
		return ;
	}
	now_time = micros();
	if(now_time < last_time)
	{
	  	dt = (float)(now_time + (0xFFFFFFFF - last_time) );
	}
	else
	{
		dt = (float)(now_time - last_time);
	}
  	last_time = now_time ;

	dt /= 1000000.0f;
	dx = -body_X*dt;
	dy = -body_Y*dt;

	body_C *=0.61591103507271171941830624465355f;
	body_C -=(((int)body_C)/360)*360;
	if(body_C<0)
		body_C+=360;

//	u1_printf("%d\t%d\t%d\r\n",(int)dx,(int)dy,(int)body_C);
//	body_Y=(float)(_pMotorMassage->motor1.speed+_pMotorMassage->motor2.speed+_pMotorMassage->motor3.speed+_pMotorMassage->motor4.speed)/4;
//	body_X=(float)(_pMotorMassage->motor1.speed-_pMotorMassage->motor2.speed+_pMotorMassage->motor3.speed-_pMotorMassage->motor4.speed)/4;
//	body_C=(float)(-_pMotorMassage->motor1.position+_pMotorMassage->motor2.position+_pMotorMassage->motor3.position-_pMotorMassage->motor4.position)/4*90/25844;
	// if(body_C>=180.0f)
	// 	body_C=body_C-360.0f;
	// else if(body_C<-180.0)
	// 	body_C=body_C+360.0f;
//	if(body_C>=-180&&body_C<180)
		{
// 			car_location.gobal_X+=(body_Y*sin(body_C*Pi/180.0f)+body_X*cos(body_C*Pi/180.0f));
// 			car_location.gobal_Y+=(body_Y*cos(body_C*Pi/180.0f)-body_X*sin(body_C*Pi/180.0f));
			// car_location.gobal_X+=(body_Y*sin(10*Pi/180)+body_X*cos(10*Pi/180));
 		//  	car_location.gobal_Y+=(body_Y*cos(10*Pi/180)-body_X*sin(10*Pi/180));
			// car_location.gobal_X+=(dy*sin(body_C*D2R)+dx*cos(body_C*D2R));
			// car_location.gobal_Y+=(dy*cos(body_C*D2R)-dx*sin(body_C*D2R));
			// car_location.car_body_C=body_C;
//			car_location.gobal_X = sin(-1*D2R);
		}
		// temp[0] = arm_sin_f32(90*D2R);
		car_location.gobal_X += (dy*arm_sin_f32(body_C*D2R)+dx*arm_cos_f32(body_C*D2R));
		car_location.gobal_Y += (dy*arm_cos_f32(body_C*D2R)-dx*arm_sin_f32(body_C*D2R));
		car_location.gobal_Z=body_C;
		
}
