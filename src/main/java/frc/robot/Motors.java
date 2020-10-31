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
import com.ctre.phoenix.motorcontrol.StatusFrameEnhanced;
import com.ctre.phoenix.motorcontrol.InvertType;
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
                left.setInverted(InvertType.OpposeMaster);
                left.follow(right);
            }
        }
    }

    public static class Cart {
        public static WPI_TalonSRX motor;

        public static void init() {
            if (Constants.Cart.isSubsystemEnabled) {
                initMotor();
                initLimitSwitches();
                initPID();
                initEncoder();
            }
        }

        private static void initMotor() {
            motor = new WPI_TalonSRX(Constants.Cart.motorId);
            motor.setInverted(true);
		}
                    

		private static void initEncoder() {
            motor.configSelectedFeedbackSensor(FeedbackDevice.QuadEncoder, Constants.Cart.PID.pidIdx, Constants.timeOut);
        }

        private static void initLimitSwitches() {
            motor.configReverseLimitSwitchSource(LimitSwitchSource.FeedbackConnector, LimitSwitchNormal.NormallyClosed);
            motor.configForwardLimitSwitchSource(LimitSwitchSource.FeedbackConnector, LimitSwitchNormal.NormallyOpen);
        }

        private static void initPID() {
            motor.configOpenloopRamp(Constants.Cart.PID.openloopRamp);
            motor.configClosedloopRamp(Constants.Cart.PID.closedloopRamp);
            motor.setStatusFramePeriod(StatusFrameEnhanced.Status_13_Base_PIDF0, Constants.Cart.PID.framePeriod,
                    Constants.timeOut);
            motor.setStatusFramePeriod(StatusFrameEnhanced.Status_10_MotionMagic, Constants.Cart.PID.framePeriod,
                    Constants.timeOut);
            motor.configNominalOutputForward(Constants.Cart.PID.nominalOutForward, Constants.timeOut);
            motor.configNominalOutputReverse(Constants.Cart.PID.nominalOutReverse, Constants.timeOut);
            motor.configPeakOutputForward(Constants.Cart.PID.peakOutForward, Constants.timeOut);
            motor.configPeakOutputReverse(Constants.Cart.PID.peakOutReverse, Constants.timeOut);
            motor.selectProfileSlot(Constants.Cart.PID.slotIdx, Constants.Cart.PID.pidIdx);
            motor.config_kF(Constants.Cart.PID.slotIdx, Constants.Cart.PID.kf, Constants.timeOut);
            motor.config_kP(Constants.Cart.PID.slotIdx, Constants.Cart.PID.kP, Constants.timeOut);
            motor.config_kI(Constants.Cart.PID.slotIdx, Constants.Cart.PID.kI, Constants.timeOut);
            motor.config_kD(Constants.Cart.PID.slotIdx, Constants.Cart.PID.kD, Constants.timeOut);
            motor.config_IntegralZone(Constants.Cart.PID.slotIdx, Constants.Cart.PID.integralZone);
            motor.configMotionCruiseVelocity(Constants.Cart.PID.motionCruiseVelocity, Constants.timeOut);
            motor.configMotionAcceleration(Constants.Cart.PID.motionAcceleration, Constants.timeOut);
        }
    }

    public static class LiftingUnit {
        public static void init() {
            
        }
    }

    public static void init() {
        Gripper.init();
        Cart.init();
        LiftingUnit.init();
    }
}
