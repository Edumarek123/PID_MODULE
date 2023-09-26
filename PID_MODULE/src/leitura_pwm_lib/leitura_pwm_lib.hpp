//
// Created by Edumarek on 13/06/2023.
//

#ifndef LEITURA_PWM_LIB_HPP
#define LEITURA_PWM_LIB_HPP

#include <Arduino.h>

struct LeituraPWM
{
    // variaveis
    double porta;
    double valor;
    double coeficienteConversao;
    long tempoAnterior;

    void (*interrupcao)();

    // construtores e metodos
    LeituraPWM(int _porta, double _coeficiente);
    void prepara_porta(void (*interrupcao)(void));
    void calcula_valor();
    double realiza_leitura();
};

#endif // LEITURA_PWM_LIB_HPP
