package frc.ArduinoAbsoluteEncoder;

import java.nio.ByteBuffer;

import edu.wpi.first.wpilibj.I2C;

public class AbsoluteEncoderI2C extends I2C {
    public enum Register {
        GET_ABS_POSITION((byte) 0x00), SET_HOME((byte) 0x01), GET_HOME((byte) 0x01), GET_REL_POSITION((byte) 0x02),
        GET_ALL_POSITIONS((byte) 0x30), GET_CURRENT_ERROR((byte) 0x10), GET_LAST_ERROR((byte) 0x11),
        CLEAR_ERROR((byte) 0x12), GET_VERSION((byte) 0x20), INIT_TEST((byte) 0x40), ILLEGAL_VALUE((byte) -1);

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

    private byte address;
    private I2C.Port port;

    public static class Exception extends java.lang.Exception {
        public Exception(String message) {
            super(message);
        }

        public static class InvalidSEQ extends Exception {
            public InvalidSEQ(String message) {
                super(message);
            }
        }

        public static class InvalidCRC extends Exception {
            public InvalidCRC(String message) {
                super(message);
            }
        }

        public static class InitFailed extends Exception {
            public InitFailed(String message) {
                super(message);
            }
        }
    }

    private static byte SEQ = 0x00;

    private static byte calcCRC(ByteBuffer data) {
        byte crc = 0x00;
        for (int i = data.capacity() - 1; i >= 0; i--) {
            byte extract = data.get((data.capacity() - 1) - i);
            for (byte j = 8; j >= 0; j--) {
                byte sum = (byte) ((crc ^ extract) & 0x01);
                crc >>= 1;
                if (sum != 0) {
                    crc ^= 0x8C;
                }
                extract >>= 1;
            }
        }
        return crc;

    }

    private void increaseSEQ() {
        if (SEQ == 0xff)
            SEQ = 0x01;
        else
            SEQ++;
    }

    private void directWrite(ByteBuffer data) throws Exception {
        if (super.writeBulk(data, data.capacity()))
            throw new Exception(String.format("Faild to write I2C on address: %d", address));
    }

    public void write(ByteBuffer buffer) throws Exception {
        assert buffer.capacity() <= 0xff : "length of data must be less than 256";
        ByteBuffer data = ByteBuffer.allocate(buffer.capacity() - 2);
        data.put(SEQ);
        data.put((byte) buffer.capacity());
        data.put(buffer);
        data.put(calcCRC(data));
        directWrite(data);
        increaseSEQ();
    }
    
    private void directRead(ByteBuffer received) throws Exception {
        if (readOnly(received, received.capacity()))
            throw new Exception(String.format("failed to read I2C on address: %d", address));
        if (calcCRC(ByteBuffer.wrap(received.array(), 0, received.capacity() - 2)) != received
                .get(received.array().length - 1))
            throw new Exception.InvalidCRC(String.format("Recived invalid CRC on address: %d", address));
        if (received.get(0) != SEQ)
            throw new Exception.InvalidSEQ(String.format("recived wrong SEQ on address: %d", address));
    }

    public void read(Register register, ByteBuffer received) throws Exception {
        write(ByteBuffer.wrap(new byte[] { register.address }));
        directRead(received);
    }

    private static final byte initTestResult = (byte) 0xff;

    private void initTest() throws Exception {
        ByteBuffer initBuffer = ByteBuffer.allocate(1);
        read(Register.INIT_TEST, initBuffer);
        if (initBuffer.get(0) != initTestResult)
            throw new Exception.InitFailed(String.format("Failed to initialize device with I2C address: %d", address)); 
    }

    public AbsoluteEncoderI2C(I2C.Port port, byte address) throws Exception {
        super(port, address);
        this.address = address;
        this.port = port;
        initTest();
    }
}
