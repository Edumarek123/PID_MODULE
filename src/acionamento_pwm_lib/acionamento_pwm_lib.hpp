//
// Created by Edumarek on 13/06/2023.
//

#ifndef ACIONAMENTO_PWM_LIB_HPP
#define ACIONAMENTO_PWM_LIB_HPP

#include <Arduino.h>

struct AcionamentoPWM
{
    // variaveis
    double porta;
    double coeficienteConversao;

    int maxPWM;
    int minPWM;
    int canal;
    int frequencia;
    int resolucao;

    // construtores e metodos
    AcionamentoPWM(int _porta, double _coeficiente, int _canal, int _frequencia, int _resolucao);
    void prepara_porta();
    void comuta_valor(double valor);
};

#endif // ACIONAMENTO_PWM_LIB_HPP
