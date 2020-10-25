/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.robot;

import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;

public class Motors {
    public static class Gripper {

        public static WPI_TalonSRX right = new WPI_TalonSRX(Constants.Gripper.rightId);
        public static WPI_TalonSRX left = new WPI_TalonSRX(Constants.Gripper.leftId);

        public static void init() {
            if (Constants.Gripper.isSubsystemEnabled) {
                right.configFactoryDefault();
                right.setInverted(false);
                left.configFactoryDefault();
                left.setInverted(true);
                left.follow(right);
            }
        }
    }

    public static void init() {
        Gripper.init();
    }
}
