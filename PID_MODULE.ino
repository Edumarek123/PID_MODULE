#include "src/leitura_adc_lib/leitura_adc_lib.hpp"
#include "src/leitura_pulsos_lib/leitura_pulsos_lib.hpp"
#include "src/leitura_pwm_lib/leitura_pwm_lib.hpp"
#include "src/acionamento_pwm_lib/acionamento_pwm_lib.hpp"
#include "src/PID_lib/PID_lib.hpp"
#include "BluetoothSerial.h"

// Pinos
#define PIN_Y 23
#define PIN_H 2
#define PIN_R 4

//BLUETOOTH
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;

//VARIAVEIS C PWM
double COEFICIENTE_C = 1;
const int CANAL_C = 0;
double FREQUENCIA_C = 1000;
double RESOLUCAO_C = 10;

//VARIAVEIS H
double COEFICIENTE_H = 1;
int MODO_H = 0;
volatile double VALOR_H = 0;

//VARIAVEIS R
double COEFICIENTE_R = 1;
int MODO_R = 0;
volatile double VALOR_R = 0;

//VARIAVEIS C
double KP = 1;
double KI = 1;
double KD = 1;
double N = 1;
double TA = 20;
double MIN_UK = 0;
double MAX_UK = 0;
double VALOR_UK = MIN_UK;

//SAIDA PWM
AcionamentoPWM SAIDA_PWM(PIN_Y, COEFICIENTE_C, CANAL_C, FREQUENCIA_C, 10);

//SENSORES
LeituraADC SENSOR_ADC(PIN_H, COEFICIENTE_H);
LeituraPulsos SENSOR_PULSOS(PIN_H, COEFICIENTE_H);
LeituraPWM SENSOR_PWM(PIN_H, COEFICIENTE_H);

//REFERENCIAS
LeituraADC REFERENCIA_ADC(PIN_R, COEFICIENTE_R);
LeituraPWM REFERENCIA_PWM(PIN_R, COEFICIENTE_R);

//CONTROLADOR
ControladorPID CONTROLADOR_PID(KP, KI, KD, N, TA, MIN_UK, MAX_UK);

//TIMER
const int CANAL_TIMER = 3;
hw_timer_t * TIMER_CALCULOS = NULL;
unsigned long TICKS_TIMER = 1000000 * TA;

//FUNCOES
void leitura_H();
void leitura_R();
void altera_modo_H(int novoModo);
void altera_modo_R(int novoModo);
void envia_parametros_bluetooth();
void recebe_parametros_bluetooth();


void IRAM_ATTR calculos(){
  // leitura_H();
  // leitura_R();

  // VALOR_UK = CONTROLADOR_PID.calcula_acao_controle(VALOR_R, VALOR_H);
  // SAIDA_PWM.comuta_valor(1);

  envia_parametros_bluetooth();
}


void setup()
{
  // SERIAL
  Serial.begin(115200);
  SerialBT.begin("PID_MODULE");

  //TIMER
  TIMER_CALCULOS = timerBegin(CANAL_TIMER, 240, true);
  timerAttachInterrupt(TIMER_CALCULOS, &calculos, true);
  timerAlarmWrite(TIMER_CALCULOS, TICKS_TIMER, true); 
  timerAlarmEnable(TIMER_CALCULOS);   

  // altera_modo_H();
  altera_modo_R(MODO_R);

  delay(500);
}

void loop()
{
  recebe_parametros_bluetooth();

  delay(100);
}

void leitura_H()
{
  switch (MODO_H)
  {
    case 0:
      VALOR_H = SENSOR_ADC.realiza_leitura();
      break;
    case 1:
      VALOR_H = SENSOR_PULSOS.realiza_leitura();
      break;
    case 2:
      VALOR_H = SENSOR_PWM.realiza_leitura();
      break;
    default:
        VALOR_H = -127;
  }
}

void leitura_R()
{
  switch (MODO_R)
  {
    case 0:
      VALOR_R = REFERENCIA_ADC.realiza_leitura();
      break;
    case 1:
      VALOR_R = REFERENCIA_PWM.realiza_leitura();
      break;
    default:
      VALOR_R = -127;
  }
}

void altera_modo_R(int novoModo)
{
  MODO_R = novoModo;
  switch (MODO_R)
  {
    case 0:
      REFERENCIA_ADC.prepara_porta();
      break;
    case 1:
      REFERENCIA_PWM.prepara_porta([]{REFERENCIA_PWM.calcula_valor();});
      break;
    default:
      //erro
      Serial.println("Erro");
  }
}

void altera_modo_H(int novoModo)
{
  MODO_H = novoModo;
  switch (MODO_H)
  {
    case 0:
      SENSOR_ADC.prepara_porta();
      break;
    case 1:
      SENSOR_PULSOS.prepara_porta([]{SENSOR_PULSOS.soma_pulsos();});
      break;
    case 2:
      SAIDA_PWM.prepara_porta();
      break;
    default:
      //erro
      Serial.println("Erro");
  }
}

void envia_parametros_bluetooth()
{
  SerialBT.println("PARAMETROS");
  SerialBT.println(String(COEFICIENTE_C) + " " + String(COEFICIENTE_H) + " " + String(COEFICIENTE_R));
  SerialBT.println(String(MODO_H) + " " + String(MODO_R));
  SerialBT.println(String(KP) + " " + String(KI) + " " + String(KD) + " " + String(N) + " " + String(TA) + " " + String(MIN_UK) + " " + String(MAX_UK));
  SerialBT.println(String(FREQUENCIA_C));
}

void recebe_parametros_bluetooth()
{
  if (SerialBT.available()) {
    String informacao = SerialBT.readString();
    Serial.println(informacao);
  }
}
