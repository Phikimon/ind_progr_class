#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_STRING_SIZE 32

// \brief Prints int array 'arr' of size 'size'
// \param arr input array
// \param size number of ints in arr
void print_int_array(int* arr, int size)
{
	printf("{");
	for (int i = 0; i < size; i++) {
		printf("%d", arr[i]);
		if (i != size - 1)
			printf(", ");
	}
	printf("}\n");
}

// \brief Returns to res difference of sets 0..(m-1) and inp
// \details Uses O(N) memory, O(N) computation time,
//          definitely could be improved
// \param inp input set
// \param n number of ints in inp
// \param m size of inclusive set 0..(m-1)
// \param res pointer to array to store result to
void get_difference(int* inp, int n, int m, int* res)
{
	int j = 0;
	char* buf = (char*)calloc(m, sizeof(*buf));
	assert(buf);
	for (int i = 0; i < n; i++)
		buf[inp[i]] = 1;
	for (int i = 0; i < m; i++) {
		if (buf[i] == 0)
			res[j++] = i;
	}
	free(buf);
}

// \brief reads int value from stdin
int read_int(int* res)
{
	long val;
	char* endptr;
	char str[MAX_STRING_SIZE] = {0};
	assert(MAX_STRING_SIZE == 32);
	if (scanf("%32s", str) != 1) {
		printf("\nUnexpected end of input\n");
		return 1;
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
	*res = (int)val;
	return 0;
}

int main(void)
{
	int N = 0, M = 0;
	// Input
	if (read_int(&N) != 0)
		return 1;
	if (read_int(&M) != 0)
		return 1;
	int* input = (int*)calloc(N, sizeof(input[0]));
	assert(input);
	int* res = (int*)calloc(M - N, sizeof(res[0]));
	assert(res);
	for (int i = 0; i < N; i++)
		if (read_int(&input[i]) != 0)
			return 1;

	// Logic
	get_difference(input, N, M, res);

	// Output
	printf("inp[%d] = ", N);
	print_int_array(input, N);
	printf("\n");
	printf("res[%d] = ", M - N);
	print_int_array(res, M - N);
	printf("\n");

	free(input);
	free(res);
	return 0;
}
