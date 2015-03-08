#include "user_interface.h"
#include "espmissingincludes.h"
#include <string.h>
#include <osapi.h>
#include "save_params.h"

extern int currLedState;

void write_params(void)
{
        spi_flash_erase_sector(ESP_PARAM_START_SEC + ESP_PARAM_SAVE_0);
        spi_flash_write((ESP_PARAM_START_SEC + ESP_PARAM_SAVE_0) * SPI_FLASH_SEC_SIZE,
                        (uint32 *)&currLedState, sizeof(currLedState));
        os_printf("After write_params %d\n", currLedState);

}

void read_params(void)
{
	spi_flash_read((ESP_PARAM_START_SEC + ESP_PARAM_SAVE_0) * SPI_FLASH_SEC_SIZE,
               (uint32 *)&currLedState, sizeof(currLedState));
	os_printf("After read_params %d\n", currLedState);
}
