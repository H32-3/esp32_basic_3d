#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include <stdio.h>
#include "lcd.h"
#include <math.h>
#include "3d.h"

/////////////////////////////////////////////////////////////////////////////////////
/* I GOT THE 3D DRAWING CODE FROM https://www.youtube.com/watch?v=9NtaxMUltUA&t=0s */
/////////////////////////////////////////////////////////////////////////////////////

#define MOSI	23
#define CLK	19
#define CS	22
#define DC	21
#define RST	18

void app_main(){
	spi_bus_config_t buscfg= {.mosi_io_num=MOSI,.sclk_io_num=CLK,.quadwp_io_num=-1,.quadhd_io_num=-1};
	spi_bus_initialize(HSPI_HOST, &buscfg, 1);

	do3d();
}

