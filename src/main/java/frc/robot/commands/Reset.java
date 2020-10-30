/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.robot.commands;

import edu.wpi.first.wpilibj2.command.CommandBase;
import edu.wpi.first.wpilibj2.command.SequentialCommandGroup;
import frc.robot.subsystems.Cart;
import frc.robot.subsystems.Gripper;

public class Reset extends SequentialCommandGroup {
	static private class StopAllMotors extends CommandBase {
		StopAllMotors() {
			addRequirements(Gripper.getInstance(), Cart.getInstance());
		}

		@Override
		public void initialize() {
			Gripper.getInstance().stop();
			Cart.getInstance().stop();
		}

		@Override
		public boolean isFinished() {
			return true;
		}
	}
	
	public Reset() {
		addCommands(new StopAllMotors(),new ZeroCart(), new MoveCartToCenter());
	}
}
