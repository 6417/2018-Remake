/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.robot.commands;

import edu.wpi.first.wpilibj2.command.CommandBase;
import frc.robot.Constants;
import frc.robot.subsystems.Cart;

public class MoveCartToBack extends CommandBase {
	public MoveCartToBack() {
		addRequirements(Cart.getInstance());
	}

	@Override
	public void initialize() {
		Cart.getInstance().moveto(0);
	}

	@Override
	public void end(boolean interrupted) {
	}

	@Override
	public boolean isFinished() {
		return Cart.getInstance().getEncoderTicks() + Constants.Cart.PID_Tolerance >= 0 && Cart.getInstance().getEncoderTicks() - Constants.Cart.PID_Tolerance <= 0;
	}
}
