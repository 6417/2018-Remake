/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.ArduinoAbsoluteEncoder;

import edu.wpi.first.wpilibj.I2C;

public class ArduinoAbsoluteEncoder {
    public static class Request {
        public static final byte GET_ABS_POSITION = 0x00;
        public static final byte SET_HOME = 0x01;
        public static final byte GET_REL_POSITION = 0x02;
    }

    private boolean inverted = false;
    private I2C device;

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

    public void setInverted(boolean inverted) {
        this.inverted = inverted;
    }

    /**
     * sets The current position of the encoder to 0
     * 
     * @return Returns true if transfer was successfull
     */
    public boolean setHome(byte pos) {
        return !device.write(Request.SET_HOME, pos);
    }

    /**
     * @return Returns the current absolute position of the encoder. Returns -1 if
     *         the transfer wasn't successfull.
     */
    public int getAbsPosition() {
        byte[] buffer = new byte[1];
        if (device.read(Request.GET_ABS_POSITION, 1, buffer))
            return -1;
        if (!inverted)
            return buffer[0];
        else
            return 127 - buffer[0];
    }

    /**
     * @return Returns the current relative position of the encoder to the home
     *         point. Returns -1 if the transfer wasn't successfull.
     */
    public int getRelPosition() {
        byte[] buffer = new byte[1];
        if (device.read(Request.GET_REL_POSITION, 1, buffer))
            return -1;

        if (!inverted)
            return buffer[0];
        else
            return 127 - buffer[0];
    }
}
