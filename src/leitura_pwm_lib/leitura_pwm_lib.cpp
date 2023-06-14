//
// Created by Edumarek on 13/06/2023.
//

#include "leitura_pwm_lib.hpp"

//------------------CONSTRUTORES&DESTRUTORES-------------------//
LeituraPWM::LeituraPWM(int _porta, double _coeficiente)
{
    porta = _porta;
    coeficienteConversao = _coeficiente;
    tempoAnterior = micros();
}

//---------------------------METODOS---------------------------//
void LeituraPWM::prepara_porta(void (*interrupcao)(void))
{
    pinMode(porta, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(porta), interrupcao, CHANGE);
}

void LeituraPWM::calcula_valor()
{
    long tempoAtual = micros();

    if (tempoAtual > tempoAnterior)
    {
        valor = (tempoAtual - tempoAnterior) * coeficienteConversao;
        valor /= 1000;         // us to ms
        valor = 19.01 - valor; // logica servo
        tempoAnterior = tempoAtual;

        // limitador
        if (valor > 1)
            valor = 1;
        if (valor < 0)
            valor = 0;
    }
}

double LeituraPWM::realiza_leitura()
{
    return valor;
}
