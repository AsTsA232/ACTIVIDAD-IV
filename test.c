
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <string.h>

// Dirección I2C de la pantalla OLED SSD1306
#define OLED_I2C_ADDR 0x3c  
#define SDA_PIN 0
#define SCL_PIN 1
// Comandos de control SSD1306
#define CMD_DISPLAY_OFF 0xAE
#define CMD_DISPLAY_ON 0xAF
#define CMD_SET_CONTRAST 0x81
#define CMD_SET_MEMORY_MODE 0x20
#define CMD_SET_NORMAL_DISPLAY 0xA6
#define CMD_SET_PAGE_START 0xB0
#define CMD_SET_COLUMN_ADDR 0x21


// Enviar comandos OLED
void commandos(uint8_t com) {
    uint8_t buf[2] = {0x00, com};  // 0x00 indica que es un comando
    i2c_write_blocking(i2c0, OLED_I2C_ADDR, buf, 2, false);
}

// Enviar datos al OLED
void envdat(uint8_t data) {
    uint8_t buf[2] = {0x40, data};  // 0x40 indica que son datos
    i2c_write_blocking(i2c0, OLED_I2C_ADDR, buf, 2, false);
}

// Datos de la fuente 
const uint8_t font_5x8[95][5] = {
    {0x7E, 0x09, 0x09, 0x09, 0x7E},  // A
    {0x7F, 0x49, 0x49, 0x49, 0x36},  // B
    {0x3E, 0x41, 0x41, 0x41, 0x22},  // C
    
};

// Escribir un carácter en la pantalla
void esc_char(char c) {
    if (c < 'A' || c > 'C') return;  
    const uint8_t* char_data = font_5x8[c - ' '];  // datos del carácter

    for (int i = 0; i < 5; i++) {
       envdat(char_data[i]);  // Enviar los 5 bytes de datos del carácter
    }
    envdat(0x00);  // Espacio 
}

// Escribir una cadena de texto 
void esc_string(const char* str) {
    while (*str) {
        esc_char(*str++);
    }
}

int main() {
    stdio_init_all();
    i2c_init(i2c_default, 400*1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

  commandos(CMD_DISPLAY_OFF);  // Apagar la pantalla (por seguridad)
  commandos(CMD_SET_MEMORY_MODE);  // Establecer el modo de memoria
  commandos(0x00);  // Modo de direccionamiento horizontal
  commandos(CMD_SET_NORMAL_DISPLAY);  // Establecer la pantalla en modo normal
  commandos(CMD_SET_CONTRAST);  // Establecer el contraste
  commandos(0x7F);  // Contraste medio-alto
  commandos(CMD_DISPLAY_ON); 
    
    esc_string("ABC");  
    sleep_ms(1000);
}
