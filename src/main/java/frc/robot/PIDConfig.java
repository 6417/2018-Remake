package frc.robot;

import java.util.LinkedHashMap;
import java.util.Optional;

import com.ctre.phoenix.motorcontrol.StatusFrameEnhanced;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;

public class PIDConfig {
	/**
	 * <h> PIDConfigurator class wich configures the PID off a WPI_TalonSRX </h>
	 * @see
	 * <p>
	 * 	You have to set all non optional values or the defualt will be used. If
	 * 	you don't initialize the optional values, the associated WPI_TalonSRX
	 * 	function won't be called
	 * </p>
	 */

	public static class PIDConfigurator {
		public LinkedHashMap<StatusFrameEnhanced, Integer> statusFramePeriods;

		public double nominalOutputForward = 0.0;
		public double nominalOutputReverse = 0.0;
		public double peakOutputForward = 0.0;
		public double peakOutputReverse = 0.0;

		public int slotIdx = 0;
		public int pidIdx = 0;

		public double kP = 0.0;
		public double kI = 0.0;
		public double kD = 0.0;

		public int integralZone = 0;

		public int cruiseVelocity = 0;
		public int acceleration = 0;

		public Optional<Integer> allowableClosedloopError;
		public Optional<Double> openloopRamp;
		public Optional<Double> closedloopRamp;

		protected PIDConfigurator() {
			statusFramePeriods = new LinkedHashMap<StatusFrameEnhanced, Integer>();
			allowableClosedloopError = Optional.empty();
			openloopRamp = Optional.empty();
			closedloopRamp = Optional.empty();
		}

		/**
		 * Configures the given motor with the specified settings from this class.
		 * @param motor
		 * The motor to be configured.
		 */
		public void configMotor(WPI_TalonSRX motor) {
			configLoopRamps(motor);
			setStatusFramePeriods(motor);
			configOutputs(motor);
			configPIDConstants(motor);
			configSpeeds(motor);
		}

		protected void configSpeeds(WPI_TalonSRX motor) {
			motor.configMotionCruiseVelocity(cruiseVelocity, Constants.timeOut);
            motor.configMotionAcceleration(acceleration, Constants.timeOut);
		}

		protected void configPIDConstants(WPI_TalonSRX motor) {
			motor.selectProfileSlot(slotIdx, pidIdx);
            motor.config_kP(slotIdx, kP, Constants.timeOut);
            motor.config_kI(slotIdx, kI, Constants.timeOut);
            motor.config_kD(slotIdx, kD, Constants.timeOut);
			motor.config_IntegralZone(slotIdx, integralZone);
			configAllowableClosedloopError(motor);
		}

		protected void configAllowableClosedloopError(WPI_TalonSRX motor) {
			if (allowableClosedloopError.isPresent())
            	motor.configAllowableClosedloopError(slotIdx, allowableClosedloopError.get(), Constants.timeOut);
		}

		protected void configOutputs(WPI_TalonSRX motor) {
			motor.configNominalOutputForward(nominalOutputForward, Constants.timeOut);
            motor.configNominalOutputReverse(nominalOutputReverse, Constants.timeOut);
            motor.configPeakOutputForward(peakOutputForward, Constants.timeOut);
            motor.configPeakOutputReverse(peakOutputReverse, Constants.timeOut);
		}

		protected void setStatusFramePeriods(WPI_TalonSRX motor) {
			for (StatusFrameEnhanced k : statusFramePeriods.keySet())
				motor.setStatusFramePeriod(k, statusFramePeriods.get(k), Constants.timeOut);
		}

		protected void configLoopRamps(WPI_TalonSRX motor) {
			if (openloopRamp.isPresent())
				motor.configOpenloopRamp(openloopRamp.get());			
			if (closedloopRamp.isPresent())
				motor.configOpenloopRamp(closedloopRamp.get());
		}
	}

	/**
	 * PIDFConfigurator class wich configures the PIDF off a WPI_TalonSRX
	 * @see
	 * For full documentation have a look at the docs of {@link PIDConfig #PIDConfigurator}
	 */
	public static class PIDFConfigurator extends PIDConfigurator {
		public double kF;

		@Override
		protected void configPIDConstants(WPI_TalonSRX motor) {
			super.configPIDConstants(motor);
			motor.config_kF(slotIdx, kF, Constants.timeOut);
		}
	}
}