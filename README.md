# esp32_basic_3d
Basic 3d rendering for the esp32 and the ili9341 display

based on the code from this video https://www.youtube.com/watch?v=9NtaxMUltUA

I'm using a frame buffer and transferring it to the display with dma

This is certainly not fully optimized but it's pretty fast

To upload you need the ESP-IDF, after you get that you can just type make flash and it should compile and upload !
