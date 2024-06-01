/*
 * ICM20948.h
 *
 *  Created on: Oct 3, 2022
 *      Author: zeroi
 */

#ifndef INC_ICM20948_H_
#define INC_ICM20948_H_

#include"i2c.h"
#include <array>

class ICM20948 {
public:

	enum class AccelSensitivity: uint8_t{
		SENS_2G,
		SENS_4G,
		SENS_8G,
		SENS_16G,
	};

	enum class GyroSensitivity: uint8_t{
		SENS_250,
		SENS_500,
		SENS_1000,
		SENS_2000,
	};

	const struct{
		uint8_t WHO_AM_I = 0x00;
		uint8_t PWR_MGMT_1 = 0x06;
		uint8_t PWR_MGMT_2 = 0x07;
		uint8_t INT_PIN_CFG = 0x0F;
		uint8_t BANK_SEL = 0x7F;
		uint8_t ACCEL_SAMPLE_DIV = 0x10;
		uint8_t ACCEL_SAMPLE_DIV_2 = 0x11;
		uint8_t ACCEL_CONFIG = 0x14;
		uint8_t ACCEL_CONFIG_2 = 0x15;
		uint8_t GYRO_SAMPLE_DIV = 0x0;
		uint8_t GYRO_CONFIG = 0x01;
		uint8_t GYRO_CONFIG_2 = 0x02;
		uint8_t INT_ENABLE = 0x11;
		uint8_t REG_ACCEL_H[3]={0x2D,0x2F,0x31};
		uint8_t REG_ACCEL_L[3]={0x2E,0x30,0x32};
		uint8_t REG_GYRO_H[3]={0x33,0x35,0x37};
		uint8_t REG_GYRO_L[3]={0x34,0x36,0x38};
	}REGISTER;

	enum class Address:uint8_t{
		LOW=0x68,
		HIGH=0x69,
	};
	ICM20948(Address address)
			:address(address){};


	uint8_t whoami();
	float getAccel(uint8_t axis);
	float getGyro(uint8_t axis);
	void getAccelBurst(std::array<float,3> &value);
	void getGyroBurst(std::array<float,3> &value);
	void get6ValueBurst(std::array<float,3> &accel, std::array<float,3> &gyro);

	void pwrmgmt1(uint8_t data);
	void pwrmgmt2(uint8_t data);
	void reset();
	bool changeUserBank(const uint8_t bank);
	bool accelConfig(const AccelSensitivity fssel,const bool enableDLPF,const uint8_t configDLPF);
	bool gyroConfig(const GyroSensitivity fssel,const bool enableDLPF,const uint8_t configDLPF);
	void intPinConfig(uint8_t value);
	void intenable(uint8_t value=1);

	virtual void memWrite(uint8_t memAddress, uint8_t *pData, uint8_t length=1){}
	virtual void memRead(uint8_t memAddress, uint8_t *pData, uint8_t length=1){}

	const uint8_t DISABLE_SENSORS=0x3F;
	const uint8_t ENABLE_SENSORS=0x00;
	const uint8_t BIT_H_RESET=0x80;
	const uint8_t BTT_CLK_PLL=0x01;
	const uint8_t BIT_INT_ACTL=0x80;
	const uint8_t BIT_INT_OPEN=0x40;
protected:

	const Address address;
	AccelSensitivity _accelsensitivity;
	GyroSensitivity _gyrosensitivity;
	const float ACCEL_SENSITIVITY[4]={16384.0,8192.0,4096.0,2048.0};
	const float GYRO_SENSITIVITY[4]={7509.6,3754.8,1877.4,938.71};
//	const float GYRO_SENSITIVITY[4]={131,65.5,32.8,16.4};

private:
};

#endif /* INC_ICM20948_H_ */
