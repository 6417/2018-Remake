/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.ArduinoAbsoluteEncoder;

import java.nio.ByteBuffer;

import edu.wpi.first.wpilibj.I2C;

public class ArduinoAbsoluteEncoder {
    public enum Register {
        GET_ABS_POSITION((byte) 0x00), SET_HOME((byte) 0x01), GET_REL_POSITION((byte) 0x02), ILLEGAL_VALUE((byte) -1);

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

    private boolean inverted = false;
    private I2C device;
    public static final int maxTicks = 127;
    public static final int minTicks = 0;
    private static Register currentRegister = Register.ILLEGAL_VALUE;
    private double distancePerPulse = 1.0;

    /**
     * Creates an ArduinoEncoder object wich is connected to the arduino trough I2C
     * 
     * @param port          The I2C port the device is connected to.
     * 
     * @param deviceAddress The address of the device on the I2C bus.
     */

    public ArduinoAbsoluteEncoder(I2C.Port port, int deviceAdress) {
        device = new I2C(port, deviceAdress);
    }

    /**
     * @param data
     */
    private boolean write(byte[] data) {
        if (device.writeBulk(data, data.length))
            return false;
        currentRegister = Register.valueOf(data[0]);
        return true;
    }

    private boolean read(Register register, ByteBuffer received) {
        if (currentRegister != register) {
            if (!write(new byte[] { register.address }))
                return false;
        }

        return device.readOnly(received, 1);
    }

    public void setInverted(boolean inverted) {
        this.inverted = inverted;
    }

    /**
     * sets The current position of the encoder to 0
     * 
     * @return Returns true if transfer was successfull
     */
    public boolean setHome(byte pos) {
        return write(new byte[] {Register.SET_HOME.address, pos});
    }

    /**
     * @return Returns the current absolute position of the encoder. Returns -1 if
     *         the transfer wasn't successfull.
     */
    public double getAbsPosition() {
        ByteBuffer buffer = ByteBuffer.allocate(1);
        if (read(Register.GET_ABS_POSITION, buffer))
            return -1;
        if (!inverted)
            return (double) buffer.get(0) * distancePerPulse;
        else
            return (maxTicks - buffer.get(0)) * distancePerPulse;
    }

    /**
     * @return Returns the current relative position of the encoder to the home
     *         point. Returns -1 if the transfer wasn't successfull.
     */
    public double getRelPosition() {
        ByteBuffer buffer = ByteBuffer.allocate(1);
        if (read(Register.GET_REL_POSITION, buffer))
            return -1;
        if (!inverted)
            return (double) buffer.get(0) * distancePerPulse;
        else
            return (maxTicks - buffer.get(0)) * distancePerPulse;
    }

    public void setDistancePerPulse(double distance) {
        distancePerPulse = distance;
    }
}
