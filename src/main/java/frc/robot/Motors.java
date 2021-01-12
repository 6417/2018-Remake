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
            frontRightModule = new SwerveModule(Constants.SwerveDrive.Front.Right.motorRotationId,
                    Constants.SwerveDrive.Front.Right.motorSpeedId,
                    Constants.SwerveDrive.Front.Right.encoderRotationPort,
                    Constants.SwerveDrive.Front.Right.encoderRotationDeviceAdress,
                    Constants.SwerveDrive.Front.Right.moduleLocation, Constants.SwerveDrive.pidConst,
                    Constants.SwerveDrive.Front.Right.homePoint);
            frontLeftModule = new SwerveModule(Constants.SwerveDrive.Front.Left.motorRotationId,
                    Constants.SwerveDrive.Front.Left.motorSpeedId, Constants.SwerveDrive.Front.Left.encoderRotationPort,
                    Constants.SwerveDrive.Front.Left.encoderRotationDeviceAdress,
                    Constants.SwerveDrive.Front.Left.moduleLocation, Constants.SwerveDrive.pidConst,
                    Constants.SwerveDrive.Front.Left.homePoint);
            backRightModule = new SwerveModule(Constants.SwerveDrive.Back.Right.motorRotationId,
                    Constants.SwerveDrive.Back.Right.motorSpeedId, Constants.SwerveDrive.Back.Right.encoderRotationPort,
                    Constants.SwerveDrive.Back.Right.encoderRotationDeviceAdress,
                    Constants.SwerveDrive.Back.Right.moduleLocation, Constants.SwerveDrive.pidConst,
                    Constants.SwerveDrive.Back.Right.homePoint);
            backLeftModule = new SwerveModule(Constants.SwerveDrive.Back.Left.motorRotationId,
                    Constants.SwerveDrive.Back.Left.motorSpeedId, Constants.SwerveDrive.Back.Left.encoderRotationPort,
                    Constants.SwerveDrive.Back.Left.encoderRotationDeviceAdress,
                    Constants.SwerveDrive.Back.Left.moduleLocation, Constants.SwerveDrive.pidConst,
                    Constants.SwerveDrive.Back.Left.homePoint);

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
