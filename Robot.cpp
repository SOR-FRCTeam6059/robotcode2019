#include <frc/Joystick.h>
#include <frc/TimedRobot.h>
#include <frc/Timer.h>
#include <frc/PWMVictorSPX.h>
#include <frc/drive/DifferentialDrive.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/DoubleSolenoid.h>
#include <frc/Solenoid.h>
#include <frc/Compressor.h>

/*
bool prevState = false;
bool currState = false;
bool driveTankMode = false;

while (IsOperatorControl())
{
    //
    // Check for button 6 press.
    //
    currState = secondaryController->GetRawButton(6);
    if (currState != prevState)
    {
        if (currState)
        {
            //
            // Button has been pressed.
            //
            driveTankMode = !driveTankMode;
        }
        else
        {
            //
            // Button has been released.
            //
        }
        prevState = currState;
    }

    if (driveTankMode)
    {
        //
        // Do tank drive.
        //
   }
    else
    {
        //
        // Do arcade drive.
        //
   }

    wait(0.02);
}
*/



/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/



class Robot : public frc::TimedRobot {
 public:
  Robot() {}

  void RobotInit() override {}
  void RobotPeriodic() override {}
  void AutonomousInit() override {}
  void AutonomousPeriodic() override {}

  void TeleopInit() override {
  compressor.Start();
  }

  void TeleopPeriodic() override {
    //Drivebase(Works)
    //The * -1 is to inverse motors, and the / 2 is to slow down regular turing and the / 3 if for presicion drive
    //the else if statments is mostly likely unneeded, will test with and without so see if it is needed
    // 3 = A button
    if (xbox.GetRawButton(3) == 1) {
      Drive.ArcadeDrive(xbox.GetRawAxis(1) * -1 / 2, xbox.GetRawAxis(0) / 2); 
    } else if (xbox.GetRawButton(3) == 0) { 
      Drive.ArcadeDrive(xbox.GetRawAxis(1) * -1, xbox.GetRawAxis(0) / 2);
    } else {
      Drive.ArcadeDrive(xbox.GetRawAxis(1) * -1, xbox.GetRawAxis(0) / 2);
    }

    //HAB lift
    //Front(Untested)
    //Using trigger axis for ease of use for operator but idk if the code will work like I have it
    if (xbox.GetRawAxis(2) >= .5) {
      s_HABfront.Set(frc::DoubleSolenoid::kForward);
    } else if (xbox.GetRawAxis(2) == 0) {
      s_HABfront.Set(frc::DoubleSolenoid::kReverse);
    } else {
      s_HABfront.Set(frc::DoubleSolenoid::kOff);
    }
    //Back(Untested)
    if (xbox.GetRawAxis(3) >= .5) {
      s_HABback.Set(frc::DoubleSolenoid::kForward);
    } else if (xbox.GetRawAxis(3) == 0) {
      s_HABback.Set(frc::DoubleSolenoid::kReverse);
    } else {
      s_HABback.Set(frc::DoubleSolenoid::kOff);
    }

    //Forklift (Works)
    //POV is 0=UP 90=Right 180=Down 270=Left and 45 for all the angles
    // the "== 1" might have to be "== 0/180" corresponcding to the angle
    if (arcadePad.GetPOV(0) == 0) {
      m_forklift.Set(.5);
    } else if (arcadePad.GetPOV(0) == 180) {
      m_forklift.Set(-.5);
    } else {
      m_forklift.Set(0);
    }
    
    //Ball Intake (Untested)
    //This is the button test
    // / 3 is to small need to change on next upload
    // the / 3 is since one of the motors is currently missing a gearbox but we will have the gear box before comp
    if (arcadePad.GetRawButton(1) == 1) {
      m_leftIntake.Set(-1 / 3);
      m_rightIntake.Set(-1);
    } else if (arcadePad.GetRawButton(3) == 1) {
      m_leftIntake.Set(1 / 3);
      m_rightIntake.Set(1);
    } else {
      m_leftIntake.Set(0);
      m_rightIntake.Set(0);
    }
    //(Untested)
    //This is the button/axis test, this is what we want if it works but can use code abouve incase this does not work
    //The button layout with the top code is better than this one so idk what we will use in the end
    //R2 and L2 axis(buttons on physical board)
    m_leftIntake.Set(arcadePad.GetRawAxis(3) * -1 / 3); //devide 3 only there for now since we don't have the 2nd gearbox
    m_rightIntake.Set(arcadePad.GetRawAxis(2));

    //Hatch Panel Launcher(Works)
    //4 = Y on Qanda Arcade
    if (arcadePad.GetRawButton(4) == 1) {
      s_panelLauncher.Set(frc::DoubleSolenoid::kForward);
      s_panelGetter.Set(frc::DoubleSolenoid::kForward);
    } else if (arcadePad.GetRawButton(4) == 0) {
      s_panelLauncher.Set(frc::DoubleSolenoid::kReverse);
      s_panelGetter.Set(frc::DoubleSolenoid::kReverse);
    } else {
      s_panelLauncher.Set(frc::DoubleSolenoid::kOff);
      s_panelGetter.Set(frc::DoubleSolenoid::kOff);
    }
    //Slider/Panel Getter(Untested)
    /*
    if (arcadePad.GetRawButton(6) == 1) {
      s_panelGetter.Set(frc::DoubleSolenoid::kReverse);
    } else if (arcadePad.GetRawButton(6) == 0) {
      s_panelGetter.Set(frc::DoubleSolenoid::kForward);
    } else {
      s_panelGetter.Set(frc::DoubleSolenoid::kOff);
    }
    */

   //Compressor toggle?
   if (xbox.GetRawButton(7) == 1) {
     if (compressor.Enabled == true) {
       compressor.Enabled == false;
     }
     else if (compressor.Enabled == false) {
       compressor.Enabled == true;
     }
   }
  }

 private:
  //Declartions
  frc::Joystick xbox{1};
  frc::Joystick arcadePad{2};
  frc::Compressor compressor;
  //Motors
  //Drivebase
  frc::PWMVictorSPX m_frontLeft{0};
  frc::PWMVictorSPX m_backLeft{1};
  frc::PWMVictorSPX m_frontRight{2};
  frc::PWMVictorSPX m_backRight{3};
  //Ball Intake
  frc::PWMVictorSPX m_leftIntake{4}; //No Gearbox Currently
  frc::PWMVictorSPX m_rightIntake{5};
  //Forlift
  frc::PWMVictorSPX m_forklift{6};
  //Groups
  //Drivebase
  frc::SpeedControllerGroup m_left{m_frontLeft, m_backLeft};
  frc::SpeedControllerGroup m_right{m_frontRight, m_backRight}; 
  //Setting Drive
  frc::DifferentialDrive Drive{m_left, m_right};

  //Pneumatics/Solenoids
  //Hatch Panel Launcher
  frc::DoubleSolenoid s_panelLauncher{0, 1};
  frc::DoubleSolenoid s_panelGetter{2, 3};
  frc::DoubleSolenoid s_HABfront{4, 5};
  frc::DoubleSolenoid s_HABback{6, 7};
};

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif