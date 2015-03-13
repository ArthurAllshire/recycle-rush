#include "Robot.h"
#include <lib-4774/Functions.h>

#include <Commands/Auton/BinToteAuto.h>
#include <Commands/Auton/BinAuto.h>
#include <Commands/Auton/ThreeToteAuto.h>
#include <Commands/Move/MoveForward.h>
#include <Commands/Auton/ToteAuto.h>


void Robot::RobotInit()
{
    CommandBase::init();
    lw = LiveWindow::GetInstance();
    ir = new AnalogInput(0);
    binToteAuto = new BinToteAuto();
    binAuto = new BinAuto();
    threeToteAuto = new ThreeToteAuto();
    moveToAutoZone = new MoveForward(1.0);
    toteAuto = new ToteAuto();

    autoChooser = new SendableChooser();
    autoChooser->AddDefault("Bin", binAuto);
    autoChooser->AddObject("Bin Tote", binToteAuto);
    autoChooser->AddObject("Three Tote", threeToteAuto);
    autoChooser->AddObject("Move To Auto", moveToAutoZone);
    autoChooser->AddObject("Tote Auto", toteAuto);
    SmartDashboard::PutData("Autonomous Mode: ", autoChooser);


}

void Robot::DisabledPeriodic()
{
    Scheduler::GetInstance()->Run();
    PutDashboard();

    CommandBase::elevator->endSwitchTripped = false;
    CommandBase::elevator->readySwitchTopTripped = false;
    CommandBase::elevator->readySwitchBottomTripped = false;
    CommandBase::elevator->binSwitchTripped = false;
    irTripped = false;

}

void Robot::AutonomousInit()
{
    /*if (autonomousCommand != NULL)
        autonomousCommand->Start();*/

    CommandBase::chassis->DisablePID();
    autonomousCommand = (Command*) autoChooser->GetSelected();
    autonomousCommand->Start();
}

void Robot::AutonomousPeriodic()
{
    Scheduler::GetInstance()->Run();
    PutDashboard();
}

void Robot::TeleopInit()
{
    // This makes sure that the autonomous stops running when
    // teleop starts running. If you want the autonomous to
    // continue until interrupted by another command, remove
    // this line or comment it out.
    if (autonomousCommand != NULL)
        autonomousCommand->Cancel();
    CommandBase::chassis->EnablePID();
}

void Robot::TeleopPeriodic()
{
    Scheduler::GetInstance()->Run();
    PutDashboard();
    SmartDashboard::PutNumber("Encoder Cm: ", CommandBase::elevator->getEncoder());
}

void Robot::TestPeriodic()
{
    lw->Run();
}

void Robot::PutDashboard() {
    SmartDashboard::PutData(CommandBase::imu);

    SmartDashboard::PutNumber("RollDeg", lib4774::r2d(CommandBase::imu->GetRoll()));
    SmartDashboard::PutNumber("PitchDeg", lib4774::r2d(CommandBase::imu->GetPitch() ));
    SmartDashboard::PutNumber("YawDeg", lib4774::r2d(CommandBase::imu->GetYaw()));

    SmartDashboard::PutBoolean("End Switch Tripped", CommandBase::elevator->endSwitchTripped);
    SmartDashboard::PutBoolean("Ready Switch Top Tripped", CommandBase::elevator->readySwitchTopTripped);
    SmartDashboard::PutBoolean("Ready Switch Bottom Tripped", CommandBase::elevator->readySwitchBottomTripped);
    SmartDashboard::PutBoolean("Bin Switch Tripped", CommandBase::elevator->binSwitchTripped);

    if(ir->GetValue() > 1500) {
        irTripped = true;
    }
    SmartDashboard::PutBoolean("Ir Trippped: ", irTripped);
    SmartDashboard::PutNumber("Ir Sensor", ir->GetValue());
    CommandBase::elevator->PutDashboard();
    CommandBase::chassis->PutDashboard();
}


START_ROBOT_CLASS(Robot);

