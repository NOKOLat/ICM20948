/*
 * ICM20948_USER.h
 *
 *  Created on: Feb 10, 2023
 *      Author: OHYA Satoshi
 */

#ifndef ICM20948_ICM20948_USER_H_
#define ICM20948_ICM20948_USER_H_

#include "ICM20948.h"
#include "ICM20948_HAL.h"


template<class T = ICM20948>
class ICM20948_USER{
public:
	ICM20948_USER(T *icm20948):icm20948(icm20948){}

	void init(){
		icm20948->reset();
		icm20948->accelConfig(ICM20948::AccelSensitivity::SENS_2G,true,7);
		icm20948->gyroConfig(ICM20948::GyroSensitivity::SENS_500, true, 7);

		uint8_t tmp=3;
		icm20948->memWrite(ICM20948::REGISTER::BANK2::GYRO_SMPLRT_DIV, tmp);
		tmp=4;
		icm20948->memWrite(ICM20948::REGISTER::BANK2::GYRO_CONFIG_2, tmp);
		tmp=0;
		icm20948->memWrite(ICM20948::REGISTER::BANK2::ACCEL_CONFIG_2, tmp);
		tmp=3;
		icm20948->memWrite(ICM20948::REGISTER::BANK2::ACCEL_SMPLRT_DIV_2, tmp);
		icm20948->changeUserBank(ICM20948::REGISTER::BANK::BANK0);

		icm20948->pwrmgmt1(0x01);
		icm20948->intPinConfig(0b00010000);
		icm20948->intenable();
		icm20948->pwrmgmt2(0b0);
	}

	void update(){};

private:
	T *icm20948;
};

#endif /* ICM20948_ICM20948_USER_H_ */
