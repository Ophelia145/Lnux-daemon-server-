#ifndef READ_ELF_H
#define READ_ELF_H
#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
void read_elf_file (const char *pid, int client_fd);

#endif
