/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.robot.commands;

import edu.wpi.first.wpilibj2.command.CommandBase;
import frc.robot.subsystems.LiftingUnit;

public class MoveLiftingUnitTo extends CommandBase {
	private int pos;

	public MoveLiftingUnitTo(int pos) {
		this.pos = pos;
		addRequirements(LiftingUnit.getInstance());
	}

	@Override
	public void initialize() {
		LiftingUnit.getInstance().moveto(pos);
	}
}
