/*
 * ICM20948_USER.cpp
 *
 *  Created on: Feb 10, 2023
 *      Author: OHYA Satoshi
 */

#include "ICM20948_USER.h"

void ICM20948_USER::init(){
	icm20948.reset();
	icm20948.pwrmgmt2(icm20948.DISABLE_SENSORS);
	icm20948.accelConfig(ICM20948::AccelSensitivity::SENS_2G,false,0);
	icm20948.gyroConfig(ICM20948::GyroSensitivity::SENS_500, false, 0);
	icm20948.pwrmgmt2(icm20948.ENABLE_SENSORS);
	icm20948.intPinConfig(0b01110000);
	icm20948.intenable();
}

