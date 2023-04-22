#include <stdio.h>

int main() {
    FILE *fp;
    char ch;

    // Open the file in read mode
    fp = fopen("/root/flag.txt", "r");

    // If the file cannot be opened, exit with error
    if (fp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    // Read and output each character in the file
    while ((ch = fgetc(fp)) != EOF) {
        printf("%c", ch);
    }

    // Close the file
    fclose(fp);

    return 0;
}