#include "CommandBase.h"

// Initialize a single static instance of all of your subsystems to NULL
OI* CommandBase::oi = NULL;
Chassis* CommandBase::chassis = NULL;
Mpu6050* CommandBase::imu = NULL;
Elevator* CommandBase::elevator = NULL;
LightRing* CommandBase::lightRing = NULL;
DriverStation* CommandBase::ds = NULL;
ToteIntake* CommandBase::toteIntake = NULL;

CommandBase::CommandBase(char const *name) :
		Command(name)
{
}

CommandBase::CommandBase() :
		Command()
{

}

void CommandBase::init()
{
	// Create a single static instance of all of your subsystems. The following
	// line should be repeated for each subsystem in the project.
  imu = new Mpu6050(I2C::kMXP);
  chassis = new Chassis();
  elevator = new Elevator();
  lightRing = new LightRing();
  ds = DriverStation::GetInstance();
  toteIntake = new ToteIntake();
  //always initialise last as it depends on subsystems
  oi = new OI();
}
