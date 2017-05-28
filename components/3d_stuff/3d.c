#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include <stdio.h>
#include "lcd.h"
#include <math.h>
#include "3dData.h"
#include "3d.h"

/////////////////////////////////////////////////////////////////////////////////////
/* I GOT THE 3D DRAWING CODE FROM https://www.youtube.com/watch?v=9NtaxMUltUA&t=0s */
/////////////////////////////////////////////////////////////////////////////////////

#define MOSI	23
#define CLK	19
#define CS	22
#define DC	21
#define RST	18

#define magni 5
//COMMENT OUT THESE DEFINES TO SPEED UP THE RENDERING WHEN NOT USING SOME ROTATION AXIS:
//#define XROT
#define YROT
//#define ZROT

// Decimal bit count in our fixed point calculations
#define PRECISION_BITS 5

// 1 << PRECISION BITS
#define PRECISION_SHIFTED 33

// These are our fixed point arithmetic macros
#define MULT(x, y) ((x*y) >> PRECISION_BITS)
#define TOFP(f) (PRECISION_SHIFTED*f)
#define TOINT(f) (f >> PRECISION_BITS)

#define TRANSLATION_X 30;
#define TRANSLATION_Y 50;

//Hight/Width size
#define hs 230//320
#define ws 150//240

// Screen Hight / Width Size
#define shs 320
#define sws 240

#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }

float rx=0,ry=0,rz=0;
void do3d() {
	printf("hello");
	screen_setup(CS,RST,DC);
	while(1){
		wfb();				//write the frame buffer to the display using dma
		fill_screen(0);			//fill the frame buffer with 0
#ifdef XROT
		rx +=  0.01;
		int16_t sx = TOFP(sin(rx));
		int16_t cx = TOFP(cos(rx));
#endif

#ifdef YROT
		ry += 0.15;
		int16_t sy = TOFP(sin(ry));
		int16_t cy = TOFP(cos(ry));
#endif

#ifdef ZROT
		rz +=  0.01;
		int16_t sz = TOFP(sin(rz));
		int16_t cz = TOFP(cos(rz));
#endif

		int16_t tx;
		int16_t ty;
		int16_t tz;

		int16_t x;
		int16_t y;
		int16_t z;

		int16_t m = sizeof(vertices) / 12;

		int16_t i;
		for (i = 0; i < m; i++) {
			int16_t arraypos = i * 6;
			x = tx = vertices[arraypos + 0];
			y = ty = vertices[arraypos + 1];
			z = tz = vertices[arraypos + 2];

#ifdef XROT
			ty = MULT(y , cx) + MULT(z , sx);
			tz = MULT(-y , sx) + MULT(z , cx);
#endif

			y = ty;
			z = tz;

#ifdef YROT
			tx = MULT(x , cy) + MULT(z , sy);
			tz = MULT(-x , sy) + MULT(z , cy);
#endif

			x = tx;
			z = tz;

#ifdef ZROT
			tx = MULT(x , cz) + MULT(y , sz);
			ty = MULT(-x , sz) + MULT(y , cz);
#endif

			int16_t x0 = TOINT(tx) + TRANSLATION_X;
			int16_t y0 = TOINT(-ty) + TRANSLATION_Y;

			x = tx = vertices[arraypos + 3];
			y = ty = vertices[arraypos + 4];
			z = tz = vertices[arraypos + 5];

#ifdef XROT
			ty = MULT(y , cx) + MULT(z , sx);
			tz = MULT(-y , sx) + MULT(z , cx);
#endif

			y = ty;
			z = tz;

#ifdef YROT
			tx = MULT(x , cy) + MULT(z , sy);
			tz = MULT(-x , sy) + MULT(z , cy);
#endif

			x = tx;
			z = tz;

#ifdef ZROT
			tx = MULT(x , cz) + MULT(y , sz);
			ty = MULT(-x , sz) + MULT(y , cz);
#endif

			int16_t x1 = TOINT(tx) + TRANSLATION_X;
			int16_t y1 = TOINT(-ty) + TRANSLATION_Y;

			draw_line(x0 * magni, y0 * magni, x1 * magni, y1 * magni, 0xFFFF);
		}
	}
}
