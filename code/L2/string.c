#include <string.h>
#include <stdio.h>
#include <stdlib.h>

double compute_mean(double data[], unsigned cnt) {
    if (cnt == 0) return 0.0;

    double sum = 0.0;
    for (int i = 0; i < cnt; i++) {
        sum += data[i];
    }
    return sum / cnt;
}

void calc_mean() {
    double data[100];
    unsigned max_sz = sizeof(data) / sizeof(data[0]); // capacity calculation
    unsigned cnt = 0;

    char buf[100];
    printf("Please enter numbers (up to 100):\n");
    while (fgets(buf, sizeof(buf), stdin) && cnt < max_sz) {
        if (strcmp(buf, "end\n") == 0) break;
        data[cnt] = strtod(buf, NULL); // Convert string to double
        cnt += 1;
    }

    printf("We read %u numbers. They are:\n", cnt);
    for (int i = 0; i < cnt; i++) {
        printf("Number %d: %f\n", i, data[i]);
    }

    printf("Their mean is %f\n", compute_mean(data, cnt));
}


int main() {
    char s[] = {'T', 'h', 'i', 's', '\0' };
    char s2[] = "ABCDE";

    // %zu is the format specifier for the "size_t" type
    printf("s2=%s: Size of s2: %zu; Length of s2: %zu\n", s2, sizeof(s2), strlen(s2));
    for (int i = 0; i < sizeof(s2); i++) {
        printf("Character %d is '%c' (%#x)\n", i, s2[i], s2[i]);
    }

    // Convert lowercase to UPPERCASE
    char s3[] = "This is a twilight world\n";
    for (int i = 0; i < strlen(s3); i++) {
        if (s3[i] >= 'a' && s3[i] <= 'z') {
            s3[i] = s3[i] - 'a' + 'A';
        }
    }
    printf("s3 now is: %s\n", s3);

    // We cannot directly do s4 = s3; assignment
    // We use strncpy for safety. Be careful of the terminating zero
    char s4[10] = {'\0'};
    strncpy(s4, s3, sizeof(s4) - 1);

    // Or do this
    // strncpy(s4, s3, sizeof(s4));
    // s4[sizeof(s4) - 1] = '\0';
    printf("s4=%s\n", s4);

    // String comparison. Returns 0 if two strings are the same
    printf("Cmp: %d\n", strcmp("ABCD", "aBCD"));

    char buf[100];
    while (1) {
        printf("> ");
        fgets(buf, sizeof(buf), stdin);
        if (strcmp(buf, "quit\n") == 0) {
            printf("Exiting...\n");
            return 0;
        } else if (strcmp(buf, "mean\n") == 0) {
            calc_mean();
        } else {
            printf("You entered %s\n", buf);
        }
    }
}
