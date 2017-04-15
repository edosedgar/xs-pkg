#include "stdio.h"
#include "bigdigits.h"


int main(int argc, char* argv[]) {
	DIGIT_T* dummy;
	DIGIT_T* number = (DIGIT_T*) calloc(LENGTH_BIG_INT, sizeof(DIGIT_T));
	DIGIT_T* result = (DIGIT_T*) calloc(LENGTH_BIG_INT, sizeof(DIGIT_T));
	DIGIT_T* temp   = (DIGIT_T*) calloc(LENGTH_BIG_INT, sizeof(DIGIT_T));

	mpSetDigit(temp, 1, LENGTH_BIG_INT);

	int i = 0;
	for (i = 1; i <= 100000; i++) {
		mpSetDigit(number, i, LENGTH_BIG_INT);
		mpMultiply(result, temp, number, LENGTH_BIG_INT/2);
		dummy = temp;
		temp = result;
		result = dummy;
	}
	//mpPrintDecimal("result: ", temp, LENGTH_BIG_INT, "\n");
	return 0;
}
