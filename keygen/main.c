#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>

#define MAX_STRING_SIZE 64

// login_hash() and pass_hash() functions
// are translated from assembly to C and
// simplified a bit comparing to the source
// https://paste.ubuntu.com/p/XXFk7B9kyn/
unsigned char login_hash(const char *login) {
    unsigned int res = -1;
    int j = 0;

    while (login[j] != '\0')
    {
        res ^= login[j++];
        for (int i = 0; i < 8; i++) {
            if ((res & 1) == 0)
                res >>= 1;
            else
                res = (res >> 1) ^ 0xEDB88320;
        }
    }
    return ~res;
}

// This function is not called in this program,
// it was used for debugging. It is left here
// to see the reason in generate_password()
unsigned char pass_hash(const char *proc)
{
    int len = strlen(proc);
    int res = 0;

    for (int i = 0; i < len; ++i)
        res += proc[i] ^ 0x99;
    return res;
}

// This function puts random alphanumeric letters
// in the password until it is possible to get
// exact desired sum by adding a single character
int generate_password(unsigned char sum, char** res)
{
    static char buf[MAX_STRING_SIZE] = {0};
    int i = 0;
    int term = 0;

    while (sum != 0) {
        if (isalnum(sum ^ 0x99))
            term = sum;
        else
            term = ('a' + rand() % ('z' - 'a')) ^ 0x99;
        buf[i++] = term ^ 0x99;

        sum -= term;
        if (i == MAX_STRING_SIZE)
            return 1;
    }
    buf[i] = '\0';
    *res = buf;
    return 0;
}

void print_passwords(char* login, long n)
{
    unsigned char hash = (unsigned)login_hash(login);
    char* pass = NULL;
    /* Since random function is used there is
     * a theoretical chance of failure */
    while (generate_password(hash, &pass));
    for (int i = 0; i < n; i++) {
        printf("password #%2d = ", i);
        /* Concatenating 'Y99' to password, formally speaking,
         * produces new password. The trick here is that
         * ('Y' ^ 0x99) + ('9' ^ 0x99) + ('9' ^ 0x99) = 256,
         * and since hashes are casted to a char, this doesn't
         * change validity of the password. */
        for (int j = 0; j < i; j++)
            printf("Y99");
        printf("%s\n", pass);
    }
}

int main(void)
{
    char *endptr = NULL;
    // Ah sh*t, here we go again with the user input
    char login[MAX_STRING_SIZE] = {};
    printf("login = ");
    assert(MAX_STRING_SIZE == 64);
    if (scanf("%64s", login) != 1)
        return 1;
    if (login[MAX_STRING_SIZE - 1] != '\0') {
        printf("Login too long\n");
        return 1;
    }

    char str[MAX_STRING_SIZE] = {};
    printf("number of passwords to generate = ");
    assert(MAX_STRING_SIZE == 64);
    if (scanf("%64s", str) != 1)
        return 1;
    if (str[MAX_STRING_SIZE - 1] != '\0') {
        printf("\nNumber of logins too long\n");
        return 1;
    }
    long n = strtol(str, &endptr, 10);
    if ((*str == '\0') || (*endptr != '\0')) {
        printf("\nFailed to convert string to long integer\n");
        return 1;
    }
    if ((n == LONG_MIN) || (n == LONG_MAX)) {
        printf("\nOverflow/underflow occured\n");
        return 1;
    }

    unsigned char loginp = login_hash(login);
    printf("login_hash = password_hash = 0x%02x\n", loginp);

    print_passwords(login, n);
}
