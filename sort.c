#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

int TESTS = 0;
int SWAPS = 0;
int COMPRASIONS = 0;

typedef struct test
{
    int size;
    int type;
    double *arr;
    double *arr_selection;
    double *arr_shell;
    int selection_valid;
    int shell_valid;
    int swaps_shell;
    int compares_shell;
    int swaps_selection;
    int compares_selection;
} test;

//Оболочка функции сравнения элементов массива
int compare(double a, double b)
{
    COMPRASIONS++;
    if (a > b)
        return 1;
    return 0;
}

//Оболочка функции обмена элементов массива
void swap(double *a, double *b)
{
    SWAPS++;
    double t = *a;
    *a = *b;
    *b = t;
}

//Компаратор сортировки по неубыванию
int comp_up(const void *a, const void *b)
{
    if (*(double *)a > *(double *)b)
        return 1;
    if (*(double *)a < *(double *)b)
        return -1;
    return 0;
}

//Компаратор сортировки по возрастанию
int comp_down(const void *a, const void *b)
{
    if (*(double *)a < *(double *)b)
        return 1;
    if (*(double *)a > *(double *)b)
        return -1;
    return 0;
}

//Сортировка Шелла
void sort_shell(double *arr, int size)
{
    int step = 1;
    while (step < size)
        step = (step + 1) * 2 - 1;
    step /= 2;
    for (int dist = step;  dist > 0;  dist /= 2) {
        for (int i = dist;  i < size;  i++) {
            double temp = arr[i];
            int j = i;
            for (j = i;  j >= dist && compare(arr[j-dist], temp);  j -= dist)
                swap(&arr[j], &arr[j - dist]);
            arr[j] = temp;
        }
    }
}

//Сортировка подсчетом
void sort_selection(double *arr, int size)
{
    for (int first = 0;  first < size - 1;  first++) {
        int min = first;
        for (int j = first + 1;  j < size;  j++) {
            if (compare(arr[min], arr[j]))
                min = j;
        }
        if (arr[min] != arr[first])
            swap(&arr[min], &arr[first]);
    }
}

//Функция генерирования случайного числа типа double
double gen_rand_double(void)
{
    union {
        double a;
        unsigned char b[sizeof(double)];
    } u;

    u.a = NAN;
    while (!isfinite(u.a)) {
        for (unsigned i = 0;  i < sizeof(u.b);  i++)
            u.b[i] = rand() % 256;
    }
    return u.a;
}

//Функция генерирования массива размером size из double определенного типа
double * gen_rand_array(int size, int type)
{
    double *arr = malloc(sizeof(double) * size);
    for (int i = 0;  i < size;  i++)
        arr[i] = gen_rand_double();

    if (type == 1)
        qsort(arr, size, sizeof(double), comp_up);
    else if (type == -1)
        qsort(arr, size, sizeof(double), comp_down);
    return arr;
}

//Проверка 2 массивов на полное соответствие
int arrays_equal(double *a, double *b, int size)
{
    for (int i = 0;  i < size;  i++) {
        if (a[i] != b[i])
            return 0;
    }

    return 1;
}

//Функция отработки обеих сортировок на тесте
void do_test(test *a)
{
    a->compares_selection = a->compares_shell = a->swaps_selection = a->swaps_shell = 0;

    COMPRASIONS = SWAPS = 0;
    sort_selection(a->arr_selection, a->size);
    a->swaps_selection = SWAPS;
    a->compares_selection = COMPRASIONS;

    COMPRASIONS = SWAPS = 0;
    sort_shell(a->arr_shell, a->size);
    a->swaps_shell = SWAPS;
    a->compares_shell = COMPRASIONS;

    qsort(a->arr, a->size, sizeof(double), comp_up);
    a->selection_valid = arrays_equal(a->arr, a->arr_selection, a->size);
    a->shell_valid = arrays_equal(a->arr, a->arr_shell, a->size);
}

//Функция печати отчета одного теста
void do_report(test *a, int number)
{
    printf("\n\nTEST %d with %d elements sorted in ", number + 1, a->size);
    if (a->type == 1)
        printf("increasing order:");
    else if (a->type == -1)
        printf("decreasing order:");
    else
        printf("random order:");
    printf("\n\nSelection sort:\n");
    printf("Compares: %d\n", a->compares_selection);
    printf("Swaps: %d\n", a->swaps_selection);
    printf("Valid: ");
    printf(a->selection_valid == 1 ? "True" : "False");
    printf("\n\nShell sort:\n");
    printf("Compares: %d\n", a->compares_shell);
    printf("Swaps: %d\n", a->swaps_shell);
    printf("Valid: ");
    printf(a->shell_valid == 1 ? "True" : "False");
}

//Функция стирания массивов из динамической памяти
void erase_test(test *a)
{
    free(a->arr);
    free(a->arr_selection);
    free(a->arr_shell);
}

//Функций геренации массивов для теста
void generate_test(test *a, int number)
{
    a->arr = gen_rand_array(a->size, a->type);
    a->arr_selection = malloc(a->size * sizeof(double));
    a->arr_shell = malloc(a->size * sizeof(double));
    memcpy(a->arr_selection, a->arr, a->size * sizeof(double));
    memcpy(a->arr_shell, a->arr, a->size * sizeof(double));
    printf("TEST %d : array initialized\n", number + 1);
}


int main(void)
{
    srand(time(NULL));

    TESTS = 16;
    //printf("Input the tests amount: ");
    //scanf("%d", &TESTS);
    //printf("\n");
    test tests_arr[TESTS];

    tests_arr[0].size = 10; tests_arr[0].type = 1;
    tests_arr[1].size = 10; tests_arr[1].type = -1;
    tests_arr[2].size = 10; tests_arr[2].type = 0;
    tests_arr[3].size = 10; tests_arr[3].type = 0;
    tests_arr[4].size = 100; tests_arr[4].type = 1;
    tests_arr[5].size = 100; tests_arr[5].type = -1;
    tests_arr[6].size = 100; tests_arr[6].type = 0;
    tests_arr[7].size = 100; tests_arr[7].type = 0;
    tests_arr[8].size = 1000; tests_arr[8].type = 1;
    tests_arr[9].size = 1000; tests_arr[9].type = -1;
    tests_arr[10].size = 1000; tests_arr[10].type = 0;
    tests_arr[11].size = 1000; tests_arr[11].type = 0;
    tests_arr[12].size = 10000; tests_arr[12].type = 1;
    tests_arr[13].size = 10000; tests_arr[13].type = -1;
    tests_arr[14].size = 10000; tests_arr[14].type = 0;
    tests_arr[15].size = 10000; tests_arr[15].type = 0;
    //printf("Input the information about each test with template 'TEST x : a b', where\na - size of an array\nb - type of an array (1 - increasing, -1 - decreasing, else - random)\nExample:\nTEST x : 100 0\n\n");
    //for (int i = 0;  i < TESTS;  i++) {
    //    printf("TEST %d : ", i + 1);
    //    scanf("%d %d", &(tests_arr[i].size), &(tests_arr[i].type));
    //} printf("\n");

    for (int i = 0;  i < TESTS;  i++)
        generate_test(&tests_arr[i], i);
    for (int i = 0;  i < TESTS;  i++)
        do_test(&tests_arr[i]);
    printf("\n\n=================TEST RESULTS=================");
    for (int i = 0;  i < TESTS; i++)
        do_report(&tests_arr[i], i);
    printf("\n\n=================TEST RESULTS=================\n\n");
    for (int i = 0;  i < TESTS;  i++)
        erase_test(&tests_arr[i]);
    printf("ARRAYS ERASED\n");

    return 0;
}

