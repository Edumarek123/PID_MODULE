//
// Created by Edumarek on 13/06/2023.
//

#include "leitura_adc_lib.hpp"

//------------------CONSTRUTORES&DESTRUTORES-------------------//
LeituraADC::LeituraADC(int _porta, double _coeficiente)
{
    porta = _porta;
    coeficienteConversao = _coeficiente;
}

//---------------------------METODOS---------------------------//
void LeituraADC::prepara_porta()
{
    pinMode(porta, OUTPUT);
}

double LeituraADC::realiza_leitura()
{
    return (analogRead(porta) * coeficienteConversao);
}
