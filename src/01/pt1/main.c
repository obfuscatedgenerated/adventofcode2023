#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define LINE_BUF_SIZE 256
#define RADIX 10

int main(int argc, const char * argv[]) {
    // open file from path at command line
    // probably dangerous, don't care. fuck you.

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s", strerror(errno));
        return 1;
    }

    unsigned long int sum = 0;

    // iterate over lines until EOF
    char line_buf[LINE_BUF_SIZE];
    size_t line_num = 0;
    while(fgets(line_buf, sizeof (line_buf), file)) {
        line_num++;

        char digit_buf[3] = { 0 };

        // find first digit by iteration forward
        for (size_t idx = 0; idx < strlen(line_buf); idx++) {
            char c = line_buf[idx];

            if (c < '0' || c > '9') {
                continue;
            }

            // found the first digit
            digit_buf[0] = c;
            break;
        }

        // check a digit was found
        if (digit_buf[0] == '\0') {
            fprintf(stderr, "No digits on line %zu", line_num);
            return 1;
        }

        // find second digit by iteration backward
        for (size_t idx_plus_one = strlen(line_buf); idx_plus_one > 0; idx_plus_one--) {
            char c = line_buf[idx_plus_one - 1];

            if (c < '0' || c > '9') {
                continue;
            }

            // found the second digit
            digit_buf[1] = c;
            break;
        }

        // impossible to have found digit before but not now

        // convert digit string to number
        errno = 0;
        char *leftover;
        long int value = strtol(digit_buf, &leftover, RADIX);

        if (errno) {
            fprintf(stderr, "Errno %d on line %zu: %s", errno, line_num, strerror(errno));
            return 1;
        }

        if (value < 0) {
            fprintf(stderr, "Negative value on line %zu", line_num);
            return 1;
        }

        sum += value;
    }

    printf("Result: %lu", sum);
    return 0;
}
