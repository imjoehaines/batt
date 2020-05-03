#include <stdio.h>
#include <unistd.h>

enum Mode {
    NONE,
    TMUX,
    SH,
};

int main(int argc, char **argv)
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

    enum Mode mode = NONE;
    int option;

    while ((option = getopt(argc, argv, "ts")) != -1) {
        switch (option) {
            case 't':
                mode = TMUX;
                break;

            case 's':
                mode = SH;
                break;
        }
    }

    // reference for colours:
    // https://github.com/morhetz/gruvbox

    int fg = 239;
    int bg;

    if (percentage >= 80) {
        bg = 142;
    } else if (percentage >= 50) {
        bg = 108;
    } else if (percentage >= 20) {
        bg = 175;
    } else if (percentage >= 10) {
        bg = 214;
    } else {
        fg = 229;
        bg = 167;
    }

    switch (mode) {
        case TMUX:
            printf(
                "#[fg=colour%d, bg=colour%d, bold] %d%% #[fg=default, bg=default, nobold]",
                fg,
                bg,
                percentage
            );
            break;

        case SH:
            // We use the bg colour here for the foreground because
            // it's less disruptive
            printf("\\033[38;5;%dm%d%%\\033[0m", bg, percentage);
            break;

        case NONE:
            printf("%d\n", percentage);
            break;
    }
    
    return 0;
}
