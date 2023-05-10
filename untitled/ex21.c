// Nir Koren 316443902

#include <stdio.h> // don't forget to delete this line
#include <unistd.h>
#include <fcntl.h>
#define BUFFER_SIZE 1024

int my_read(int fd, char *buffer, int size, int read_from, int *is_end_of_file) {
    char buff[BUFFER_SIZE];
    int offset = BUFFER_SIZE;
    int read_max = BUFFER_SIZE;
    int i;
    for (i = 0; i < size; ++i) {
        if (offset == read_max) {
            read_max = read(fd, buff, BUFFER_SIZE);
            if (read_max == 0) {
                *is_end_of_file = 1;
                return i;
            }
            if (read_max == -1) {
                perror("Error in: read");
                return -1;
            }
            offset = 0;
        }
        buffer[i + BUFFER_SIZE - read_from] = buff[offset];
        offset++;
    }
    return i + BUFFER_SIZE - read_from;
}

int check_identical(char *buff1, char *buff2, int size) {
    for (int i = 0; i < size; ++i) {
        if (buff1[i] != buff2[i]) {
            return 0;
        }
    }
    return 1;
}

char to_lower(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + 32;
    }
    return c;
}

int check_space(char c) {
    if (c == ' ' || c == '\n') {
        return 1;
    }
    return 0;
}

int check_similar(char *buff1, char *buff2, int size1, int size2, int* offset1, int* offset2) {
    int i = 0;
    int j = 0;
    while (i < size1 && j < size2) {
        if (check_space(buff1[i])) {
            i++;
            continue;
        }
        if (check_space(buff2[j])) {
            j++;
            continue;
        }
        if (to_lower(buff1[i]) != to_lower(buff2[j])) {
            return 0;
        }
        i++;
        j++;
    }
    *offset1 = i;
    *offset2 = j;

    while (j < size2) {
        if (!check_space(buff2[j])) {
            return 0;
        }
        j++;
    }

    while (i < size1) {
        if (!check_space(buff1[i])) {
            return 0;
        }
        i++;
    }
    return 1;
}


void make_offset_the_beginning_of_the_buffer(int offset, char *buffer, int size) {
    for (int i = 0; i < size - offset; ++i) {
        buffer[i] = buffer[i + offset];
    }
}



// this function will return 1 if the files are identical, 3 if they are similar and 2 if they are different
// similar means that they have the same characters not including spaces and new lines, and case-insensitive

int main(int argc, char const *argv[]) {
    // check if the user entered 2 files
    if (argc != 3) {
        perror("Usage: comp.out <file1> <file2>");
        return -1;
    }

    // open the files
    int mode = 1;
    char const *file1 = argv[1];
    char const *file2 = argv[2];
    int fp1 = open(file1, O_RDONLY);
    int fp2 = open(file2, O_RDONLY);
    if (fp1 < 0 || fp2 < 0) {
        perror("Error in: open");
        return -1;
    }

    // read the files
    char buff1[BUFFER_SIZE];
    char buff2[BUFFER_SIZE];
    int offset1 = BUFFER_SIZE;
    int offset2 = BUFFER_SIZE;
    int is_end_of_file1 = 0;
    int is_end_of_file2 = 0;
    int read1 = 0;
    int read2 = 0;

    do {

        read1 = my_read(fp1, buff1, sizeof(buff1), offset1, &is_end_of_file1);
        read2 = my_read(fp2, buff2, sizeof(buff2), offset2, &is_end_of_file2);

        if (read1 == -1 || read2 == -1) {
            perror("Error in: read");
            close(fp1);
            close(fp2);
            return -1;
        }
        if (read1 != read2) {
            mode = 3;
        }
        if (mode == 1) {
            if (check_identical(buff1, buff2, read1) == 0) {
                mode = 3;
            }
        }
        if (mode == 3) {
            if (check_similar(buff1, buff2, read1, read2, &offset1, &offset2) == 0) {
                close(fp1);
                close(fp2);
                return 2;
            }
            make_offset_the_beginning_of_the_buffer(offset1, buff1, sizeof(buff1));
            make_offset_the_beginning_of_the_buffer(offset2, buff2, sizeof(buff2));
        }
        if (is_end_of_file1 && is_end_of_file2 && read1 == 0 && read2 == 0) {
            break;
        }
    } while (1);

    close(fp1);
    close(fp2);
    return mode;
}