/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.robot;

import java.util.Optional;

import com.ctre.phoenix.motorcontrol.StatusFrameEnhanced;

public final class Constants {
    public static final int timeOut = 30;
    public static final int resetButtonId = 3;

    public static class Joystick {
        public static final int drivePort = 1;
        public static final int controllPort = 4;
    }

    public static class Gripper {
        public static final boolean isSubsystemEnabled = true;
        public static final int leftId = 13;
        public static final int rightId = 1;
        public static final double motorSpeed = 0.5;
        public static final int forwardButtonId = 1;
        public static final int backwardButtonId = 2;
    }

    public static class Cart {
        public static final boolean isSubsystemEnabled = true;
        public static final int motorId = 3;
        public static final int driveLenght = 858600;
        public static final int centerPos = 450000;
        public static final double zeroingSpeed = -0.5;

        public static final int moveToCenterButtonId = 4;
        public static final int moveToFrontButtonId = 5;
        public static final int moveToBackButtonId = 6;

        public static final int PID_Tolerance = 500;

        public static PIDConfig.PIDFConfigurator PID = new PIDConfig.PIDFConfigurator() {
            {
                openloopRamp = Optional.of(1.0);
                closedloopRamp = Optional.of(0.0);

                kF = 0.0465;
                kP = 0.8;
                kI = 0;
                kD = 80;
                
                statusFramePeriods.put(StatusFrameEnhanced.Status_10_MotionMagic, 10);
                statusFramePeriods.put(StatusFrameEnhanced.Status_13_Base_PIDF0, 10);
                integralZone = 500;
                acceleration = 22000;
                cruiseVelocity = 11000;
                nominalOutputForward = 0;
                nominalOutputReverse = 0;
                peakOutputForward = 1;
                peakOutputReverse = -1;
                pidIdx = 0;
                slotIdx = 0;
            }
        };
    }

    public static class LiftingUnit {
        public static final boolean isSubsystemEnabled = true;

        public static final int masterId = 12;
        public static final int followerId = 9;

        public static final double zeroingSpeed = 0.05;

        public static final int driveLenght = 200000;
        public static final int scalePosition = 150000;
        public static final int switchPosition = 60000;
        public static final int basePosition = 0;

        public static final int moveLiftingUnitToTopButtonId = 7;
        public static final int moveLiftingUnitToScalePositionButtonId = 8;
        public static final int moveLiftingUnitToSwitchButtonId = 9;
        public static final int moveLiftingUnitToBottomButtonId = 10;
        public static final int PID_Tolerance = 200;
        public static PIDConfig.PIDFConfigurator PID = new PIDConfig.PIDFConfigurator() {
            {
                pidIdx = 0;
                slotIdx = 0;
                statusFramePeriods.put(StatusFrameEnhanced.Status_10_MotionMagic, 10);
                statusFramePeriods.put(StatusFrameEnhanced.Status_13_Base_PIDF0, 10);
                nominalOutputForward = 0;
                nominalOutputReverse = 0;
                peakOutputForward = 1;
                peakOutputReverse = -1;

                kF = 0.8525;
                kP = 1.2;
                kI = 0.0;
                kD = 18.0;

                integralZone = 200;
                allowableClosedloopError = Optional.of(30);
                cruiseVelocity = 5333;
                acceleration = 4000;
            }
        };
    }
}
