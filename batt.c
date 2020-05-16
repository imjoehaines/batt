#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

enum Mode {
    PLAIN,
    TMUX,
    SHELL,
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

    enum Mode mode = SHELL;
    int option;
    bool add_newline = true;

    while ((option = getopt(argc, argv, "tpn")) != -1) {
        switch (option) {
            case 't':
                mode = TMUX;
                break;

            case 'p':
                mode = PLAIN;
                break;

            case 'n':
                add_newline = false;
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

        case SHELL:
            // We use the bg colour here for the foreground because
            // it's less disruptive
            printf("\033[38;5;%dm%d%%\033[0m", bg, percentage);
            break;

        case PLAIN:
            printf("%d", percentage);
            break;
    }

    if (add_newline) {
        printf("\n");
    }
    
    return 0;
}
