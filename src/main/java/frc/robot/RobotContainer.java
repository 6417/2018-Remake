/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.robot;

import edu.wpi.first.wpilibj.GenericHID;
import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.XboxController;
import edu.wpi.first.wpilibj2.command.Command;
import edu.wpi.first.wpilibj2.command.button.JoystickButton;
import frc.robot.commands.GripperBackwards;
import frc.robot.commands.GripperForward;

public class RobotContainer {
	private Joystick joystickDrive = new Joystick(Constants.Joystick.drivePort);
	private Joystick joystickControll = new Joystick(Constants.Joystick.controllPort);

	private JoystickButton gripperForwardButton = new JoystickButton(joystickControll, Constants.Gripper.forwardButtonId);
	private JoystickButton gripperBackwardButton = new JoystickButton(joystickControll, Constants.Gripper.backwardButtonId);

	public RobotContainer() {
		configureButtonBindings();
	}

	private void configureButtonBindings() {
		gripperBackwardButton.whileHeld(new GripperBackwards());
		gripperForwardButton.whileHeld(new GripperForward());
	}
}
