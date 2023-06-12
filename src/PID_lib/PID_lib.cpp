//
// Created by Edumarek on 12/02/2022.
//

#include "PID_lib.hpp"

//------------------CONSTRUTORES&DESTRUTORES-------------------//
ControladorPID::ControladorPID(_ponto tA, _ponto min_uK, _ponto max_uK)
{
    Ta = tA;
    min_Uk = min_uK;
    max_Uk = max_uK;
}

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

ControladorPID::ControladorPID(const ControladorPID &c)
{
    Kp = c.Kp;
    Kd = c.Kd;
    Ki = c.Ki;
    N = c.N;
    Ta = c.Ta;
    min_Uk = c.min_Uk;
    max_Uk = c.max_Uk;
}

ControladorPID::~ControladorPID()
{
}

//---------------------------METODOS---------------------------//

#if defined __linux__
void ControladorPID::Imprimir_Parametros()
{
    std::cout << "Kp: " << Kp << " | Kd: " << Kd << " | Ki: " << Ki << " | N: " << N << " | Ta: " << Ta << std::endl;
}
#else
void ControladorPID::Imprimir_Parametros()
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
#endif

#if defined __linux__
_ponto ControladorPID::Calcula_Acao_Controle(_ponto referencia, _ponto saida)
{
    clock_t tInicio = clock();
    double a, b, c, d, e;

    // Calcula erro
    Yk[0] = saida;
    Ek[0] = referencia - saida;

    // Calcula acao de controle
    a = Kp * (1 + (N * Ta)) + (Ki * Ta) * (1 + (N * Ta)) + (Kd * N);
    b = -(Kp * (2 + (N * Ta)) + (Ki * Ta) + (2 * Kd * N));
    c = Kp + (Kd * N);
    d = 1 + (N * Ta);
    e = -(2 + (N * Ta));

    Uk[0] = (_ponto)((-e * Uk[1]) - (1 * Uk[2]) + (a * Ek[0]) + (b * Ek[1]) + (c * Ek[2])) / d; // by mateus

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

    // Atualizacao parametros
    iteracoesAcaoControle++;
    if (iteracoesAcaoControle >= limiteIteracoesAcaoControle)
    {
        iteracoesAcaoControle = 0;
        Atualiza_Parametros();
        Imprimir_Parametros();
    }

    if (calcularTempoExecucao)
        tExecucao = (unsigned long)((1000 * (tInicio - clock())) / CLOCKS_PER_SEC);

    return Uk[0];
}

#else
_ponto ControladorPID::Calcula_Acao_Controle(_ponto referencia, _ponto saida)
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

    // Atualizacao parametros
    if (imprimirParametros)
        Imprimir_Parametros();

    if (calcularTempoExecucao)
        tExecucao = micros() - tInicio;

    return Uk[0];
}
#endif