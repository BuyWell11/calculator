#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <complex.h>
#define complex _Complex
#pragma STDC CX_LIMITED_RANGE on
#define SIZE 50

typedef struct string_ {
    char str[SIZE];
}strings;

typedef struct variable {
    char name[20];
    char original[SIZE];
    char polish[SIZE];
    double complex res;
}variables;

complex double CMPLX(double real, double imag);

int prior(int symb)
{
    switch (symb)
    {
        case'(':
            return 0;
        case '+':
        case '-':
            return 1;
        case'*':
        case'/':
            return 2;
        case'^':
            return 3;
    }
    return 3;
}



int push(strings stack[], unsigned cur, char buff[])
{
    if (cur < SIZE)
    {
        strcpy(stack[cur].str, buff);
        cur++;
    }
    return cur;
}

int empty(const strings stack[], unsigned cur)
{
    if (cur == 0)
        return 1;
    else return 0;
}

int pop(strings stack[], unsigned cur)
{
    if (empty(stack, cur) != 1)
    {
        --cur;
    }
    return cur;
}

int is_fun(strings fun[SIZE], char buff[SIZE], int cur_fun) {
    for (int i = 0; i < cur_fun; i++) {
        if (strcmp(fun[i].str, buff) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_const(strings con[SIZE], char buff[SIZE], int cur_con) {
    for (int i = 0; i < cur_con; i++) {
        if (strcmp(con[i].str, buff) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_complex(char temp[]) {
    int i = 0, k = 0;
    for(int j = 0; j < strlen(temp); j++){
        if(temp[j] == 'i'){
            i++;
        }
        if(temp[j]>='0' && temp[j]<='9'){
            k++;
        }
    }
    if(k > 0 && i > 0){
        return 1;
    }
    else{
        return 0;
    }
}

double complex calculate_fun(double complex stack[SIZE], char temp[SIZE], int cur)
{
    if (strcmp(temp, "cos") == 0)
    {
        return ccos(stack[cur - 1]);
    }
    else if (strcmp(temp, "sin") == 0)
    {
        return csin(stack[cur - 1]);
    }
    else if (strcmp(temp, "tg") == 0)
    {
        return csin(stack[cur - 1]) / ccos(stack[cur - 1]);
    }
    else if (strcmp(temp, "ctg") == 0)
    {
        return ccos(stack[cur - 1]) / csin(stack[cur - 1]);
    }
    else if (strcmp(temp, "exp") == 0)
    {
        return cexp(stack[cur - 1]);
    }
    else if (strcmp(temp, "log") == 0)
    {
        return clog(stack[cur - 1]);
    }
    else if (strcmp(temp, "sqrt") == 0)
    {
        return csqrt(stack[cur - 1]);
    }
    else if (strcmp(temp, "sqr") == 0)
    {
        return cpow(stack[cur - 1], 2);
    }
    else if (strcmp(temp, "abs") == 0)
    {
        return cabs(stack[cur - 1]);
    }
    else if (strcmp(temp, "~") == 0) {
        return (-1 * stack[cur - 1]);
    }
    else if (strcmp(temp, "real") == 0) {
        return creal(stack[cur - 1]);
    }
    else if (strcmp(temp, "imag") == 0) {
        return cimag(stack[cur - 1]);
    }
    //real & imag (?)
}

double calculate_const(double stack[SIZE], char temp[SIZE], int cur)
{
    if (strcmp(temp, "pi") == 0)
    {
        return 3.14159;
    }
    else if (strcmp(temp, "e") == 0)
    {
        return 2.71828;
    }
}

double complex calculate(double complex stack[SIZE], char temp[SIZE], int cur)
{
    double complex x = stack[cur - 2];
    double complex y = stack[cur - 1];
    switch (temp[0])
    {
        case '+':
            return x + y;
        case '-':
            return x - y;
        case '*':
            return x * y;
        case '/':
            return x / y;
        case '^':
            return cpow(x, y);
    }
}

int is_sign(char temp[SIZE])
{
    if (strcmp(temp, "+") == 0 || strcmp(temp, "-") == 0 || strcmp(temp, "*") == 0 ||
        strcmp(temp, "/") == 0 || strcmp(temp, "^") == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int symb_in(char temp[]){
    for(int i = 0; i < strlen(temp); i++){
        if(temp[i]>='a' && temp[i]<='z'){
            return 1;
        }
    }
    return 0;
}

double complex variab(variables var[SIZE], char temp[SIZE], int curva)
{
    for (int i = curva - 1; i >= 0; --i)
    {
        if (strcmp(temp, var[i].name) == 0)
        {
            return var[i].res;
        }
    }
}

int main()
{
    variables var[SIZE] = { 0 };
    FILE* fun = fopen("function.txt", "r");
    FILE* con = fopen("const.txt", "r");
    FILE* input = fopen("input.txt", "r");
    FILE* counting = fopen("count.txt", "r+");
    FILE* ans = fopen("output.txt", "w");
    strings fun_arr[SIZE] = { 0 };
    strings con_arr[SIZE] = { 0 };
    strings stack[SIZE] = { 0 };
    char str[SIZE] = { 0 };
    char c = '0';
    int curva = 0, cur_fun = 0, cur_con = 0;
    int len = 0, cur = 0;
    while (!feof(fun)) {
        fscanf(fun, "%s", fun_arr[cur_fun].str);
        cur_fun++;
    }
    while (!feof(con)) {
        fscanf(con, "%s", con_arr[cur_con].str);
        cur_con++;
    }
    while (!feof(input)) {
        fscanf(input, "%s", var[curva].name);
        c = fgetc(input);
        c = fgetc(input);
        c = fgetc(input);
        char buff[SIZE] = { 0 };
        char check[SIZE] = { 0 };
        while (c != '\n' && c != EOF) {
            fscanf(input, "%s", buff);
            if (is_fun(fun_arr, buff, cur_fun) == 1) {
                if (empty(stack, cur) == 1) {
                    cur = push(stack, cur, buff);
                }
                else if (prior(buff[0]) > prior(stack[cur - 1].str[0])) {
                    cur = push(stack, cur, buff);
                }
                else if (prior(buff[0]) <= prior(stack[cur - 1].str[0])) {
                    while (prior(buff[0]) <= prior(stack[cur - 1].str[0]) && !empty(stack, cur)) {
                        strcat(var[curva].polish, stack[cur - 1].str);
                        var[curva].polish[strlen(var[curva].polish)] = ' ';
                        cur = pop(stack, cur);
                    }
                    cur = push(stack, cur, buff);
                }
            }
            else if (strcmp(buff, "(") == 0) {
                cur = push(stack, cur, buff);
            }
            else if (strcmp(buff, ")") == 0) {
                while (strcmp(stack[cur - 1].str, "(") != 0) {
                    strcat(var[curva].polish, stack[cur - 1].str);
                    var[curva].polish[strlen(var[curva].polish)] = ' ';
                    cur = pop(stack, cur);
                }
                cur = pop(stack, cur);
            }
            else if (strcmp(buff, "+") == 0 || strcmp(buff, "-") == 0 || strcmp(buff, "*") == 0 || strcmp(buff, "/") == 0 || strcmp(buff, "^") == 0) {
                if (empty(stack, cur) == 1) {
                    cur = push(stack, cur, buff);
                }
                else if (prior(buff[0]) > prior(stack[cur - 1].str[0])) {
                    cur = push(stack, cur, buff);
                }
                else if (prior(buff[0]) <= prior(stack[cur - 1].str[0])) {
                    while (prior(buff[0]) <= prior(stack[cur - 1].str[0]) && !empty(stack, cur)) {
                        strcat(var[curva].polish, stack[cur - 1].str);
                        var[curva].polish[strlen(var[curva].polish)] = ' ';
                        cur = pop(stack, cur);
                    }
                    cur = push(stack, cur, buff);
                }
            }
            else {
                strcat(var[curva].polish, buff);
                var[curva].polish[strlen(var[curva].polish)] = ' ';
            }
            c = fgetc(input);
        }
        if (empty(stack, cur) != 1) {
            while (empty(stack, cur) != 1) {
                strcat(var[curva].polish, stack[cur - 1].str);
                var[curva].polish[strlen(var[curva].polish)] = ' ';
                cur = pop(stack, cur);
            }
        }
        curva++;
        len = 0;
        cur = 0;
    }

    for (int i = 0; i < curva; i++) {
        printf("%s\n", var[i].polish);
    }

    for (int i = curva - 1; i >= 0; --i)
    {
        fprintf(counting, "%s\n", var[i].polish);
    }

    rewind(counting);

    double complex stack1[SIZE] = { 0 };
    int cnt = 0;

    for (int i = curva - 1; i >= 0; --i)
    {
        char temp[SIZE] = { 0 };

        while (cnt != strlen(var[i].polish))
        {
            fscanf(counting, "%s", temp);
            c = fgetc(counting);

            if (atof(temp) != 0 && symb_in(temp) == 0)
            {
                stack1[cur] = CMPLX(atof(temp),0);
                cur++;
            }
            else if (is_fun(fun_arr, temp, cur_fun) == 1)
            {
                stack1[cur - 1] = calculate_fun(stack1, temp, cur);
            }
            else if (is_const(con_arr, temp, cur_con) == 1)
            {
                stack1[cur] = CMPLX(calculate_const(stack1, temp, cur), 0);
                cur++;
            }
            else if (is_sign(temp) == 1)
            {
                stack1[cur - 2] = calculate(stack1, temp, cur);
                cur--;
            }
            else if (is_complex(temp) == 1) {
                temp[strlen(temp)-1] = '\0';
                stack1[cur] = CMPLX(0,atof(temp));
                cur++;
                cnt++;
            }
            else
            {
                stack1[cur] = variab(var, temp, curva);
                cur++;
            }

            cnt += strlen(temp) + 1;
        }

        var[i].res = stack1[0];
        cur = 0;
        cnt = 0;
    }

    for (int i = 0; i < curva; ++i)
    {
        if(cimag(var[i].res) == cimag(0)) {
            fprintf(ans, "%s = %lf\n", var[i].name, creal(var[i].res));
        }
        else {
            fprintf(ans,"%s = %.2f %+.2fi\n",var[i].name, creal(var[i].res), cimag(var[i].res));
        }
    }

    fclose(fun);
    fclose(con);
    fclose(input);
    fclose(counting);
    fclose(ans);
}

complex double CMPLX(double real, double imag) {
    return real + imag * I;
}
