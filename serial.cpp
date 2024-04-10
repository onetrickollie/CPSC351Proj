#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    char line[100];
    int location_count = 0;

    // Read each location from the file
    while (fgets(line, sizeof(line), file) != NULL) {
        location_count++;
        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            return 1;
        } else if (pid == 0) {
            // Child process
            printf("Child PID: %d, Parent PID: %d\n", getpid(), getppid());
            printf("UID: %d, GID: %d\n", getuid(), getgid());

            double val1, val2;
            // Read latitude and longitude from the line
            sscanf(line, "%lf %lf", &val1, &val2);

            // Construct the URL string
            char URL[100];
            snprintf(URL, sizeof(URL), "https://api.open-meteo.com/v1/forecast?latitude=%0.6lf&longitude=%0.6lf&current_weather=True", val1, val2);

            // Execute curl to fetch weather information
            char filename[20];
            sprintf(filename, "file%d.json", location_count);
            execlp("/usr/bin/curl", "curl", "-o", filename, URL, NULL);
            perror("execlp failed"); // This will only execute if execlp fails
            exit(1);
        } else {
            wait(NULL);
        }
    }

    fclose(file);
    return 0;
}

    
