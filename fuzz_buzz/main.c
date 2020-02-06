#include <stdio.h>

int sum_of_hex_digits(int val) {
    int res = 0;
    for (int i = 0; (i < sizeof(val) * 2) && (val != 0); i++) {
        res += val & 0xF;
        val >>= 4;
    }
    return res;
}

// See https://raw.githubusercontent.com/Phikimon/ind_progr_class/master/resource/div_by_5.png
int is_div_by_5(int val) {
    do {
        val = sum_of_hex_digits(val);
    } while (val > 0xF);
    return (val == 0x5) || (val == 0xA) || (val == 0xF);
}

int diff_between_odd_even_digits(int val) {
    int sum_odd = 0, sum_even = 0;
    for (int i = 0; (i < sizeof(val) * 8) && (val != 0); i++) {
        if ((i & 1) == 0)
            sum_odd  += (val & 1);
        else
            sum_even += (val & 1);
        val >>= 1;
    }
    return (sum_odd > sum_even) ? sum_odd - sum_even : sum_even - sum_odd;
}

// See https://stackoverflow.com/questions/39385971/how-to-know-if-a-binary-number-divides-by-3
int is_div_by_3(int val) {
    do {
        val = diff_between_odd_even_digits(val);
    } while (val >= 3);
    return (val == 0);
}

int main(void) {
    int val = -1;
    while (val != 0) {
        if (scanf("%d", &val) != 1) {
            printf("Invalid value fed to input\n");
            return 1;
        }
        int div_by_3 = is_div_by_3(val);
        int div_by_5 = is_div_by_5(val);
        if ((div_by_3) && (!div_by_5))
            printf("fizz ");
        if ((!div_by_3) && (div_by_5))
            printf("buzz ");
        if ((div_by_3) && (div_by_5))
            printf("fizzbuzz ");
    }
    printf("\n");

    return 0;
}

