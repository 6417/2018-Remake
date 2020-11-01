/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.robot;

import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj2.command.button.JoystickButton;
import frc.robot.commands.GripperBackwards;
import frc.robot.commands.GripperForward;
import frc.robot.commands.MoveCartToBack;
import frc.robot.commands.MoveCartToCenter;
import frc.robot.commands.MoveCartToFront;
import frc.robot.commands.ResetCommand;

public class RobotContainer {
	public Joystick joystickDrive = new Joystick(Constants.Joystick.drivePort);
	public Joystick joystickControll = new Joystick(Constants.Joystick.controllPort);

	private JoystickButton gripperForwardButton = new JoystickButton(joystickControll, Constants.Gripper.forwardButtonId);
	private JoystickButton gripperBackwardButton = new JoystickButton(joystickControll, Constants.Gripper.backwardButtonId);
	private JoystickButton resetButton = new JoystickButton(joystickControll, Constants.resetButtonId);
	private JoystickButton moveCartToCenterButton = new JoystickButton(joystickControll, Constants.Cart.moveToCenterButtonId);
	private JoystickButton moveCartToFrontButton = new JoystickButton(joystickControll, Constants.Cart.moveToFrontButtonId);
	private JoystickButton moveCartToBackButton = new JoystickButton(joystickControll, Constants.Cart.moveToBackButtonId);

	public RobotContainer() {
		configureButtonBindings();
	}

	private void configureButtonBindings() {
		gripperBackwardButton.whileHeld(new GripperBackwards());
		gripperForwardButton.whileHeld(new GripperForward());
		resetButton.whenPressed(new ResetCommand());
		moveCartToCenterButton.whenPressed(new MoveCartToCenter());
		moveCartToFrontButton.whenPressed(new MoveCartToFront());
		moveCartToBackButton.whenPressed(new MoveCartToBack());
	}
}
