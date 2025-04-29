#include "daemon.h"

int
main ()
{
  int sock_fd = socket (AF_UNIX, SOCK_STREAM, 0);
  if (sock_fd < 0)
    {
      perror ("Ошибка создания сокета");
      return EXIT_FAILURE;
    }

  struct sockaddr_un server_addr = { 0 };
  server_addr.sun_family = AF_UNIX;
  strncpy (server_addr.sun_path, SOCKET_PATH,
           sizeof (server_addr.sun_path) - 1);

  if (connect (sock_fd, (struct sockaddr *)&server_addr, sizeof (server_addr))
      < 0)
    {
      perror ("Ошибка подключения к серверу");
      return EXIT_FAILURE;
    }

  char buffer[1024];
  printf ("Введите команду (list, kill <pid>, readelf <file>, shutdown):\n");
  while (fgets (buffer, sizeof (buffer), stdin))
    {
      // Отправка команды на сервер
      if (send (sock_fd, buffer, strlen (buffer), 0) < 0)
        {
          perror ("Ошибка отправки данных");
          break;
        }

      // Ожидание ответа от сервера
      memset (buffer, 0, sizeof (buffer));
      ssize_t bytes = recv (sock_fd, buffer, sizeof (buffer) - 1, 0);
      if (bytes > 0)
        {
          printf ("Ответ от сервера:\n%s\n", buffer);
        }
      else if (bytes == 0)
        {
          printf ("Соединение закрыто сервером.\n");
          break;
        }
      else
        {
          perror ("Ошибка получения данных");
          break;
        }

      printf ("Введите следующую команду:\n");
    }

  close (sock_fd);
  return EXIT_SUCCESS;
}
