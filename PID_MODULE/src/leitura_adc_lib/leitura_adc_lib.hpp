//
// Created by Edumarek on 13/06/2023.
//

#ifndef LEITURA_ADC_LIB_HPP
#define LEITURA_ADC_LIB_HPP

#include <Arduino.h>

struct LeituraADC
{
    // variaveis
    double porta;
    double coeficienteConversao;

    // construtores e metodos
    LeituraADC(int _porta, double _coeficiente);
    void prepara_porta();
    double realiza_leitura();
};

#endif // LEITURA_ADC_LIB_HPP
