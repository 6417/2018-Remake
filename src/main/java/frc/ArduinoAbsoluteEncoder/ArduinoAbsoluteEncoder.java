/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.ArduinoAbsoluteEncoder;

import java.nio.Buffer;
import java.nio.ByteBuffer;

import edu.wpi.first.wpilibj.DriverStation;
import edu.wpi.first.wpilibj.I2C;

public class ArduinoAbsoluteEncoder {
    public enum Error {
        BAD_I2C_REGISTER_REQUEST((byte) 2), BAD_I2C_REGISTER_ACCESS((byte) 3), I2C_TO_MUCH_DATA_RECIEVED((byte) 4),
        I2C_TO_LESS_DATA_RECIEVED((byte) 5), NO_ERROR((byte) 0);

        public final byte address;

        private Error(byte address) {
            this.address = address;
        }

        public static Error valueOf(byte address) {
            for (Error e : values()) {
                if (e.address == address) {
                    return e;
                }
            }
            return null;
        }
    }

    public static class Positions {
        public double relPos;
        public double absPos;
        public byte homePos;
    }

    private boolean inverted = false;
    private I2C.Port port;
    private byte adress;
    private AbsoluteEncoderI2C device;
    public static final int maxTicks = 127;
    public static final int minTicks = 0;
    private double minOut = minTicks;
    private double maxOut = maxTicks;
    private static AbsoluteEncoderI2C.Register currentRegister = AbsoluteEncoderI2C.Register.ILLEGAL_VALUE;
    private Positions positions = new Positions();

    /**
     * Creates an ArduinoEncoder object wich is connected to the arduino trough I2C
     * 
     * @param port          The I2C port the device is connected to.
     * 
     * @param deviceAddress The address of the device on the I2C bus.
     */

    public ArduinoAbsoluteEncoder(I2C.Port port, byte deviceAdress) throws AbsoluteEncoderI2C.Exception {
        device = new AbsoluteEncoderI2C(port, deviceAdress);
        this.port = port;
        adress = deviceAdress;
    }

    private double map(double val, double inMin, double inMax, double outMin, double outMax) {
        return outMin + ((outMax - outMin) / (inMax - inMin)) * (val - inMin);

    }

    public void setInverted(boolean inverted) {
        this.inverted = inverted;
    }

    /**
     * sets The current position of the encoder to 0
     * 
     * @return Returns false if transfer was successfull
     */
    public void setHome(byte pos) throws AbsoluteEncoderI2C.Exception {
        device.write(ByteBuffer.wrap(new byte[] { AbsoluteEncoderI2C.Register.SET_HOME.address, pos }));
    }

    public byte getHome() throws AbsoluteEncoderI2C.Exception {
        ByteBuffer buffer = ByteBuffer.allocate(1);
        device.read(AbsoluteEncoderI2C.Register.GET_HOME, buffer);
        return buffer.get(0);
    }

    public double getAbsPosition() throws Exception {
        ByteBuffer buffer = ByteBuffer.allocate(1);
        device.read(AbsoluteEncoderI2C.Register.GET_ABS_POSITION, buffer);
        return mapPosition(buffer.get(0));
    }

    private Error[] getErrors() throws Exception {
        return new Error[] { Error.valueOf(getCurrentError().get(0)), Error.valueOf(getLastError().get(0)) };
    }

    private ByteBuffer getLastError() throws Exception {
        ByteBuffer lastError = ByteBuffer.allocate(1);
        device.read(AbsoluteEncoderI2C.Register.GET_LAST_ERROR, lastError);
        return lastError;
    }

    private ByteBuffer getCurrentError() throws Exception {
        ByteBuffer currentError = ByteBuffer.allocate(1);
        device.read(AbsoluteEncoderI2C.Register.GET_CURRENT_ERROR, currentError);
        return currentError;
    }

    private void printErrors() throws Exception {
        for (Error e : getErrors())
            printError(e);
    }

    private void printError(Error e) {
        if (e != Error.NO_ERROR)
            System.err.println("[ArduinoAbsoluteEncoder Error] " + e.name());
    }

    /**
     * @return Returns the current relative position of the encoder to the home
     *         point. Returns -1 if the transfer wasn't successfull.
     */
    public double getRelPosition() throws Exception {
        ByteBuffer buffer = ByteBuffer.allocate(1);
        device.read(AbsoluteEncoderI2C.Register.GET_REL_POSITION, buffer);
        printErrors();
        return mapPosition(buffer.get(0));
    }

    private double mapPosition(byte pos) {
        if (!inverted)
            return map(pos, minTicks, maxTicks, minOut, maxOut);
        else
            return map((maxTicks - pos), minTicks, maxTicks, minOut, maxOut);
    }

    public Positions getPositions() throws Exception {
        ByteBuffer buffer = ByteBuffer.allocate(3);
        device.read(AbsoluteEncoderI2C.Register.GET_ALL_POSITIONS, buffer);
        printErrors();
        positions.absPos = mapPosition(buffer.get(0));
        positions.homePos = buffer.get(1);
        positions.relPos = mapPosition(buffer.get(2));
        return positions;
    }

    public void setOutputRange(double minOut, double maxOut) {
        this.minOut = minOut;
        this.maxOut = maxOut;
    }

    public void restart() throws AbsoluteEncoderI2C.Exception {
        device.close();
        device = null;
        device = new AbsoluteEncoderI2C(port, adress);
    }

    public int getAddress() {
        return adress;
    }
}
