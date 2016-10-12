#define _XOPEN_SOURCE
#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"

int main() {
        fprintf(stdout, "%s\n", crypt("", "ghnjden"));
        return 0;
}
