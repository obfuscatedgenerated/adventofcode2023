// this code gets REALLY silly
// if you're a recruiter reading this, just know i'm better than this :o

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>

#define LINE_BUF_SIZE 256
#define RADIX 10

unsigned int read_single_digit(char num_char) {
    errno = 0;
    char *leftover;
    char digit_buf[2] = { num_char, 0 };
    long int value = strtol(digit_buf, &leftover, RADIX);

    if (errno) {
        fprintf(stderr, "Errno %d: %s", errno, strerror(errno));
        exit(1);
    }

    if (value < 0 || value > 9) {
        fputs("Somehow got an illegal value from a single digit", stderr);
        exit(1);
    }

    return value;
}

// returns 0 if invalid otherwise value
// null-termination not required
unsigned int parse_num_str(const char *num_str, unsigned int n_chars) {
    // worst parser ever??!??!?!?!?!

    switch (n_chars) {
        case 3:
            // one, two, or six
            switch (num_str[0]) {
                case 'o':
                    return (num_str[1] == 'n' && num_str[2] == 'e');
                case 't':
                    return 2 * (num_str[1] == 'w' && num_str[2] == 'o');
                case 's':
                    return 6 * (num_str[1] == 'i' && num_str[2] == 'x');
                default:
                    return 0;
            }
        case 4:
            // four, five, or nine
            switch (num_str[0]) {
                case 'f':
                    if (num_str[1] == 'o') {
                        return 4 * (num_str[2] == 'u' && num_str[3] == 'r');
                    } else if (num_str[1] == 'i') {
                        return 5 * (num_str[2] == 'v' && num_str[3] == 'e');
                    } else {
                        return 0;
                    }
                case 'n':
                    return 9 * (num_str[1] == 'i' && num_str[2] == 'n' && num_str[3] == 'e');
                default:
                    return 0;
            }
        case 5:
            // three, seven, or eight
            switch (num_str[0]) {
                case 't':
                    return 3 * (num_str[1] == 'h' && num_str[2] == 'r' && num_str[3] == 'e' && num_str[4] == 'e');
                case 's':
                    return 7 * (num_str[1] == 'e' && num_str[2] == 'v' && num_str[3] == 'e' && num_str[4] == 'n');
                case 'e':
                    return 8 * (num_str[1] == 'i' && num_str[2] == 'g' && num_str[3] == 'h' && num_str[4] == 't');
                default:
                    return 0;
            }
        default:
            return 0;
    }
}

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

        unsigned int digits_read = 0;

        // iterate forward over the line, trying to parse number strings
        // 1 subtracted from each strlen to ignore \n
        for (size_t idx = 0; idx < strlen(line_buf) - 1; idx++) {
            // every english number is either 3 chars, 4 chars, or 5 chars

            // TODO dry
            // as if i'm ever going to go back to this monstrosity

            // this loop should only read the first digit
            // should go backwards to read the other

            // if this is just a digit, add that
            char c = line_buf[idx];
            if (c >= '0' && c <= '9') {
                sum += 10 * read_single_digit(c);
                digits_read++;
                break;
            }

            unsigned int remaining = strlen(line_buf) - idx - 2;

            if (remaining < 3) {
                // can't read anymore!
                continue;
            }

            // try 3 char first
            char triple[3] = { line_buf[idx], line_buf[idx + 1], line_buf[idx + 2] };
            unsigned int triple_res = parse_num_str(triple, 3);

            if (triple_res != 0) {
                sum += 10 * triple_res;
                digits_read++;
                break;
            }

            if (remaining < 4) {
                // can't read 4 but might be able to read 3 later
                continue;
            }

            // now try 4 char
            char quadruple[4] = { line_buf[idx], line_buf[idx + 1], line_buf[idx + 2], line_buf[idx + 3] };
            unsigned int quadruple_res = parse_num_str(quadruple, 4);

            if (quadruple_res != 0) {
                sum += 10 * quadruple_res;
                digits_read++;
                break;
            }

            if (remaining < 5) {
                // can't read 5 but might be able to read 3 or 4 later
                continue;
            }

            char quintuple[5] = { line_buf[idx], line_buf[idx + 1], line_buf[idx + 2], line_buf[idx + 3], line_buf[idx + 4] };
            unsigned int quintuple_res = parse_num_str(quintuple, 5);

            if (quintuple_res != 0) {
                sum += 10 * quintuple_res;
                digits_read++;
                break;
            }
        }

        if (digits_read == 0) {
            fprintf(stderr, "Missing first digit on line %zu", line_num);
            return 1;
        }

        // now go backwards to get the other, but we must reverse the reading order
        for (size_t remaining = strlen(line_buf); remaining > 0; remaining--) {
            size_t idx = remaining - 2;

            // if this is just a digit, add that
            char c = line_buf[idx];
            if (c >= '0' && c <= '9') {
                sum += read_single_digit(c);
                digits_read++;
                break;
            }


            if (remaining < 3) {
                // can't read anymore!
                continue;
            }

            // try 3 char first
            char triple[3] = { line_buf[idx - 2], line_buf[idx - 1], line_buf[idx] };
            unsigned int triple_res = parse_num_str(triple, 3);

            if (triple_res != 0) {
                sum += triple_res;
                digits_read++;
                break;
            }

            if (remaining < 4) {
                // can't read 4 but might be able to read 3 later
                continue;
            }

            // now try 4 char
            char quadruple[4] = { line_buf[idx - 3], line_buf[idx - 2], line_buf[idx - 1], line_buf[idx] };
            unsigned int quadruple_res = parse_num_str(quadruple, 4);

            if (quadruple_res != 0) {
                sum += quadruple_res;
                digits_read++;
                break;
            }

            if (remaining < 5) {
                // can't read 5 but might be able to read 3 or 4 later
                continue;
            }

            char quintuple[5] = { line_buf[idx - 4], line_buf[idx - 3], line_buf[idx - 2], line_buf[idx - 1], line_buf[idx] };
            unsigned int quintuple_res = parse_num_str(quintuple, 5);

            if (quintuple_res != 0) {
                sum += quintuple_res;
                digits_read++;
                break;
            }
        }

        if (digits_read == 1) {
            fprintf(stderr, "Missing second digit on line %zu", line_num);
            return 1;
        }
    }

    printf("Result: %lu", sum);
    return 0;
}
