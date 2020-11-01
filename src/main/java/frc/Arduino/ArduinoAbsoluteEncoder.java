/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.Arduino;

import edu.wpi.first.wpilibj.I2C;

public class ArduinoAbsoluteEncoder {
    private I2C device;
    /**
     * Creates an ArduinoEncoder object wich is connected to the arduino trough I2C 
     */
    ArduinoAbsoluteEncoder(I2C.Port port, int deviceAdress) {
        device = new I2C(port, deviceAdress);
    }

    /**
     * sets The current position of the encoder to 0
     * 
     * @return
     * Returns true if transfer was successfull
     */
    public boolean setHome() {
        byte[] buffer = new byte[1];
        return !device.read(0x01, 1, buffer);
    }

    /**
     * @return
     * Returns the current position of the encoder. 
     * Returns -1 if the transfer wasn't successfull
     */
    public byte getPosition() {
        byte[] buffer = new byte[1];
        if (device.read(0x00, 1, buffer))
            return -1;
        return buffer[0];
    }
}
