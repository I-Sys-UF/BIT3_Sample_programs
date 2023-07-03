/* 各種定数設定 */
#define toggle_delay 100  // MCP3008 との通信時に送るクロックの周波数に関連:
#define Sensor_thres 200  // フォトセンサの閾値:

/* 各種 GPIO に名前付け */
#define LED1 4
#define LED2 3
#define LED3 2
#define LED4 1
#define LED5 0

#define MCP3008_CLK  6
#define MCP3008_Dout 7
#define MCP3008_Din  8
#define MCP3008_CS   9

#define DRV8835_AIN1 13
#define DRV8835_AIN2 12
#define DRV8835_BIN1 11
#define DRV8835_BIN2 10
#define DRV8835_MODE 14

#define Tr 15

#define SW_MODE 17
#define SW_BACK 16

/* ADC のチャンネル設定 */
#define V_Sensor1 4
#define V_Sensor2 3
#define V_Sensor3 2
#define V_Sensor4 1
#define V_Sensor5 0
#define V_Battery 7

/* モータドライバのモード設定 */
#define IN_IN_MODE        0
#define PHASE_ENABLE_MODE 1

/* 関数のプロトタイプ宣言 */
void LED_init(void);                              // LED を使えるようにするやつ:
void ADC_init(void);                              // ADC を使えるようにするやつ:
void IR_init(void);                               // 赤外線 LED を使えるようにするやつ:
void Motor_init(uint8_t mode);                    // モタドラを使えるようにするやつ:
void SW_init(void);                               // スイッチを使えるようにするやつ:
void LED_num(uint8_t num);                        // 数字を指定してバイナリで表示するやつ:
uint16_t ADC_Read(uint8_t ch);                    // チャンネルを指定して ADC 読むやつ:
void toggle(void);                                // GPIO をただトグルさせるだけのやつ:
float get_Battery_Voltage(void);                  // バッテリの電圧を取得するやつ:
int16_t get_Sensor_Level(uint8_t ch);             // LED を点滅させながら差分を取得し返すやつ:
uint8_t get_mode_number(void);                    // スイッチで選択したモードを取得するやつ:
void debug_ADC(void);                             // ADC の情報を読み取りシリアルに表示するやつ:
void flash_LED(void);                             // センサの値に応じて LED 光らせるやつ:
void boot_Motion(void);                           // 起動時の LED 表示をいい感じにするやつ:
void change_Motor_Voltage(int16_t L, int16_t R);  // モータ回すやつ:

/* ユーザが使用する関数 */
void run0();
void run1();
void run2();
void run3();
void run4();
void run5();
void run6();
void run7();
void run8();
void run9();
void run10();
void run11();
void run12();
void run13();
void run14();
void run15();

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.print("Program Started.\n");

  LED_init();
  ADC_init();
  IR_init();
  Motor_init(PHASE_ENABLE_MODE);
  SW_init();

  delay(125);
  boot_Motion();
  delay(125);
}

void loop() {
  switch(get_mode_number()) {
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
      run0();
      break;
  }
}

void LED_init(void) {
  /* 初期状態を設定 */
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);

  Serial.print("LED Initialized.\n");
}

void ADC_init(void) {
  /* 初期状態を設定 */
  pinMode(MCP3008_CS  , OUTPUT); digitalWrite(MCP3008_CS  , HIGH);  // Low-Active なので基本状態は HIGH:
  pinMode(MCP3008_CLK , OUTPUT); digitalWrite(MCP3008_CLK ,  LOW);  // 通常時はデータをセットしないので基本状態は LOW:
  pinMode(MCP3008_Din , OUTPUT); digitalWrite(MCP3008_Din ,  LOW);  // 同上:
  pinMode(MCP3008_Dout,  INPUT);                                    // 向こうが出力なのでこちらは入力:

  Serial.print("ADC Initialized.\n");
}

void IR_init(void) {
  pinMode(Tr, OUTPUT);

  Serial.print("IR Initialized.\n");
}

void Motor_init(uint8_t mode) {
  pinMode(DRV8835_AIN1, OUTPUT);
  pinMode(DRV8835_AIN2, OUTPUT);
  pinMode(DRV8835_BIN1, OUTPUT);
  pinMode(DRV8835_BIN2, OUTPUT);
  pinMode(DRV8835_MODE, OUTPUT); digitalWrite(DRV8835_MODE, mode);

  Serial.print("Motor Initialized.\n");
}

void SW_init(void) {
  pinMode(SW_MODE,  INPUT_PULLUP);
  pinMode(SW_BACK,  INPUT_PULLUP);

  Serial.print("SW Initialized.\n");
}

void LED_num(uint8_t num) {
  /* 対応するビットの値に合わせて LED を光らせる */
  digitalWrite(LED1, (num & 0b10000) == 0 ? LOW : HIGH);
  digitalWrite(LED2, (num & 0b01000) == 0 ? LOW : HIGH);
  digitalWrite(LED3, (num & 0b00100) == 0 ? LOW : HIGH);
  digitalWrite(LED4, (num & 0b00010) == 0 ? LOW : HIGH);
  digitalWrite(LED5, (num & 0b00001) == 0 ? LOW : HIGH);
}

uint16_t ADC_Read(uint8_t ch) {
  /* データシート通りに波形を作って読み取る */

  /* チャンネル指定シーケンス */
  digitalWrite(MCP3008_CS  ,  LOW);  // ADC を起動し通信を開始:
  toggle();
  digitalWrite(MCP3008_Din , HIGH);  // スタートビット送信:
  toggle();
  digitalWrite(MCP3008_Din , HIGH);  // SGL/DIFF を選択, Single-end なので常時 HIGH:
  toggle();
  digitalWrite(MCP3008_Din , (ch & 0b100) == 0 ? LOW : HIGH);  // ch 指定 D2:
  toggle();
  digitalWrite(MCP3008_Din , (ch & 0b010) == 0 ? LOW : HIGH);  // ch 指定 D1:
  toggle();
  digitalWrite(MCP3008_Din , (ch & 0b001) == 0 ? LOW : HIGH);  // ch 指定 D0:
  toggle();
  digitalWrite(MCP3008_Din ,  LOW);
  toggle();

  /* 読み出しシーケンス */
  uint16_t adc = 0;

  for(uint8_t i = 0; i < 10; i++) {
    toggle();
    adc  = adc << 1;
    adc += digitalRead(MCP3008_Dout);
  }

  /* 初期状態に戻す */
  digitalWrite(MCP3008_CS  , HIGH);
  digitalWrite(MCP3008_CLK ,  LOW);
  digitalWrite(MCP3008_Din ,  LOW);
  
  return adc;
}

void toggle(void) {
  delayMicroseconds(toggle_delay);
  digitalWrite(MCP3008_CLK , HIGH);
  delayMicroseconds(toggle_delay);
  digitalWrite(MCP3008_CLK ,  LOW);
}

float get_Battery_Voltage(void) {
  uint16_t adc = ADC_Read(V_Battery);
  float voltage = (adc * 3.3 * 5) / 1024.0;  // リファレンス電圧と入力前の分圧を考慮:
  return voltage;
}

int16_t get_Sensor_Level(uint8_t ch) {
  digitalWrite(Tr, HIGH);
  delayMicroseconds(10);
  uint16_t LEDisHIGH = ADC_Read(ch);
  digitalWrite(Tr,  LOW);
  delayMicroseconds(10);
  uint16_t LEDisLOW  = ADC_Read(ch);
   int16_t level = LEDisLOW - LEDisHIGH;
  return level;
}

uint8_t get_mode_number(void) {
  int8_t num = -1;
  uint16_t mode, back;
  bool number_isDetermined = false;

  Serial.print("function get_mode_number has called.\n");

  while(!number_isDetermined) {
    /* スイッチの状態を読み取って変数をいじる: */
    mode = (mode << 1) + digitalRead(SW_MODE);
    back = (back << 1) + digitalRead(SW_BACK);

    /* どちらかのスイッチが長押しされたらループを抜ける */
    if(mode == 0x8000 || back == 0x8000) {
      uint16_t flash_delay = 200;
      LED_num(0);
      delay(flash_delay);
      LED_num(15 << 1);
      delay(flash_delay);
      LED_num(0);
      delay(flash_delay);
      LED_num(15 << 1);
      delay(flash_delay);
      LED_num(0);
      
      number_isDetermined = true;
    }
    
    /* 現在の num の値を表示 */
    if(!number_isDetermined) {
      LED_num((num + 1) << 1);
    }

    /* MODE スイッチが押されたら num を加算 */
    if((mode & 0b111) == 0b100) {
      num += 1;
    }

    /* BACK スイッチが押されたら num を減算 */
    if((back & 0b111) == 0b100) {
      num -= 1;
    }
    
    /* num が範囲外に出たらもとに戻す */
    if(num >= 16) {
      num = 0;
    }

    if(num < 0) {
      num = 15;
    }

    delay(50);
  }

  char buffer[8];
  sprintf(buffer, "mode%2d\n", num);
  Serial.print(buffer);

  delay(1000);

  return num;
}

void debug_ADC(void) {
  static bool isAlreadyRunned = false;
  if(isAlreadyRunned) {
    // 何も実行しない:
  }else {
    Serial.print("\n\n");
    Serial.print("Sensor1, Sensor2, Sensor3, Sensor4, Sensor5, Battery voltage\n");
    Serial.print("------------------------------------------------------------\n");
    isAlreadyRunned = true;
  }
  char buffer[64];
  sprintf(buffer, "%7d, %7d, %7d, %7d, %7d, %11f [V]\n", get_Sensor_Level(V_Sensor1),
                                                         get_Sensor_Level(V_Sensor2),
                                                         get_Sensor_Level(V_Sensor3),
                                                         get_Sensor_Level(V_Sensor4),
                                                         get_Sensor_Level(V_Sensor5),
                                                         get_Battery_Voltage());
  Serial.print(buffer);
}

void flash_LED(void) {
  digitalWrite(LED1, (get_Sensor_Level(V_Sensor1) > Sensor_thres) ? HIGH : LOW);
  digitalWrite(LED2, (get_Sensor_Level(V_Sensor2) > Sensor_thres) ? HIGH : LOW);
  digitalWrite(LED3, (get_Sensor_Level(V_Sensor3) > Sensor_thres) ? HIGH : LOW);
  digitalWrite(LED4, (get_Sensor_Level(V_Sensor4) > Sensor_thres) ? HIGH : LOW);
  digitalWrite(LED5, (get_Sensor_Level(V_Sensor5) > Sensor_thres) ? HIGH : LOW);
}

void boot_Motion(void) {
  digitalWrite(LED1, HIGH);
  delay(50);
  digitalWrite(LED2, HIGH);
  delay(50);
  digitalWrite(LED3, HIGH);
  delay(50);
  digitalWrite(LED4, HIGH);
  delay(50);
  digitalWrite(LED5, HIGH);
  delay(500);
  
  digitalWrite(LED1,  LOW);
  delay(50);
  digitalWrite(LED2,  LOW);
  delay(50);
  digitalWrite(LED3,  LOW);
  delay(50);
  digitalWrite(LED4,  LOW);
  delay(50);
  digitalWrite(LED5,  LOW);
  delay(500);
}

void change_Motor_Voltage(int16_t L, int16_t R) {
  /* バッテリ電圧を取得。これにより、バッテリ電圧が変動しても指令値と出力電圧の整合性を保てる */
  float Vbat = get_Battery_Voltage();

  int16_t Vmotor_L, Vmotor_R;
  
  Vmotor_L = L / (1000 * 5);
  Vmotor_R = R / (1000 * 5);

  /* 左側のモータの制御 */
  if(Vmotor_L > 0) {
    analogWrite(DRV8835_AIN1,  Vmotor_L);
    analogWrite(DRV8835_AIN2,         0);
  }else if(Vmotor_L < 0) {
    analogWrite(DRV8835_AIN1,         0);
    analogWrite(DRV8835_AIN2, -Vmotor_L);
  }else if(Vmotor_L == 0) {
    analogWrite(DRV8835_AIN1,         0);
    analogWrite(DRV8835_AIN2,         0);
  }

  /* 右側のモータの制御 */
  if(Vmotor_R > 0) {
    analogWrite(DRV8835_BIN1,  Vmotor_R);
    analogWrite(DRV8835_BIN2,         0);
  }else if(Vmotor_R < 0) {
    analogWrite(DRV8835_BIN1,         0);
    analogWrite(DRV8835_BIN2, -Vmotor_R);
  }else if(Vmotor_R == 0) {
    analogWrite(DRV8835_BIN1,         0);
    analogWrite(DRV8835_BIN2,         0);
  }
}

void run0() {  // ADC デバッグ
  while(true) {
  debug_ADC();
  flash_LED();
  delay(10);
  }
}

void run1() {
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

void run13() {
}

void run14() {
}

void run15() {
}
