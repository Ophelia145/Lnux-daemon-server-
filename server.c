#include "daemon.h"
#include "read_ELF.h"
void
handle_client (int client_fd)
{
  char buffer[1024] = { 0 };

  while (1)
    {
      ssize_t bytes = recv (client_fd, buffer, sizeof (buffer) - 1, 0);

      if (bytes <= 0)
        {
          printf ("Клиент закрыл соединение.\n");
          break;
        }

      buffer[bytes] = '\0'; // Завершаем строку
      printf ("Получена команда: %s\n", buffer);

      if (strncmp (buffer, "list", 4) == 0)
        {
          FILE *proc = popen ("ls /proc | grep -E '^[0-9]+$'", "r");
          while (fgets (buffer, sizeof (buffer), proc))
            {
              send (client_fd, buffer, strlen (buffer), 0);
            }
          pclose (proc);
        }
      else if (strncmp (buffer, "kill", 4) == 0)
        {
          pid_t pid = atoi (buffer + 5);
          if (kill_process (pid) == 0)
            {
              snprintf (buffer, sizeof (buffer), "Процесс %d завершен\n", pid);
              send (client_fd, buffer, strlen (buffer), 0);
            }
        }
      else if (strncmp (buffer, "readelf ", 8) == 0)
        {
          char pid[32];
          sscanf (buffer + 8, "%s", pid);
          printf ("Чтение ELF-файла для процесса с PID: %s\n", pid);
          read_elf_file (pid, client_fd);
        }

      else if (strncmp (buffer, "shutdown", 8) == 0)
        {
          printf ("Сервер остановлен\n");
          send (client_fd, "Сервер завершает работу\n", 24, 0);
          close (client_fd);
          unlink (SOCKET_PATH);
          exit (EXIT_SUCCESS);
        }
      else
        {
          send (client_fd, "Неизвестная команда\n", 20, 0);
        }

      // Очистка буфера для следующей команды
      memset (buffer, 0, sizeof (buffer));
    }
}

int
main ()
{
  init_daemon ();

  int server_fd = socket (AF_UNIX, SOCK_STREAM, 0); //дескриптор сокета
  if (server_fd < 0)
    {
      perror ("Ошибка создания сокета");
      exit (EXIT_FAILURE);
    }

  struct sockaddr_un server_addr = { 0 };
  server_addr.sun_family = AF_UNIX;
  strncpy (server_addr.sun_path, SOCKET_PATH,
           sizeof (server_addr.sun_path) - 1);

  unlink (SOCKET_PATH);
  if (bind (server_fd, (struct sockaddr *)&server_addr, sizeof (server_addr))
      < 0)
    {
      perror ("Ошибка привязки сокета");
      exit (EXIT_FAILURE);
    }

  listen (server_fd, 5);
  printf ("Демон запущен, ждем подключения.......\n");

  while (1)
    {
      int client_fd = accept (server_fd, NULL, NULL);
      if (client_fd < 0)
        {
          perror ("Ошибка подключения клиента");
          continue;
        }
      handle_client (client_fd);
    }

  close (server_fd);
  return 0;
}
