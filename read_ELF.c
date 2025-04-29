#include "read_ELF.h"
#include "daemon.h"

void
read_elf_file (const char *pid, int client_fd)
{
  char path[256];
  snprintf (path, sizeof (path), "/proc/%s/exe", pid);

  if (access (path, F_OK) != 0)
    {
      perror ("Файл ELF не найден");
      send (client_fd, "ошибка: ELF-файл не найден\n", 28, 0);
      return;
    }

  int fd = open (path, O_RDONLY);
  if (fd < 0)
    {
      perror ("ошибка открытия ELF-файла");
      send (client_fd, "ошибка: не удалось открыть ELF-файл\n", 37, 0);
      return;
    }

  Elf64_Ehdr ehdr;
  if (read (fd, &ehdr, sizeof (ehdr)) != sizeof (ehdr))
    {
      perror ("ошибка чтения ELF-заголовка");
      send (client_fd, "ошибка: не удалось прочитать ELF-заголовок\n", 42, 0);
      close (fd);
      return;
    }

  if (ehdr.e_ident[EI_MAG0] != ELFMAG0 || ehdr.e_ident[EI_MAG1] != ELFMAG1
      || ehdr.e_ident[EI_MAG2] != ELFMAG2 || ehdr.e_ident[EI_MAG3] != ELFMAG3)
    {
      send (client_fd, "ошибка: файл не является ELF\n", 29, 0);
      close (fd);
      return;
    }

  char response[1024];
  snprintf (response, sizeof (response),
            "Файл %s является ELF-файлом\n"
            "  Тип: %d\n"
            "  Машина: %d\n"
            "  Версия: %d\n"
            "  Начальный адрес: 0x%lx\n"
            "  Размер заголовка: %d байт\n",
            path, ehdr.e_type, ehdr.e_machine, ehdr.e_version, ehdr.e_entry,
            ehdr.e_ehsize);

  send (client_fd, response, strlen (response), 0);

  close (fd);
}
