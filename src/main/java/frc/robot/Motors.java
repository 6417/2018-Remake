/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.robot;

import com.ctre.phoenix.motorcontrol.FeedbackDevice;
import com.ctre.phoenix.motorcontrol.InvertType;
import com.ctre.phoenix.motorcontrol.LimitSwitchNormal;
import com.ctre.phoenix.motorcontrol.LimitSwitchSource;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;

import edu.wpi.first.wpilibj.kinematics.SwerveDriveKinematics;
import frc.robot.subsystems.SwerveModule;

public class Motors {
    public static class Gripper {

        public static WPI_TalonSRX right;
        public static WPI_TalonSRX left;

        public static void init() {
            if (Constants.Gripper.isSubsystemEnabled) {
                initMotors();
                configRightMotor();
                configLeftMotor();
            }
        }

        private static void configLeftMotor() {
            left.configFactoryDefault();
            left.setInverted(InvertType.OpposeMaster);
            left.follow(right);
        }

        private static void configRightMotor() {
            right.configFactoryDefault();
            right.setInverted(false);
        }

        private static void initMotors() {
            right = new WPI_TalonSRX(Constants.Gripper.rightId);
            left = new WPI_TalonSRX(Constants.Gripper.leftId);
        }
    }

    public static class Cart {
        public static WPI_TalonSRX motor;

        public static void init() {
            if (Constants.Cart.isSubsystemEnabled) {
                initMotor();
                initLimitSwitches();
                initEncoder();
                Constants.Cart.PID.configMotor(motor);
            }
        }

        private static void initMotor() {
            motor = new WPI_TalonSRX(Constants.Cart.motorId);
            motor.setInverted(true);
        }

        private static void initEncoder() {
            motor.configSelectedFeedbackSensor(FeedbackDevice.QuadEncoder, Constants.Cart.PID.pidIdx,
                    Constants.timeOut);
        }

        private static void initLimitSwitches() {
            motor.configReverseLimitSwitchSource(LimitSwitchSource.FeedbackConnector, LimitSwitchNormal.NormallyClosed);
            motor.configForwardLimitSwitchSource(LimitSwitchSource.FeedbackConnector, LimitSwitchNormal.NormallyOpen);
        }
    }

    public static class LiftingUnit {
        public static WPI_TalonSRX master;
        public static WPI_TalonSRX follower;

        public static void init() {
            if (Constants.LiftingUnit.isSubsystemEnabled) {
                initMotors();
                initEncoder();
                initLimitSwitches();
                initPID();
            }
        }

        private static void initPID() {
            Constants.Cart.PID.configMotor(master);
            master.setSensorPhase(true);
        }

        private static void initEncoder() {
            master.configSelectedFeedbackSensor(FeedbackDevice.QuadEncoder, Constants.LiftingUnit.PID.pidIdx,
                    Constants.timeOut);
        }

        private static void initLimitSwitches() {
            master.configReverseLimitSwitchSource(LimitSwitchSource.FeedbackConnector,
                    LimitSwitchNormal.NormallyClosed);
        }

        private static void initMotors() {
            constructMotors();
            configMaster();
            configFollower();
        }

        private static void configFollower() {
            follower.configFactoryDefault();
            follower.follow(master);
            follower.setInverted(InvertType.OpposeMaster);
        }

        private static void configMaster() {
            master.configFactoryDefault();
            master.setInverted(false);
        }

        private static void constructMotors() {
            master = new WPI_TalonSRX(Constants.LiftingUnit.masterId);
            follower = new WPI_TalonSRX(Constants.LiftingUnit.followerId);
        }
    }

    public static class SwerveDrive {
        public static SwerveModule frontLeftModule;
        public static SwerveModule frontRightModule;
        public static SwerveModule backRightModule;
        public static SwerveModule backLeftModule;

        public static SwerveDriveKinematics kinematics;

        public static void init() {
            constructMotors();
        }

        private static void constructMotors() {
            frontLeftModule = new SwerveModule(Constants.SwerveDrive.frontLeftMotorRotationId,
                    Constants.SwerveDrive.frontLeftMotorSpeedId, Constants.SwerveDrive.frontLeftEncoderRotationPort,
                    Constants.SwerveDrive.frontLeftEncoderRotationDeviceAdress,
                    Constants.SwerveDrive.frontLeftModuleLocation, Constants.SwerveDrive.pidConst);
            frontRightModule = new SwerveModule(Constants.SwerveDrive.frontRightMotorRotationId,
                    Constants.SwerveDrive.frontRightMotorSpeedId, Constants.SwerveDrive.frontRightEncoderRotationPort,
                    Constants.SwerveDrive.frontRightEncoderRotationDeviceAdress,
                    Constants.SwerveDrive.frontRightModuleLocation, Constants.SwerveDrive.pidConst);
            backRightModule = new SwerveModule(Constants.SwerveDrive.backRightMotorRotationId,
                    Constants.SwerveDrive.backRightMotorSpeedId, Constants.SwerveDrive.backRightEncoderRotationPort,
                    Constants.SwerveDrive.backRightEncoderRotationDeviceAdress,
                    Constants.SwerveDrive.backRightModuleLocation, Constants.SwerveDrive.pidConst);
            backLeftModule = new SwerveModule(Constants.SwerveDrive.backLeftMotorRotationId,
                    Constants.SwerveDrive.backLeftMotorSpeedId, Constants.SwerveDrive.backLeftEncoderRotationPort,
                    Constants.SwerveDrive.backLeftEncoderRotationDeviceAdress,
                    Constants.SwerveDrive.backLeftModuleLocation, Constants.SwerveDrive.pidConst);

            kinematics = new SwerveDriveKinematics(frontLeftModule.location, frontRightModule.location,
                    backLeftModule.location, backRightModule.location);
        }
    }

    public static void init() {
        Gripper.init();
        Cart.init();
        LiftingUnit.init();
        SwerveDrive.init();
    }
}
