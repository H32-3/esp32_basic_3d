#define rotate
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

void wfb();
void fill_screen(uint16_t color);
void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void screen_setup(int cs,int rst, int dc);
int screen_height();
int screen_width();
void pixel(uint16_t y,uint16_t x,uint16_t color);
void fill_rect(uint16_t x,uint16_t y,uint16_t x1, uint16_t y1,uint16_t color);
void draw_circle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
int color565(uint8_t r, uint8_t g,uint8_t b);
