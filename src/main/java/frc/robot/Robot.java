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

	ShuffleBoardInformation frontRightEncoderPosition = new ShuffleBoardInformation("Encoder Positoins", "Front Left", 0.0);
	ShuffleBoardInformation frontLeftEncoderPosition = new ShuffleBoardInformation("Encoder Positoins", "Front Right", 0.0);
	ShuffleBoardInformation backLeftEncoderPosition = new ShuffleBoardInformation("Encoder Positoins", "Back Right", 0.0);
	ShuffleBoardInformation backRightEncoderPosition= new ShuffleBoardInformation("Encoder Positoins", "Back Left", 0.0);

	public void teleopInit() {
		CommandScheduler.getInstance().schedule(new InitialCommand());
		SwerveDrive.getInstance();
		frontLeftEncoderPosition.update(0.0);
		frontRightEncoderPosition.update(0.0);
		backRightEncoderPosition.update(0.0);
		backLeftEncoderPosition.update(0.0);
	}
	private double map(double val, double inMin, double inMax, double outMin, double outMax) {
        return outMin + ((outMax - outMin) / (inMax - inMin)) * (val - inMin);
    }
	@Override
	public void teleopPeriodic() {
		final double desiredRotation = Math.PI;
		try{
		frontLeftEncoderPosition.update(map(Motors.SwerveDrive.frontLeftModule.getEncoderPos(), -Math.PI, Math.PI, ArduinoAbsoluteEncoder.minTicks, ArduinoAbsoluteEncoder.maxTicks) - Motors.SwerveDrive.frontLeftModule.encoder.getHome());
		frontRightEncoderPosition.update(map(Motors.SwerveDrive.frontRightModule.getEncoderPos(), -Math.PI, Math.PI, ArduinoAbsoluteEncoder.minTicks, ArduinoAbsoluteEncoder.maxTicks) - Motors.SwerveDrive.frontRightModule.encoder.getHome());
		backRightEncoderPosition.update(map(Motors.SwerveDrive.backRightModule.getEncoderPos(),  -Math.PI, Math.PI, ArduinoAbsoluteEncoder.minTicks, ArduinoAbsoluteEncoder.maxTicks) - Motors.SwerveDrive.backRightModule.encoder.getHome());
		backLeftEncoderPosition.update(map(Motors.SwerveDrive.backLeftModule.getEncoderPos(), -Math.PI, Math.PI, ArduinoAbsoluteEncoder.minTicks, ArduinoAbsoluteEncoder.maxTicks) - Motors.SwerveDrive.backLeftModule.encoder.getHome());
		// if (Motors.SwerveDrive.backLeftModule.encoder.getRelPosition()
		// - 2 * Math.PI / ArduinoAbsoluteEncoder.maxTicks < desiredRotation
		// && Motors.SwerveDrive.backLeftModule.encoder.getRelPosition()
		// + 2 * Math.PI / ArduinoAbsoluteEncoder.maxTicks > desiredRotation)
		// Motors.SwerveDrive.backLeftModule.setDesiredState(new SwerveModuleState(0.0, new Rotation2d(desiredRotation)));
		// Motors.SwerveDrive.backRightModule.setDesiredState(new SwerveModuleState(0.0, new Rotation2d(desiredRotation)));
		// Motors.SwerveDrive.frontRightModule.setDesiredState(new SwerveModuleState(0.0, new Rotation2d(desiredRotation)));
		// Motors.SwerveDrive.frontLeftModule.setDesiredState(new SwerveModuleState(0.0, new Rotation2d(desiredRotation)));
		System.out.println(Motors.SwerveDrive.backLeftModule.encoder.getRelPosition()
				/ (2 * Math.PI / ArduinoAbsoluteEncoder.maxTicks));
		} catch (Exception e) {}
	}

	@Override
	public void testInit() {
		CommandScheduler.getInstance().cancelAll();
	}

	@Override
	public void testPeriodic() {
	}
}
