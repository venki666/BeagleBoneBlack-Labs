// 2014-06-25 by Frank.gao based on Cagdas Caglak's BBB_I2C
// change  I2C_SLAVE TO I2C_READ_SLAVE of BBB_I2C::readByte(...) for test.

/*
 * Copyright 2014 Cagdas Caglak, cagdascaglak@gmail.com, http://expcodes.blogspot.com/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "BBB_I2C.h"

/** Default Constructor
 * @funtion BBB_I2C()
 * 
 */
BBB_I2C::BBB_I2C() {
    busAddr = I2C_BUS;
    path = (char*) calloc(PATH_SIZE, sizeof (char));
    sprintf(path, "/dev/i2c-%d", busAddr);
}

/**
 * @funtion BBB_I2C()
 * @param busAddr I2C Bus address.
 */
BBB_I2C::BBB_I2C(uint8_t busAddr) {
    this->busAddr = busAddr;
    path = (char*) calloc(PATH_SIZE, sizeof (char));
    sprintf(path, "/dev/i2c-%d", this->busAddr);
}

/** Default Destructor
 * @funtion ~BBB_I2C()
 * 
 */
BBB_I2C::~BBB_I2C() {
    free(path);
}

/**
 * @function writeBit(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t data, int bitNum)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param data Writing data.
 * @param bitNum Bit Number for writing.
 * @return void.
 */
void BBB_I2C::writeBit(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t data, int bitNum) {
    int8_t temp = readByte(DEV_ADD, DATA_REGADD);
    if (data == 0) {
        temp = temp & ~(1 << bitNum);
    } else if (data == 1) {
        temp = temp | (1 << bitNum);
    } else {
        printf("Value must be 0 or 1! --> Address %d.\n", DEV_ADD);
        exit(1);
    }

    writeByte(DEV_ADD, DATA_REGADD, temp);

}

/**
 * @function writeBitNoExit(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t data, int bitNum)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param data Writing data.
 * @param bitNum Bit Number for writing.
 * @return void.
 */
void BBB_I2C::writeBitNoExit(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t data, int bitNum) {
    int8_t temp = readByte(DEV_ADD, DATA_REGADD);
    if (data == 0) {
        temp = temp & ~(1 << bitNum);
    } else if (data == 1) {
        temp = temp | (1 << bitNum);
    } else {
        printf("Value must be 0 or 1! --> Address %d.\n", DEV_ADD);
    }

    writeByte(DEV_ADD, DATA_REGADD, temp);

}

/**
 * @function writeBits(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t data, int length, int startBit)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param length Bits length.
 * @param startBit Starting point of the data.
 * @return void.
 */
void BBB_I2C::writeBits(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t data, int length, int startBit) {
    int8_t temp = readByte(DEV_ADD, DATA_REGADD);
    uint8_t bits = 1;
    uint8_t i = 0;

    while (i < length - 1) {
        bits = (bits << 1);
        ++bits;
        ++i;
    }

    temp &= ~(bits << startBit);

    temp |= (data << startBit);

    writeByte(DEV_ADD, DATA_REGADD, temp);

}

/**
 * @function writeBitsNoExit(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t data, int length, int startBit)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param length Bits length.
 * @param startBit Starting point of the data.
 * @return void.
 */
void BBB_I2C::writeBitsNoExit(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t data, int length, int startBit) {
    int8_t temp = readByteNoExit(DEV_ADD, DATA_REGADD);
    uint8_t bits = 1;
    uint8_t i = 0;

    while (i < length - 1) {
        bits = (bits << 1);
        ++bits;
        ++i;
    }

    temp &= ~(bits << startBit);

    temp |= (data << startBit);

    writeByteNoExit(DEV_ADD, DATA_REGADD, temp);

}

/**
 * @function writeByte(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t data)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param data Writing data.
 * @return void.
 */
void BBB_I2C::writeByte(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t data) {

    int file;
    

    if ((file = open(path, O_RDWR)) < 0) {
        printf("%s do not open. Address %d.\n", path, DEV_ADD);
        exit(1);
    }

//    if (ioctl(file, I2C_SLAVE, DEV_ADD) < 0) {
    if (ioctl(file, I2C_SLAVE_FORCE, DEV_ADD) < 0) {        // FG: can't work for E2PROM 24c32L if use I2C_SLAVE mode.
        printf("Can not join I2C Bus. Address %d.\n", DEV_ADD);
        exit(1);
    }

    uint8_t buffer[2];

    buffer[0] = DATA_REGADD;
    buffer[1] = data;

    if (write(file, buffer, 2) != 2) {
        printf("Can not write data. Address %d.\n", DEV_ADD);
        exit(1);
    }

    close(file);

}

/**
 * @function writeByteNoExit(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t data)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param data Writing data.
 * @return void.
 */
void BBB_I2C::writeByteNoExit(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t data) {

    int file;

    if ((file = open(path, O_RDWR)) < 0) {
        printf("%s do not open. Address %d.\n", path, DEV_ADD);
    }

    if (ioctl(file, I2C_SLAVE, DEV_ADD) < 0) {
        printf("Can not join I2C Bus. Address %d.\n", DEV_ADD);
    }

    uint8_t buffer[2];

    buffer[0] = DATA_REGADD;
    buffer[1] = data;

    if (write(file, buffer, 2) != 2) {
        printf("Can not write data. Address %d.\n", DEV_ADD);
    }

    close(file);

}

/**
 * @function writeByteBuffer(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t *data, uint8_t length)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param *data Data storage array.
 * @param length Array length.
 * @return void.
 */
void BBB_I2C::writeByteBuffer(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t *data, uint8_t length) {

    int file;

    if ((file = open(path, O_RDWR)) < 0) {
        printf("%s do not open. Address %d.\n", path, DEV_ADD);
        exit(1);
    }

    if (ioctl(file, I2C_SLAVE, DEV_ADD) < 0) {
        printf("Can not join I2C Bus. Address %d.\n", DEV_ADD);
        exit(1);
    }

    uint8_t buffer[1];
    buffer[0] = DATA_REGADD;

    if (write(file, buffer, 1) != 1) {
        printf("Can not write data. Address %d.\n", DEV_ADD);
        exit(1);
    }

    if (write(file, data, length) != length) {
        printf("Can not write data. Address %d.\n", DEV_ADD);
        exit(1);
    }

    close(file);
}

/**
 * @function writeByteBufferNoExit(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t *data, uint8_t length)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param *data Data storage array.
 * @param length Array length.
 * @return void.
 */
void BBB_I2C::writeByteBufferNoExit(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t *data, uint8_t length) {

    int file;

    if ((file = open(path, O_RDWR)) < 0) {
        printf("%s do not open. Address %d.\n", path, DEV_ADD);
    }

    if (ioctl(file, I2C_SLAVE, DEV_ADD) < 0) {
        printf("Can not join I2C Bus. Address %d.\n", DEV_ADD);
    }

    uint8_t buffer[1];
    buffer[0] = DATA_REGADD;

    if (write(file, buffer, 1) != 1) {
        printf("Can not write data. Address %d.\n", DEV_ADD);
    }

    if (write(file, data, length) != length) {
        printf("Can not write data. Address %d.\n", DEV_ADD);
    }

    close(file);
}

/**
 * @function writeByteArduino(uint8_t DEV_ADD, int8_t data)
 * @param DEV_ADD Arduino Device Address.
 * @param data Writing data.
 * @return void.
 */
void BBB_I2C::writeByteArduino(uint8_t DEV_ADD, int8_t data) {

    int file;

    if ((file = open(path, O_RDWR)) < 0) {
        printf("%s do not open. Address %d.\n", path, DEV_ADD);
        exit(1);
    }

    if (ioctl(file, I2C_SLAVE, DEV_ADD) < 0) {
        printf("Can not join I2C Bus. Address %d.\n", DEV_ADD);
        exit(1);
    }

    int8_t buffer[1];
    buffer[0] = data;

    if (write(file, buffer, 1) != 1) {
        printf("Can not write data. Address %d.\n", DEV_ADD);
        exit(1);
    }

    close(file);

}

/**
 * @function writeByteArduinoNoExit(uint8_t DEV_ADD, int8_t data)
 * @param DEV_ADD Arduino Device Address.
 * @param data Writing data.
 * @return void.
 */
void BBB_I2C::writeByteArduinoNoExit(uint8_t DEV_ADD, int8_t data) {

    int file;

    if ((file = open(path, O_RDWR)) < 0) {
        printf("%s do not open. Address %d.\n", path, DEV_ADD);
    }

    if (ioctl(file, I2C_SLAVE, DEV_ADD) < 0) {
        printf("Can not join I2C Bus. Address %d.\n", DEV_ADD);
    }

    int8_t buffer[1];
    buffer[0] = data;

    if (write(file, buffer, 1) != 1) {
        printf("Can not write data. Address %d.\n", DEV_ADD);
    }

    close(file);

}

/**
 * @function writeByteBufferArduino(uint8_t DEV_ADD, uint8_t *data, uint8_t length)
 * @param DEV_ADD Arduino Device Address.
 * @param *data Data storage array.
 * @param length Array length.
 * @return void.
 */
void BBB_I2C::writeByteBufferArduino(uint8_t DEV_ADD, uint8_t *data, uint8_t length) {

    int file;

    if ((file = open(path, O_RDWR)) < 0) {
        printf("%s do not open. Address %d.\n", path, DEV_ADD);
        exit(1);
    }

    if (ioctl(file, I2C_SLAVE, DEV_ADD) < 0) {
        printf("Can not join I2C Bus. Address %d.\n", DEV_ADD);
        exit(1);
    }
    
    if (write(file, data, length) != length) {
        printf("Can not write data. Address %d.\n", DEV_ADD);
        exit(1);
    }

    close(file);
}

/**
 * @function writeByteBufferArduinoNoExit(uint8_t DEV_ADD, uint8_t *data, uint8_t length)
 * @param DEV_ADD Arduino Device Address.
 * @param *data Data storage array.
 * @param length Array length.
 * @return void.
 */
void BBB_I2C::writeByteBufferArduinoNoExit(uint8_t DEV_ADD, uint8_t *data, uint8_t length) {

    int file;

    if ((file = open(path, O_RDWR)) < 0) {
        printf("%s do not open. Address %d.\n", path, DEV_ADD);
    }

    if (ioctl(file, I2C_SLAVE, DEV_ADD) < 0) {
        printf("Can not join I2C Bus. Address %d.\n", DEV_ADD);
    }

    if (write(file, data, length) != length) {
        printf("Can not write data. Address %d.\n", DEV_ADD);
    }

    close(file);
}

/**
 * @function readBit(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t bitNum)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param bitNum Bit Number for reading.
 * @return uint8_t bit value.
 */

uint8_t BBB_I2C::readBit(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t bitNum) {
    int8_t temp = readByte(DEV_ADD, DATA_REGADD);
    return (temp >> bitNum) % 2;
}

/**
 * @function readBitNoExit(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t bitNum)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param bitNum Bit Number for reading.
 * @return uint8_t bit value.
 */
uint8_t BBB_I2C::readBitNoExit(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t bitNum) {
    int8_t temp = readByteNoExit(DEV_ADD, DATA_REGADD);
    return (temp >> bitNum) % 2;
}

/**
 * @function readBits(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t length, uint8_t startBit)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param length Bits length.
 * @param startBit Starting point of the value.
 * @return uint8_t bit value.
 */
uint8_t BBB_I2C::readBits(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t length, uint8_t startBit) {
    int8_t temp = readByte(DEV_ADD, DATA_REGADD);
    if (length + startBit > 8){
        printf("Bits read overload! length= %02x, startbit = %02x.\n",length, startBit);
        exit(1);
    }
    else if (length == 8) return temp;
    else    return (temp >> startBit) % (uint8_t) pow(2, length);
}

/**
 * @function readBits(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t length, uint8_t startBit)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param length Bits length.
 * @param startBit Starting point of the value.
 * @return uint8_t bit value.
 */
uint8_t BBB_I2C::readBitsNoExit(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t length, uint8_t startBit) {
    int8_t temp = readByteNoExit(DEV_ADD, DATA_REGADD);
    return (temp >> startBit) % (uint8_t) pow(2, length);
}

/**
 * @function readByte(uint8_t DEV_ADD, uint8_t DATA_REGADD)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @return uint8_t bit value.
 */
uint8_t BBB_I2C::readByte(uint8_t DEV_ADD, uint8_t DATA_REGADD) {

    int file;

    if ((file = open(path, O_RDWR)) < 0) {
        printf("%s do not open. Address %d.\n", path, DEV_ADD);
        exit(1);
    }
    
 //  if (ioctl(file, I2C_SLAVE, DEV_ADD) < 0) {
    if (ioctl(file, I2C_SLAVE_FORCE, DEV_ADD) < 0) {        // FG: can't work for E2PROM 24c32L if use I2C_SLAVE mode.
        printf("Can not join I2C Bus. Address %d.\n", DEV_ADD);
        exit(1);
    }

    uint8_t buffer[1];
    buffer[0] = DATA_REGADD;

    if (write(file, buffer, 1) != 1) {
        printf("Can not write data. Address %d.\n", DEV_ADD);
        exit(1);
    }

    uint8_t value[1];

    if (read(file, value, 1) != 1) {
        printf("Can not read data. Address %d.\n", DEV_ADD);
        exit(1);
    }

    close(file);
    return value[0];
}

/**
 * @function readByteNoExit(uint8_t DEV_ADD, uint8_t DATA_REGADD)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @return uint8_t bit value.
 */
uint8_t BBB_I2C::readByteNoExit(uint8_t DEV_ADD, uint8_t DATA_REGADD) {

    int file;

    if ((file = open(path, O_RDWR)) < 0) {
        printf("%s do not open. Address %d.\n", path, DEV_ADD);
    }

    if (ioctl(file, I2C_SLAVE, DEV_ADD) < 0) {
        printf("Can not join I2C Bus. Address %d.\n", DEV_ADD);
    }

    uint8_t buffer[1];
    buffer[0] = DATA_REGADD;

    if (write(file, buffer, 1) != 1) {
        printf("Can not write data. Address %d.\n", DEV_ADD);
    }

    uint8_t value[1];

    if (read(file, value, 1) != 1) {
        printf("Can not read data. Address %d.\n", DEV_ADD);
    }

    close(file);

    return value[0];
}

/**
 * @function readByteBuffer(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t *data, uint8_t length)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param *data Data storage array.
 * @param length Array length.
 * @return void.
 */
void BBB_I2C::readByteBuffer(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t *data, uint8_t length) {

    int file;

    if ((file = open(path, O_RDWR)) < 0) {
        printf("%s do not open. Address %d.\n", path, DEV_ADD);
        exit(1);
    }

    if (ioctl(file, I2C_SLAVE, DEV_ADD) < 0) {
        printf("Can not join I2C Bus. Address %d.\n", DEV_ADD);
        exit(1);
    }

    uint8_t buffer[1];
    buffer[0] = DATA_REGADD;

    if (write(file, buffer, 1) != 1) {
        printf("Can not write data. Address %d.\n", DEV_ADD);
        exit(1);
    }

    if (read(file, data, length) != length) {
        printf("Can not read data. Address %d.\n", DEV_ADD);
        exit(1);
    }

    close(file);

}

/**
 * @function readByteBufferNoExit(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t *data, uint8_t length)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param *data Data storage array.
 * @param length Array length.
 * @return void.
 */
void BBB_I2C::readByteBufferNoExit(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t *data, uint8_t length) {

    int file;

    if ((file = open(path, O_RDWR)) < 0) {
        printf("%s do not open. Address %d.\n", path, DEV_ADD);
    }

    if (ioctl(file, I2C_SLAVE, DEV_ADD) < 0) {
        printf("Can not join I2C Bus. Address %d.\n", DEV_ADD);
    }

    uint8_t buffer[1];
    buffer[0] = DATA_REGADD;

    if (write(file, buffer, 1) != 1) {
        printf("Can not write data. Address %d.\n", DEV_ADD);
    }

    if (read(file, data, length) != length) {
        printf("Can not read data. Address %d.\n", DEV_ADD);
    }

    close(file);

}

/**
 * @function readByteBufferArduino(uint8_t DEV_ADD, uint8_t* data, uint8_t length)
 * @param DEV_ADD Arduino Device Address.
 * @param *data Data storage array.
 * @param length Array length.
 * @return void.
 */
void BBB_I2C::readByteBufferArduino(uint8_t DEV_ADD, uint8_t* data, uint8_t length) {

    int file;

    if ((file = open(path, O_RDWR)) < 0) {
        printf("%s do not open. Address %d.\n", path, DEV_ADD);
        exit(1);
    }

    if (ioctl(file, I2C_SLAVE, DEV_ADD) < 0) {
        printf("Can not join I2C Bus. Address %d.\n", DEV_ADD);
        exit(1);
    }

    if (read(file, data, length) != length) {
        printf("Can not read data. Address %d.\n", DEV_ADD);
        exit(1);
    }

    close(file);

}

/**
 * @function readByteBufferArduino(uint8_t DEV_ADD, uint8_t* data, uint8_t length)
 * @param DEV_ADD Arduino Device Address.
 * @param *data Data storage array.
 * @param length Array length.
 * @return void.
 */
void BBB_I2C::readByteBufferArduinoNoExit(uint8_t DEV_ADD, uint8_t* data, uint8_t length) {

    int file;

    if ((file = open(path, O_RDWR)) < 0) {
        printf("%s do not open. Address %d.\n", path, DEV_ADD);
    }

    if (ioctl(file, I2C_SLAVE, DEV_ADD) < 0) {
        printf("Can not join I2C Bus. Address %d.\n", DEV_ADD);
    }

    if (read(file, data, length) != length) {
        printf("Can not read data. Address %d.\n", DEV_ADD);
    }

    close(file);

}

/**
 * @function readWord(uint8_t DEV_ADD, uint8_t MSB, uint8_t LSB)
 * @param DEV_ADD Arduino Device Address.
 * @param MSB 16-bit values Most Significant Byte Address.
 * @param LSB 16-bit values Less Significant Byte Address..
 * @return void.
 */
int16_t BBB_I2C::readWord(uint8_t DEV_ADD, uint8_t MSB, uint8_t LSB) {

    uint8_t msb = readByte(DEV_ADD, MSB);
    uint8_t lsb = readByte(DEV_ADD, LSB);     
  
    return ((int16_t) msb << 8) +lsb;
}

/**
 * @function readWordNoExit(uint8_t DEV_ADD, uint8_t MSB, uint8_t LSB)
 * @param DEV_ADD Arduino Device Address.
 * @param MSB 16-bit values Most Significant Byte Address.
 * @param LSB 16-bit values Less Significant Byte Address..
 * @return void.
 */
int16_t BBB_I2C::readWordNoExit(uint8_t DEV_ADD, uint8_t MSB, uint8_t LSB) {

    uint8_t msb = readByteNoExit(DEV_ADD, MSB);

    uint8_t lsb = readByteNoExit(DEV_ADD, LSB);

    return ((int16_t) msb << 8) +lsb;
}
