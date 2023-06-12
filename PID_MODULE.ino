#include "src/PID_lib/PID_lib.hpp"

// Pinos
#define PIN_MOTOR1 25
#define PIN_MOTOR2 33
#define PIN_SENSOR_ANALOGICO 2
#define PIN_REFERENCIA_ANALOGICO 4

// PWM
#define RESOLUCAO_PWM = 10;
#define CANAL_PWM = 0;
const int MIN_PWM = 0;
const int MAX_PWM = pow(2, RESOLUCAO_PWM) - 1;
int FREQUENCIA_PWM = 5000;

// Sensor
double VALOR_SENSOR = 0;

// Sensor Analogico
#define RESOLUCAO_ADC 12
double SINAL_A0_MAX = pow(2, RESOLUCAO_ADC) - 1; // bits
double SINAL_A0_MIN = 0;                         // bits

// Sensor Pulsos
double COEFICIENTE_PARA_RPM = 1;

// Parametros Controlador
double KP = 0;
double KD = 0;
double KI = 0;
double N = 0;
double TA = 0.1; // segundos
double UK_MIN = 0;
double UK_MAX = 1;

// Controlador PID
ControladorPID CC(KP, KD, KI, N, TA, UK_MIN, UK_MAX);

// Referencia
double REFERENCIA = 0;

// Referencia Analogico
#define RESOLUCAO_REFERENCIA 12
double SINAL_REFERENCIA_MIN = 0;
double SINAL_REFERENCIA_MAX = pow(2, RESOLUCAO_REFERENCIA) - 1; // bits

// Variaveis Globais

// Funcoes
double leitura_sensor_analogico();
double leitura_sensor_pulsos();
double leitura_referencia_analogico();
void comuta_acao_controle(double valor);
void altera_parametros_bt();

void imprimi_informacoes(double distanciaAtual, double sinalControle, double referencia, bool monitorSerial);

void setup()
{
    // SERIAL
    Serial.begin(500000);

    // PINOS
    pinMode(PIN_MOTOR2, OUTPUT);
    digitalWrite(PIN_MOTOR2, HIGH);

    // PWM
    ledcAttachPin(PIN_MOTOR1, CANAL_PWM);
    ledcSetup(CANAL_PWM, FREQUENCIA_PWM, RESOLUCAO_PWM);
    comuta_acao_controle(UK_MIN);

    // SENSOR

    delay(500);
}

void loop()
{
    if (0) // Leitura BT
        altera_parametros_bt();

    VALOR_SENSOR = leitura_sensor_analogico();

    double sinalControle = CC.Calcula_Acao_Controle(REFERENCIA, VALOR_SENSOR);

    comuta_acao_controle(sinalControle);

    // imprimi_informacoes(distanciaAtual, 100 * sinalControle, REFERENCIA, false);
}

double leitura_sensor_analogico()
{
    double leitura = (analogRead(PIN_SENSOR_ANALOGICO);
    leitura -= SINAL_A0_MIN;
    leitura /= SINAL_A0_MAX;

    return leitura;
}

double leitura_referencia_analogico()
{
    double leitura = (analogRead(PIN_REFERENCIA_ANALOGICO);
    leitura -= SINAL_REFERENCIA_MIN;
    leitura /= SINAL_REFERENCIA_MAX;

    return leitura;
}

void comuta_acao_controle(double valor)
{
    valor = MAX_PWM * (1 - valor);
    ledcWrite(CANAL_PWM, (int)valor);
}

void altera_parametros_bt()
{
}

void imprimi_informacoes(double distanciaAtual, double sinalControle, double referencia, bool monitorSerial)
{
    Serial.print("Max ");
    Serial.print(DISTANCIA_MAX);
    Serial.print(" Min ");
    Serial.print(DISTANCIA_MIN);
    Serial.print(" Referenncia ");
    Serial.print(referencia);
    Serial.print(" Atual ");
    Serial.print(distanciaAtual);
    Serial.print(" Sinalcontrole ");
    Serial.print(sinalControle);
    Serial.print(" TA ");
    Serial.print(micros() - T_PASSADO);
    Serial.println("");
}
