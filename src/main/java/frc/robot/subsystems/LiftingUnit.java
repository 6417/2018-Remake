/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.robot.subsystems;

import com.ctre.phoenix.motorcontrol.ControlMode;

import edu.wpi.first.wpilibj2.command.SubsystemBase;
import frc.robot.Constants;
import frc.robot.Motors;
import frc.robot.subsystems.emptySubsystems.EmptyLiftingUnit;

public class LiftingUnit extends SubsystemBase {
	private static LiftingUnit instance = null;

	protected LiftingUnit() {

	}

	public static LiftingUnit getInstance() {
		if (Constants.LiftingUnit.isSubsystemEnabled) {
			if (instance == null)
				instance = new LiftingUnit();
		} else if (instance == null) 
			instance = new EmptyLiftingUnit();
		return instance;
	}

	@Override
	public void periodic() {
		if (getLimitSwitchBottom())
			Motors.LiftingUnit.master.setSelectedSensorPosition(0);
	}

	public boolean getLimitSwitchBottom() {
		return Motors.LiftingUnit.master.isRevLimitSwitchClosed() == 0;
	}

	public void moveto(int pos) {
		Motors.LiftingUnit.master.set(ControlMode.MotionMagic, pos);
	}

	public void setVelocity(double speed) {
		Motors.LiftingUnit.master.set(speed);
	}

	public int getEncoderTicks() {
		return Motors.LiftingUnit.master.getSelectedSensorPosition();
	}	
	
	public int getEncoderVelocity() {
		return Motors.LiftingUnit.master.getSelectedSensorVelocity();
	}

	public void stop() {
		Motors.LiftingUnit.master.stopMotor();
	}
}
