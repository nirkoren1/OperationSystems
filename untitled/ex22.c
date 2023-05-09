// Nir Koren 316443902

#include <stdio.h> // don't forget to delete this line
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/wait.h>
#include <stdlib.h>

#define CONFIG_ROW_SIZE 155

void get_config_details(char const *config_file_path, char subdirs[CONFIG_ROW_SIZE], char input_file[CONFIG_ROW_SIZE],
                        char output_file[CONFIG_ROW_SIZE]) {
    int config_file = open(config_file_path, O_RDONLY);
    char buff[CONFIG_ROW_SIZE * 3];
    int read_bytes = read(config_file, buff, CONFIG_ROW_SIZE * 3);

    // separate the rows
    int i = 0;
    int j = 0;
    int k = 0;
    while (i < read_bytes) {
        if (buff[i] == '\n') {
            // put null terminator
            if (j == 0) {
                subdirs[k] = '\0';
            } else if (j == 1) {
                input_file[k] = '\0';
            } else {
                output_file[k] = '\0';
            }
            j++;
            k = 0;
            i++;
            continue;
        }
        if (j == 0) {
            subdirs[k] = buff[i];
        } else if (j == 1) {
            input_file[k] = buff[i];
        } else {
            output_file[k] = buff[i];
        }
        i++;
        k++;
    }
}

int check_c_file(char *file_name) {
    int i = 0;
    while (file_name[i] != '\0') {
        i++;
    }
    if (file_name[i - 1] == 'c' && file_name[i - 2] == '.') {
        return 1;
    }
    return 0;
}

int compile(char *file_name) {
    char *args[] = {"gcc", file_name, "-o", "my_exec", NULL};
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        // child process
        // add the pid and the command to the history
        int ex_result = execvp(args[0], args);
        if (ex_result == -1) {
            exit(-1);
        }
    } else if (pid > 0) {
        // parent process
        int status;
        waitpid(pid, &status, 0);
        if (status == 256) {
            return 1;
        }
        return 0;
    } else {
        perror("Error in: fork");
        return -1;
    }
}

int chdir_prev() {
    int chdir_res = chdir("..");
    if (chdir_res == -1) {
        perror("Error in: chdir");
        return -1;
    }
    return 0;
}

int write_grade_to_csv(int results_fd, char *student_name, int grade) {
    int grade_bkp = grade;
    char buff[CONFIG_ROW_SIZE];
    // copy student name
    int i = 0;
    while (student_name[i] != '\0') {
        buff[i] = student_name[i];
        i++;
    }
    buff[i] = ',';
    i++;
    // copy grade
    int grade_copy = grade;
    int digits = 0;
    while (grade_copy != 0) {
        grade_copy /= 10;
        digits++;
    }
    int j = i + digits;
    while (grade != 0) {
        buff[j - 1] = (char) (grade % 10 + '0');
        grade /= 10;
        j--;
    }
    if (grade_bkp == 0) {
        buff[i] = '0';
        digits = 1;
    }
    // add ','
    buff[i + digits] = ',';
    char *reason;
    if (grade_bkp == 100) {
        reason = "EXCELLENT";
    } else if (grade_bkp == 75) {
        reason = "SIMILAR";
    } else if (grade_bkp == 50) {
        reason = "WRONG";
    } else if (grade_bkp == 20) {
        reason = "TIMEOUT";
    } else if (grade_bkp == 10) {
        reason = "COMPILATION_ERROR";
    } else if (grade_bkp == 0) {
        reason = "NO_C_FILE";
    }
    // copy reason
    int k = 0;
    while (reason[k] != '\0') {
        buff[i + digits + 1 + k] = reason[k];
        k++;
    }
    buff[i + digits + 1 + k] = '\n';
    buff[i + digits + 1 + k + 1] = '\0';
    int write_res = write(results_fd, buff, i + digits + 1 + k + 1);
    if (write_res == -1) {
        perror("Error in: write");
        return -1;
    }
    return 0;
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        perror("Usage: %s <config_file>\n");
        return 1;
    }
    char const *config_file_path = argv[1];
    char subdirs[CONFIG_ROW_SIZE];
    char input_file[CONFIG_ROW_SIZE];
    char output_file[CONFIG_ROW_SIZE];
    get_config_details(config_file_path, subdirs, input_file, output_file);

    DIR *students = opendir(subdirs);
    if (students == NULL) {
        printf("Not a valid directory\n");
        return -1;
    }

    int input_fd = open(input_file, O_RDONLY);
    if (input_fd == -1) {
        printf("Input file not exist\n");
        return -1;
    }

    int output_fd = open(output_file, O_RDONLY);
    if (output_fd == -1) {
        printf("Output file not exist\n");
        return -1;
    }

    int results_fd = open("results2.csv", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (results_fd == -1) {
        perror("Error in: open\n");
        return -1;
    }

    struct dirent *student_entry;
    struct dirent *inside_student_entry;

    int chdir_res = chdir(subdirs);
    if (chdir_res == -1) {
        perror("Error in: chdir\n");
        return -1;
    }

    while ((student_entry = readdir(students))) {
        if (student_entry->d_name[0] == '.') {
            continue;
        }

        printf("%s:\n", student_entry->d_name);
        DIR *inside_dirs = opendir(student_entry->d_name);
        if (inside_dirs == NULL) {
            continue;
        }

        // dup2 error descriptor

        int chdir_res = chdir(student_entry->d_name);
        if (chdir_res == -1) {
            perror("Error in: chdir\n");
            continue;
        }

        int c_file_exits = 0;
        char *c_file_name;
        while ((inside_student_entry = readdir(inside_dirs))) {
            if (inside_student_entry->d_name[0] == '.') {
                continue;
            }
            if (check_c_file(inside_student_entry->d_name)) {
                c_file_exits = 1;
                c_file_name = inside_student_entry->d_name;
                break;
            }
        }

        if (!c_file_exits) {
            // no c file, write to results.csv
            printf("No c file\n");

            if(write_grade_to_csv(results_fd, student_entry->d_name, 0) == -1){
                continue;
            }


            chdir_res = chdir_prev();
            if (chdir_res == -1) {
                continue;
            }
            continue;
        }

        printf("c file= %s\n", c_file_name);
        if (compile(c_file_name)) {
            // compilation error, write to results.csv
            if (write_grade_to_csv(results_fd, student_entry->d_name, 10) == -1) {
                continue;
            }
            continue;
        }


        chdir_res = chdir_prev();
        if (chdir_res == -1) {
            return -1;
        }


    }

    closedir(students);
    close(input_fd);
    close(output_fd);
    close(results_fd);
    return 0;
}
