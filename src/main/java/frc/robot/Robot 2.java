/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.robot;

import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.geometry.Rotation2d;
import edu.wpi.first.wpilibj.kinematics.SwerveModuleState;
import edu.wpi.first.wpilibj2.command.CommandScheduler;
import frc.robot.commands.InitialCommand;
import frc.robot.subsystems.SwerveDrive;

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

	private double map(double val, double inMin, double inMax, double outMin, double outMax) {
		return outMin + ((outMax - outMin) / (inMax - inMin)) * (val - inMin);

	}

	public void teleopInit() {
		CommandScheduler.getInstance().schedule(new InitialCommand());
		SwerveDrive.getInstance();
	}

	@Override
	public void teleopPeriodic() {
		// if (Motors.SwerveDrive.backLeftModule.encoder.getRelPosition()
		// - 2 * Math.PI / ArduinoAbsoluteEncoder.maxTicks < desiredRotation
		// && Motors.SwerveDrive.backLeftModule.encoder.getRelPosition()
		// + 2 * Math.PI / ArduinoAbsoluteEncoder.maxTicks > desiredRotation)
		// final double desiredRotation = Math.PI;
		// Motors.SwerveDrive.frontLeftModule.setDesiredState(new SwerveModuleState(0.0, new Rotation2d(desiredRotation)));
		// Motors.SwerveDrive.frontRightModule
		// 		.setDesiredState(new SwerveModuleState(0.0, new Rotation2d(desiredRotation)));
		// Motors.SwerveDrive.backRightModule.setDesiredState(new SwerveModuleState(0.0, new Rotation2d(desiredRotation)));
		// Motors.SwerveDrive.backLeftModule.setDesiredState(new SwerveModuleState(0.0, new Rotation2d(desiredRotation)));
		try {
			SwerveDrive.getInstance().updateEncoders();
			System.out.println("Front left abs pos: " + Double.toString(
					map(Motors.SwerveDrive.frontLeftModule.encoder.getAbsPosition(), -Math.PI, Math.PI, 0, 127)));
			System.out.println("Front right abs pos: " + Double.toString(
					map(Motors.SwerveDrive.frontRightModule.encoder.getAbsPosition(), -Math.PI, Math.PI, 0, 127)));
			System.out.println("back left abs pos: " + Double.toString(
					map(Motors.SwerveDrive.backLeftModule.encoder.getAbsPosition(), -Math.PI, Math.PI, 0, 127)));
			System.out.println("back right abs pos: " + Double.toString(
					map(Motors.SwerveDrive.backRightModule.encoder.getAbsPosition(), -Math.PI, Math.PI, 0, 127)));
			System.out.println(
					"Front left home pos: " + Byte.toString(Motors.SwerveDrive.frontLeftModule.encoder.getHome()));
			System.out.println(
					"Front right home pos: " + Byte.toString(Motors.SwerveDrive.frontRightModule.encoder.getHome()));
			System.out.println(
					"Back right home pos: " + Byte.toString(Motors.SwerveDrive.backRightModule.encoder.getHome()));
			System.out.println(
					"Back left home pos: " + Byte.toString(Motors.SwerveDrive.backLeftModule.encoder.getHome()));
		} catch (Exception e) {
		}
		// } catch (Exception e) {
		// }
	}

	@Override
	public void testInit() {
		CommandScheduler.getInstance().cancelAll();
	}

	@Override
	public void testPeriodic() {
	}
}
