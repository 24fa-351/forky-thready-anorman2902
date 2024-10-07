#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void create_forky_pattern_1(int number_of_processes);
void create_forky_pattern_2(int number_of_processes);
void create_forky_pattern_3(int current, int max);

void process(int process_id) {
    int sleep_time = rand() % 8 + 1;
    printf("Process %d (PID %d) beginning.\n", process_id, getpid());
    sleep(sleep_time);
    printf("Process %d (PID %d) exiting.\n", process_id, getpid());
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number_of_processes> <pattern number>\n", argv[0]);
        return 1;
    }

    int number_of_processes;
    int pattern_number;

    if (sscanf(argv[1], "%d", &number_of_processes) != 1 || 
    sscanf(argv[2], "%d", &pattern_number) != 1) {
        fprintf(stderr, "Error: Invalid input format. Please provide integers for number_of_processes and pattern_number.\n");
        return 1;
    }

    if (number_of_processes < 1 || number_of_processes > 256) {
        fprintf(stderr, "Error: Invalid values. Number of processes should be between 1 and 256.\n");
        return 1;
    }

    srand(time(NULL));

    if (pattern_number == 1) {
        create_forky_pattern_1(number_of_processes);
    } else if (pattern_number == 2) {
        create_forky_pattern_2(number_of_processes);
    } else if (pattern_number == 3) {
        create_forky_pattern_3(1, number_of_processes);
    } else {
        fprintf(stderr, "Errror: Invalid number! Choose 1, 2, or 3.\n");
        return 1;
    }
    return 0;
}

void create_forky_pattern_1(int number_of_processes) {
    for (int i = 1  ; i <= number_of_processes; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            printf("Process %d (PID %d) created by Main (PID %d)\n", i, getpid(), getpid());
            process(i);
            exit(0);
        } else if (pid > 0) {
            printf("Main process (PID %d) creating Process %d (PID %d).\n", getpid(), i, pid);;
        } else {
            perror("fork");
            exit(1);
        }
    }

    for (int i = 1; i <= number_of_processes; i++) {
        wait(NULL);
    }
}

void create_forky_pattern_2(int number_of_processes) {
    printf("Main process (PID %d) starting.\n", getpid());

    for (int i = 1; i <= number_of_processes; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            printf("Process %d (PID %d) created by Process %d (PID %d)\n", i, getpid(), i - 1 == 0 ? 0 : i - 1, getppid());
            process(i);
            exit(0);
        } else if (pid > 0) {
            if (i == 1) {
                printf("Main process (PID %d) creating Process %d (PID %d).\n", getpid(), i, pid);
            } else {
                printf("Process %d (PID %d) creating Process %d (PID %d).\n", i - 1, getpid(), i, pid);
            }
            wait(NULL);
        } else {
            perror("fork");
            exit(1);
        }
    }
    printf("Main process (PID %d) exiting.\n", getpid());
}


void create_forky_pattern_3(int current, int max) {
    if (current > max) {
        return;
    }

    if (current == 1) {
        printf("Main process (PID %d) creating Process %d.\n", getpid(), current);
    }

    pid_t left_pid = fork();
    if (left_pid == 0 && current * 2 <= max) {
        printf("Process %d (PID %d) created by Process %d (PID %d)\n", current * 2, getpid(), current, getppid());
        process(current * 2);
        exit(0);
    } else if (left_pid > 0) {
        printf("Process %d (PID %d) creating Process %d (PID %d).\n", current, getpid(), current * 2, left_pid);
    } else {
        perror("fork");
        exit(1);
    }

    pid_t right_pid = fork();
    if (right_pid == 0 && current * 2 + 1 <= max) {
        printf("Process %d (PID %d) created by Process %d (PID %d)\n", current * 2 + 1, getpid(), current, getppid());
        process(current * 2 + 1);
        exit(0);
    } else if (right_pid > 0) {
        printf("Process %d (PID %d) creating Process %d (PID %d).\n", current, getpid(), current * 2 + 1, right_pid);
    } else {
        perror("fork");
        exit(1);
    }

    wait(NULL);
    wait(NULL);
}
