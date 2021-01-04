/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.robot;

import edu.wpi.first.wpilibj.I2C;
import edu.wpi.first.wpilibj.geometry.Translation2d;
import frc.ArduinoAbsoluteEncoder.ArduinoAbsoluteEncoder;
import frc.robot.subsystems.SwerveModule;

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
        public static final boolean isSubsystemEnabled = false;
        public static final int leftId = 13;
        public static final int rightId = 1;
        public static final double motorSpeed = 0.5;
        public static final int forwardButtonId = 1;
        public static final int backwardButtonId = 2;
    }

    public static class Cart {
        public static final boolean isSubsystemEnabled = false;
        public static final int motorId = 3;
        public static final int driveLenght = 858600;
        public static final int centerPos = 450000;
        public static final double zeroingSpeed = -0.5;

        public static final int moveToCenterButtonId = 4;
        public static final int moveToFrontButtonId = 5;
        public static final int moveToBackButtonId = 6;

        public static final int PID_Tolerance = 500;

        public static final PIDConfig.PIDFConfigurator PID = new PIDConfig.PIDFConfigurator();
        static {
            PID.openloopRamp = Optional.of(1.0);
            PID.closedloopRamp = Optional.of(0.0);

            PID.kF = 0.0465;
            PID.kP = 0.8;

            PID.kI = 0;
            PID.kD = 80;

            PID.statusFramePeriods.put(StatusFrameEnhanced.Status_10_MotionMagic, 10);
            PID.statusFramePeriods.put(StatusFrameEnhanced.Status_13_Base_PIDF0, 10);
            PID.integralZone = 500;
            PID.acceleration = 22000;
            PID.cruiseVelocity = 11000;
            PID.nominalOutputForward = 0;
            PID.nominalOutputReverse = 0;
            PID.peakOutputForward = 1;
            PID.peakOutputReverse = -1;
            PID.pidIdx = 0;
            PID.slotIdx = 0;
        }
    }

    public static class LiftingUnit {
        public static final boolean isSubsystemEnabled = false;

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
        public static final PIDConfig.PIDFConfigurator PID = new PIDConfig.PIDFConfigurator();
        static {
            PID.pidIdx = 0;
            PID.slotIdx = 0;
            PID.statusFramePeriods.put(StatusFrameEnhanced.Status_10_MotionMagic, 10);
            PID.statusFramePeriods.put(StatusFrameEnhanced.Status_13_Base_PIDF0, 10);
            PID.nominalOutputForward = 0;
            PID.nominalOutputReverse = 0;
            PID.peakOutputForward = 1;
            PID.peakOutputReverse = -1;

            PID.kF = 0.8525;
            PID.kP = 1.2;
            PID.kI = 0.0;
            PID.kD = 18.0;

            PID.integralZone = 200;
            PID.allowableClosedloopError = Optional.of(30);
            PID.cruiseVelocity = 5333;
            PID.acceleration = 4000;
        }
    }

    public static class SwerveDrive {
        public static final boolean isSubsystemEnabled = true;
        public static final long allowableTimeOfEncoderFaliure = 500; // in millisecods
        public static final int reinitializeMotorsButtonId = 1;

        public static class Front {
            public static class Right {
                public static final int motorRotationId = 4;
                public static final int motorSpeedId = 7;
                public static final I2C.Port encoderRotationPort = I2C.Port.kOnboard;
                public static final Translation2d moduleLocation = new Translation2d(0.0, 0.0);
                public static final int encoderRotationDeviceAdress = 1;
                public static final byte homePoint = 84;
            }
            
            public static class Left {
                public static final I2C.Port encoderRotationPort = I2C.Port.kOnboard;
                public static final int motorSpeedId = 6;
                public static final int motorRotationId = 5;
                public static final Translation2d moduleLocation = new Translation2d(0.0, 0.0);
                public static final int encoderRotationDeviceAdress = 4;
                public static final byte homePoint = 32;
            }
        }        
        
        public static class Back {
            public static class Right {
                public static final int motorSpeedId = 11;
                public static final int motorRotationId = 2;
                public static final I2C.Port encoderRotationPort = I2C.Port.kOnboard;
                public static final Translation2d moduleLocation = new Translation2d(0.0, 0.0);
                public static final int encoderRotationDeviceAdress = 2;
                public static final byte homePoint = 0;
            }

            public static class Left {
                public static final int motorSpeedId = 8;
                public static final int motorRotationId = 10;
                public static final I2C.Port encoderRotationPort = I2C.Port.kOnboard;
                public static final Translation2d moduleLocation = new Translation2d(0.0, 0.0);
                public static final int encoderRotationDeviceAdress = 3;
                public static final byte homePoint = 0;
            }
        }

        public static final SwerveModule.PIDConstants pidConst = new SwerveModule.PIDConstants();
            static {
               pidConst.kP = 4.15;
               pidConst.kI = 1.25;
               pidConst.kD = 0.05;
               
               pidConst.tolerance = 3 * 2 * Math.PI / ArduinoAbsoluteEncoder.maxTicks; 
               pidConst.acceleration = 0.0;
               pidConst.cruiseVelocity = 0.0;
            }
    }
}
