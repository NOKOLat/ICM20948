/*
 * ICM20948.cpp
 *
 *  Created on: Oct 3, 2022
 *      Author: zeroi
 */

#include "ICM20948.h"

uint8_t ICM20948::whoami(){
	uint8_t adress=0;
	memRead(REGISTER.WHO_AM_I, &adress);
	return adress;
}

void ICM20948::pwrmgmt1(uint8_t data){
	 memWrite(REGISTER.PWR_MGMT_1, &data);
}

void ICM20948::pwrmgmt2(uint8_t data){
	 memWrite(REGISTER.PWR_MGMT_2, &data);
}

void ICM20948::reset(){
	uint8_t n=0b1<<7;
	memWrite(REGISTER.PWR_MGMT_1, &n);
	HAL_Delay(10);
}

bool ICM20948::changeUserBank(const uint8_t bank){
	if(bank>3 || bank<0){
		return false;
	}
	uint8_t data=bank<<4;
	memWrite(REGISTER.BANK_SEL, &data);
	return true;
}

bool ICM20948::accelConfig(const AccelSensitivity fssel,const bool enableDLPF,const uint8_t configDLPF){

	_accelsensitivity=fssel;
	if(configDLPF>7 || configDLPF<0){
		return false;
	}
	uint8_t data=0;
	data |=configDLPF<<3;
	data |=(uint8_t)fssel<<1;
	data |=enableDLPF;
	changeUserBank(2);

	memWrite(REGISTER.ACCEL_CONFIG, &data);
	changeUserBank(0);
	return true;

}

bool ICM20948::gyroConfig(const GyroSensitivity fssel,const bool enableDLPF,const uint8_t configDLPF){

	_gyrosensitivity=fssel;
	if(configDLPF>7||configDLPF<0){
		return false;
	}
	uint8_t data=0;
	data |=configDLPF<<3;
	data |=(uint8_t)fssel<<1;
	data |=enableDLPF;
	changeUserBank(2);

	memWrite(REGISTER.GYRO_CONFIG, &data);
	changeUserBank(0);
	return true;

}

float ICM20948::getAccel(uint8_t axis){
	int8_t buf[2];
	memRead(REGISTER.REG_ACCEL_H[axis],(uint8_t*)buf, 2);

	int16_t accel=(int16_t)buf[0]<<8 | (int16_t)buf[1];

	return (float)accel/ACCEL_SENSITIVITY[(uint8_t)_accelsensitivity];
}

float ICM20948::getGyro(uint8_t axis){
	int8_t buf[2];
	memRead(REGISTER.REG_GYRO_H[axis],(uint8_t*)buf, 2);

	int16_t gyro=(int16_t)buf[0]<<8 | (int16_t)buf[1];

	return (float)gyro/GYRO_SENSITIVITY[(uint8_t)_gyrosensitivity];
}

void ICM20948::getAccelBurst(std::array<float,3> &value){
	const uint8_t headRegAddr = REGISTER.REG_ACCEL_H[0];
	uint8_t buffer[6]={};

	memRead(headRegAddr, (uint8_t*)buffer,6);

	for(uint8_t n=0;n<3;n++){
		value[n] = (float)((int16_t)buffer[2*n]<<8 | (int16_t)buffer[2*n+1])/ACCEL_SENSITIVITY[(uint8_t)_accelsensitivity];
	}
}

void ICM20948::getGyroBurst(std::array<float,3> &value){
	const uint8_t headRegAddr = REGISTER.REG_GYRO_H[0];
	uint8_t buffer[6]={};

	memRead(headRegAddr, (uint8_t*)buffer, 6);

	for(uint8_t n=0;n<3;n++){
		value[n] = (float)((int16_t)buffer[2*n]<<8 | (int16_t)buffer[2*n+1])/GYRO_SENSITIVITY[(uint8_t)_gyrosensitivity];
	}
}

void ICM20948::get6ValueBurst(std::array<float,3> &accel, std::array<float,3> &gyro){
	const uint8_t headRegAddr = REGISTER.REG_ACCEL_H[0];
	uint8_t buffer[12]={};

	memRead(headRegAddr, buffer, 12);

	for(uint8_t n=0;n<3;n++){
		int16_t tmp = (int16_t)buffer[2*n]<<8 | (int16_t)buffer[2*n+1];
		accel[n] = (float)(tmp)/ACCEL_SENSITIVITY[(uint8_t)_accelsensitivity];
	}
	for(uint8_t n=0;n<3;n++){
		int16_t tmp = (int16_t)buffer[2*n+6]<<8 | (int16_t)buffer[2*n+1+6];
		gyro[n] = (float)(tmp)/GYRO_SENSITIVITY[(uint8_t)_gyrosensitivity];
	}
}

void ICM20948::intPinConfig(uint8_t value){
	memWrite(REGISTER.INT_PIN_CFG, &value);
}

void ICM20948::intenable(uint8_t value){
	memWrite(REGISTER.INT_ENABLE, &value);
}
