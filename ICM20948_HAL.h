/*
 * ICM20948HAL.h
 *
 *  Created on: Feb 10, 2023
 *      Author: satoshi
 */

#ifndef ICM20948_ICM20948_HAL_H_
#define ICM20948_ICM20948_HAL_H_

#ifdef USE_HAL_DRIVER

#include "ICM20948.h"

class ICM20948_HAL: public ICM20948 {
public:
	ICM20948_HAL(I2C_HandleTypeDef *hi2c,Address address)
	:ICM20948(address),hi2c(hi2c){}

	void memWrite(uint8_t memAddress, uint8_t *pData, uint8_t length=1) override;
	void memRead(uint8_t memAddress, uint8_t *pData, uint8_t length=1) override;
private:
	 I2C_HandleTypeDef *hi2c;

};

#endif /* USE_HAL_DRIVER */

#endif /* ICM20948_ICM20948_HAL_H_ */
