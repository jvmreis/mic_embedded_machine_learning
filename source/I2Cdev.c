#include "I2Cdev.h"

int8_t I2Cdev_readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data) {
    return BOARD_LPI2C_Receive(BOARD_ACCEL_I2C_BASEADDR, devAddr, regAddr, 1, data, length);
}

int8_t I2Cdev_readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data) {
    return I2Cdev_readBytes(devAddr, regAddr, 1, data);
}

int8_t I2Cdev_readWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data) {
    uint8_t buffer[length * 2];
    int status = BOARD_LPI2C_Receive(BOARD_ACCEL_I2C_BASEADDR, devAddr, regAddr, 1, buffer, length * 2);
    if (status == 0) {
        for (uint8_t i = 0; i < length; i++) {
            data[i] = ((uint16_t)buffer[2*i] << 8) | buffer[2*i + 1];
        }
        return length;
    }
    return -1;
}

int8_t I2Cdev_readWord(uint8_t devAddr, uint8_t regAddr, uint16_t *data) {
    return I2Cdev_readWords(devAddr, regAddr, 1, data);
}

int8_t I2Cdev_readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data) {
    uint8_t b;
    if (I2Cdev_readByte(devAddr, regAddr, &b) == 0) {
        *data = (b >> bitNum) & 0x01;
        return 1;
    }
    return -1;
}

int8_t I2Cdev_readBitW(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t *data) {
    uint16_t w;
    if (I2Cdev_readWord(devAddr, regAddr, &w) == 1) {
        *data = (w >> bitNum) & 0x01;
        return 1;
    }
    return -1;
}

int8_t I2Cdev_readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data) {
    uint8_t b;
    if (I2Cdev_readByte(devAddr, regAddr, &b) == 0) {
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        b &= mask;
        b >>= (bitStart - length + 1);
        *data = b;
        return 1;
    }
    return -1;
}

int8_t I2Cdev_readBitsW(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t *data) {
    uint16_t w;
    if (I2Cdev_readWord(devAddr, regAddr, &w) == 1) {
        uint16_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        w &= mask;
        w >>= (bitStart - length + 1);
        *data = w;
        return 1;
    }
    return -1;
}

bool I2Cdev_writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t* data) {
    return BOARD_LPI2C_Send(BOARD_ACCEL_I2C_BASEADDR, devAddr, regAddr, 1, data, length) == 0;
}

bool I2Cdev_writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data) {
    return I2Cdev_writeBytes(devAddr, regAddr, 1, &data);
}

bool I2Cdev_writeWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t* data) {
    uint8_t buffer[length * 2];
    for (uint8_t i = 0; i < length; i++) {
        buffer[2*i]     = (data[i] >> 8) & 0xFF;
        buffer[2*i + 1] = data[i] & 0xFF;
    }
    return BOARD_LPI2C_Send(BOARD_ACCEL_I2C_BASEADDR, devAddr, regAddr, 1, buffer, length * 2) == 0;
}

bool I2Cdev_writeWord(uint8_t devAddr, uint8_t regAddr, uint16_t data) {
    return I2Cdev_writeWords(devAddr, regAddr, 1, &data);
}

bool I2Cdev_writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data) {
    uint8_t b;
    if (I2Cdev_readByte(devAddr, regAddr, &b) == 0) {
        b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
        return I2Cdev_writeByte(devAddr, regAddr, b);
    }
    return false;
}

bool I2Cdev_writeBitW(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t data) {
    uint16_t w;
    if (I2Cdev_readWord(devAddr, regAddr, &w) == 1) {
        w = (data != 0) ? (w | (1 << bitNum)) : (w & ~(1 << bitNum));
        return I2Cdev_writeWord(devAddr, regAddr, w);
    }
    return false;
}

bool I2Cdev_writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data) {
    uint8_t b;
    if (I2Cdev_readByte(devAddr, regAddr, &b) == 0) {
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        data <<= (bitStart - length + 1);
        data &= mask;
        b &= ~mask;
        b |= data;
        return I2Cdev_writeByte(devAddr, regAddr, b);
    }
    return false;
}

bool I2Cdev_writeBitsW(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t data) {
    uint16_t w;
    if (I2Cdev_readWord(devAddr, regAddr, &w) == 1) {
        uint16_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        data <<= (bitStart - length + 1);
        data &= mask;
        w &= ~mask;
        w |= data;
        return I2Cdev_writeWord(devAddr, regAddr, w);
    }
    return false;
}
