#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

#define FAIL_IF_MSG( EXP , MSG) ( {if( EXP == -1) {printf(MSG);exit(EXIT_FAILURE);}} )

void calcul_tata();
void generare_fiu1();
void generare_fiu2();

int N=0;

int main()
{
	pid_t pid_fiu1, pid_fiu2;

	printf("Dati numarul intreg N:");
	scanf("%d", &N); // TODO: tratați excepțiile de citire a unui număr întreg


	/* Crearea procesului fiu #1. */
	if(-1 == (pid_fiu1=fork()) )
	{
		perror("Eroare la fork #1");  return 1;
	}

	/* Ramificarea execuției după primul apel fork. */
	if(pid_fiu1 == 0)
	{   /* Zona de cod executată doar de către fiul #1. */
		printf("\n[P1] Procesul fiu 1, cu PID-ul: %d.\n", getpid());

		generare_fiu1();
		return 0;
	}
	else
	{   /* Zona de cod executată doar de către părinte. */
		printf("\n[P0] Procesul tata, cu PID-ul: %d.\n", getpid());

		/* Crearea procesului fiu #2. */
		if(-1 == (pid_fiu2=fork()) )
		{
			perror("Eroare la fork #2");  return 2;
		}
		
		/* Ramificarea execuției după al doilea apel fork. */
		if(pid_fiu2 == 0)
		{   /* Zona de cod executată doar de către fiul #2. */
			printf("\n[P2] Procesul fiu 2, cu PID-ul: %d.\n", getpid());

			generare_fiu2();
			return 0;
		}
		else
		{   /* Zona de cod executată doar de către părinte. */

			wait(NULL);
			wait(NULL);
			// Mai întâi aștept terminarea ambilor fii, iar apoi fac calculul.

			calcul_tata();
			return 0;
		}
	}
}

void calcul_tata()
{
    int operanzi;
    FAIL_IF_MSG((operanzi = open("operanzi.bin", O_RDWR)), "Eroare la deschidere\n");    

    int operatori;
    FAIL_IF_MSG((operatori = open("operatori.txt", O_RDWR)), "Eroare la deschidere\n");

    int firstNum, secondNum;
    char op;
    int rez;
    for(int i = 0; i < N; i++)
    {
        FAIL_IF_MSG( read(operanzi, &firstNum, 4), "Eroare la citirea primului numar!\n");
        FAIL_IF_MSG( read(operanzi, &secondNum, 4), "Eroare la citirea numarului 2!\n");
        FAIL_IF_MSG( read(operatori, &op, 1), "Eroare la citirea operatorului!\n");

        switch (op)
        {
        case '+':
            rez = firstNum + secondNum;
            printf("%d %c %d = %d\n", firstNum, op, secondNum, rez);
            break;
        
        case '-':
            rez = firstNum - secondNum;
            printf("%d %c %d = %d\n", firstNum, op, secondNum, rez);
            break;

        case '*':
            rez = firstNum * secondNum;
            printf("%d %c %d = %d\n", firstNum, op, secondNum, rez);
            break;

        case '/':
            if(secondNum != 0)
            {
                rez = firstNum/secondNum;
                printf("%d %c %d = %d\n", firstNum, op, secondNum, rez);
                break;                
            }
            else
            {
                printf("%d %c %d = 0\n", firstNum, op, secondNum);
                break;
            }

        default:
            break;
        }
    }


    FAIL_IF_MSG(close(operanzi), "Eroare la inchidere\n");
    FAIL_IF_MSG(close(operatori), "Eroare la inchidere\n");
}

void generare_fiu1()
{
    srand((unsigned int)time(NULL));
	int operanzi;
    FAIL_IF_MSG((operanzi = open("operanzi.bin", O_RDWR | O_TRUNC)), "Eroare la deschidere\n");

    for(int i = 0; i < N; i++)
    {
        int rand1 = rand() %1000;
        int rand2 = rand() %1000;

        int writtenBits;
        FAIL_IF_MSG( (writtenBits = write(operanzi, &rand1, 4) != 4) , "Error at writing!\n");
        FAIL_IF_MSG( (writtenBits = write(operanzi, &rand2, 4) != 4) , "Error at writing!\n");
    }

    FAIL_IF_MSG(close(operanzi), "Eroare la inchidere\n");
}

void generare_fiu2()
{
    int operatori;
    FAIL_IF_MSG((operatori = open("operatori.txt", O_RDWR | O_TRUNC)), "Eroare la deschidere\n");

    char elemente[4] = {'+', '-', '*', '/'};
    srand((unsigned int)time(NULL));
    int bytesOut;
	for(int i = 0; i < N; i++)
    {
        int random = rand() % 4;
        FAIL_IF_MSG( (bytesOut = write(operatori, elemente + random, 1)) , "Error at writing the operators!\n");
    }

    FAIL_IF_MSG(close(operatori), "Eroare la inchidere\n");
}