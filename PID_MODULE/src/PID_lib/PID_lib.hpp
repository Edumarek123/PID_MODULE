//
// Created by Edumarek on 12/02/2022.
//

#ifndef PID_LIB_HPP
#define PID_LIB_HPP

#include <Arduino.h>

#define _ponto double

struct ControladorPID
{
    // parametros controlador
    _ponto Kp = 0;
    _ponto Kd = 0;
    _ponto Ki = 0;
    _ponto N = 1;
    _ponto Ta = 1;
    _ponto min_Uk = 0;
    _ponto max_Uk = 1;
    // variaveis controlador
    _ponto Ek[3] = {0, 0, 0}; // erro
    _ponto Uk[3] = {0, 0, 0}; // acao de controle
    _ponto Yk[2] = {0, 0};    // Saida
    // variaveis auxiliares
    unsigned long tExecucao = 0; // ms

    ControladorPID(_ponto kP, _ponto kD, _ponto tI, _ponto N, _ponto tA, _ponto min_uK, _ponto max_uK);
    void imprimir_parametros();
    _ponto calcula_acao_controle(_ponto referencia, _ponto saida);
};

#endif // PID_LIB_HPP
