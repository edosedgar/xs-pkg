#ifndef BIGDIGITS_H_
#define BIGDIGITS_H_ 1

#include <stdlib.h>

#define BITS_PER_DIGIT 32

/* The basic BigDigit element, an unsigned 32-bit integer */
typedef uint32_t DIGIT_T;

/* Computes w = u + v, returns carry
 */
DIGIT_T mpAdd(DIGIT_T w[], const DIGIT_T u[], const DIGIT_T v[],
	      size_t ndigits);

/* Computes product w = u * v
 */
int mpMultiply(DIGIT_T w[], const DIGIT_T u[], const DIGIT_T v[],
	       size_t ndigits);

/* Sets a = d where d is a single digit
 */
void mpSetDigit(DIGIT_T a[], DIGIT_T d, size_t ndigits);


/* Print in decimal format with optional prefix and suffix strings
 */
void mpPrintDecimal(const char *prefix, const DIGIT_T *a, size_t ndigits, const char *suffix);

/* Converts big digit a into a string in decimal format, where s has size
 * smax including the terminating zero.
 * @returns number of chars required excluding leading zeroes.
 */
size_t mpConvToDecimal(const DIGIT_T a[], size_t ndigits, char *s, size_t smax);

/* Converts big digit a into string of octets, in big-endian order,
 * padding to nbytes or truncating if necessary.
 * @returns number of non-zero octets required.
 */
size_t mpConvToOctets(const DIGIT_T a[], size_t ndigits, unsigned char *c,
		      size_t nbytes);

void mpFail(char *msg);

#endif	/* BIGDIGITS_H_ */
