#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static char *
get_tty_name(void)
{
  return ttyname(STDIN_FILENO);
}

static char *
get_filename(void)
{
  char *filename = malloc(sizeof(char) * 255);
  char *tty = get_tty_name();
  char *pch;

  while ((pch = strchr(tty, '/')))
    *pch = '_';

  sprintf(filename, "/tmp/baps1_%u_%s", getuid(), tty);

  return filename;
}

static time_t
load_previous_time(char *filename)
{
  FILE *timefile = fopen(filename, "r");
  time_t prev;

  fscanf(timefile, "%ld", &prev);
  fclose(timefile);

  return prev;
}

static time_t
get_previous_time(char *filename)
{
  time_t prev;

  if (access(filename, R_OK) == 0) {
    prev = load_previous_time(filename);
  }
  else {
    prev = 0;
  }

  return prev;
}

static int
get_tty_number(void)
{
  char *tty = get_tty_name();
  char *num = strrchr(tty, '/') + 1;

  return atoi(num);
}

static char
get_tty_type(void)
{
  char *tty = get_tty_name();

  strtok(tty, "/");             /* dev */

  return strtok(NULL, "/")[0];
}

static int
save_current_time(char *filename)
{
  FILE *timefile = fopen(filename, "w");
  time_t now;

  if (!timefile)
    return 0;

  time(&now);
  fprintf(timefile, "%ld", now);
  fclose(timefile);

  return 1;
}

static void
print_time_since(time_t lasttime)
{
  double h, m, s;
  time_t now;

  time(&now);
  s = difftime(now, lasttime);

  if (s >= 60.0) {
    m = floor(s / 60.0);
    s -= m * 60;

    if (m >= 60.0) {
      h = floor(m / 60.0);
      m -= h * 60;

      printf("%.0lfh", h);
    }

    printf("%.0lfm", m);
  }

  printf("%.0lfs", s);
}

int
main()
{
  char *filename = get_filename();
  time_t lasttime = get_previous_time(filename);

  printf("%c%d:", get_tty_type(), get_tty_number());

  if (lasttime > 0) {
    print_time_since(lasttime);
  }
  else {
    puts("new");
  }

  save_current_time(filename);
  free(filename);

  return EXIT_SUCCESS;
}
