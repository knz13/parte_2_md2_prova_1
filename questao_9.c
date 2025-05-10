#include <stdio.h>
#include <math.h>

struct resultado_euclides_extendido
{
    int x;
    int y;
};

typedef struct resultado_euclides_extendido resultado_euclides_extendido;

int calcular_mod(int a, int z)
{
    return a % z;
}

int calcular_div(int a, int z)
{
    return a / z;
}

int calcular_mdc(int a, int b)
{
    while (b != 0)
    {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int eh_primo(int n)
{
    if (n <= 1)
        return 0;
    if (n <= 3)
        return 1;
    if (n % 2 == 0 || n % 3 == 0)
        return 0;

    for (int i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
            return 0;
    }
    return 1;
}

int calcular_totiente(int n)
{
    int resultado = n;
    for (int i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
        {
            while (n % i == 0)
            {
                n /= i;
            }
            resultado -= resultado / i;
        }
    }
    if (n > 1)
    {
        resultado -= resultado / n;
    }
    return resultado;
}

int potencia_modular(int base, int expoente, int modulo)
{
    int resultado = 1;
    base = base % modulo;
    while (expoente > 0)
    {
        if (expoente & 1)
        {
            resultado = (resultado * base) % modulo;
        }
        base = (base * base) % modulo;
        expoente >>= 1;
    }
    return resultado;
}

resultado_euclides_extendido calcular_euclides_extendido(int a, int b, int *mdc)
{
    int valoresX[100];
    int valoresY[100];
    int valoresA[100];
    int valoresB[100];
    int currentIndex = 0;
    int currentA = a;
    int currentB = b;

    while (1)
    {
        int tempX = calcular_div(currentA, currentB);
        int tempY = calcular_mod(currentA, currentB);

        if (tempY == 0)
        {
            break;
        }

        valoresX[currentIndex] = tempX;
        valoresY[currentIndex] = tempY;
        valoresA[currentIndex] = currentA;
        valoresB[currentIndex] = currentB;

        currentA = currentB;
        currentB = tempY;
        currentIndex++;
    }
    *mdc = valoresY[currentIndex - 1];

    if (valoresY[currentIndex - 1] != 1)
    {
        resultado_euclides_extendido resultado;
        resultado.x = -1;
        resultado.y = -1;

        // não são primos entre si;

        return resultado;
    }

    // voltando

    // Parte para trás - calculando coeficientes
    resultado_euclides_extendido resultado;
    resultado.x = 0;
    resultado.y = 1;

    for (int i = currentIndex - 1; i >= 0; i--)
    {
        int temp = resultado.x;
        resultado.x = resultado.y;
        resultado.y = temp - valoresX[i] * resultado.y;
    }

    return resultado;
}

int main()
{
    int h, g, n;
    printf("Digite os valores de h, g e n: ");
    scanf("%d %d %d", &h, &g, &n);
    printf("\nValores recebidos: h = %d, g = %d, n = %d\n", h, g, n);

    // 1. Verificar se G e n são primos
    printf("\n1. Verificando se %d e %d são primos entre si...\n", g, n);
    int mdc;
    resultado_euclides_extendido resultado = calcular_euclides_extendido(n, g, &mdc);

    if (resultado.x == -1 && resultado.y == -1)
    {
        printf("Não é possível calcular a divisão pois o mdc(%d,%d) = %d, e %d é diferente de 1\n", n, g, mdc, mdc);
        return 0;
    }
    printf("mdc(%d,%d) = %d, logo são primos entre si!\n", n, g, mdc);

    // 2. Calcular o inverso de G em Zn
    printf("\n2. Calculando o inverso de %d em Z%d...\n", g, n);
    int g_inverso = resultado.y;
    if (g_inverso < 0)
    {
        g_inverso = (g_inverso % n + n) % n;
    }
    printf("O inverso de %d em Z%d é %d\n", g, n, g_inverso);

    // 3. Dividir H por G
    printf("\n3. Calculando %d/%d em Z%d...\n", h, g, n);
    int a = (h * g_inverso) % n;
    if (a < 0)
    {
        a = (a % n + n) % n;
    }
    printf("Resultado da divisão: %d\n", a);

    // 4. Verificar se a e n1 são coprimos
    printf("\n4. Verificando se %d e n1 são coprimos...\n", a);
    int n1;
    printf("Digite o valor de n1: ");
    scanf("%d", &n1);
    printf("n1 = %d\n", n1);

    int mdc_a_n1 = calcular_mdc(a, n1);
    if (mdc_a_n1 != 1)
    {
        printf("a e n1 não são coprimos (mdc = %d)\n", mdc_a_n1);
        return 0;
    }
    printf("mdc(%d,%d) = %d, logo são coprimos!\n", a, n1, mdc_a_n1);

    // 5. Verificar se n1 é primo
    printf("\n5. Verificando se %d é primo...\n", n1);
    int x1;
    if (eh_primo(n1))
    {
        printf("%d é primo!\n", n1);
        // 6. Aplicar Pequeno Teorema de Fermat
        x1 = n1 - 1;
        printf("6. Aplicando o Pequeno Teorema de Fermat: x1 = %d - 1 = %d\n", n1, x1);
    }
    else
    {
        printf("%d não é primo!\n", n1);
        // 7. Aplicar Teorema de Euler
        x1 = calcular_totiente(n1);
        printf("6. Aplicando o Teorema de Euler: x1 = φ(%d) = %d\n", n1, x1);
    }

    // Calcular o resultado final
    printf("\nCalculando %d^%d mod %d...\n", a, x1, n1);
    int resultado_final = potencia_modular(a, x1, n1);
    printf("\nResultado final: %d\n", resultado_final);

    return 0;
}