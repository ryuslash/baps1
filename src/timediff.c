#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static char *
get_tty_name(void)
{ /* Returns the name of the tty connected to the file descriptor for
     `stdin'. */
  return ttyname(STDIN_FILENO);
}

static char *
get_filename(void)
{ /* Generate the filename to be used for the current tty. Uses both
     the user ID of the calling user and the name of the current tty
     to make it unique. */
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
{ /* Loads the previously saved timestamp from FILENAME. This function
     assumes the file exists and is readable. */
  FILE *timefile = fopen(filename, "r");
  time_t prev;

  fscanf(timefile, "%ld", &prev);
  fclose(timefile);

  return prev;
}

static time_t
get_previous_time(char *filename)
{ /* Tries to get the previously saved timestamp from FILENAME, but if
     it can't read the file returns 0. */
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
{ /* Gets the number of the current tty. It assumes that the number is
     an integer and consists of everything after the last `/' of the
     tty's name. */
  char *tty = get_tty_name();
  char *num = strrchr(tty, '/') + 1;

  return atoi(num);
}

static char
get_tty_type(void)
{ /* Gets the type of the tty represented by the first letter of the
     type, either `t' or `p'. Assumes that the type comes immediately
     after `/dev/'. */
  char *tty = get_tty_name();

  strtok(tty, "/");             /* dev */

  return strtok(NULL, "/")[0];
}

static int
save_current_time(char *filename)
{ /* Save the timestamp for this moment to FILENAME. Returns `1' when
     succesful, `0' when the file couldn't be opened. */
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
{ /* Print the hours, minutes and seconds that have passed since
     LASTTIME to `stdout'. */
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

void
timediff(void)
{ /* This function prints a single letter represetation of the type
     and number of the current tty and the time elapsed since the last
     call to this program on this tty, or `new'. It then saves a
     timestamp to a file in /tmp/ so that it can check the elapsed
     time during its next run. */
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
}
