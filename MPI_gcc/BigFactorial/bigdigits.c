#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "bigdigits.h"

void mpFail(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

/*******************************/
/* MEMORY ALLOCATION FUNCTIONS */
/*******************************/
#define ALLOC_BYTES(b,n) do {						\
	(b)=calloc(n, 1); 						\
	if (!(b)) 							\
		mpFail("ALLOC_BYTES: Unable to allocate memory.");	\
	} while(0)

#define FREE_BYTES(b) do { \
	free(b);	   \
	} while(0)

int spMultiply(uint32_t p[2], uint32_t x, uint32_t y)
{
	/* Use a 64-bit temp for product */
	uint64_t t = (uint64_t)x * (uint64_t)y;
	/* then split into two parts */
	p[1] = (uint32_t)(t >> 32);
	p[0] = (uint32_t)(t & 0xFFFFFFFF);

	return 0;
}

DIGIT_T mpAdd(DIGIT_T w[], const DIGIT_T u[], const DIGIT_T v[], size_t ndigits)
{
	DIGIT_T k;
	size_t j;

	assert(w != v);

	k = 0;

	for (j = 0; j < ndigits; j++)
	{
		w[j] = u[j] + k;
		if (w[j] < k)
			k = 1;
		else
			k = 0;

		w[j] += v[j];
		if (w[j] < v[j])
			k++;
	}

	return k;
}

int mpMultiply(DIGIT_T w[], const DIGIT_T u[], const DIGIT_T v[], size_t ndigits)
{
	DIGIT_T k, t[2];
	size_t i, j, m, n;

	assert(w != u && w != v);

	m = n = ndigits;

	for (i = 0; i < 2 * m; i++)
		w[i] = 0;

	for (j = 0; j < n; j++)
	{
		if (v[j] == 0)
			w[j + m] = 0;
		else
		{
			k = 0;
			for (i = 0; i < m; i++)
			{
				spMultiply(t, u[i], v[j]);

				t[0] += k;
				if (t[0] < k)
					t[1]++;
				t[0] += w[i+j];
				if (t[0] < w[i+j])
					t[1]++;

				w[i + j] = t[0];
				k = t[1];
			}
			w[j + m] = k;
		}
	}
	return 0;
}

void mpSetDigit(DIGIT_T a[], DIGIT_T d, size_t ndigits)
{
	size_t i;

	for (i = 1; i < ndigits; i++)
	{
		a[i] = 0;
	}
	a[0] = d;
}

static size_t mpSizeof(const DIGIT_T a[], size_t ndigits)
{
	while(ndigits--)
	{
		if (a[ndigits] != 0)
			return (++ndigits);
	}
	return 0;
}
static size_t mpBitLength(const DIGIT_T d[], size_t ndigits)
/* Returns no of significant bits in d */
{
	size_t n, i, bits;
	DIGIT_T mask;

	if (!d || ndigits == 0)
		return 0;

	n = mpSizeof(d, ndigits);
	if (0 == n) return 0;

	for (i = 0, mask = HIBITMASK; mask > 0; mask >>= 1, i++)
	{
		if (d[n-1] & mask)
			break;
	}

	bits = n * BITS_PER_DIGIT - i;

	return bits;
}
void mpPrintHex(const char *prefix, const DIGIT_T *p, size_t len,
                const char *suffix)
{
	if (prefix) printf("%s", prefix);
	/* Trim leading digits which are zero */
	while (len--)
	{
		if (p[len] != 0)
			break;
	}
	len++;
	if (0 == len) len = 1;
	/* print first digit without leading zeros */
	printf("%x", p[--len]);
	while (len--)
	{
		printf("%08x" , p[len]);
	}
	if (suffix) printf("%s", suffix);
}

void mpPrintDecimal(const char *prefix, const DIGIT_T *a, size_t len,
		    const char *suffix)
{
	char *s;
	size_t nc;
	nc = mpConvToDecimal(a, len, NULL, 0);
	ALLOC_BYTES(s, nc + 1);
	nc = mpConvToDecimal(a, len, s, nc + 1);
	if (prefix) printf("%s", prefix);
	printf("%s", s);
	if (suffix) printf("%s", suffix);
	FREE_BYTES(s);
}





/* Convert big digit a into string of octets, in big-endian order,
 * padding on the left to nbytes or truncating if necessary.
 * Return number of octets required excluding leading zero bytes.
 */
size_t mpConvToOctets(const DIGIT_T a[], size_t ndigits, unsigned char *c,
		      size_t nbytes)

{
	int j, k, len;
	DIGIT_T t;
	size_t i, noctets, nbits;

	nbits = mpBitLength(a, ndigits);
	noctets = (nbits + 7) / 8;

	len = (int)nbytes;

	for (i = 0, j = len - 1; i < ndigits && j >= 0; i++)
	{
		t = a[i];
		for (k = 0; j >= 0 && k < BITS_PER_DIGIT; j--, k += 8)
			c[j] = (unsigned char)(t >> k);
	}

	for ( ; j >= 0; j--)
		c[j] = 0;

	return (size_t)noctets;
}

/* Returns ceil(x) as a non-negative integer or 0 if x < 0 */
static size_t uiceil(double x)
{
	size_t c;

	if (x < 0)
		return 0;
	c = (size_t)x;
	if ((x - c) > 0.0)
		c++;

	return c;
}

static int mpIsZero(const DIGIT_T a[], size_t ndigits)
{
	size_t i;

	for (i = 0; i < ndigits; i++)
	{
		if (a[i] != 0)
			return 0;
	}

	return (!0);	/* True */
}

/* Convert big digit a into a string in given base format,
 * where s has max size smax.
 * Return number of chars set excluding leading zeroes.
 * smax can be 0 to find out the required length.
 */
static size_t conv_to_base(const DIGIT_T a[], size_t ndigits, char *s,
			   size_t smax, int base)

{
	uint8_t *bytes, *newdigits;
	const char DEC_DIGITS[] = "0123456789";
	const char HEX_DIGITS[] = "0123456789abcdef";
	size_t newlen, nbytes, nchars;
	size_t n;
	unsigned long t;
	size_t i, j, isig;
	const char *digits;
	double factor;

	switch (base)
	{
	case 10:
		digits = DEC_DIGITS;
		/* log(256)/log(10)=2.40824 */
		factor = 2.40824;
		break;
	case 16:
		digits = HEX_DIGITS;
		/* log(256)/log(16)=2.0 */
		factor = 2.0;
		break;
	default:
		assert (10 == base || 16 == base);
		return 0;
	}

	/* Set up output string with null chars */
	if (smax > 0 && s)
	{
		memset(s, '0', smax-1);
		s[smax-1] = '\0';
	}

	/* Catch zero input value (return 1 not zero) */
	if (mpIsZero(a, ndigits))
	{
		if (smax > 0 && s)
			s[1] = '\0';
		return 1;
	}

	/* First, we convert to 8-bit octets (bytes),
	 * which are easier to handle
	 */
	nbytes = ndigits * BITS_PER_DIGIT / 8;
	ALLOC_BYTES(bytes, nbytes);

	n = mpConvToOctets(a, ndigits, bytes, nbytes);

	/* Create some temp storage for int values */
	newlen = uiceil(n * factor);
	ALLOC_BYTES(newdigits, newlen);

	for (i = 0; i < nbytes; i++)
	{
		t = bytes[i];
		for (j = newlen; j > 0; j--)
		{
			t += (unsigned long)newdigits[j-1] * 256;
			newdigits[j-1] = (unsigned char)(t % base);
			t /= base;
		}
	}

	/* Find index of leading significant digit */
	for (isig = 0; isig < newlen; isig++)
		if (newdigits[isig])
			break;

	nchars = newlen - isig;

	/* Convert to a null-terminated string of decimal chars
	 * up to limit, unless user has specified null or size == 0
	 */
	if (smax > 0 && s)
	{
		for (i = 0; i < nchars && i < smax-1; i++)
		{
			s[i] = digits[newdigits[isig+i]];
		}
		s[i] = '\0';
	}

	FREE_BYTES(bytes);
	FREE_BYTES(newdigits);

	return nchars;
}

/* Convert big digit a into a string in decimal format,
 * where s has max size smax.
 * Return number of chars set excluding leading zeroes.
 */
size_t mpConvToDecimal(const DIGIT_T a[], size_t ndigits, char *s, size_t smax)

{
	return conv_to_base(a, ndigits, s, smax, 10);
}
