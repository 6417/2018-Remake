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
import frc.robot.subsystems.emptySubsystems.EmptyCart;

public class Cart extends SubsystemBase {
	private static Cart instance = null;

	protected Cart() {

	}

	public static Cart getInstance() {
		if (Constants.Cart.isSubsystemEnabled) {
			if (instance == null)
				instance = new Cart();
		} else if (instance == null)
			instance = new EmptyCart();
		return instance;
	}

	@Override
	public void periodic() {
		if (this.getLimitSwitchFront())
			Motors.Cart.motor.setSelectedSensorPosition(Constants.Cart.driveLenght);

		if (this.getLimitSwitchBack())
			Motors.Cart.motor.setSelectedSensorPosition(0);
	}

	public void moveto(int pos) {
		Motors.Cart.motor.set(ControlMode.MotionMagic, pos);
	}

	public void setVelocity(double v) {
		Motors.Cart.motor.set(v);
	}

	public void stop() {
		Motors.Cart.motor.stopMotor();
	}

	public boolean getLimitSwitchFront() {
		return Motors.Cart.motor.isFwdLimitSwitchClosed() == 1;
	}

	public boolean getLimitSwitchBack() {
		return Motors.Cart.motor.isRevLimitSwitchClosed() == 0;
	}

	public int getEncoderTicks() {
		return Motors.Cart.motor.getSelectedSensorPosition();
	}

	public int getEncoderVelocity() {
		return Motors.Cart.motor.getSelectedSensorVelocity();
	}
}
