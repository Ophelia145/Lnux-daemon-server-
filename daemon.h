#ifndef DAEMON_H
#define DAEMON_H
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#define SOCKET_PATH "/tmp/unix_socket_demo"

void init_daemon ();
void list_process_ids (const char *path);
int kill_process (pid_t pid);
int is_directory (const char *path);

#endif
