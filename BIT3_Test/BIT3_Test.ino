#include "BIT3.h"

/* タイマー割り込み用 */
struct repeating_timer st_timer;

/* 割り込み関数 */
bool Timer(struct repeating_timer *t) {
  check_battery();
  return true;
}

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

  add_repeating_timer_us(10000, Timer, NULL, &st_timer);
}

void loop() {
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

void run0() {  // ライントレース:
  while(true){
    uint16_t sensor1 = get_sensor_level(ch_sensor1);
    uint16_t sensor2 = get_sensor_level(ch_sensor2);
    uint16_t sensor3 = get_sensor_level(ch_sensor3);
    uint16_t sensor4 = get_sensor_level(ch_sensor4);
    uint16_t sensor5 = get_sensor_level(ch_sensor5);

         if(sensor1  > sensor_thres_1 && sensor2 <= sensor_thres_2 && sensor3 <= sensor_thres_3 && sensor4 <= sensor_thres_4){  //○×××
      change_motor_voltage( 3 * Speed, 12 * Speed);
    }
    else if(sensor1  > sensor_thres_1 && sensor2  > sensor_thres_2 && sensor3 <= sensor_thres_3 && sensor4 <= sensor_thres_4){  //○○××
      change_motor_voltage( 5 * Speed,  8 * Speed);
    }
    else if(sensor1 <= sensor_thres_1 && sensor2  > sensor_thres_2 && sensor3 <= sensor_thres_3 && sensor4 <= sensor_thres_4){  //×○××
      change_motor_voltage( 7 * Speed,  9 * Speed);
    }
    else if(sensor1 <= sensor_thres_1 && sensor2  > sensor_thres_2 && sensor3  > sensor_thres_3 && sensor4 <= sensor_thres_4){  //×○○×
      change_motor_voltage( 8 * Speed,  8 * Speed);
    }
    else if(sensor1 <= sensor_thres_1 && sensor2 <= sensor_thres_2 && sensor3  > sensor_thres_3 && sensor4 <= sensor_thres_4){  //××○×
      change_motor_voltage( 9 * Speed,  7 * Speed);
    }
    else if(sensor1 <= sensor_thres_1 && sensor2 <= sensor_thres_2 && sensor3  > sensor_thres_3 && sensor4  > sensor_thres_4){  //××○○
      change_motor_voltage( 8 * Speed,  5 * Speed);
    }
    else if(sensor1 <= sensor_thres_1 && sensor2 <= sensor_thres_2 && sensor3 <= sensor_thres_3 && sensor4  > sensor_thres_4){  //×××○
      change_motor_voltage(12 * Speed,  3 * Speed);
    }
    else{
      change_motor_voltage( 8 * Speed,  8 * Speed);
    }

    flash_LED();

    delay(10);
  }
}

void run1() {  // キャリブレーション:
  uint16_t sensor[5], min[5] = {65535, 65535, 65535, 65535, 65535}, ave[5], max[5] = {0, 0, 0, 0, 0};

  Serial.print("sensor Calibration will run While \"EXEC\" is pressing.\n");

  while(digitalRead(SW_EXEC) == HIGH);
  while(digitalRead(SW_EXEC) ==  LOW) {
    sensor[0] = get_sensor_level(ch_sensor1);
    sensor[1] = get_sensor_level(ch_sensor2);
    sensor[2] = get_sensor_level(ch_sensor3);
    sensor[3] = get_sensor_level(ch_sensor4);
    sensor[4] = get_sensor_level(ch_sensor5);

    for(uint8_t i = 0; i < 5; i++) {
      if(sensor[i] < min[i]) {
        min[i] = sensor[i];
      }
      
      if(sensor[i] > max[i]) {
        max[i] = sensor[i];
      }

      ave[i] = (min[i] + max[i]) / 2;
    }
  }
  
  Serial.print("\nsensor level :   min /   ave /   max\n");
  char buffer[40];
  for(uint8_t i = 0; i < 5; i++) {
    sprintf(buffer, "     sensor%d : %5d / %5d / %5d\n", i + 1, min[i], ave[i], max[i]);
    Serial.print(buffer);
  }

  float fix_value = 0.6;

  sensor_thres_1 = ave[0] * fix_value;
  sensor_thres_2 = ave[1] * fix_value;
  sensor_thres_3 = ave[2] * fix_value;
  sensor_thres_4 = ave[3] * fix_value;
  sensor_thres_5 = ave[4] * fix_value;

  Serial.print("\nsensor Calibration end\n\n");
}

void run2() {
}

void run3() {
}

void run4() {
}

void run5() {
}

void run6() {
}

void run7() {
}

void run8() {
}

void run9() {
}

void run10() {
}

void run11() {
}

void run12() {
}

void run13() {  // モータテスト1:
  Serial.print("Both motor Test Start.\n");
  for(int16_t i = 0; i <= 3000; i += 10) {
    change_motor_voltage(i, i);
    Serial.print("motor voltage = ");
    Serial.print(i);
    Serial.print("\n");
    delay(50);
  }

  change_motor_voltage(0, 0);
  delay(1000);

  Serial.print("L motor Test Start.\n");
  for(int16_t i = 0; i <= 3000; i += 10) {
    change_motor_voltage(i, 0);
    Serial.print("motor voltage = ");
    Serial.print(i);
    Serial.print("\n");
    delay(50);
  }

  change_motor_voltage(0, 0);
  delay(1000);

  Serial.print("R motor Test Start.\n");
  for(int16_t i = 0; i <= 3000; i += 10) {
    change_motor_voltage(0, i);
    Serial.print("motor voltage = ");
    Serial.print(i);
    Serial.print("\n");
    delay(50);
  }

  change_motor_voltage(0, 0);
  delay(1000);

  Serial.print("motor Test end.\n");
}

void run14() {  // モータテスト2:
  while(true) {
    for(int16_t i = 0; i <= 3000; i += 10) {
      change_motor_voltage(i, i);
      delay(100);
    }
    delay(1000);
    for(int16_t i = 3000; i >= 0; i -= 10) {
      change_motor_voltage(i, i);
      delay(100);
    }
    delay(1000);
  }
}

void run15() {  // ADC デバッグ:
  while(true) {
  debug_ADC();
  flash_LED();
  delay(10);
  }
}
