#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int LED_PIN_R = 4;
volatile int btn_flag_r = 0;

const int BTN_PIN_G = 26;
const int LED_PIN_G = 6;
volatile int btn_flag_g = 0;

void gpio_callback(uint gpio, uint32_t events) {
  if (events == 0x4 && gpio == BTN_PIN_R) {         // fall edge
    btn_flag_r = 1;
  } else if (events == 0x8 && gpio == BTN_PIN_G) {  // rise edge
    btn_flag_g = 1;
  }
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);
  gpio_set_irq_enabled_with_callback(BTN_PIN_R, 
                                     GPIO_IRQ_EDGE_RISE | 
                                     GPIO_IRQ_EDGE_FALL, 
                                     true,
                                     &gpio_callback);

  gpio_init(LED_PIN_R);
  gpio_set_dir(LED_PIN_R, GPIO_OUT);

  gpio_init(BTN_PIN_G);
  gpio_set_dir(BTN_PIN_G, GPIO_IN);
  gpio_pull_up(BTN_PIN_G);
  gpio_set_irq_enabled_with_callback(BTN_PIN_G, 
    GPIO_IRQ_EDGE_RISE | 
    GPIO_IRQ_EDGE_FALL, 
    true,
    &gpio_callback);

  gpio_init(LED_PIN_G);
  gpio_set_dir(LED_PIN_G, GPIO_OUT);

  int led_status_r = 0;
  int led_status_g = 0;

  while (true) {
    if (btn_flag_r){
      btn_flag_r = 0; // clean IRS flag
      led_status_r = !led_status_r;
      gpio_put(LED_PIN_R, led_status_r);
    }
    if (btn_flag_g){
      btn_flag_g = 0; // clean IRS flag
      led_status_g = !led_status_g;
      gpio_put(LED_PIN_G, led_status_g);
    }
  }
}