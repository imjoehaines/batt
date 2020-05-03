#include <stdio.h>

int main()
{
    FILE *handle = fopen("/sys/class/power_supply/BAT0/capacity", "r");

    if (!handle) {
        perror("fopen()");
        return 1;
    }

    int percentage = -1;

    int success = fscanf(handle, "%d", &percentage);

    if (!success) {
        perror("fscanf()");
        return 1;
    }

    if (percentage < 0 || percentage > 100) {
        printf("Bad percentage: '%d'\n", percentage);
        return 1;
    }

    fclose(handle);

    printf("%d\n", percentage);

    return 0;
}
