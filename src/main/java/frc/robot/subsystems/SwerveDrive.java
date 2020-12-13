/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.robot.subsystems;

import edu.wpi.first.wpilibj.kinematics.ChassisSpeeds;
import edu.wpi.first.wpilibj.kinematics.SwerveModuleState;
import edu.wpi.first.wpilibj2.command.SubsystemBase;
import frc.robot.Constants;
import frc.robot.Motors;
import frc.robot.commands.DefaultDriveCommand;
import frc.robot.subsystems.emptySubsystems.EmptySwerveDrive;

public class SwerveDrive extends SubsystemBase {
	public enum Mode {
		FIELD_ORIENTED, ROBOT_ORIENTED
	}

	private static SwerveDrive instance = null;

	protected SwerveDrive() {

	}

	public static SwerveDrive getInstance() {
		if (Constants.SwerveDrive.isSubsystemEnabled) {
			if (instance == null) {
				instance = new SwerveDrive();
				instance.setDefaultCommand(new DefaultDriveCommand());
			}
		} else if (instance == null)
			instance = new EmptySwerveDrive();
		return instance;
	}

	public void setSpeeds(ChassisSpeeds speeds) {
		SwerveModuleState[] moduleStates = Motors.SwerveDrive.kinematics.toSwerveModuleStates(speeds);
		// Motors.SwerveDrive.frontLeftModule.setDesiredState(moduleStates[0]);
		// Motors.SwerveDrive.frontRightModule.setDesiredState(moduleStates[1]);
		Motors.SwerveDrive.backLeftModule.setDesiredState(moduleStates[2]);
		// Motors.SwerveDrive.backRightModule.setDesiredState(moduleStates[3]);
	}
	public void updateEncoders() {
		Motors.SwerveDrive.frontRightModule.updateEncoder();
		Motors.SwerveDrive.frontLeftModule.updateEncoder();
		Motors.SwerveDrive.backRightModule.updateEncoder();
		Motors.SwerveDrive.backLeftModule.updateEncoder();
	}
}
