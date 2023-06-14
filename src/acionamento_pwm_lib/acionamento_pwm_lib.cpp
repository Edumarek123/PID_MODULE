//
// Created by Edumarek on 13/06/2023.
//

#include "acionamento_pwm_lib.hpp"

//------------------CONSTRUTORES&DESTRUTORES-------------------//
AcionamentoPWM::AcionamentoPWM(int _porta, double _coeficiente, int _canal, int _frequencia, int _resolucao)
{
    porta = _porta;
    coeficienteConversao = _coeficiente;
    canal = _canal;
    frequencia = _frequencia;
    resolucao = _resolucao;
    minPWM = 0;
    maxPWM = pow(2, resolucao) - 1;
}

//---------------------------METODOS---------------------------//
void AcionamentoPWM::prepara_porta()
{
    ledcAttachPin(porta, canal);
    ledcSetup(canal, frequencia, resolucao);
}

void AcionamentoPWM::comuta_valor(double valor)
{
    valor *= coeficienteConversao;
    valor *= maxPWM;

    // limitador
    if (valor > maxPWM)
        valor = maxPWM;
    if (valor < minPWM)
        valor = minPWM;

    ledcWrite(canal, (int)valor);
}
