//
// Created by Edumarek on 13/06/2023.
//

#include "leitura_pulsos_lib.hpp"

//------------------CONSTRUTORES&DESTRUTORES-------------------//
LeituraPulsos::LeituraPulsos(int _porta, double _coeficiente)
{
    porta = _porta;
    pulsos = 0;
    tempoAnterior = micros();
    coeficienteConversao = _coeficiente;
}

//---------------------------FUNCOES---------------------------//
void LeituraPulsos::prepara_porta(void (*interrupcao)(void))
{
    pinMode(porta, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(porta), interrupcao, 2);
}

void LeituraPulsos::soma_pulsos()
{
    pulsos++;
}

double LeituraPulsos::realiza_leitura()
{
    long tempoAtual = micros();
    long deltaTempo = (tempoAtual - tempoAnterior);
    tempoAnterior = tempoAtual;

    double valor = (coeficienteConversao * pulsos) * deltaTempo;
    valor /= 1000000;
    pulsos = 0;

    return valor;
}
