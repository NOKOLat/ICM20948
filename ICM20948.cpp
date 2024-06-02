/*
 * ICM20948.cpp
 *
 *  Created on: Oct 3, 2022
 *      Author: zeroi
 */

#include "ICM20948.h"

uint8_t ICM20948::whoami(){
	uint8_t adress=0;
	memRead(REGISTER::BANK0::WHO_AM_I, &adress);
	return adress;
}

void ICM20948::pwrmgmt1(uint8_t data){
	 memWrite(REGISTER::BANK0::PWR_MGMT_1, &data);
}

void ICM20948::pwrmgmt2(uint8_t data){
	memWrite(REGISTER::BANK0::PWR_MGMT_2, &data);
}

void ICM20948::reset(){
	uint8_t n=0b1<<7;
	memWrite(REGISTER::BANK0::PWR_MGMT_1, &n);
	HAL_Delay(10);
}

bool ICM20948::changeUserBank(REGISTER::BANK bank){
	uint8_t data=(uint8_t)bank<<4;
	memWrite(REGISTER::BANK0::REG_BANK_SEL, &data);
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

	memWrite(REGISTER::BANK2::ACCEL_CONFIG, &data);

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

	memWrite(REGISTER::BANK2::GYRO_CONFIG_1, &data);
	return true;

}

void ICM20948::readAccel(){
	memRead(REGISTER::BANK0::ACCEL_XOUT_H, (uint8_t*)rawAccel.data(),6);
	requireCalcAccel = true;
}

void ICM20948::readGyro(){
	memRead(REGISTER::BANK0::GYRO_XOUT_H, (uint8_t*)rawGyro.data(),6);
	requireCalcGyro = true;
}

void ICM20948::readIMU(){
	int16_t buf[6]={};
	memRead(REGISTER::BANK0::ACCEL_XOUT_H, (uint8_t*)buf,12);
	for(uint8_t n=0; n<3; n++){
		rawAccel[n]=((int16_t)buf[2*n]<<8 | (int16_t)buf[2*n+1]);
		rawGyro[n] = (int16_t)buf[2*n+6]<<8 | (int16_t)buf[2*n+1+6];
	}
	requireCalcAccel = true;
	requireCalcGyro = true;
}

float ICM20948::getAccel(AXSIS axsis){
	if(requireCalcAccel){
		for(uint8_t n=0; n<3; n++){
			accel[n] = calculateAccel(rawAccel[n]);
		}
		requireCalcAccel = false;
	}

	return accel[(uint8_t)axsis];
}

float ICM20948::getGyro(AXSIS axsis){
	if(requireCalcGyro){
		for(uint8_t n=0; n<3; n++){
			gyro[n] = calculateGyro(rawGyro[n]);
		}
		requireCalcGyro = false;
	}

	return gyro[(uint8_t)axsis];
}

void ICM20948::getAccel(std::array<float,3> &value){
	if(requireCalcAccel){
		for(uint8_t n=0; n<3; n++){
			accel[n] = calculateAccel(rawAccel[n]);
		}
		requireCalcAccel = false;
	}

	value = accel;
}

void ICM20948::getGyro(std::array<float,3> &value){
	if(requireCalcGyro){
		for(uint8_t n=0; n<3; n++){
			gyro[n] = calculateGyro(rawGyro[n]);
		}
		requireCalcGyro = false;
	}
	value = gyro;
}

void ICM20948::getIMU(std::array<float,3> &accel, std::array<float,3> &gyro){
	getAccel(accel);
	getGyro(gyro);
}

void ICM20948::intPinConfig(uint8_t value){
	memWrite(REGISTER::BANK0::INT_PIN_CFG, &value);
}

void ICM20948::intenable(uint8_t value){
	memWrite(REGISTER::BANK0::INT_ENABLE, &value);
}

void ICM20948::memWrite(REGISTER reg, uint8_t *pData, uint8_t length){
	if(this->currentBank != reg.bank){
		changeUserBank(reg.bank);
	}
	memWrite(reg.address, pData, length);
}
void ICM20948::memRead(REGISTER reg, uint8_t *pData, uint8_t length){
	if(this->currentBank != reg.bank){
		changeUserBank(reg.bank);
	}
	memRead(reg.address, pData, length);
}

float ICM20948::calculateAccel(const int16_t raw){
	return raw / ACCEL_SENSITIVITY[(uint8_t)_accelsensitivity];
}

float ICM20948::calculateGyro(const int16_t raw){
	return raw / GYRO_SENSITIVITY[(uint8_t)_gyrosensitivity];
}
