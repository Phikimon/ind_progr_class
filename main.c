#include <stdio.h>

// Is presented as const variable to avoid
// VLA and dynamic allocation(come on, this
// is first task)
const int M = 5;

void print_int_array(int* arr, int size) {
    printf("{");
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i != size - 1)
            printf(", ");
    }
    printf("}\n");
}

// Uses O(N) memory, O(N) computation time,
// definitely could be improved
void return_difference(int* inp, int size, int* res) {
    int j = 0;
    char buf[M] = {0};
    for (int i = 0; i < size; i++)
        buf[inp[i]] = 1;
    for (int i = 0; i < M; i++) {
        if (buf[i] == 0)
            res[j++] = i;
    }
}

// Not const to avoid messing with cast to (int*) and
// ugly argument types in return_difference()
int INP[] = {4, 1, 3};

int main(void) {
    int res[M];
    const int size_of_inp = sizeof(INP)/sizeof(INP[0]);
    return_difference((int*) INP, size_of_inp, (int*)res);

    printf("inp[%d] = ", size_of_inp);
    print_int_array(INP, size_of_inp);
    printf("\n");
    printf("res[%d] = ", M - size_of_inp);
    print_int_array(res, M - size_of_inp);
    printf("\n");

    return 0;
}

