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
import edu.wpi.first.wpilibj2.command.CommandScheduler;
import frc.ArduinoAbsoluteEncoder.ArduinoAbsoluteEncoder;
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

	ShuffleBoardInformation frontRightEncoderPosition = new ShuffleBoardInformation("Encoder Positoins", "Front Right",
			0.0);
	ShuffleBoardInformation frontLeftEncoderPosition = new ShuffleBoardInformation("Encoder Positoins", "Front Left",
			0.0);
	ShuffleBoardInformation backLeftEncoderPosition = new ShuffleBoardInformation("Encoder Positoins", "Back Left",
			0.0);
	ShuffleBoardInformation backRightEncoderPosition = new ShuffleBoardInformation("Encoder Positoins", "Back Right",
			0.0);
	ShuffleBoardInformation frontRightEncoderHome = new ShuffleBoardInformation("Encoder Positoins", "Front Right Home",
			0.0);
	ShuffleBoardInformation frontLeftEncoderHome = new ShuffleBoardInformation("Encoder Positoins", "Front Left Home",
			0.0);
	ShuffleBoardInformation backLeftEncoderHome = new ShuffleBoardInformation("Encoder Positoins", "Back Left Home",
			0.0);
	ShuffleBoardInformation backRightEncoderHome = new ShuffleBoardInformation("Encoder Positoins", "Back Right Home",
			0.0);

	public void teleopInit() {
		CommandScheduler.getInstance().schedule(new InitialCommand());
		SwerveDrive.getInstance();
		frontLeftEncoderPosition.update(0.0);
		frontRightEncoderPosition.update(0.0);
		backRightEncoderPosition.update(0.0);
		backLeftEncoderPosition.update(0.0);
		Motors.SwerveDrive.frontLeftModule.setHome();
		Motors.SwerveDrive.frontRightModule.setHome();
		Motors.SwerveDrive.backRightModule.setHome();
		Motors.SwerveDrive.backLeftModule.setHome();
	}

	private double map(double val, double inMin, double inMax, double outMin, double outMax) {
		return outMin + ((outMax - outMin) / (inMax - inMin)) * (val - inMin);
	}

	@Override
	public void teleopPeriodic() {
		final double desiredRotation = Math.PI;
		frontLeftEncoderPosition
				.update(Math.round(map(Motors.SwerveDrive.frontLeftModule.getAbsPos(), -Math.PI, Math.PI, 0, 127)));
		frontRightEncoderPosition
				.update(Math.round(map(Motors.SwerveDrive.frontRightModule.getAbsPos(), -Math.PI, Math.PI, 0, 127)));
		backRightEncoderPosition
				.update(Math.round(map(Motors.SwerveDrive.backRightModule.getAbsPos(), -Math.PI, Math.PI, 0, 127)));
		backLeftEncoderPosition
				.update(Math.round(map(Motors.SwerveDrive.backLeftModule.getAbsPos(), -Math.PI, Math.PI, 0, 127)));
		frontLeftEncoderHome.update(Motors.SwerveDrive.frontLeftModule.getHome());
		frontRightEncoderHome.update(Motors.SwerveDrive.frontRightModule.getHome());
		backRightEncoderHome.update(Motors.SwerveDrive.backRightModule.getHome());
		backLeftEncoderHome.update(Motors.SwerveDrive.backLeftModule.getHome());
		// if (Motors.SwerveDrive.backLeftModule.encoder.getRelPosition()
		// - 2 * Math.PI / ArduinoAbsoluteEncoder.maxTicks < desiredRotation
		// && Motors.SwerveDrive.backLeftModule.encoder.getRelPosition()
		// + 2 * Math.PI / ArduinoAbsoluteEncoder.maxTicks > desiredRotation)
		// Motors.SwerveDrive.backLeftModule
		// .setDesiredState(new SwerveModuleState(0.0, new
		// Rotation2d(desiredRotation)));
		// Motors.SwerveDrive.backRightModule
		// .setDesiredState(new SwerveModuleState(0.0, new
		// Rotation2d(desiredRotation)));
		// Motors.SwerveDrive.frontRightModule
		// .setDesiredState(new SwerveModuleState(0.0, new
		// Rotation2d(desiredRotation)));
		// Motors.SwerveDrive.frontLeftModule
		// .setDesiredState(new SwerveModuleState(0.0, new
		// Rotation2d(desiredRotation)));
	}

	@Override
	public void testInit() {
		CommandScheduler.getInstance().cancelAll();
	}

	@Override
	public void testPeriodic() {
	}
}
