#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>

#define MAX_STRING_SIZE 32
#define ABS(X) (((X) > 0) ? (X) : -(X))

int sum_of_hex_digits(long int val) {
    int res = 0;
    //            there are two hex words in a byte
    //                   VVVVVVVVVVVVVVV
    for (int i = 0; (i < sizeof(val) * 2) && (val != 0); i++) {
        res += (int)(val & 0xF);
        val >>= 4;
    }
    return res;
}

// See https://raw.githubusercontent.com/Phikimon/ind_progr_class/master/resource/div_by_5.png
int is_div_by_5(long int val) {
    if (val == 0)
        return 1;
    do {
        val = sum_of_hex_digits(val);
    } while (val > 0xF);
    return (val == 0x5) || (val == 0xA) || (val == 0xF);
}

int diff_between_odd_even_digits(long int val) {
    int sum_odd = 0, sum_even = 0;
    for (int i = 0; (i < sizeof(val) * CHAR_BIT) && (val != 0); i++) {
        if ((i & 1) == 0)
            sum_odd  += (val & 1);
        else
            sum_even += (val & 1);
        val >>= 1;
    }
    return (sum_odd > sum_even) ? sum_odd - sum_even : sum_even - sum_odd;
}

// See https://stackoverflow.com/questions/39385971/how-to-know-if-a-binary-number-divides-by-3
int is_div_by_3(long int val) {
    do {
        val = diff_between_odd_even_digits(val);
    } while (val >= 3);
    return (val == 0);
}

int main(void) {
    long int val = -1;
    char str[MAX_STRING_SIZE] = {0};
    char *endptr = NULL;
    char delim = '\0';

    // Find out if the file is being redirected to stdin
    if (isatty(STDIN_FILENO)) {
        printf("Use ^D to mark end of input\n");
        // Use \n as a delimiter for fancier output
        // if input is terminal
        delim = '\n';
    } else
        delim = ' ';

    while (1) {
        assert(MAX_STRING_SIZE == 32);
        if (scanf("%32s", str) != 1) {
            /* End of input */
            break;
        }
        if (str[MAX_STRING_SIZE - 1] != '\0') {
            printf("\nOne of input words is too long\n");
            return 1;
        }
        val = strtol(str, &endptr, 10);
        if ((*str == '\0') || (*endptr != '\0')) {
            printf("\nFailed to convert string to long integer\n");
            return 1;
        }
        if ((val == LONG_MIN) || (val == LONG_MAX)) {
            printf("\nOverflow/underflow occured\n");
            return 1;
        }

        int div_by_3 = is_div_by_3(ABS(val));
        int div_by_5 = is_div_by_5(ABS(val));
        if ((div_by_3) && (!div_by_5))
            printf("fizz%c", delim);
        else if ((!div_by_3) && (div_by_5))
            printf("buzz%c", delim);
        else if ((div_by_3) && (div_by_5))
            printf("fizzbuzz%c", delim);
        else
            printf("%ld%c", val, delim);
    }
    printf("\n");

    return 0;
}

