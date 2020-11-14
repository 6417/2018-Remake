/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.robot.commands;

import edu.wpi.first.wpilibj.kinematics.ChassisSpeeds;
import edu.wpi.first.wpilibj2.command.CommandBase;
import frc.robot.RobotContainer;
import frc.robot.subsystems.SwerveDrive;

public class DefaultDriveCommand extends CommandBase {
	public DefaultDriveCommand() {
		addRequirements(SwerveDrive.getInstance());
	}

	@Override
	public void initialize() {
	}

	@Override
	public void execute() {
		SwerveDrive.getInstance().setSpeeds(new ChassisSpeeds(RobotContainer.getInstance().getDriveJoystickY(),
				RobotContainer.getInstance().getDriveJoystickX(), RobotContainer.getInstance().getDriveJoystickZ()));
	}

	@Override
	public void end(boolean interrupted) {
	}

	@Override
	public boolean isFinished() {
		return false;
	}
}
