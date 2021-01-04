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
import frc.robot.commands.MoveLiftingUnitToBottomRobotDisabled;
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
		CommandScheduler.getInstance().schedule(new MoveLiftingUnitToBottomRobotDisabled());
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
		System.out.println(SwerveDrive.getInstance().getDefaultCommand().getClass());

		CommandScheduler.getInstance().cancel(SwerveDrive.getInstance().getDefaultCommand());
	}

	@Override
	public void teleopPeriodic() {
		final double desiredRotation = Math.PI / 2;
		// if (Motors.SwerveDrive.backLeftModule.encoder.getRelPosition()
		// - 2 * Math.PI / ArduinoAbsoluteEncoder.maxTicks < desiredRotation
		// && Motors.SwerveDrive.backLeftModule.encoder.getRelPosition()
		// + 2 * Math.PI / ArduinoAbsoluteEncoder.maxTicks > desiredRotation)
		Motors.SwerveDrive.backLeftModule.setDesiredState(new SwerveModuleState(0.0, new Rotation2d(desiredRotation)));

		System.out.println(Motors.SwerveDrive.backLeftModule.encoder.getAbsPosition()
				/ (2 * Math.PI / ArduinoAbsoluteEncoder.maxTicks));
	}

	@Override
	public void testInit() {
		CommandScheduler.getInstance().cancelAll();
	}

	@Override
	public void testPeriodic() {
	}
}
