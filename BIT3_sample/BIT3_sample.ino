#include "BIT3.h"
#include "run.h"

/* ユーザが定義する関数のプロトタイプ宣言 */


void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.print("Program Started.\n");

  LED_init();
  ADC_init();
  IR_init();
  motor_init(PHASE_ENABLE_MODE);
  SW_init();

  delay(125);
  boot_motion();
  delay(125);
}

void loop() {
  check_battery();

  Serial.print("Please select a mode.\n");

  uint8_t mode = get_mode_number();

  switch(mode) {
    case 0:
      run0();
      break;

    case 1:
      run1();
      break;

    case 2:
      run2();
      break;

    case 3:
      run3();
      break;

    case 4:
      run4();
      break;

    case 5:
      run5();
      break;

    case 6:
      run6();
      break;

    case 7:
      run7();
      break;

    case 8:
      run8();
      break;

    case 9:
      run9();
      break;

    case 10:
      run10();
      break;

    case 11:
      run11();
      break;

    case 12:
      run12();
      break;

    case 13:
      run13();
      break;

    case 14:
      run14();
      break;

    case 15:
      run15();
      break;

    default:
      break;
  }
}
