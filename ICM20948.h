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

	struct REGISTER{
		enum class BANK0: uint8_t{
			WHO_AM_I = 0,
			USER_CTRL = 3,
			LP_CONFIG = 5,
			PWR_MGMT_1,
			PWR_MGMT_2,
			INT_PIN_CFG = 15,
			INT_ENABLE,
			INT_ENABLE_1,
			INT_ENABLE_2,
			INT_ENABLE_3,
			INT_STATUS = 25,
			INT_STATUS_2,
			INT_STATUS_3,
			DELAY_TIMEH = 40,
			DELAY_TIMEL,
			ACCEL_XOUT_H = 45,
			ACCEL_XOUT_L,
			ACCEL_YOUT_H,
			ACCEL_YOUT_L,
			ACCEL_ZOUT_H,
			ACCEL_ZOUT_L,
			GYRO_XOUT_H = 51,
			GYRO_XOUT_L,
			GYRO_YOUT_H,
			GYRO_YOUT_L,
			GYRO_ZOUT_H,
			GYRO_ZOUT_L,
			TEMP_OUT_H = 57,
			TEMP_OUT_L,
			DATA_RDY_STATUS = 116,
			REG_BANK_SEL = 127,
		};
		
		enum class BANK1: uint8_t{
			SELF_TEST_X_GYRO = 2,
			SELF_TEST_Y_GYRO,
			SELF_TEST_Z_GYRO,
			SELF_TEST_X_ACCEL = 14,
			SELF_TEST_Y_ACCEL,
			SELF_TEST_Z_ACCEL,
			XA_OFFS_H = 20,
			XA_OFFS_L,
			YA_OFFS_H,
			YA_OFFS_L,
			ZA_OFFS_H,
			ZA_OFFS_L,
			TIMEBASE_CORRECTION_PLL = 40,
			REG_BANK_SEL = 127,
		};
		
		enum class BANK2:uint8_t{
			GYRO_SMPLRT_DIV,
			GYRO_CONFIG_1,
			GYRO_CONFIG_2,
			XG_OFFS_USRH,
			XG_OFFS_USRL,
			YG_OFFS_USRH,
			YG_OFFS_USRl,
			ZG_OFFS_USRH,
			ZG_OFFS_USRl,
			ODR_AILGN_EN,
			ACCEL_SMPLRT_DIV_1 = 16,
			ACCEL_SMPLRT_DIV_2,
			ACCEL_INT_CTRL,
			ACCEL_WOM_THR,
			ACCEL_CONFIG,
			ACCEL_CONFIG_2,
			FSYNC_CONFIG = 82,
			TEMP_CONFIG,
			MOD_CTRL_USR,
			REG_BANK_SEL = 127,
		};
		
		enum class BANK{
			BANK0,
			BANK1,
			BANK2,
			BANK3,
		};
		
	    REGISTER():bank(BANK::BANK0),address(0){}
	    REGISTER(BANK0 arg):bank(BANK::BANK0),address((uint8_t)arg){}
	    REGISTER(BANK1 arg):bank(BANK::BANK1),address((uint8_t)arg){}
	    REGISTER(BANK2 arg):bank(BANK::BANK2),address((uint8_t)arg){}

	    BANK bank;
	    uint8_t address;
	};

	enum class Address:uint8_t{
		LOW=0x68,
		HIGH=0x69,
	};

	enum class AXSIS:uint8_t{
		X,
		Y,
		Z,
	};

	ICM20948(Address address)
			:address(address){};


	uint8_t whoami();

	//read IMU output registers and store into raw values array
	void readAccel();
	void readGyro();
	void readIMU();

	//return the raw values array
	std::array<int16_t, 3> getRawAccel(){return rawAccel;}
	std::array<int16_t, 3> getRawGyro(){return rawGyro;}

	//calculate acceles and angular or one of them velosity from raw values array and return the result.
	float getAccel(AXSIS axsis);
	float getGyro(AXSIS axsis);
	void getAccel(std::array<float,3> &value);
	void getGyro(std::array<float,3> &value);
	void getIMU(std::array<float,3> &accel, std::array<float,3> &gyro);

	void pwrmgmt1(uint8_t data);
	void pwrmgmt2(uint8_t data);
	void reset();
	bool changeUserBank(REGISTER::BANK bank);
	bool accelConfig(const AccelSensitivity fssel,const bool enableDLPF,const uint8_t configDLPF);
	bool gyroConfig(const GyroSensitivity fssel,const bool enableDLPF,const uint8_t configDLPF);
	void intPinConfig(uint8_t value);
	void intenable1(uint8_t value=1);

	const uint8_t DISABLE_SENSORS=0x3F;
	const uint8_t ENABLE_SENSORS=0x00;
	const uint8_t BIT_H_RESET=0x80;
	const uint8_t BTT_CLK_PLL=0x01;
	const uint8_t BIT_INT_ACTL=0x80;
	const uint8_t BIT_INT_OPEN=0x40;

	void memWrite(REGISTER reg, uint8_t *pData, uint8_t length = 1);
	void memWrite(REGISTER reg, uint8_t data){memWrite(reg,&data);}
	void memRead(REGISTER reg, uint8_t *pData, uint8_t length = 1);

	const float ACCEL_SENSITIVITY[4]={16384,8192,4096,2048};

	//coefficient for conversion from raw value to radian per sec.
	const float GYRO_SENSITIVITY[4]={7509.643229221,3754.82161461,1877.410807305,938.705403653};
//	const float GYRO_SENSITIVITY[4]={131.068,65.534,32.767,16.3835};

protected:

	const Address address;
	REGISTER::BANK currentBank;
	AccelSensitivity _accelsensitivity;
	GyroSensitivity _gyrosensitivity;
private:
	virtual void __memWrite(uint8_t memAddress, uint8_t *pData, uint8_t length=1)=0;
	virtual void __memRead(uint8_t memAddress, uint8_t *pData, uint8_t length=1)=0;

	std::array<int16_t,3> rawAccel;
	std::array<int16_t,3> rawGyro;
	std::array<float,3> accel;
	std::array<float,3> gyro;

	bool requireCalcAccel = false;
	bool requireCalcGyro = false;

	float calculateAccel(const int16_t raw);
	float calculateGyro(const int16_t raw);
};

#endif /* INC_ICM20948_H_ */
