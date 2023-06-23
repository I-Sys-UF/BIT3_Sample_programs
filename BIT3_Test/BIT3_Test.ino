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

#define V_Battery 7

#define Tr 15

#define SW_MODE 17
#define SW_BACK 16

/* 各種定数設定 */
#define times 50
#define toggle_delay 100

/* 関数のプロトタイプ宣言 */
void LED_Init(void);            // LED を使えるようにするやつ:
void ADC_Init(void);            // ADC を使えるようにするやつ:
void LED_num(uint8_t num);      // 数字を指定してバイナリで表示するやつ:
uint16_t ADC_Read(uint8_t ch);  // チャンネルを指定して ADC 読むやつ:
void toggle(void);              // GPIO をただトグルさせるだけのやつ:

void setup() {
  pinMode(Tr, OUTPUT);
  LED_Init();
  ADC_Init();
  Serial.begin(115200);
}

void loop() {
  char buffer[32];
  sprintf(buffer, "%4d, %4d, %4d, %4d, %4d, %4d, %4d, %4d\n", ADC_Read(0), ADC_Read(1), ADC_Read(2), ADC_Read(3), ADC_Read(4), ADC_Read(5), ADC_Read(6), ADC_Read(7));
  Serial.print(buffer);

  delay(500);

  digitalWrite(Tr, HIGH);

  sprintf(buffer, "%4d, %4d, %4d, %4d, %4d, %4d, %4d, %4d\n", ADC_Read(0), ADC_Read(1), ADC_Read(2), ADC_Read(3), ADC_Read(4), ADC_Read(5), ADC_Read(6), ADC_Read(7));
  Serial.print(buffer);

  delay(500);

  digitalWrite(Tr,  LOW);
}

void LED_Init(void) {
  /* 初期状態を設定 */
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
}

void ADC_Init(void) {
  /* 初期状態を設定 */
  pinMode(MCP3008_CS  , OUTPUT); digitalWrite(MCP3008_CS  , HIGH);  // Low-Active なので基本状態は HIGH:
  pinMode(MCP3008_CLK , OUTPUT); digitalWrite(MCP3008_CLK ,  LOW);  // 通常時はデータをセットしないので基本状態は  LOW:
  pinMode(MCP3008_Din , OUTPUT); digitalWrite(MCP3008_Dout,  LOW);  // 同上:
  pinMode(MCP3008_Dout,  INPUT);
}

void LED_num(uint8_t num) {
  /* 対応するビットの値に合わせて LED を光らせる */
  digitalWrite(LED1, (num & 0b00001) == 0 ? LOW : HIGH);
  digitalWrite(LED2, (num & 0b00010) == 0 ? LOW : HIGH);
  digitalWrite(LED3, (num & 0b00100) == 0 ? LOW : HIGH);
  digitalWrite(LED4, (num & 0b01000) == 0 ? LOW : HIGH);
  digitalWrite(LED5, (num & 0b10000) == 0 ? LOW : HIGH);
}

uint16_t ADC_Read(uint8_t ch) {
  /* データシート通りに波形を作って読み取る */

  /* チャンネル指定シーケンス */
  digitalWrite(MCP3008_CS  ,  LOW);  // ADC との通信を開始:
  toggle();
  digitalWrite(MCP3008_Din , HIGH);  // スタートビット:
  toggle();
  digitalWrite(MCP3008_Din , HIGH);  // SGL/DIFF, Single-end なので常時 HIGH:
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


  ADC_Init();  // 初期状態に戻す:

  return adc;
}

void toggle(void) {
  delayMicroseconds(toggle_delay);
  digitalWrite(MCP3008_CLK , HIGH);
  delayMicroseconds(toggle_delay);
  digitalWrite(MCP3008_CLK ,  LOW);
}
