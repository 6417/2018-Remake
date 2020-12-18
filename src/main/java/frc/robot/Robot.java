/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.robot;

import edu.wpi.first.wpilibj.TimedRobot;
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
		// Motors.SwerveDrive.backLeftModule.setDesiredState(new SwerveModuleState(0.0, new Rotation2d(desiredRotation)));
	}

	@Override
	public void testInit() {
		CommandScheduler.getInstance().cancelAll();
	}

	@Override
	public void testPeriodic() {
	}
}
