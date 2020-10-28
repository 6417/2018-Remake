/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.robot.subsystems;

import edu.wpi.first.wpilibj2.command.SubsystemBase;
import frc.robot.Constants;
import frc.robot.Motors;
import frc.robot.subsystems.emptySubsystems.EmptyGripper;

public class Gripper extends SubsystemBase {
	static Gripper instance = null;

	protected Gripper() {

	}

	public static Gripper getInstance() {
		if (Constants.Gripper.isSubsystemEnabled) {
			if (instance == null)
				instance = new Gripper();
		} else if (instance == null)
			instance = new EmptyGripper();
		return instance;
	}

	public void set(double speed) {
		Motors.Gripper.right.set(speed);
	}

	public void stop() {
		Motors.Gripper.right.stopMotor();
	}
}
