#include "lcd/lcd.h"
#include "utils.h"
#include "assembly/example.h"
#include "GameControl.h"

void Inp_init(void) {
  rcu_periph_clock_enable(RCU_GPIOA);
  rcu_periph_clock_enable(RCU_GPIOC);

  gpio_init(GPIOA, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ,
            GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
  gpio_init(GPIOC, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ,
            GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
}

void IO_init(void) {
  Inp_init(); // inport init
  Lcd_Init(); // LCD init
}

void Board_self_test(void) {
  Initial_selection();

  Initial_scene_drawing();
}

int main(void) {
  IO_init();
  Board_self_test();
}
