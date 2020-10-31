/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.robot.commands;

import edu.wpi.first.wpilibj2.command.CommandBase;
import frc.robot.Constants;
import frc.robot.subsystems.Gripper;

public class GripperBackwards extends CommandBase {

	public GripperBackwards() {
		addRequirements(Gripper.getInstance());
	}

	@Override
	public void initialize() {
		Gripper.getInstance().set(-Constants.Gripper.motorSpeed);
	}

	@Override
	public void execute() {
	}

	@Override
	public void end(boolean interrupted) {
		Gripper.getInstance().stop();
	}

	@Override
	public boolean isFinished() {
		return false;
	}
}
