/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.robot;

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

        public static class PID {
            public static final int tolerance = 500;
            public static final double openloopRamp = 1;
            public static final double closedloopRamp = 0;
            public static final double kf = 0.0465;
            public static final double kP = 0.8;
            public static final double kI = 0;
            public static final double kD = 80;
            public static final int framePeriod = 10;
            public static final int integralZone = 500;
            public static final int motionAcceleration = 22000;
            public static final int motionCruiseVelocity = 11000;
            public static final int nominalOutForward = 0;
            public static final int nominalOutReverse = 0;
            public static final int peakOutForward = 1;
            public static final int peakOutReverse = -1;
            public static final int pidIdx = 0;
            public static final int slotIdx = 0;
        }
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

        public static class PID {
            public static final int tolerance = 200;
            public static final int pidIdx = 0;
            public static final int slotIdx = 0;
            public static final int framePeriod = 10;
            public static final double nominalOutForward = 0;            
            public static final double nominalOutReverse = 0;            
            public static final double peakOutForward = 1;
            public static final double peakOutReverse = -1;
            
            public static final double kF = 0.8525;
            public static final double kP = 1.2;
            public static final double kI = 0.0;
            public static final double kD = 18.0;
            
            public static final int integralZone = 200;
            public static final int allowableClosedloopError = 30;
            public static final int cruiseVelocity = 5333;
            public static final int maxVelocityEncoderUnitsPer100ms = 4000;
        }
    }
}
