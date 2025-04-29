#include "daemon.h"

void
init_daemon ()
{
  pid_t pid = fork ();

  if (pid < 0)
    {
      perror ("не создался процесс");
      exit (EXIT_FAILURE);
    }
  if (pid > 0)
    {
      exit (EXIT_SUCCESS); //завершаем родительский процесс чтоб сироту, что мы
                           //создали усыновил init, кот контролит фоновые
                           //процессы - это сделает сироту фоновым
    }

  if (setsid () < 0)
    {
      perror ("Ошибка при создании сессии");
      exit (EXIT_FAILURE);
    }

  signal (SIGCHLD, SIG_IGN); //игнорим сигнал завершения дочерних процессов

  pid = fork ();
  if (pid < 0)
    {
      perror ("Ошибка при форке");
      exit (EXIT_FAILURE);
    }
  if (pid > 0)
    {
      exit (EXIT_SUCCESS);
    }

  fclose (stdin);
  fclose (stdout);
  fclose (stderr);
}
