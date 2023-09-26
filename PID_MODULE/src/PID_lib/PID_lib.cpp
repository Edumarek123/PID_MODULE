//
// Created by Edumarek on 12/02/2022.
//

#include "PID_lib.hpp"

//------------------CONSTRUTORES&DESTRUTORES-------------------//
ControladorPID::ControladorPID(_ponto kP, _ponto kD, _ponto kI, _ponto n, _ponto tA, _ponto min_uK, _ponto max_uK)
{
    Kp = kP;
    Kd = kD;
    Ki = kI;
    Ta = tA;
    N = n;
    min_Uk = min_uK;
    max_Uk = max_uK;
}
//---------------------------METODOS---------------------------//
void ControladorPID::imprimir_parametros()
{
    Serial.print("Kp: ");
    Serial.print(Kp, 4);
    Serial.print(" | Kd: ");
    Serial.print(Kd, 4);
    Serial.print(" | Ki: ");
    Serial.print(Ki, 4);
    Serial.print(" | N: ");
    Serial.print(N, 2);
    Serial.print("\n");
}
_ponto ControladorPID::calcula_acao_controle(_ponto referencia, _ponto saida)
{
    unsigned long tInicio = micros();
    _ponto a, b, c, d, e, f;

    // Calcula erro
    Yk[0] = saida;
    Ek[0] = referencia - saida;

    // Calcula acao de controle
    a = (4 * Kp) + (4 * Kd * N) + (2 * Ki * Ta) + (2 * Kp * N * Ta) + (Ki * N * Ta * Ta);
    b = (2 * Ki * N * Ta * Ta) - (8 * Kd * N) - (8 * Kp);
    c = (4 * Kp) + (4 * Kd * N) - (2 * Ki * Ta) - (2 * Kp * N * Ta) + (Ki * N * Ta * Ta);
    d = -8;
    e = 4 - (2 * Ta * N);
    f = 4 + (2 * N * Ta);

    Uk[0] = ((Ek[0] * a) + (Ek[1] * b) + (Ek[2] * c) - (Uk[1] * d) - (Uk[2] * e)) / f;

    // Limitador acao controle
    if (Uk[0] > max_Uk)
        Uk[0] = max_Uk;
    if (Uk[0] <= min_Uk)
        Uk[0] = min_Uk;

    // Atualiza Saidas
    Yk[1] = Yk[0];

    // Atualiza erros
    Ek[2] = Ek[1];
    Ek[1] = Ek[0];

    // Atualiza acoes de controle
    Uk[1] = Uk[0];
    Uk[2] = Uk[1];

    tExecucao = micros() - tInicio;

    return Uk[0];
}