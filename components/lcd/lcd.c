#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "lcd.h"
#include <stdio.h>

#define height	320
#define width	240

spi_device_handle_t spi;

// Color definitions
#define BLACK       0x0000      /*   0,   0,   0 */
#define NAVY        0x000F      /*   0,   0, 128 */
#define DARKGREEN   0x03E0      /*   0, 128,   0 */
#define DARKCYAN    0x03EF      /*   0, 128, 128 */
#define MAROON      0x7800      /* 128,   0,   0 */
#define PURPLE      0x780F      /* 128,   0, 128 */
#define OLIVE       0x7BE0      /* 128, 128,   0 */
#define LIGHTGREY   0xC618      /* 192, 192, 192 */
#define DARKGREY    0x7BEF      /* 128, 128, 128 */
#define BLUE        0x001F      /*   0,   0, 255 */
#define GREEN       0x07E0      /*   0, 255,   0 */
#define CYAN        0x07FF      /*   0, 255, 255 */
#define RED         0xF800      /* 255,   0,   0 */
#define MAGENTA     0xF81F      /* 255,   0, 255 */
#define YELLOW      0xFFE0      /* 255, 255,   0 */
#define WHITE       0xFFFF      /* 255, 255, 255 */
#define ORANGE      0xFD20      /* 255, 165,   0 */
#define GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define PINK        0xF81F

static uint16_t fb[320*240];
void wfb(){
	static spi_transaction_t trans;
	int g;
	trans.length=16*width;
	trans.flags=0;
//	while(1){
		g=0;
		while(g!=height){
			trans.tx_buffer=fb+g*width;
			spi_device_queue_trans(spi, &trans, portMAX_DELAY);
			g++;
		}
//	}
//	spi_device_get_trans_result(spi, NULL, portMAX_DELAY);

}

uint16_t dma_fix_data(uint16_t data){ // let's say data = 0x1234
	uint8_t temp=data >> 8; // temp = 0x12
	data=data<<8; // data = 0x3400
	return(data+temp); // return 0x3412
}

void spi_send( const uint8_t cmd) {
	spi_transaction_t t;
	memset(&t, 0, sizeof(t));       //Zero out the transaction
	t.length=8;                     //Command is 8 bits
	t.tx_buffer=&cmd;               //The data is the cmd itself
	spi_device_transmit(spi, &t);  //Transmit!
}

void writecommand( uint8_t data,int dc){
	gpio_set_level(dc,0);
	spi_send( data);
}

void writedata( uint8_t data,int dc){
	gpio_set_level(dc,1);
	spi_send( data);
}

void lcdinitd(int rst, int dc) {
	gpio_set_direction(dc, GPIO_MODE_OUTPUT);
	gpio_set_direction(rst, GPIO_MODE_OUTPUT);

	gpio_set_level(rst, 0);
	vTaskDelay(100 / portTICK_RATE_MS);
	gpio_set_level(rst, 1);
	vTaskDelay(100 / portTICK_RATE_MS);

	writecommand(0xEF,dc);  writedata(0x03,dc);  writedata(0x80,dc);  writedata(0x02,dc);
	writecommand(0xCF,dc);  writedata(0x00,dc);  writedata(0XC1,dc);  writedata(0X30,dc);
	writecommand(0xED,dc);  writedata(0x64,dc);  writedata(0x03,dc);  writedata(0X12,dc);  writedata(0X81,dc);
	writecommand(0xE8,dc);  writedata(0x85,dc);  writedata(0x00,dc);  writedata(0x78,dc);
	writecommand(0xCB,dc);  writedata(0x39,dc);  writedata(0x2C,dc);  writedata(0x00,dc);  writedata(0x34,dc);  writedata(0x02,dc);
	writecommand(0xF7,dc);  writedata(0x20,dc);
	writecommand(0xEA,dc);  writedata(0x00,dc);  writedata(0x00,dc);
	writecommand(0x23,dc);  writedata(0x23,dc);
	writecommand(0x10,dc);  writedata(0x10,dc);
	writecommand(0xC5,dc);  writedata(0x3e,dc);  writedata(0x28,dc);
	writecommand(0xC7,dc);  writedata(0x86,dc);
	writecommand(0x36,dc);  writedata(0x48,dc);
	writecommand(0x3A,dc);  writedata(0x55,dc);
	writecommand(0xB1,dc);  writedata(0x00,dc);  writedata(0x18,dc);
	writecommand(0xB6,dc);  writedata(0x08,dc);  writedata(0x82,dc);  writedata(0x27,dc);
	writecommand(0xF2,dc);  writedata(0x00,dc);
	writecommand(0x26,dc);  writedata(0x01,dc);
	writecommand(0xE0,dc);  writedata(0x0F,dc);  writedata(0x31,dc);  writedata(0x2B,dc);  writedata(0x0C,dc);  writedata(0x0E,dc);  writedata(0x08,dc);  writedata(0x4E,dc);  writedata(0xF1,dc);  writedata(0x37,dc);  writedata(0x07,dc);  writedata(0x10,dc);  writedata(0x03,dc);  writedata(0x0E,dc); writedata(0x09,dc);  writedata(0x00,dc);
	writecommand(0xE1,dc);  writedata(0x00,dc);  writedata(0x0E,dc);  writedata(0x14,dc);  writedata(0x03,dc);  writedata(0x11,dc);  writedata(0x07,dc);  writedata(0x31,dc);  writedata(0x08,dc);  writedata(0x0F,dc);  writedata(0x0C,dc);  writedata(0x36,dc);  writedata(0x0F,dc);
	writecommand(0x11,dc);
	writecommand(0x29,dc);

}
void writedata32( uint32_t d,int dc) {
	gpio_set_level(dc,1);
	spi_send(d >> 24);
	spi_send(d >> 16);
	spi_send(d >> 8);
	spi_send(d);
}

#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
	if(x0>x1 && y0 > y1) return;
	if (steep) {
		_swap_int16_t(x0, y0);
		_swap_int16_t(x1, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	for (; x0 <= x1; x0++) {
		if (steep) {
			if (x0+(y0*width) < (height+1)*(width+1) && x0 < width)		
				pixel(y0,x0,color);
		} else {
			if (y0+(x0*width) < (height+1)*(width+1) && y0 < width)
				pixel(x0,y0, color);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}


void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h,int dc) {
	uint32_t xa = ((uint32_t)x << 16) | (x+w-1);
	uint32_t ya = ((uint32_t)y << 16) | (y+h-1);
	writecommand(0x2A,dc); // Column addr set
	writedata32(xa,dc);
	writecommand(0x2B,dc); // Row addr set
	writedata32(ya,dc);
	writecommand(0x2C,dc); // write to RAM
}
/*
   void wfb(int dc){
   setAddrWindow(0,0,width,height,dc);
   while(1){
   for(int g=0;g<=width*height-1;g++){
   writedata(fb[g] >> 8,dc);
   writedata(fb[g],dc);
   }
   }
   }
   */


void screen_setup(int cs,int rst, int dc){
	spi_device_interface_config_t devcfg={
		.clock_speed_hz=32000000,	//Clock out at 10 MHz
		.mode=0,			//SPI mode 0
		.spics_io_num=cs,		//CS pin
		.queue_size=height,	
	};

	spi_bus_add_device(HSPI_HOST, &devcfg, &spi);

	lcdinitd(rst, dc);
	
	setAddrWindow(0,0,width,height,dc);
	gpio_set_level(dc,1);
//	xTaskCreatePinnedToCore(&wfb, "wfb",2048,NULL,2,NULL,1);

}


int screen_height(){
#ifdef rotate
	return width;
#else
	return height;
#endif
}
int screen_width(){
#ifdef rotate
	return height;
#else
	return width;
#endif
}

void fill_screen(uint16_t color){
	for(int g=0;g<screen_height()*screen_width();g++)
		fb[g]=dma_fix_data(color);
}
void pixel(uint16_t x, uint16_t y,uint16_t color){
	if(x<=0 || y<=0 || x>screen_width() || y>screen_height())return;
#ifdef rotate
	x=screen_width()-1-x;
	//y=height-y;
	fb[y+(x*width)]=dma_fix_data(color);
#else
	fb[x+(y*width)]=dma_fix_data(color);
#endif
}
void fill_rect(uint16_t x,uint16_t y,uint16_t x1, uint16_t y1,uint16_t color){
	int xtemp=x;
	for (;y<y1;y++)
		for(x=xtemp;x<x1;x++)
			pixel(x,y,color);
}
void draw_circle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	pixel(x0  , y0+r, color);
	pixel(x0  , y0-r, color);
	pixel(x0+r, y0  , color);
	pixel(x0-r, y0  , color);

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		pixel(x0 + x, y0 + y, color);
		pixel(x0 - x, y0 + y, color);
		pixel(x0 + x, y0 - y, color);
		pixel(x0 - x, y0 - y, color);
		pixel(x0 + y, y0 + x, color);
		pixel(x0 - y, y0 + x, color);
		pixel(x0 + y, y0 - x, color);
		pixel(x0 - y, y0 - x, color);
	}
}
int color565( uint8_t r, uint8_t g,uint8_t b ) {
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}
