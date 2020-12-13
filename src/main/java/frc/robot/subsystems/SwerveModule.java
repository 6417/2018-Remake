package frc.robot.subsystems;

import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;

import edu.wpi.first.wpilibj.I2C;
import edu.wpi.first.wpilibj.controller.ProfiledPIDController;
import edu.wpi.first.wpilibj.controller.SimpleMotorFeedforward;
import edu.wpi.first.wpilibj.geometry.Translation2d;
import edu.wpi.first.wpilibj.kinematics.SwerveModuleState;
import edu.wpi.first.wpilibj.trajectory.TrapezoidProfile;
import frc.ArduinoAbsoluteEncoder.ArduinoAbsoluteEncoder;

public class SwerveModule {
    public static class PIDConstants {
        public double kF;
        public double kP;
        public double kI;
        public double kD;

        public double acceleration;
        public double cruiseVelocity;
    }

    private WPI_TalonSRX motorRotation;
    private WPI_TalonSRX motorSpeed;
    public ArduinoAbsoluteEncoder encoder;
    public Translation2d location;
    private ProfiledPIDController pid;
    private SimpleMotorFeedforward rotationFeedforward;
    private double lastPosition;

    public SwerveModule(int roationId, int speedId, I2C.Port encoderPort, int encoderDeviceAdress,
            Translation2d location, PIDConstants pidConstants, byte encoderHomePoint) {
        motorRotation = new WPI_TalonSRX(roationId);
        motorSpeed = new WPI_TalonSRX(speedId);

        encoder = new ArduinoAbsoluteEncoder(encoderPort, encoderDeviceAdress);
        this.location = location;

        pid = new ProfiledPIDController(pidConstants.kP, pidConstants.kI, pidConstants.kD,
                new TrapezoidProfile.Constraints(pidConstants.cruiseVelocity, pidConstants.acceleration));
        pid.enableContinuousInput(-Math.PI, Math.PI);
        rotationFeedforward = new SimpleMotorFeedforward(pidConstants.kS, pidConstants.kV);

        encoder.setDistancePerPulse(2 * Math.PI / ArduinoAbsoluteEncoder.maxTicks);
        encoder.setHome(encoderHomePoint);
    }

    public void setDesiredState(SwerveModuleState state) {
        final double turnOutput = pid.calculate(encoder.getRelPosition(), state.angle.getRadians());

        final double turnFeedforward = rotationFeedforward.calculate(pid.getSetpoint().velocity);

    public void updateEncoder() {
        lastPosition = encoder.getRelPosition();
    }
}
