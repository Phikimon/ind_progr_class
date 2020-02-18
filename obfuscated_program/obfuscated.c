#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// to be compiled under x86

///unsigned* map;

unsigned int function(char* input, unsigned int len)
{
    int a1 = (int)input;
    int v2;
    unsigned int result;
    int v4;
    int v5;
    int v6;
    int v7;
    int *v8;
    uint32_t *v9;
    uint32_t *v10;
    unsigned int k;
    uint32_t *v12;
    int v13;
    unsigned int j;
    unsigned int i;

    v10 = malloc(2 * len);
    v4 = 0;
    for ( i = 0; len >> 2 > i; ++i )
    {
        v2 = ((uint32_t*)a1)[i];
        v8 = &v4;
        v7 = (int)&v10[2 * i];
        v6 = v2;
        v10[2 * i] = v2;
        *(uint32_t *)(v7 + 4) = *v8;
        if ( *v8 )
        {
            v5 = *(uint32_t *)(*v8 + 4);
            *(uint32_t *)(*v8 + 4) = v7 ^ v5;
        }
        *v8 = v7;
    }
    unsigned* ptr = NULL;
    for ( j = 0; ; ++j )
    {
        result = (len >> 2) - 1;
        if ( result <= j )
            break;
        v13 = 0;
        v12 = v10;
        for ( k = 0; (len >> 2) - j - 1 > k; ++k )
        {
            if ( *v12 > *(uint32_t *)(v12[1] ^ v13) )
            {
                *(uint32_t *)(v12[1] ^ v13) ^= *v12;
                *v12 ^= *(uint32_t *)(v12[1] ^ v13);
                *(uint32_t *)(v12[1] ^ v13) ^= *v12;
            }
            v9 = (uint32_t *)(v12[1] ^ v13);
            v13 = (int)v12;
            v12 = v9;
        }
    }
    ///map = v10;
    return result;
}

///int main(void)
///{
///    int input[] = {-1, 155, 16};
///    function(input, sizeof(input));
///
///    unsigned* ptr = NULL;
///    for (int i = 0; i < sizeof(input)/sizeof(input[0]); i++) {
///        printf("[%2d]: ptr = %p, data = %u, nptr = %p\n",
///                i, &map[i * 2], map[i * 2], (unsigned*)((unsigned)ptr ^ (unsigned)map[i * 2 + 1]));
///        ptr = &map[i * 2];
///    }
///    return 0;
///}
