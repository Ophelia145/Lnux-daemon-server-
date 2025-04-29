#include "daemon.h"

int
is_numeric (const char *str)
{
  while (*str)
    {
      if (!isdigit (*str))
        {
          return 0;
        }
      str++;
    }
  return 1;
}

int
is_directory (const char *path)
{
  struct stat st;

  if (stat (path, &st) == 0
      && S_ISDIR (st.st_mode)) //режим = тип файла и права доступа

    {
      return 1;
    }
  return 0;
}

void
list_process_ids (const char *path)
{
  DIR *dir = opendir (path);
  if (!dir)
    {
      perror ("не открылась /proc");
      return;
    }

  printf ("Список процессов:\n");
  struct dirent *entry;
  while ((entry = readdir (dir)) != NULL)
    {
      if (is_directory (path) && is_numeric (entry->d_name))
        {
          printf ("PID: %s\n", entry->d_name);
        }
    }

  closedir (dir);
}

int
kill_process (pid_t pid)
{
  if (kill (pid, SIGKILL) == 0)
    {
      printf ("Процесс %d успешно завершен\n", pid);
      return 0;
    }
  else
    {
      perror ("Не удалось завершить процесс");
      return -1;
    }
}
