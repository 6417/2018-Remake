/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.robot;

import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.I2C.Port;
import edu.wpi.first.wpilibj.geometry.Rotation2d;
import edu.wpi.first.wpilibj.kinematics.ChassisSpeeds;
import edu.wpi.first.wpilibj.kinematics.SwerveModuleState;
import edu.wpi.first.wpilibj2.command.CommandBase;
import edu.wpi.first.wpilibj2.command.CommandScheduler;
import frc.ArduinoAbsoluteEncoder.ArduinoAbsoluteEncoder;
import frc.robot.commands.DefaultDriveCommand;
import frc.robot.commands.InitialCommand;
import frc.robot.commands.MoveLiftingUnitTo;
import frc.robot.subsystems.SwerveDrive;
import frc.robot.subsystems.SwerveModule;

public class Robot extends TimedRobot {
	private RobotContainer m_robotContainer;

	@Override
	public void robotInit() {
		RobotContainer.getInstance();
		Motors.init();
	}

	@Override
	public void robotPeriodic() {
		CommandScheduler.getInstance().run();
	}

	@Override
	public void disabledInit() {
	}

	@Override
	public void disabledPeriodic() {
	}

	@Override
	public void autonomousInit() {
		CommandScheduler.getInstance().schedule(new InitialCommand());
	}

	@Override
	public void autonomousPeriodic() {
	}

	public void teleopInit() {
		CommandScheduler.getInstance().schedule(new InitialCommand());
		SwerveDrive.getInstance();
	}

	@Override
	public void teleopPeriodic() {

	}

	private static class TestScope {
		public static ShuffleBoardInformation frontRightEncoderPosition = new ShuffleBoardInformation(
				"Encoder Positoins", "Front Right", 0.0);
		public static ShuffleBoardInformation frontLeftEncoderPosition = new ShuffleBoardInformation(
				"Encoder Positoins", "Front Left", 0.0);
		public static ShuffleBoardInformation backLeftEncoderPosition = new ShuffleBoardInformation("Encoder Positoins",
				"Back Left", 0.0);
		public static ShuffleBoardInformation backRightEncoderPosition = new ShuffleBoardInformation(
				"Encoder Positoins", "Back Right", 0.0);
		public static ShuffleBoardInformation frontRightEncoderHome = new ShuffleBoardInformation("Encoder Positoins",
				"Front Right Home", 0.0);
		public static ShuffleBoardInformation frontLeftEncoderHome = new ShuffleBoardInformation("Encoder Positoins",
				"Front Left Home", 0.0);
		public static ShuffleBoardInformation backLeftEncoderHome = new ShuffleBoardInformation("Encoder Positoins",
				"Back Left Home", 0.0);
		public static ShuffleBoardInformation backRightEncoderHome = new ShuffleBoardInformation("Encoder Positoins",
				"Back Right Home", 0.0);
		public static ShuffleBoardInformation frontRightEncoderRelPos = new ShuffleBoardInformation("Encoder Positoins",
				"Front Right RelPos", 0.0);
		public static ShuffleBoardInformation frontLeftEncoderRelPos = new ShuffleBoardInformation("Encoder Positoins",
				"Front Left RelPos", 0.0);
		public static ShuffleBoardInformation backLeftEncoderRelPos = new ShuffleBoardInformation("Encoder Positoins",
				"Back Left RelPos", 0.0);
		public static ShuffleBoardInformation backRightEncoderRelPos = new ShuffleBoardInformation("Encoder Positoins",
				"Back Right RelPos", 0.0);

		public static double map(double val, double inMin, double inMax, double outMin, double outMax) {
			return outMin + ((outMax - outMin) / (inMax - inMin)) * (val - inMin);
		}

		public static class ShowShuffleboardInformationCommand extends CommandBase {
			@Override
			public void execute() {
				TestScope.frontLeftEncoderPosition.update(Math.round(
						TestScope.map(Motors.SwerveDrive.frontLeftModule.getAbsPos(), -Math.PI, Math.PI, 0, 127)));
				TestScope.frontRightEncoderPosition.update(Math.round(
						TestScope.map(Motors.SwerveDrive.frontRightModule.getAbsPos(), -Math.PI, Math.PI, 0, 127)));
				TestScope.backRightEncoderPosition.update(Math.round(
						TestScope.map(Motors.SwerveDrive.backRightModule.getAbsPos(), -Math.PI, Math.PI, 0, 127)));
				TestScope.backLeftEncoderPosition.update(Math.round(
						TestScope.map(Motors.SwerveDrive.backLeftModule.getAbsPos(), -Math.PI, Math.PI, 0, 127)));
				TestScope.frontLeftEncoderHome.update(Motors.SwerveDrive.frontLeftModule.getHome());
				TestScope.frontRightEncoderHome.update(Motors.SwerveDrive.frontRightModule.getHome());
				TestScope.backRightEncoderHome.update(Motors.SwerveDrive.backRightModule.getHome());
				TestScope.backLeftEncoderHome.update(Motors.SwerveDrive.backLeftModule.getHome());
				TestScope.frontLeftEncoderRelPos.update(Math.round(
						TestScope.map(Motors.SwerveDrive.frontLeftModule.getEncoderPos(), -Math.PI, Math.PI, 0, 127)));
				TestScope.frontRightEncoderRelPos.update(Math.round(
						TestScope.map(Motors.SwerveDrive.frontRightModule.getEncoderPos(), -Math.PI, Math.PI, 0, 127)));
				TestScope.backRightEncoderRelPos.update(Math.round(
						TestScope.map(Motors.SwerveDrive.backRightModule.getEncoderPos(), -Math.PI, Math.PI, 0, 127)));
				TestScope.backLeftEncoderRelPos.update(Math.round(
						TestScope.map(Motors.SwerveDrive.backLeftModule.getEncoderPos(), -Math.PI, Math.PI, 0, 127)));
			}

			@Override
			public boolean isFinished() {
				return false;
			}
		}
	}

	TestScope test = null;

	@Override
	public void testInit() {
		CommandScheduler.getInstance().cancelAll();
		CommandScheduler.getInstance().enable();
		CommandScheduler.getInstance().schedule(new DefaultDriveCommand.UpdateEncoderCommand());
		CommandScheduler.getInstance().schedule(new TestScope.ShowShuffleboardInformationCommand());
		test = new TestScope();
		Motors.SwerveDrive.frontLeftModule.setHome();
		Motors.SwerveDrive.frontRightModule.setHome();
		Motors.SwerveDrive.backRightModule.setHome();
		Motors.SwerveDrive.backLeftModule.setHome();
	}

	@Override
	public void testPeriodic() {
		// double desiredRotation = -Math.PI;
		// Motors.SwerveDrive.frontLeftModule.setDesiredState(new SwerveModuleState(0.0, new Rotation2d(desiredRotation)));
	}
}
