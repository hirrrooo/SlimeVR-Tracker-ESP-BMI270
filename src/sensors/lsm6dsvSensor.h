/*
	SlimeVR Code is placed under the MIT license
	Copyright (c) 2023 Eiren Rain & SlimeVR contributors

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
*/

#ifndef SENSORS_LSM6DSV_H
#define SENSORS_LSM6DSV_H

#include "LSM6DSV.h"
#include "sensor.h"
#include "SensorFusion.h"
#include "magneto1.4.h"
#include "SensorFusionRestDetect.h"

#ifndef LSM6DSV_ACCEL_MAX
#define LSM6DSV_ACCEL_MAX 16
#endif

#ifndef LSM6DSV_GYRO_MAX
#define LSM6DSV_GYRO_MAX 2000
#endif

#ifndef LSM6DSV_FIFO_DATA_RATE
#define LSM6DSV_FIFO_DATA_RATE 120
#endif

#ifndef LSM6DSV_GYRO_ACCEL_RATE
#define LSM6DSV_GYRO_ACCEL_RATE 7680.0f
#endif

//#ifndef LSM6DSV_FIFO_TEMP_DATA_RATE //We should use this instead
//#define LSM6DSV_FIFO_TEMP_DATA_RATE 1.875f
//#endif

#ifndef LSM6DSV_TEMP_READ_INTERVAL
#define LSM6DSV_TEMP_READ_INTERVAL 1
#endif


#ifndef LSM6DSV_TAP_THRESHOLD
#define LSM6DSV_TAP_THRESHOLD 5 //0-32
#endif

#ifndef LSM6DSV_TAP_SHOCK_TIME
#define LSM6DSV_TAP_SHOCK_TIME 3 //0-3
#endif

#ifndef LSM6DSV_TAP_QUITE_TIME
#define LSM6DSV_TAP_QUITE_TIME 3 //0-3
#endif



#define LSM6DSV_INTERRUPT //recommended not required
// #define LSM6DSV_NO_SELF_TEST_ON_FACEDOWN



#define LSM6DSV_FUSION_ESP 0
#define LSM6DSV_FUSION_ONBOARD 1

#define LSM6DSV_FUSION_SOURCE LSM6DSV_FUSION_ONBOARD



#if (LSM6DSV_FUSION_SOURCE == LSM6DSV_FUSION_ESP)
#define LSM6DSV_GYRO_OFFSET_CAL
#define LSM6DSV_ACCEL_OFFSET_CAL
#define LSM6DSV_GYRO_SENSITIVITY_CAL
#endif

#ifdef LSM6DSV_GYRO_SENSITIVITY_CAL
#define LSM6DSV_GYRO_SENSITIVITY_SPINS 1
#endif





class LSM6DSVSensor : public Sensor {
public:
	LSM6DSVSensor(
		uint8_t id,
		uint8_t type,
		uint8_t address,
		float rotation,
		uint8_t sclPin,
		uint8_t sdaPin,
		uint8_t intPin
	);
	~LSM6DSVSensor(){};
	void motionSetup() override final;
	void motionLoop() override final;
	void sendData() override final;
	SensorStatus getSensorState() override final;

#if (LSM6DSV_FUSION_SOURCE == LSM6DSV_FUSION_ESP)
	void startCalibration(int calibrationType) override final;
	void calibrateGyro() override final;
	void calibrateAccel() override final;
	void calibrateGyroSensitivity() override final;
	void printCalibration() override final;
    void resetCalibration() override final;
#endif
	
private:
	Quat fusedRotationToQuaternion(float x, float y, float z);
	LSM6DSVStatusTypeDef runSelfTest();
	LSM6DSVStatusTypeDef readFifo(uint16_t fifo_samples);

	LSM6DSV imu;
	uint8_t m_IntPin;
	uint8_t tap = 0;
	int8_t status = 0;
	unsigned long lastData = 0;
	float temperature = 0;
	bool newTemperature = false;
	uint32_t lastTempRead = 0;
	float rawAcceleration[3];
	bool newRawAcceleration = false;
	uint32_t previousDataTime = 0;
	uint32_t currentDataTime = 0;

#if (LSM6DSV_FUSION_SOURCE == LSM6DSV_FUSION_ESP)
	LSM6DSVStatusTypeDef readNextFifoFrame();
	LSM6DSVStatusTypeDef loadIMUCalibration();
	void saveCalibration();
	SlimeVR::Configuration::LSM6DSVCalibrationConfig m_Calibration = {};
	SlimeVR::Sensors::SensorFusionRestDetect sfusion;
	float rawGyro[3];
	bool newRawGyro = false;
#endif
};

#endif
