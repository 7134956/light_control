#include "user_interface.h"
#include "espmissingincludes.h"
#include <osapi.h>
#include "save_params.h"

extern int currLedState;
os_timer_t countdown_save;

void write_params(void)
{
	os_timer_disarm(&countdown_save);
	os_timer_setfn(&countdown_save, (os_timer_func_t *)write_to_flash, NULL);
	os_timer_arm(&countdown_save, 10000, 0);
}

void read_params(void)
{
	spi_flash_read((ESP_PARAM_START_SEC + ESP_PARAM_SAVE_0) * SPI_FLASH_SEC_SIZE,
               (uint32 *)&currLedState, sizeof(currLedState));
	os_printf("After read_params %d\n", currLedState);
}

void write_to_flash(void)
{
	int savedLedState;
	spi_flash_read((ESP_PARAM_START_SEC + ESP_PARAM_SAVE_0) * SPI_FLASH_SEC_SIZE,
	               (uint32 *)&savedLedState, sizeof(currLedState));

	if(savedLedState!=currLedState)
	{
		spi_flash_erase_sector(ESP_PARAM_START_SEC + ESP_PARAM_SAVE_0);
		spi_flash_write((ESP_PARAM_START_SEC + ESP_PARAM_SAVE_0) * SPI_FLASH_SEC_SIZE,
				(uint32 *)&currLedState, sizeof(currLedState));
		os_printf("After write_params %d\n", currLedState);
	}
	else {
		os_printf("led current state equal to saved state");
	}
	os_timer_disarm(&countdown_save);
}
