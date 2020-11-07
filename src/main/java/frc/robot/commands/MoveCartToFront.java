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

public class MoveCartToFront extends CommandBase {
	public MoveCartToFront() {
		addRequirements(Cart.getInstance());
	}

	@Override
	public void initialize() {
		Cart.getInstance().moveto(Constants.Cart.driveLenght);
	}

	@Override
	public void end(boolean interrupted) {
	}

	@Override
	public boolean isFinished() {
		return Cart.getInstance().getEncoderTicks() + Constants.Cart.PID_Tolerance >= Constants.Cart.driveLenght
				&& Cart.getInstance().getEncoderTicks() - Constants.Cart.PID_Tolerance <= Constants.Cart.driveLenght;
	}
}
