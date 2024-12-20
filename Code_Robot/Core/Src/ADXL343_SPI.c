#include "ADXL343_SPI.h"

#include "spi.h"


#define ADXL_CS_LOW()   HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET)
#define ADXL_CS_HIGH()  HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET)

uint8_t ADXL343_initialization(SPI_HandleTypeDef *hspi, ADXL343 *dev)
{
	dev->hspiHandle = hspi;
	uint8_t regAddr = ADXL343_REG_DEVICE_ID | 0x80;
	uint8_t regData;
	uint8_t errNum = 0;
	HAL_StatusTypeDef status;

	// Debug message
	printf("Starting ADXL343 initialization...\r\n");

	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
	status = HAL_SPI_Transmit(dev->hspiHandle, &regAddr, 1, HAL_MAX_DELAY);
	status = HAL_SPI_Receive(dev->hspiHandle, &regData, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);


	errNum += (status != HAL_OK);


	if (regData != ADXL343_DEVICE_ID)
	{
		return 255; //
	}

	// Set the register ADXL343_REG_POWER_CTL to enable measurement mode 0b00001000 = 0x08 p.26
	regAddr = ADXL343_REG_POWER_CTL; // No |0x80 means, WRITE 1 byte
	regData = 0x08;
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
	status = HAL_SPI_Transmit(dev->hspiHandle, &regAddr, 1, HAL_MAX_DELAY);
	status = HAL_SPI_Transmit(dev->hspiHandle, &regData, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);

	regAddr = ADXL343_REG_POWER_CTL | 0x80;
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
	status = HAL_SPI_Transmit(dev->hspiHandle, &regAddr, 1, HAL_MAX_DELAY);
	status = HAL_SPI_Receive(dev->hspiHandle, &regData, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);

	errNum += (status != HAL_OK);
	uint8_t txData = 0x80;
	uint8_t rxData;
	if (rxData == 0xE5) {
			printf("SPI fonctionne : Device ID = 0x%02X\r\n", rxData);
		} else {
			printf("Erreur SPI : Device ID = 0x%02X\r\n", rxData);}

		uint8_t tapThreshold = ADXL343_read_register(&hspi1, ADXL343_REG_THRESH_TAP);
		printf("Valeur du registre THRESH_TAP: 0x%02X\r\n", tapThreshold);

		uint8_t powerCtl = ADXL343_read_register(&hspi1, ADXL343_REG_POWER_CTL);
		printf("Valeur du registre POWER_CTL: 0x%02X\r\n", powerCtl);

		uint8_t intmap = ADXL343_read_register(&hspi1, ADXL343_REG_INT_MAP);
		printf("Valeur du registre INT_MAP: 0x%02X\r\n", intmap);


		uint8_t bwrate = ADXL343_read_register(&hspi1,  ADXL343_REG_BW_RATE);
		printf("Valeur du registre RATE: 0x%02X\r\n", bwrate);


		uint8_t enable = ADXL343_read_register(&hspi1,  ADXL343_REG_INT_ENABLE);
		printf("Valeur du registre enable: 0x%02X\r\n", enable);

	return 0;
}
uint8_t ADXL343_read_register(SPI_HandleTypeDef *hspi, uint8_t regAddr)
{
	uint8_t regData;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);  // Mettre NSS à 0 pour commencer la communication SPI
	regAddr |= 0x80;
	HAL_SPI_Transmit(hspi, &regAddr, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(hspi, &regData, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);    // Mettre NSS à 1 pour terminer la communication SPI
	return regData;
}


HAL_StatusTypeDef ADXL343_get_acc_raw(ADXL343 *dev) {
	uint8_t data[6]; // Pour stocker les données X, Y, et Z
	uint8_t regAddr;

	// Lire les données des axes
	for (int i = 0; i < 3; i++) {
		regAddr = ADXL343_REG_DATAX0 + (i * 2); // Calculer l'adresse des registres
		HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
		HAL_SPI_Transmit(dev->hspiHandle, &regAddr, 1, HAL_MAX_DELAY);
		HAL_SPI_Receive(dev->hspiHandle, &data[i * 2], 2, HAL_MAX_DELAY);
		HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);
	}

	// Convertir les données en entiers 16 bits (X, Y, Z)
	dev->acc_rawX = (int16_t)((data[1] << 8) | data[0]);
	dev->acc_rawY = (int16_t)((data[3] << 8) | data[2]);
	dev->acc_rawZ = (int16_t)((data[5] << 8) | data[4]);

	// Vérification des valeurs lues
	printf("Acceleration Raw Data - X: %d, Y: %d, Z: %d\n",
			dev->acc_rawX, dev->acc_rawY, dev->acc_rawZ);

	return HAL_OK;
}


HAL_StatusTypeDef ADXL343_get_acc_norm(ADXL343 *dev) {

	float scaleFactor = 0.004;
	dev->acc_msp2X = dev->acc_rawX * scaleFactor;
	dev->acc_msp2Y = dev->acc_rawY * scaleFactor;
	dev->acc_msp2Z = dev->acc_rawZ * scaleFactor;

	return HAL_OK;
}

void ADXL343_print_raw(ADXL343 *dev, UART_HandleTypeDef *huart) {
	printf("Acceleration Raw Data - X: %d, Y: %d, Z: %d\n",
			dev->acc_rawX, dev->acc_rawY, dev->acc_rawZ);
}

void ADXL343_set_tap_parameters(ADXL343 *dev, uint8_t threshold, uint8_t duration, uint8_t latency, uint8_t window) {
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(dev->hspiHandle, &ADXL343_REG_THRESH_TAP, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(dev->hspiHandle, &threshold, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);

	// Configurer la durée du tapotement
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(dev->hspiHandle, &ADXL343_REG_DUR, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(dev->hspiHandle, &duration, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);

	// Configurer la latence pour double tapotement
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(dev->hspiHandle, &ADXL343_REG_LATENT, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(dev->hspiHandle, &latency, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);

	// Configurer la fenêtre pour double tapotement
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(dev->hspiHandle, &ADXL343_REG_WINDOW, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(dev->hspiHandle, &window, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);
}


void ADXL343_EnableTapInterrupts(ADXL343 *dev) {

	// Configurer l'interruption de mappage INT1
	uint8_t int_map = 0x40; // Mappage des interruptions à INT1
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(dev->hspiHandle, &ADXL343_REG_INT_MAP, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(dev->hspiHandle, &int_map, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);

	uint8_t int_axes = 0x05;

	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(dev->hspiHandle, &ADXL343_REG_TAP_AXES , 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(dev->hspiHandle, &int_axes, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);

	uint8_t int_enable = 0x40; // Activer SINGLE_TAP et DOUBLE_TAP (bits 6 et 5)

	// Activer les interruptions
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(dev->hspiHandle, &ADXL343_REG_INT_ENABLE, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(dev->hspiHandle, &int_enable, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);

	uint8_t int_enable_read = ADXL343_read_register(dev->hspiHandle, ADXL343_REG_INT_ENABLE);
	uint8_t int_map_read = ADXL343_read_register(dev->hspiHandle, ADXL343_REG_INT_MAP);

	printf("Valeur lue du registre INT_ENABLE après écriture: 0x%02X\n", int_enable_read);

	printf("Valeur du registre INT_MAP après écriture: 0x%02X\n", int_map_read);
}

void ADXL343_check_tap(ADXL343 *dev) {
	uint8_t intSource = ADXL343_read_register(dev->hspiHandle, ADXL343_REG_INT_SOURCE);

	if (intSource & 0x40) {
		printf("Single Tap detected!\n");
		ADXL343_read_register(dev->hspiHandle, ADXL343_REG_INT_SOURCE);
	}
}

void ADXL343_SetDataFormat(SPI_HandleTypeDef *hspi) {
	uint8_t data_format = 0x00; // Configuration de base, pas d'inversion
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi, &ADXL343_REG_DATA_FORMAT, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(hspi, &data_format, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);
}
/*
void ADXL343_SetDataFormat(SPI_HandleTypeDef *hspi) {
    // Configuration du registre DATA_FORMAT
    uint8_t data_format = 0x00; // Valeur par défaut

    // Activation du SELF_TEST (bit 7)
    data_format |= (1 << 7); // Définit le bit SELF_TEST à 1

    // Configuration en mode SPI à 3 fils (bit 6)
    data_format |= (1 << 6); // Définit le bit SPI à 1

    // Autres configurations optionnelles
    // Par exemple, pour FULL_RES (bit 3), Range ou autre; ajustez selon vos besoins.
    // data_format |= (1 << 3); // Si vous voulez activer FULL_RES

    HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);

    // Écriture dans le registre DATA_FORMAT
    HAL_SPI_Transmit(hspi, &ADXL343_REG_DATA_FORMAT, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(hspi, &data_format, 1, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);
}*/



void ADXL343_set_tap_threshold(ADXL343 *dev, uint8_t threshold) {
	uint8_t regAddr = ADXL343_REG_THRESH_TAP;
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(dev->hspiHandle, &regAddr, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(dev->hspiHandle, &threshold, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);
}

// TODO FINISH the rest of the functionalities in SPI
