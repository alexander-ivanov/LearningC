#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int i = 0;
    
    while (i < 100) {
        usleep(3000);
        if (i == 3) {
            strcpy(NULL, "Hello");
        }
        i++;
    }

    return 0;
}
