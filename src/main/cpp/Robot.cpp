
 /*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/
#include <frc/Joystick.h>
#include <frc/PWMVictorSPX.h>
#include <frc/TimedRobot.h>
#include <frc/Timer.h>
#include <frc/drive/DifferentialDrive.h>
#include <frc/livewindow/LiveWindow.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/DoubleSolenoid.h>
#include <frc/Solenoid.h>
#include <frc/Encoder.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/smartdashboard/SendableChooser.h>

constexpr double kPi = 3.14159265358979;

class Robot : public frc::TimedRobot {
  private:
  frc::SendableChooser<std::string> m_chooser;
  const std::string kAutoNameDefault = "Default";
  const std::string kAutoNameCustom = "My Auto";
  std::string m_autoSelected;

   //Declartions
  frc::Joystick gamePad{0};
  frc::Timer timer;
  frc::Encoder encoder1{0, 1};
  frc::LiveWindow& m_lw = *frc::LiveWindow::GetInstance();

  //Motors
  //Drivebase
  frc::PWMVictorSPX m_frontLeft{0};
  frc::PWMVictorSPX m_backLeft{1};
  frc::PWMVictorSPX m_frontRight{2};
  frc::PWMVictorSPX m_backRight{3};
  //HAB Lift
  frc::PWMVictorSPX m_frontLift{4};
  frc::PWMVictorSPX m_backLift{5};
  //Elevator Lift
  frc::PWMVictorSPX m_firstLayer{6}; // This may be 1 or 2 motors, not yet decided, set for 2 motors
  frc::PWMVictorSPX m_secondLayer{7}; 
  //BAG Motor Ball Thinging
  frc::PWMVictorSPX m_leftWheel{8};
  frc::PWMVictorSPX m_rightWheel{9};
  //Groups
  //Drivebase
  frc::SpeedControllerGroup m_left{m_frontLeft, m_backLeft};
  frc::SpeedControllerGroup m_right{m_frontRight, m_backRight}; 
  frc::SpeedControllerGroup m_ball{m_leftWheel, m_rightWheel};
  //Setting Drive
  frc::DifferentialDrive Drive{m_left, m_right};

  //Pneumatics/Solenoids
  //Hatch Panel Grabber(trying both on one solenoid)
  frc::DoubleSolenoid s_panelGrabber{1, 2};
  //Hatch Panel Launcher(trying 3 on one solenoid)
  frc::DoubleSolenoid s_panelLauncher{3, 4};
  //Hatch Panel Grabber F/B Setters
  static constexpr int panelGrabberForward = 1;
  static constexpr int panelGrabberReverse = 2;
  //Hatch Panel Launcher F/B Setters
  static constexpr int panelLauncherForward = 3;
  static constexpr int panelLauncherReverse = 4;

 public:
  Robot() {
    Drive.SetExpiration(0.1);
    timer.Start();
  }

  void RobotInit() override {
    encoder1.SetDistancePerPulse((kPi * 6) / 360.0);
  }

  void RobotPeriodic() override {
    frc::SmartDashboard::PutNumber("Encoder", encoder1.GetDistance());
  }

  void AutonomousInit() override {
    timer.Reset();
    timer.Start();
  }

  void AutonomousPeriodic() override {
    // Drive for 2 seconds
    if (timer.Get() < 2.0) {
      // Drive forwards half speed
      Drive.ArcadeDrive(0.5, 0.0);
    } else {
      // Stop robot
      Drive.ArcadeDrive(0.0, 0.0);
    }
  }

  void TeleopInit() override {
    encoder1.Reset();
  }

  void TeleopPeriodic() override {
    //Drive
    Drive.ArcadeDrive(gamePad.GetRawAxis(4), gamePad.GetRawAxis(5));
    //Elevator Lift(Set to axis but need to program in the 2 hights for the holes and 1 for bottom/rest)
    //No idea is this works... need to reconfigure for the right bases
    if (gamePad.GetRawButton(5) == 1) {
      encoder1.Reset();
      while (encoder1.GetDistance() > 5) {
        m_firstLayer.Set(.5);
      }     
    } else if (gamePad.GetRawButton(6) == 1) {
      encoder1.Reset();
      while (encoder1.GetDistance() > 10) {
        m_secondLayer.Set(.5);
      }
    } else {
      while (encoder1.GetDistance() > .01) {
      m_secondLayer.Set(-.5);
      }
    }
    
    //HAB lift  
    m_frontLift.Set(gamePad.GetRawAxis(2));
    m_backLift.Set(gamePad.GetRawAxis(3));
    //Ball Holder Thinging
    m_ball.Set(gamePad.GetRawAxis(0));
    //Solenoids for Panel Grabber(I think is done but I have not tested it yet)
    if (gamePad.GetRawButton(panelGrabberForward) == 1) {
      s_panelGrabber.Set(frc::DoubleSolenoid::kForward);
    } else if (gamePad.GetRawButton(panelGrabberReverse) == 0) {
      s_panelGrabber.Set(frc::DoubleSolenoid::kReverse);
    } //I do not know if I will need the else or if I shoudl change the else if to an else statement
    else {
      s_panelGrabber.Set(frc::DoubleSolenoid::kOff);
    }
    //Solenoids for Panel Launcher  
    if (gamePad.GetRawButton(panelLauncherForward) == 1) {
      s_panelLauncher.Set(frc::DoubleSolenoid::kForward);
    } else if (gamePad.GetRawButton(panelLauncherReverse) == 0) {
      s_panelGrabber.Set(frc::DoubleSolenoid::kReverse);
    } else {
      s_panelLauncher.Set(frc::DoubleSolenoid::kOff);
    }
  }

void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif

};
