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
import frc.robot.commands.MoveLiftingUnitTo;
import frc.robot.commands.ResetCommand;

public class RobotContainer {
	private static RobotContainer instance = null;
	public Joystick joystickDrive = new Joystick(Constants.Joystick.drivePort);
	public Joystick joystickControll = new Joystick(Constants.Joystick.controllPort);

	private JoystickButton resetButton = new JoystickButton(joystickControll, Constants.resetButtonId);

	private JoystickButton gripperForwardButton = new JoystickButton(joystickControll,
			Constants.Gripper.forwardButtonId);
	private JoystickButton gripperBackwardButton = new JoystickButton(joystickControll,
			Constants.Gripper.backwardButtonId);

	private JoystickButton moveCartToCenterButton = new JoystickButton(joystickControll,
			Constants.Cart.moveToCenterButtonId);
	private JoystickButton moveCartToFrontButton = new JoystickButton(joystickControll,
			Constants.Cart.moveToFrontButtonId);
	private JoystickButton moveCartToBackButton = new JoystickButton(joystickControll,
			Constants.Cart.moveToBackButtonId);

	private JoystickButton moveLiftingUnitToTopButton = new JoystickButton(joystickControll,
			Constants.LiftingUnit.moveLiftingUnitToTopButtonId);
	private JoystickButton moveLiftingUnitToScalePositionButton = new JoystickButton(joystickControll,
			Constants.LiftingUnit.moveLiftingUnitToScalePositionButtonId);
	private JoystickButton moveLiftingUnitToSwitchButton = new JoystickButton(joystickControll,
			Constants.LiftingUnit.moveLiftingUnitToSwitchButtonId);
	private JoystickButton moveLiftingUnitToBottomButton = new JoystickButton(joystickControll,
			Constants.LiftingUnit.moveLiftingUnitToBottomButtonId);

	private JoystickButton reinitializeDriveMotorsButton = new JoystickButton(joystickDrive, Constants.SwerveDrive.reinitializeMotorsButtonId);

	private RobotContainer() {
		configureButtonBindings();
	}

	public static RobotContainer getInstance() {
		if (instance == null)
			instance = new RobotContainer();
		return instance;
	}

	private void configureButtonBindings() {
		resetButton.whenPressed(new ResetCommand());

		configureGripperButtonBindings();
		configureCartButtonBindings();
		configureLiftingUnitButtonBindings();
		configureDriveButtons();
	}

	private void configureDriveButtons() {
		reinitializeDriveMotorsButton.whenPressed(() -> { Motors.SwerveDrive.init(); });
	}

	private void configureLiftingUnitButtonBindings() {
		moveLiftingUnitToTopButton.whenPressed(new MoveLiftingUnitTo(Constants.LiftingUnit.driveLenght));
		moveLiftingUnitToScalePositionButton.whenPressed(new MoveLiftingUnitTo(Constants.LiftingUnit.scalePosition));
		moveLiftingUnitToSwitchButton.whenPressed(new MoveLiftingUnitTo(Constants.LiftingUnit.switchPosition));
		moveLiftingUnitToBottomButton.whenPressed(new MoveLiftingUnitTo(Constants.LiftingUnit.basePosition));
	}

	private void configureCartButtonBindings() {
		moveCartToCenterButton.whenPressed(new MoveCartToCenter());
		moveCartToFrontButton.whenPressed(new MoveCartToFront());
		moveCartToBackButton.whenPressed(new MoveCartToBack());
	}

	private void configureGripperButtonBindings() {
		gripperBackwardButton.whileHeld(new GripperBackwards());
		gripperForwardButton.whileHeld(new GripperForward());
	}

	public double getDriveJoystickX() {
		return joystickDrive.getX();
	}

	public double getDriveJoystickY() {
		return joystickDrive.getY();
	}

	public double getDriveJoystickZ() {
		return joystickDrive.getZ();
	}
}
