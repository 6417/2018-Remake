/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.ArduinoAbsoluteEncoder;

import java.nio.Buffer;
import java.nio.ByteBuffer;

import edu.wpi.first.wpilibj.I2C;

public class ArduinoAbsoluteEncoder {
    public enum Register {
        GET_ABS_POSITION((byte) 0x00), SET_HOME((byte) 0x01), GET_HOME((byte) 0x01), GET_REL_POSITION((byte) 0x02),
        GET_ALL_POSITIONS((byte) 0x30), GET_CURRENT_ERROR((byte) 0x10), GET_LAST_ERROR((byte) 0x11),
        CLEAR_ERROR((byte) 0x12), GET_VERSION((byte) 0x20), ILLEGAL_VALUE((byte) -1);

        public final byte address;

        private Register(byte address) {
            this.address = address;
        }

        public static Register valueOf(byte address) {
            for (Register e : values()) {
                if (e.address == address) {
                    return e;
                }
            }
            return null;
        }
    }

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
    private int adress;
    private I2C device;
    public static final int maxTicks = 127;
    public static final int minTicks = 0;
    private double minOut = minTicks;
    private double maxOut = maxTicks;
    private static Register currentRegister = Register.ILLEGAL_VALUE;
    private Positions positions = new Positions();

    /**
     * Creates an ArduinoEncoder object wich is connected to the arduino trough I2C
     * 
     * @param port          The I2C port the device is connected to.
     * 
     * @param deviceAddress The address of the device on the I2C bus.
     */

    public ArduinoAbsoluteEncoder(I2C.Port port, int deviceAdress) {
        device = new I2C(port, deviceAdress);
        this.port = port;
        adress = deviceAdress;
    }

    /**
     * @param data
     */
    private boolean write(byte[] data) {
        if (device.writeBulk(data, data.length)) {
            return true;
        }

        currentRegister = Register.valueOf(data[0]);
        return false;
    }

    private double map(double val, double inMin, double inMax, double outMin, double outMax) {
        return outMin + ((outMax - outMin) / (inMax - inMin)) * (val - inMin);

    }

    private boolean read(Register register, ByteBuffer received) {
        if (currentRegister != register) {
            if (write(new byte[] { register.address }))
                return true;
        }
        return device.readOnly(received, 1);
    }

    public void setInverted(boolean inverted) {
        this.inverted = inverted;
    }

    /**
     * sets The current position of the encoder to 0
     * 
     * @return Returns false if transfer was successfull
     */
    public boolean setHome(byte pos) {
        return write(new byte[] { Register.SET_HOME.address, pos });
    }

    public byte getHome() throws Exception {
        ByteBuffer buffer = ByteBuffer.allocate(1);
        if (read(Register.GET_HOME, buffer))
            throw new Exception("Failed to read I2C");
        return buffer.get(0);
    }

    public double getAbsPosition() throws Exception {
        ByteBuffer buffer = ByteBuffer.allocate(1);
        if (read(Register.GET_ABS_POSITION, buffer))
            throw new Exception("Failed to read I2C");
        return mapPosition(buffer.get(0));
    }

    private Error[] getErrors() throws Exception {
        return new Error[] { Error.valueOf(getCurrentError().get(0)), Error.valueOf(getLastError().get(0)) };
    }

    private ByteBuffer getLastError() throws Exception {
        ByteBuffer lastError = ByteBuffer.allocate(1);
        if (read(Register.GET_LAST_ERROR, lastError))
            throw new Exception("Failed to read I2C.");
        return lastError;
    }

    private ByteBuffer getCurrentError() throws Exception {
        ByteBuffer currentError = ByteBuffer.allocate(1);
        if (read(Register.GET_CURRENT_ERROR, currentError))
            throw new Exception("Failed to read I2C.");
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
        if (read(Register.GET_REL_POSITION, buffer))
            throw new Exception("Failed to read I2C.");
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
        if (read(Register.GET_ALL_POSITIONS, buffer))
            throw new Exception("Failed to read I2C");
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

    public void restart() {
        device.close();
        device = null;
        device = new I2C(port, adress);
    }

    public int getAddress() {
        return adress;
    }
}
