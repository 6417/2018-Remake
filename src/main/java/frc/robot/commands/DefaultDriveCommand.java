/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.robot.commands;

import edu.wpi.first.wpilibj.kinematics.ChassisSpeeds;
import edu.wpi.first.wpilibj2.command.CommandBase;
import edu.wpi.first.wpilibj2.command.ParallelCommandGroup;
import frc.robot.RobotContainer;
import frc.robot.subsystems.SwerveDrive;

public class DefaultDriveCommand extends ParallelCommandGroup {
	private static class UpdateEncoderCommand extends CommandBase {
		@Override
		public void execute() {
			SwerveDrive.getInstance().updateEncoders();
		}

		@Override
		public boolean isFinished() {
			return false;
		}
	}

	private static class UpdateDriveCommand extends CommandBase {

		public UpdateDriveCommand() {

		}

		@Override
		public void initialize() {
		}

		@Override
		public void execute() {
			SwerveDrive.getInstance()
					.setSpeeds(new ChassisSpeeds(RobotContainer.getInstance().getDriveJoystickY(),
							RobotContainer.getInstance().getDriveJoystickX(),
							RobotContainer.getInstance().getDriveJoystickZ() * 2 * Math.PI / 5));
		}

		@Override
		public void end(boolean interrupted) {
		}

		@Override
		public boolean isFinished() {
			return false;
		}
	}

	public DefaultDriveCommand() {
		addRequirements(SwerveDrive.getInstance());
		addCommands(new UpdateDriveCommand(), new UpdateEncoderCommand());
	}

	@Override
	public boolean isFinished() {
		return false;
	}
}
