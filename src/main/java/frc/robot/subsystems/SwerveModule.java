package frc.robot.subsystems;

import java.sql.Driver;

import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;

import edu.wpi.first.wpilibj.DriverStation;
import edu.wpi.first.wpilibj.I2C;
import edu.wpi.first.wpilibj.controller.ProfiledPIDController;
import edu.wpi.first.wpilibj.geometry.Translation2d;
import edu.wpi.first.wpilibj.kinematics.SwerveModuleState;
import edu.wpi.first.wpilibj.trajectory.TrapezoidProfile;
import edu.wpi.first.wpilibj.trajectory.TrapezoidProfile.Constraints;
import frc.ArduinoAbsoluteEncoder.AbsoluteEncoderI2C;
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
    private ArduinoAbsoluteEncoder.Positions lastPositions = new ArduinoAbsoluteEncoder.Positions();
    private byte homePoint;

    public double getEncoderPos() {
        return lastPositions.relPos;
    }

    public double getAbsPos() {
        return lastPositions.absPos;
    }

    public SwerveModule(int roationId, int speedId, I2C.Port encoderPort, byte encoderDeviceAdress,
            Translation2d location, PIDConstants pidConstants, byte encoderHomePoint) {
        motorRotation = new WPI_TalonSRX(roationId);
        motorSpeed = new WPI_TalonSRX(speedId);

        initEncoder(encoderPort, encoderDeviceAdress);

        this.location = location;

        pid = new ProfiledPIDController(pidConstants.kP, pidConstants.kI, pidConstants.kD,
                new TrapezoidProfile.Constraints(pidConstants.cruiseVelocity, pidConstants.acceleration));
        // pid.enableContinuousInput(-Math.PI, Math.PI);
        pid.setTolerance(pidConstants.tolerance);
        pid.setConstraints(new Constraints());

        encoder.setOutputRange(-Math.PI, Math.PI);
        homePoint = encoderHomePoint;
        setHome();
    }

    private int initEncoderTrys = 0;
    private static final int maxInitEncoderTrys = 100;

    private void initEncoder(I2C.Port port, byte address) {
        initEncoderTrys++;
        if (initEncoderTrys >= maxInitEncoderTrys) {
            DriverStation.reportError("Failed to initialize encoder", false);
            System.exit(-1);
        }
        try {
            encoder = new ArduinoAbsoluteEncoder(port, address);
        } catch (AbsoluteEncoderI2C.Exception e) {
            initEncoder(port, address);
        }
    }

    private double map(double val, double inMin, double inMax, double outMin, double outMax) {
        return outMin + ((outMax - outMin) / (inMax - inMin)) * (val - inMin);

    }

    public void setDesiredState(SwerveModuleState state) {
        double turnOutput;
        turnOutput = pid.calculate(lastPositions.relPos, state.angle.getRadians());

        motorRotation.setVoltage(turnOutput);
        motorSpeed.set(state.speedMetersPerSecond); // not m/s its a number between 0 and 1
    }

    private long timeOfLastFaile = -1; // -1 means no failure

    public void updateEncoder() {
        try {
            lastPositions = encoder.getPositions();
            timeOfLastFaile = -1;
            System.out.println("succeded reading i2c");
        } catch (Exception e) {
            if (System.currentTimeMillis() - timeOfLastFaile > Constants.SwerveDrive.allowableTimeOfEncoderFaliure) {
                DriverStation.reportError(
                        "[SwerveModule.updateEncoder] Allowable time of encoder faliure has been exceeded, restarting connection to encoder now.",
                        false);
                restartEncoder();
            }
            System.out.println("Failed to read i2c");
            if (timeOfLastFaile == -1)
                timeOfLastFaile = System.currentTimeMillis();
        }
    }

    private void restartEncoder(int trys) {
        initEncoderTrys++;
        if (initEncoderTrys >= maxInitEncoderTrys)
            assert false : "Failed to restart encoder";
        try {
            encoder.restart();
        } catch (AbsoluteEncoderI2C.Exception e) {
            restartEncoder(trys++);
        }
    }

    private void restartEncoder() {
        restartEncoder(0);
    }

    public byte getHome() {
        return lastPositions.homePos;
    }

    public void setHome() {
        try {
            encoder.setHome(homePoint);
        } catch (Exception e) {
            if (System.currentTimeMillis() - timeOfLastFaile > Constants.SwerveDrive.allowableTimeOfEncoderFaliure) {
                DriverStation.reportWarning(
                        "[SwerveModule.setHome] Allowable time of encoder faliure has been exceeded, restarting connection to encoder now.",
                        false);
                restartEncoder();
            } else {
                setHome();
            }
            timeOfLastFaile = System.currentTimeMillis();
        }
    }
}
