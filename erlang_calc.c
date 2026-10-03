#include <stdio.h>
#include <math.h>


//ERLANG B
//---------------------------------------------------------------------------------------
double erlangB(double A, int C)
{
    double den = 0;

    for (int k = 0; k <= C; k++) {
        den += pow(A, k) / tgamma(k + 1);
    }

    return (pow(A, C) / tgamma(C + 1)) / den;
}


//FIND A
double findA(double target, int C)
{
    double low = 0;
    double high = 1;
    double mid;

    while (erlangB(high, C) < target) {
        high *= 2;
    }

    while ((high - low) > 0.000001) {

        mid = (low + high) / 2;

        double P = erlangB(mid, C);

        if (P < target) {
            low = mid;
        }
        else {
            high = mid;
        }
    }

    return (low + high) / 2;
}

//FIND C
int findC(double A, double target)
{
    int C = 1;

    while (erlangB(A, C) > target) {
        C++;
    }

    return C;
}


//ERLANG C
//---------------------------------------------------------------------------
double erlangC(double A, int C)
{
    if (A <= 0) {
        return 0;
    }

    if (A >= C) {
        return 1;
    }

    double partDen = 0;

    for (int k = 0; k < C; k++) {
        partDen += pow(A, k) / tgamma(k + 1);
    }

    return pow(A, C) / (pow(A, C) + tgamma(C + 1) * (1 - A / C) * partDen);
}


//FIND A
double findA_C(double target, int C)
{
    double low = 0;
    double high = C * (1.0 - 1e-12);

    double mid;

    while ((high - low) > 0.000001) {

        mid = (low + high) / 2;

        double P = erlangC(mid, C);

        if (P < target) {
            low = mid;
        }
        else {
            high = mid;
        }
    }

    return (low + high) / 2;
}


//FIND C
int findC_C(double A, double target)
{
    int C = (int)floor(A) + 1;

    while (erlangC(A, C) > target) {
        C++;
    }

    return C;
}



int main(void)
{
    int run = 1;
    int choice = 0;

    double A = 0;
    int C = 0;
    double P = 0;

    while (run) {

        printf("----------- Erlang Calculator -----------\n\n");

        printf("+++ [1] Erlang B\n");
        printf("+++ [2] Erlang C\n");
        printf("+++ [0] Exit\n");

        printf("\nInsert option: ");

        if (scanf("%d", &choice) != 1) {

            while (getchar() != '\n') { }

            printf("\nInvalid input.\n\n");

            continue;
        }


        switch (choice) {

            case 1:

                choice = 0;

                printf("\n----------- Erlang B -----------\n\n");

                printf("What do you want to calculate?\n");
                printf("(You need the other two parameters.)\n\n");

                printf("+++ [1] Probability of blocking (GoS)\n");
                printf("+++ [2] Traffic intensity (A)\n");
                printf("+++ [3] Number of Trunked channels (C)\n");

                printf("\nInsert option: ");


                if (scanf("%d", &choice) != 1) {

                    while (getchar() != '\n') { }

                    printf("\nInvalid input.\n\n");

                    break;
                }


                switch (choice) {

                    //GoS
                    case 1:

                        printf("\nEnter the value of A: ");

                        if (scanf("%lf", &A) != 1) {

                            while (getchar() != '\n') { }

                            printf("\nInvalid input.\n");

                            break;
                        }


                        printf("Enter the value of C: ");

                        if (scanf("%d", &C) != 1) {

                            while (getchar() != '\n') { }

                            printf("\nInvalid input.\n");

                            break;
                        }


                        if (A < 0 || C <= 0) {

                            printf("\nA must be >= 0 and C must be > 0.\n");

                            break;
                        }


                        P = erlangB(A, C);


                        printf("\n---------------------------------\n");
                        printf("Traffic intensity A = %.6f Erlang\n", A);
                        printf("Trunked channels C   = %d\n", C);
                        printf("Probability of blocking = %.6f\n", P);
                        printf("GoS = %.4f%%\n", P * 100);
                        printf("---------------------------------\n\n");

                        break;


                   //A
                    case 2:

                        printf("\nEnter the desired GoS: ");

                        if (scanf("%lf", &P) != 1) {

                            while (getchar() != '\n') { }

                            printf("\nInvalid input.\n");

                            break;
                        }


                        printf("Enter the value of C: ");

                        if (scanf("%d", &C) != 1) {

                            while (getchar() != '\n') { }

                            printf("\nInvalid input.\n");

                            break;
                        }


                        if (P <= 0 || P >= 1 || C <= 0) {

                            printf("\nGoS must be between 0 and 1,");
                            printf(" and C must be > 0.\n");

                            break;
                        }


                        A = findA(P, C);


                        printf("\n---------------------------------\n");
                        printf("Desired GoS = %.6f\n", P);
                        printf("Trunked channels C = %d\n", C);
                        printf("Traffic intensity A = %.6f Erlang\n", A);
                        printf("---------------------------------\n\n");

                        break;


                    //C
                    case 3:

                        printf("\nEnter the value of A: ");

                        if (scanf("%lf", &A) != 1) {

                            while (getchar() != '\n') { }

                            printf("\nInvalid input.\n");

                            break;
                        }


                        printf("Enter the desired GoS: ");

                        if (scanf("%lf", &P) != 1) {

                            while (getchar() != '\n') { }

                            printf("\nInvalid input.\n");

                            break;
                        }


                        if (A < 0 || P <= 0 || P >= 1) {

                            printf("\nA must be >= 0 and GoS must be between 0 and 1.\n");

                            break;
                        }


                        C = findC(A, P);


                        printf("\n---------------------------------\n");
                        printf("Traffic intensity A = %.6f Erlang\n", A);
                        printf("Desired GoS = %.6f\n", P);
                        printf("Required channels C = %d\n", C);
                        printf("---------------------------------\n\n");

                        break;


                    default:

                        printf("\nInvalid option.\n\n");

                        break;
                }

                break;


            case 2:

                choice = 0;

                printf("\n----------- Erlang C -----------\n\n");

                printf("What do you want to calculate?\n");
                printf("(You need the other two parameters.)\n\n");

                printf("+++ [1] Probability of waiting (GoS)\n");
                printf("+++ [2] Traffic intensity (A)\n");
                printf("+++ [3] Number of Trunked channels (C)\n");

                printf("\nInsert option: ");


                if (scanf("%d", &choice) != 1) {

                    while (getchar() != '\n') { }

                    printf("\nInvalid input.\n\n");

                    break;
                }


                switch (choice) {

                    //GoS
                    case 1:

                        printf("\nEnter the value of A: ");

                        if (scanf("%lf", &A) != 1) {

                            while (getchar() != '\n') { }

                            printf("\nInvalid input.\n");

                            break;
                        }


                        printf("Enter the value of C: ");

                        if (scanf("%d", &C) != 1) {

                            while (getchar() != '\n') { }

                            printf("\nInvalid input.\n");

                            break;
                        }


                        if (A < 0 || C <= 0) {

                            printf("\nA must be >= 0 and C must be > 0.\n");

                            break;
                        }


                        if (A >= C) {

                            printf("\nFor Erlang C, A must be smaller than C.\n");

                            break;
                        }


                        P = erlangC(A, C);


                        printf("\n---------------------------------\n");
                        printf("Traffic intensity A = %.6f Erlang\n", A);
                        printf("Trunked channels C   = %d\n", C);
                        printf("Probability of waiting = %.6f\n", P);
                        printf("GoS = %.4f%%\n", P * 100);
                        printf("---------------------------------\n\n");

                        break;


                    //A
                    case 2:

                        printf("\nEnter the desired GoS: ");

                        if (scanf("%lf", &P) != 1) {

                            while (getchar() != '\n') { }

                            printf("\nInvalid input.\n");

                            break;
                        }


                        printf("Enter the value of C: ");

                        if (scanf("%d", &C) != 1) {

                            while (getchar() != '\n') { }

                            printf("\nInvalid input.\n");

                            break;
                        }


                        if (P <= 0 || P >= 1 || C <= 0) {

                            printf("\nGoS must be between 0 and 1,");
                            printf(" and C must be > 0.\n");

                            break;
                        }


                        A = findA_C(P, C);


                        printf("\n---------------------------------\n");
                        printf("Desired GoS = %.6f\n", P);
                        printf("Trunked channels C = %d\n", C);
                        printf("Traffic intensity A = %.6f Erlang\n", A);
                        printf("---------------------------------\n\n");

                        break;


                    //C
                    case 3:

                        printf("\nEnter the value of A: ");

                        if (scanf("%lf", &A) != 1) {

                            while (getchar() != '\n') { }

                            printf("\nInvalid input.\n");

                            break;
                        }


                        printf("Enter the desired GoS: ");

                        if (scanf("%lf", &P) != 1) {

                            while (getchar() != '\n') { }

                            printf("\nInvalid input.\n");

                            break;
                        }


                        if (A < 0 || P <= 0 || P >= 1) {

                            printf("\nA must be >= 0 and GoS must be between 0 and 1.\n");

                            break;
                        }


                        C = findC_C(A, P);


                        printf("\n---------------------------------\n");
                        printf("Traffic intensity A = %.6f Erlang\n", A);
                        printf("Desired GoS = %.6f\n", P);
                        printf("Required channels C = %d\n", C);
                        printf("---------------------------------\n\n");

                        break;


                    default:

                        printf("\nInvalid option.\n\n");

                        break;
                }

                break;


            //exit
            case 0:

                run = 0;

                printf("\nExiting...\n");

                break;

            default:

                printf("\nInvalid option.\n\n");

                break;
        }
    }

    return 0;
}