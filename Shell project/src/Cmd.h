#ifndef CMD_H
#define CMD_H

#include <stddef.h>  // For NULL, size_t, mode_t
#include <stdio.h>   // For printf, FILE, perror
#include <stdlib.h>  // For malloc, free if needed
#include <string.h>  // For strcmp, strstr, snprintf
#include <sys/types.h> // For mode_t
#include <sys/stat.h> // For mkdir, stat
#include <fcntl.h>    // For open flags
#include <unistd.h>   // For close, chdir, getcwd
#include <dirent.h>   // For DIR, opendir, readdir
#include "Hadder.h"

// Command strings array
char *CMMND[] = {
    "ls", "cd", "pwd", "mkdir", "rmdir", "cp", "mv", "rm", "touch", "find", "locate",
    "cat", "less", "head", "tail", "cut", "sort", "uniq", "grep", "wc", "diff","exit"
    "uname", "hostname", "top", "ps", "df", "du", "free", "uptime", "lscpu", "lsblk",
    "whoami", "who", "id", "su", "sudo", "useradd", "passwd", "groups", "adduser",
    "chmod", "chown", "chgrp", "umask",
    "apt", "apt-get", "dpkg", "yum", "dnf", "rpm", "pacman", "zypper",
    "ping", "ifconfig", "ip", "netstat", "ss", "traceroute", "nslookup", "dig", "curl", "wget", "scp", "ssh",
    "tar", "gzip", "gunzip", "bzip2", "bunzip2", "zip", "unzip",
    "ps", "kill", "killall", "bg", "fg", "jobs", "nice", "renice", "nohup",
    "mount", "umount", "fdisk", "mkfs", "fsck", "blkid",
    "echo", "read", "test", "expr", "bc", "awk", "sed",
    "man", "info", "whatis", "apropos"
};

void cmd_ls(const char *path);
int  cmd_cd(const char *path);
void cmd_pwd(void);
int  cmd_mkdir(const char *path, mode_t mode);
int  cmd_rmdir(const char *path);
int  cmd_cp(const char *src, const char *dest);
int  cmd_mv(const char *src, const char *dest);
int  cmd_rm(const char *path);
int  cmd_touch(const char *path);
void cmd_find(const char *start_path, const char *pattern);
void cmd_locate(const char *pattern);
void cmd_cat(const char *file);
void cmd_less(const char *file);
void cmd_head(const char *file, int n);
void cmd_tail(const char *file, int n);
void cmd_cut(const char *file, char delim, int fieldnum);
void cmd_sort(const char *file);
void cmd_uniq(const char *file);
void cmd_grep(const char *file, const char *pattern);
void cmd_wc(const char *file);
void cmd_diff(const char *file1, const char *file2);
void cmd_exit(int status);

// Function definitions

void cmd_exit(int status) {
    // Exit the shell with the specified status code
    exit(status);
}
void cmd_ls(const char *path) {
    struct dirent *entry;
    DIR *dp = opendir(path ? path : ".");

    if (!dp) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp)) != NULL) {
        printf("%s  ", entry->d_name);
    }
    printf("\n");
    closedir(dp);
}

int cmd_cd(const char *path) {
    if (chdir(path) != 0) {
        perror("chdir");
        return -1;
    }
    return 0;
}

void cmd_pwd(void) {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("getcwd");
    }
}

int cmd_mkdir(const char *path, mode_t mode) {
    if (mkdir(path, mode) != 0) {
        perror("mkdir");
        return -1;
    }
    return 0;
}

int cmd_rmdir(const char *path) {
    if (rmdir(path) != 0) {
        perror("rmdir");
        return -1;
    }
    return 0;
}

int cmd_cp(const char *src, const char *dest) {
    FILE *fsrc = fopen(src, "rb");
    if (!fsrc) {
        perror("fopen src");
        return -1;
    }
    FILE *fdest = fopen(dest, "wb");
    if (!fdest) {
        perror("fopen dest");
        fclose(fsrc);
        return -1;
    }
    char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), fsrc)) > 0)
        fwrite(buf, 1, n, fdest);

    fclose(fsrc);
    fclose(fdest);
    return 0;
}

int cmd_mv(const char *src, const char *dest) {
    if (rename(src, dest) != 0) {
        perror("rename");
        return -1;
    }
    return 0;
}

int cmd_rm(const char *path) {
    if (remove(path) != 0) {
        perror("remove");
        return -1;
    }
    return 0;
}

int cmd_touch(const char *path) {
    int fd = open(path, O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    close(fd);
    return 0;
}

void cmd_find(const char *start_path, const char *pattern) {
    struct dirent *entry;
    DIR *dp = opendir(start_path);
    if (!dp) return;

    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", start_path, entry->d_name);

        if (strstr(entry->d_name, pattern) != NULL)
            printf("%s\n", full_path);

        struct stat st;
        if (stat(full_path, &st) == 0 && S_ISDIR(st.st_mode)) {
            cmd_find(full_path, pattern);
        }
    }
    closedir(dp);
}

void cmd_locate(const char *pattern) {
    if (!pattern || strlen(pattern) == 0) {
        fprintf(stderr, "Error: empty pattern for locate\n");
        return;
    }

    const char *start_path = "/";
    printf("Warning: Searching entire filesystem (slow). Use narrow pattern.\n");
    cmd_find(start_path, pattern);
}


void cmd_cat(const char *file) {
    FILE *fp = fopen(file, "r");
    if (!fp) { perror("fopen"); return; }
    int c;
    while ((c = fgetc(fp)) != EOF) {
        putchar(c);
    }
    fclose(fp);
}

// --- less: simple pager with space-to-advance one line
void cmd_less(const char *file) {
    FILE *fp = fopen(file, "r");
    if (!fp) { perror("fopen"); return; }
    int c;
    int lines_printed = 0;
    while ((c = fgetc(fp)) != EOF) {
        putchar(c);
        if (c == '\n') lines_printed++;
        if (lines_printed >= 20) {
            printf("--More--");
            int ch = getchar();
            if (ch == 'q') break;
            lines_printed = 0;
        }
    }
    fclose(fp);
}

// --- head: print first n lines
void cmd_head(const char *file, int n) {
    FILE *fp = fopen(file, "r");
    if (!fp) { perror("fopen"); return; }
    char line[4096];
    int count = 0;
    while (count < n && fgets(line, sizeof(line), fp)) {
        fputs(line, stdout);
        count++;
    }
    fclose(fp);
}

// --- tail: print last n lines (simple buffering)
void cmd_tail(const char *file, int n) {
    FILE *fp = fopen(file, "r");
    if (!fp) { perror("fopen"); return; }
    char **lines = malloc(n * sizeof(char*));
    int i, count = 0;
    for (i = 0; i < n; i++) lines[i] = NULL;

    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), fp)) {
        free(lines[count % n]);
        lines[count % n] = strdup(buffer);
        count++;
    }
    int start = count > n ? count % n : 0;
    int total = count > n ? n : count;
    for (i = 0; i < total; i++) {
        fputs(lines[(start + i) % n], stdout);
        free(lines[(start + i) % n]);
    }
    free(lines);
    fclose(fp);
}

// --- cut: print only specific field by delimiter number (1-based)
void cmd_cut(const char *file, char delim, int fieldnum) {
    FILE *fp = fopen(file, "r");
    if (!fp) { perror("fopen"); return; }
    char line[4096];
    while (fgets(line, sizeof(line), fp)) {
        int count = 1;
        char *token = strtok(line, &delim);
        while (token) {
            if (count == fieldnum) {
                printf("%s\n", token);
                break;
            }
            token = strtok(NULL, &delim);
            count++;
        }
    }
    fclose(fp);
}

// --- sort: sort lines lexicographically
int cmp_lines(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}
void cmd_sort(const char *file) {
    FILE *fp = fopen(file, "r");
    if (!fp) { perror("fopen"); return; }
    size_t lines_cap = 128;
    size_t num_lines = 0;
    char **lines = malloc(lines_cap * sizeof(char *));
    if (!lines) { perror("malloc"); fclose(fp); return; }

    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (num_lines == lines_cap) {
            lines_cap *= 2;
            lines = realloc(lines, lines_cap*sizeof(char*));
            if (!lines) { perror("realloc"); fclose(fp); return; }
        }
        lines[num_lines++] = strdup(buffer);
    }
    qsort(lines, num_lines, sizeof(char*), cmp_lines);
    for (size_t i = 0; i < num_lines; i++) {
        fputs(lines[i], stdout);
        free(lines[i]);
    }
    free(lines);
    fclose(fp);
}

// --- uniq: print unique adjacent lines only
void cmd_uniq(const char *file) {
    FILE *fp = fopen(file, "r");
    if (!fp) { perror("fopen"); return; }
    char line[4096];
    char last[4096] = "";
    while (fgets(line, sizeof(line), fp)) {
        if (strcmp(line, last) != 0) {
            fputs(line, stdout);
            strcpy(last, line);
        }
    }
    fclose(fp);
}

// --- grep: lines that contain pattern substring
void cmd_grep(const char *file, const char *pattern) {
    FILE *fp = fopen(file, "r");
    if (!fp) { perror("fopen"); return; }
    char line[4096];
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, pattern))
            printf("%s", line);
    }
    fclose(fp);
}

// --- wc: count lines, words, bytes
void cmd_wc(const char *file) {
    FILE *fp = fopen(file, "r");
    if (!fp) { perror("fopen"); return; }
    int lines = 0, words = 0, bytes = 0;
    int in_word = 0;
    int c;
    while ((c = fgetc(fp)) != EOF) {
        bytes++;
        if (c == '\n') lines++;
        if (isspace(c)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            words++;
        }
    }
    printf("%d %d %d %s\n", lines, words, bytes, file);
    fclose(fp);
}

// --- diff: print differing lines between two files (simple line by line)
void cmd_diff(const char *file1, const char *file2) {
    FILE *f1 = fopen(file1, "r");
    FILE *f2 = fopen(file2, "r");
    if (!f1 || !f2) {
        if (!f1) perror(file1);
        if (!f2) perror(file2);
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return;
    }
    char line1[4096], line2[4096];
    int line_no = 1;
    while (fgets(line1, sizeof(line1), f1) && fgets(line2, sizeof(line2), f2)) {
        if (strcmp(line1, line2) != 0) {
            printf("Line %d differs:\n< %s> %s", line_no, line1, line2);
        }
        line_no++;
    }
    // Check if one file has extra lines
    while (fgets(line1, sizeof(line1), f1)) {
        printf("Line %d only in %s: %s", line_no++, file1, line1);
    }
    while (fgets(line2, sizeof(line2), f2)) {
        printf("Line %d only in %s: %s", line_no++, file2, line2);
    }
    fclose(f1);
    fclose(f2);
}
#endif // CMD_H
