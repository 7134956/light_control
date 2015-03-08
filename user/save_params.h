/* NOTICE---this is for 512KB spi flash.
 * you can change to other sector if you use other size spi flash. */
#define ESP_PARAM_START_SEC		0x3C
#define ESP_PARAM_SAVE_0    1

void write_params(void);
void read_params(void);

