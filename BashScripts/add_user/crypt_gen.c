#define _XOPEN_SOURCE
#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"

int main() {
        fprintf(stderr, "%s\n", crypt("qwerty", "ghnjden"));
        return 0;
}
