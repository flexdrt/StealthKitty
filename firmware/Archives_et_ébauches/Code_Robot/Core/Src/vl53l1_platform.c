/*
 ******************************************************************************
 * This file is part of VL53L1 Platform
 * @attention
 * Author: CHAIR
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "main.h"

#include <string.h>
#include <time.h>
#include <math.h>
#include "z_vl53l1x_test.h"
#include "vl53l1_platform.h"
#include "VL53L1X_api.h"

extern I2C_HandleTypeDef TOF_PORT;


int8_t VL53L1_RdByte(uint16_t dev, uint16_t index, uint8_t *data) {
	if (HAL_I2C_Mem_Read(&TOF_PORT, dev, index, I2C_MEMADD_SIZE_16BIT, data, 1, I2C_COMM_TIMEOUT))
		return VL53L1__IO_ERROR;
	else
		return 0;
}

int8_t VL53L1_RdWord(uint16_t dev, uint16_t index, uint16_t *data) {
	if (HAL_I2C_Mem_Read(&TOF_PORT, dev, index, I2C_MEMADD_SIZE_16BIT, (uint8_t *)data, 2, I2C_COMM_TIMEOUT))
		return VL53L1__IO_ERROR;
	else {
		*data=__REVSH(*data);
		return 0;
	}
}

int8_t VL53L1_RdDWord(uint16_t dev, uint16_t index, uint32_t *data) {
	if (HAL_I2C_Mem_Read(&TOF_PORT, dev, index, I2C_MEMADD_SIZE_16BIT, (uint8_t *)data, 4, I2C_COMM_TIMEOUT))
		return VL53L1__IO_ERROR;
	else {
		*data=__REV(*data);
		return 0;
	}
}

int8_t VL53L1_ReadMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count){
	if (HAL_I2C_Mem_Read(&TOF_PORT, dev, index, I2C_MEMADD_SIZE_16BIT, pdata, count, HAL_MAX_DELAY))
		return VL53L1__IO_ERROR;
	else
		return 0;
}

int8_t VL53L1_WrByte(uint16_t dev, uint16_t index, uint8_t data) {
	if (HAL_I2C_Mem_Write(&TOF_PORT, dev, index, I2C_MEMADD_SIZE_16BIT, &data, 1, I2C_COMM_TIMEOUT))
		return VL53L1__IO_ERROR;
	else
		return 0;
}

int8_t VL53L1_WrWord(uint16_t dev, uint16_t index, uint16_t data) {
	data=__REVSH(data);
	if (HAL_I2C_Mem_Write(&TOF_PORT, dev, index, I2C_MEMADD_SIZE_16BIT, (uint8_t *)&data, 2, I2C_COMM_TIMEOUT))
		return VL53L1__IO_ERROR;
	else
		return 0;
}

int8_t VL53L1_WrDWord(uint16_t dev, uint16_t index, uint32_t data) {
	data=__REV(data);
	if (HAL_I2C_Mem_Write(&TOF_PORT, dev, index, I2C_MEMADD_SIZE_16BIT, (uint8_t *)&data, 4, I2C_COMM_TIMEOUT))
		return VL53L1__IO_ERROR;
	else
		return 0;
}

int8_t VL53L1_WriteMulti( uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count) {
	if (HAL_I2C_Mem_Write(&TOF_PORT, dev, index, I2C_MEMADD_SIZE_16BIT, pdata, count, I2C_COMM_TIMEOUT))
		return VL53L1__IO_ERROR;
	else
		return 0;
}

/*
int8_t VL53L1_WaitMs(uint16_t dev, int32_t wait_ms){
	return (0/(1-1)); // to be implemented
}
 */


/****************************************************
 * @brief 		set the VL53L1 Xshutdown pin
 * 				waiting for a complete boot sequence
 * @param 	1	VL53L1 enabled, wait for boot
 * 			0	VL53L1 disabled
 ****************************************************/
/*uint8_t VL53L1__Xshut(uint8_t level){
	uint8_t sensorStatus =0;
	uint8_t status =0;
	HAL_GPIO_WritePin(TOF_XSHUT_GPIO_Port, TOF_XSHUT_Pin, level);
	if (level) {
		while (!(sensorStatus & 0x01)) {
			HAL_Delay(2);
			status |= VL53L1X_BootState(TOF_ADDR, &sensorStatus);
		}
	}
	return (status);
}*/




/************************************************************
 * @brief:		Test VL53L1 presence,
 * 				initialize sensor,
 * 				set it the default Distance Mode, Timing Budget and Intermeasurement
 * @return:	0	if no errors detected in setting up VL53L1X
 ************************************************************/
uint8_t VL53L1__Init(){
	uint8_t refRegs[4] = {0,0,0,0};
	uint8_t status =0;
	//printf("Verification du capteur via I2C...\n");
	// Enable VL53L1 sensor waiting for a complete boot sequence
	/*status |= VL53L1__Xshut(1);
	if (status)
		return (status);

	HAL_Delay(4);*/
	status = VL53L1_ReadMulti(TOF_ADDR, VL53L1__MODELID_INDEX, refRegs, 4);
	if (status != 0) {
		printf("Erreur I2C lors de la lecture des registres : %d\n", status);
		return 1;
	}

	printf("Model ID : 0x%02X, Module Type : 0x%02X, Mask Revision : 0x%02X\n", refRegs[0], refRegs[1], refRegs[2]);


	//check if VL53L1X is alive and kicking. Remove MASKREV if VL53L1

	if ((refRegs[0]!=VL53L1__MODELID_VALUE) || (refRegs[1]!=VL53L1__MODULETYPE_VALUE) || (refRegs[2]!=VL53L1__MASKREV_VALUE))
	{
		printf("Valeurs incorrectes des registres. Vérifiez le câblage.\n");
		return (1);
	}

	printf("Capteur détecté avec succès ! \n");

	// Initialisation du capteur
	printf("Initialisation du capteur VL53L1X...\n");
	status |= VL53L1X_SensorInit(TOF_ADDR);
	if (status != 0) {
		printf("Échec de l'initialisation du capteur VL53L1X : %d\n", status);
		HAL_Delay(50);
		return 1;
	}

	// Calibration des offsets et xtalk
	printf("Calibration des offsets et xtalk...\n");
	status |= VL53L1X_SetOffset(TOF_ADDR, VL53L1__CALIB_OFFSET);
	if (status != 0) {
			printf("Erreur de calibration OFFSET : %d\n", status);
			HAL_Delay(50);
			return 1;
		}
	status |= VL53L1X_SetXtalk(TOF_ADDR, VL53L1__CALIB_XTALK);
	if (status != 0) {
			printf("Erreur de calibration XTALK : %d\n", status);
			HAL_Delay(50);
			return 1;
		}


	// Configuration des paramètres de mesure
	printf("Configuration des paramètres de mesure...\n");
	// VL53L1X sensor is available
	/* inizializing: default setting  */
	status |= VL53L1X_SensorInit(TOF_ADDR);
	/* initializing: device settings */
	status |= VL53L1X_SetOffset(TOF_ADDR, VL53L1__CALIB_OFFSET);
	/* initializing: project settings */
	status |= VL53L1X_SetDistanceMode(TOF_ADDR, VL53L1__DISTANCE_MODE);
	status |= VL53L1X_SetTimingBudgetInMs(TOF_ADDR, VL53L1__TIMING_BUDGET);
	status |= VL53L1X_SetInterMeasurementInMs(TOF_ADDR, VL53L1__INTERMEASUREMENT);

	if (status != 0) {
		printf("Erreur de configuration : %d\n", status);
		return 1;
	}

	printf("Capteur initialisé avec succès.\n");
	return status;
};




/*****************************************
 * @brief 			hangs, waiting for availability
 * 					of a new distance reading having a
 * 					"level" RangeStatus
 * 					in any case restart readings (clears interrupt)
 * @param	level	minimum value that RangeStatus must have
 * 					to consider wreading accepted
 * @return			0 		if errors or not accepptable reading
 * 					value	the read Distance, if not 0
 *****************************************/
uint16_t VL53L1__GetDistance(uint8_t level){
	uint16_t Distance;							// data read from VL53L1
	uint8_t RangeStatus=0;						// data read from VL53L1
	uint8_t dataReady=0;
	uint8_t status =1;


	while (status==1) {
		status=0;
		while (dataReady == 0)
			status |= VL53L1X_CheckForDataReady(TOF_ADDR, &dataReady);
		status |= VL53L1X_GetRangeStatus(TOF_ADDR, &RangeStatus);
		if (RangeStatus<=level) // no errors or just warnings
			status |= VL53L1X_GetDistance(TOF_ADDR, &Distance);
		status |= VL53L1X_ClearInterrupt(TOF_ADDR);
	}
	if (status)
		Distance=0;
	return Distance;
}
