package frc.robot.subsystems;

import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;

import edu.wpi.first.wpilibj.I2C;
import edu.wpi.first.wpilibj.controller.ProfiledPIDController;
import edu.wpi.first.wpilibj.geometry.Translation2d;
import edu.wpi.first.wpilibj.kinematics.SwerveModuleState;
import edu.wpi.first.wpilibj.trajectory.TrapezoidProfile;
import edu.wpi.first.wpilibj.trajectory.TrapezoidProfile.Constraints;
import frc.ArduinoAbsoluteEncoder.ArduinoAbsoluteEncoder;
import frc.robot.Constants;

public class SwerveModule {
    public static class PIDConstants {
        public double kP;
        public double kI;
        public double kD;
        public double tolerance;

        public double acceleration;
        public double cruiseVelocity;
    }

    private WPI_TalonSRX motorRotation;
    private WPI_TalonSRX motorSpeed;
    public ArduinoAbsoluteEncoder encoder;
    public Translation2d location;
    private ProfiledPIDController pid;
    private double lastPosition;
    private byte homePoint;

    public SwerveModule(int roationId, int speedId, I2C.Port encoderPort, int encoderDeviceAdress,
            Translation2d location, PIDConstants pidConstants, byte encoderHomePoint) {
        motorRotation = new WPI_TalonSRX(roationId);
        motorSpeed = new WPI_TalonSRX(speedId);

        encoder = new ArduinoAbsoluteEncoder(encoderPort, encoderDeviceAdress);
        this.location = location;

        pid = new ProfiledPIDController(pidConstants.kP, pidConstants.kI, pidConstants.kD,
                new TrapezoidProfile.Constraints(pidConstants.cruiseVelocity, pidConstants.acceleration));
        pid.enableContinuousInput(-Math.PI, Math.PI);
        pid.setTolerance(pidConstants.tolerance);
        pid.setConstraints(new Constraints());

        encoder.setOutputRange(-Math.PI, Math.PI);
        encoder.setHome(encoderHomePoint);
        homePoint = encoderHomePoint;
        encoder.setInverted(true);
    }

    private double map(double val, double inMin, double inMax, double outMin, double outMax) {
        return outMin + ((outMax - outMin) / (inMax - inMin)) * (val - inMin);

    }

    public void setDesiredState(SwerveModuleState state) {
        double turnOutput;
        turnOutput = pid.calculate(lastPosition, state.angle.getRadians());

        System.out.println("TrunOutput: " + Double.toString(turnOutput) + " State.angle: "
                + Double.toString(map(state.angle.getRadians(), -Math.PI, Math.PI, 0, 127)) + " Encoder Relpos: "
                + Double.toString(map(lastPosition, -Math.PI, Math.PI, 0, 127)));
        motorRotation.setVoltage(turnOutput);
    }

    private long timeOfLastFaile = -1; // -1 means no failure

    public void updateEncoder() {
        try {
            lastPosition = encoder.getRelPosition();
            timeOfLastFaile = -1;
        } catch (Exception e) {
            if (System.currentTimeMillis() - timeOfLastFaile > Constants.SwerveDrive.allowableTimeOfEncoderFaliure) {
                System.out.println(
                        "Allowable time of encoder faliure has been exceeded, restarting connection to encoder now.");
                encoder.restart();
            }
            timeOfLastFaile = System.currentTimeMillis();
        }
    }

    public void setHome() {
        try {  
            encoder.setHome(homePoint);
        } catch (Exception e) {
            if (System.currentTimeMillis() - timeOfLastFaile > Constants.SwerveDrive.allowableTimeOfEncoderFaliure) {
                System.out.println("Home point of encoder with I2C id " + Integer.toString(encoder.getAddress())
                        + "could not be set.");
                System.out.println(
                        "Allowable time of encoder faliure has been exceeded, restarting connection to encoder now.");
                encoder.restart();
            } else {
                setHome();
            }
            timeOfLastFaile = System.currentTimeMillis();
        }
    }
}
