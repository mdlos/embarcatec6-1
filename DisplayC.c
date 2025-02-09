// Embarcatec - Marcio de Arruda Fonseca

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "ws2812.pio.h"

#define BUTTON_A_PIN 5    
#define BUTTON_B_PIN 6   
#define BUTTON_JOY_PIN 22 
#define LED_G_GPIO 11   
#define LED_B_GPIO 12    
#define NUM_PIXELS 25  
#define WS2812_PIN 7      

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

bool digitos[10][NUM_PIXELS] = {
   {0, 1, 1, 1, 0,  
    0, 1, 0, 1, 0,  
    0, 1, 0, 1, 0,  
    0, 1, 0, 1, 0,  
    0, 1, 1, 1, 0}, 

   {0, 0, 1, 0, 0,  
    0, 0, 1, 0, 0,  
    0, 0, 1, 0, 0,  
    0, 1, 1, 0, 0,  
    0, 0, 1, 0, 0}, 

   {0, 1, 1, 1, 0,  
    0, 1, 0, 0, 0,  
    0, 1, 1, 1, 0,  
    0, 0, 0, 1, 0,  
    0, 1, 1, 1, 0}, 

   {0, 1, 1, 1, 0,  
    0, 0, 0, 1, 0,  
    0, 1, 1, 1, 0,  
    0, 0, 0, 1, 0,  
    0, 1, 1, 1, 0}, 

   {0, 1, 0, 0, 0,  
    0, 0, 0, 1, 0,  
    0, 1, 1, 1, 0,  
    0, 1, 0, 1, 0,  
    0, 1, 0, 1, 0}, 

   {0, 1, 1, 1, 0,  
    0, 0, 0, 1, 0,  
    0, 1, 1, 1, 0,  
    0, 1, 0, 0, 0,  
    0, 1, 1, 1, 0}, 

   {0, 1, 1, 1, 0,  
    0, 1, 0, 1, 0,  
    0, 1, 1, 1, 0,  
    0, 1, 0, 0, 0,  
    0, 1, 1, 0, 0}, 

   {0, 1, 0, 0, 0,  
    0, 0, 0, 1, 0,  
    0, 1, 0, 0, 0,  
    0, 0, 0, 1, 0,  
    0, 1, 1, 1, 0}, 

   {0, 1, 1, 1, 0,  
    0, 1, 0, 1, 0,  
    0, 1, 1, 1, 0,  
    0, 1, 0, 1, 0,  
    0, 1, 1, 1, 0}, 

   {0, 1, 1, 1, 0,  
    0, 0, 0, 1, 0,  
    0, 1, 1, 1, 0,  
    0, 1, 0, 1, 0,  
    0, 1, 1, 1, 0}  
};

bool led_green_state = false; 
bool led_blue_state = false; 

static uint32_t last_time_a = 0;
static uint32_t last_time_b = 0;
static uint32_t last_time_joy = 0;

static ssd1306_t ssd;
static PIO pio_global;
static uint sm_global;

static const uint32_t DEBOUNCE_DELAY = 600; 

void exibir_numero_na_matriz(int numero, PIO pio, uint sm, uint32_t *led_buffer) {
    if (numero >= 0 && numero <= 9) {
        for (int i = 0; i < NUM_PIXELS; i++) {
            if (digitos[numero][i]) {
                led_buffer[i] = 0xFFFFFF;  
            } else {
                led_buffer[i] = 0x000000; 
            }
        }
        for (int i = 0; i < NUM_PIXELS; i++) {
            pio_sm_put_blocking(pio, sm, led_buffer[i]);
        }
    }
}

void exibir_mensagem_no_display(ssd1306_t *ssd, const char *mensagem) {
    ssd1306_fill(ssd, false); 
    ssd1306_draw_string(ssd, mensagem, 10, 30); 
    ssd1306_send_data(ssd);
}

void desligar_matriz_leds(PIO pio, uint sm) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        pio_sm_put_blocking(pio, sm, 0x000000);
    }
}

void gpio_callback(uint gpio, uint32_t events) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    
    if (gpio == BUTTON_A_PIN) {
        if (current_time - last_time_a < DEBOUNCE_DELAY) return;
        last_time_a = current_time;
        led_green_state = !led_green_state;
        gpio_put(LED_G_GPIO, led_green_state);
        const char *mensagem = led_green_state ? "LED Verde - ligado" : "LED Verde - Desligado";
        printf("%s\n", mensagem);
        exibir_mensagem_no_display(&ssd, mensagem);
        
    } else if (gpio == BUTTON_B_PIN) {
        if (current_time - last_time_b < DEBOUNCE_DELAY) return;
        last_time_b = current_time;
        led_blue_state = !led_blue_state;
        gpio_put(LED_B_GPIO, led_blue_state);
        const char *mensagem = led_blue_state ? "LED Azul - ligado" : "LED Azul -  Desligado";
        printf("%s\n", mensagem);
        exibir_mensagem_no_display(&ssd, mensagem);
        
    } else if (gpio == BUTTON_JOY_PIN) {
        if (current_time - last_time_joy < DEBOUNCE_DELAY) return;
        last_time_joy = current_time;
        desligar_matriz_leds(pio_global, sm_global);
        printf("Matriz de LED - Desligada\n");
        exibir_mensagem_no_display(&ssd, "Matriz de LED - Desligada");
    }
}

int main() {
    stdio_init_all(); 
    sleep_ms(300); 
    
    printf("RP2040 inicializado.\n");

    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);

    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);

    gpio_init(BUTTON_JOY_PIN);
    gpio_set_dir(BUTTON_JOY_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_JOY_PIN);

    gpio_init(LED_G_GPIO);
    gpio_set_dir(LED_G_GPIO, GPIO_OUT);
    gpio_put(LED_G_GPIO, 0);

    gpio_init(LED_B_GPIO);
    gpio_set_dir(LED_B_GPIO, GPIO_OUT);
    gpio_put(LED_B_GPIO, 0);
    
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    pio_global = pio0;
    sm_global = 0;
    uint offset = pio_add_program(pio_global, &ws2812_program);
    ws2812_program_init(pio_global, sm_global, offset, WS2812_PIN, 800000, false);

    uint32_t led_buffer[NUM_PIXELS] = {0};

    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, gpio_callback);
    gpio_set_irq_enabled(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BUTTON_JOY_PIN, GPIO_IRQ_EDGE_FALL, true);

    char received_char = '\0';

    while (true) {

        if (stdio_usb_connected()) {
            int ch = getchar_timeout_us(0);
            if (ch != PICO_ERROR_TIMEOUT) {
                received_char = (char) ch;
                printf("Recebido: %c\n", received_char);
                char mensagem[2] = {received_char, '\0'};
                exibir_mensagem_no_display(&ssd, mensagem);
                if (received_char >= '0' && received_char <= '9') {
                    int numero = received_char - '0';
                    exibir_numero_na_matriz(numero, pio_global, sm_global, led_buffer);
                }
            }
        }
        sleep_ms(100);
    }
}
