/*
 * ICM20948HAL.cpp
 *
 *  Created on: Feb 10, 2023
 *      Author: satoshi
 */

#include "ICM20948_HAL.h"

void ICM20948_HAL::memWrite(uint8_t memAddress, uint8_t *pData, uint8_t length){
	HAL_I2C_Mem_Write(hi2c, (uint16_t)address<<1, memAddress, 1, pData, length, 1000);
}

void ICM20948_HAL::memRead(uint8_t memAddress, uint8_t *pData, uint8_t length){
	HAL_I2C_Mem_Read(hi2c, (uint16_t)address<<1, memAddress, 1, pData, length, 1000);
}
