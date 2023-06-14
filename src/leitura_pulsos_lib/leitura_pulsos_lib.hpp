//
// Created by Edumarek on 13/06/2023.
//

#ifndef LEITURA_PULSOS_LIB_HPP
#define LEITURA_PULSOS_LIB_HPP

#include <Arduino.h>

struct LeituraPulsos
{
    // variaveis
    double porta;
    double coeficienteConversao;
    long tempoAnterior;

    unsigned long pulsos;
    void (*interrupcao)();

    // construtores e metodos
    LeituraPulsos(int _porta, double _coeficiente);
    void prepara_porta(void (*interrupcao)(void));
    void soma_pulsos();
    double realiza_leitura();
};

#endif // LEITURA_PULSOS_LIB_HPP
