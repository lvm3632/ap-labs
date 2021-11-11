#define _XOPEN_SOURCE 500 // Refered to: X/Open 5, incorporating POSIX 1995
#include "logger.h"
#include <errno.h>
#include <ftw.h>
#include <limits.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <unistd.h>

int inotifyFd;
char *path;
#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

char *findstr(char *string, char *patrn, int *begin, int *end, int sub,
              int topOrEnd) {
  int i, w = 0, len;
  char *word = NULL;
  regex_t rgT;
  regmatch_t match;
  regcomp(&rgT, patrn, REG_EXTENDED);
  if ((regexec(&rgT, string, 1, &match, 0)) == 0) {
    // Si hay que restar caracteres
    if (sub != 0) {
      // Resta al top
      if (topOrEnd == 1) {
        *begin = (int)match.rm_so + sub;
        *end = (int)match.rm_eo;
      } else {
        *begin = (int)match.rm_so;
        *end = (int)match.rm_eo + sub;
      }
    } else {
      // Si es 0, no se resta nada.
      *begin = (int)match.rm_so;
      *end = (int)match.rm_eo;
    }
    len = *end - *begin;
    word = malloc(len + 1);

    for (i = *begin; i < *end; i++) {
      word[w] = string[i];
      w++;
    }
    word[w] = 0;
  } else {
    // printf("no hizo match");
    return NULL;
  }
  regfree(&rgT);
  return word;
}

char *arr_lista[999999];
int size = 0;

static int display_info(const char *fpath, const struct stat *sb, int tflag,
                        struct FTW *ftwbuf) {
  if (tflag == FTW_D) {
    inotify_add_watch(inotifyFd, fpath,
                      IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO);
    // arr_lista[size++] = strdup(fpath);
  } else {
    inotify_add_watch(inotifyFd, fpath,
                      IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO);
    // printf("%s\n", fpath);
    arr_lista[size++] = strdup(fpath);
  }

  return 0;
}

void tree() {
  if (nftw(path, display_info, 20, 0) == -1) {
    perror("nftw");
    exit(EXIT_FAILURE);
  }
}
int cookie = 0;
char name[499];
static void printInotifyEvent(struct inotify_event *evt) {
  // File
  if (!(evt->mask & IN_ISDIR)) {
    if ((evt->mask & IN_MOVED_FROM)) {
      cookie = evt->cookie;
      strcpy(name, evt->name);
      // printf("Rename: %s", evt->name);
      tree();
    }

    if ((evt->mask & IN_MOVED_TO)) {
      if (cookie == evt->cookie && (strcmp(name, evt->name) != 0)) {
        cookie = 0;
        printf("- [File Rename] -");
        if (evt->len > 0) {
          printf("%s -> %s\n", name, evt->name);
        }
      }
      // tree();
    }
  } else {
    if ((evt->mask & IN_MOVED_FROM)) {
      cookie = evt->cookie;
      strcpy(name, evt->name);
      // printf("Rename: %s", evt->name);
      tree();
    }

    if ((evt->mask & IN_MOVED_TO)) {
      if (cookie == evt->cookie && (strcmp(name, evt->name) != 0)) {
        cookie = 0;
        printf("- [Directory - Rename] -");
        if (evt->len > 0) {
          printf("%s -> %s\n", name, evt->name);
        }
      }
      // tree();
    }
  }

  if ((evt->mask & IN_CREATE) && !(evt->mask & IN_ISDIR)) {
    tree();
    char patron[500];
    strcpy(patron, "[\\/](.*?)[\\/](.*?)[\\/]");
    strcat(patron, evt->name);
    char *busqueda;
    int isSub = 0;
    int a, b;
    for (int i = size - 1; i >= 0 && arr_lista[i] != NULL; i--) {
      busqueda = findstr(arr_lista[i], patron, &a, &b, 0, 0);
      if (busqueda != NULL) {
        printf("- [File - Create] - %s\n", busqueda);
        isSub = 1;
        break;
      }
    }
    if (isSub != 1) {
      printf("- [File - Create] - %s\n", evt->name);
      isSub = 0;
    }
    size = 0;
  }
  if ((evt->mask & IN_DELETE) && !(evt->mask & IN_ISDIR)) {
    printf("- [File - Removal] - %s\n", evt->name);
  }
  if ((evt->mask & IN_CREATE) && (evt->mask & IN_ISDIR)) {
    printf("- [Directory - Create] - %s\n", evt->name);
    tree();
  }
  if ((evt->mask & IN_DELETE) && (evt->mask & IN_ISDIR)) {
    printf("- [Directory - Removal] - %s\n", evt->name);
    tree();
  }
}

int main(int argc, char *argv[]) {
  // Place your magic here
  if (argc < 2) {
    printf("Please provide a folder directory. \n");
    printf("Example: ./monitor /home/gitpod\n");

    return -1;
  }

  if (argc < 2 || strcmp(argv[1], "--help") == 0) {
    printf("%s pathname...\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char buf[BUF_LEN] __attribute__((aligned(8)));
  ssize_t numRead;
  char *p;
  struct inotify_event *event;

  inotifyFd = inotify_init();

  if (inotifyFd == -1) {
    printf("errExit: inotify_init");
    exit(EXIT_FAILURE);
  }

  printf("Starting File/Directory Monitor on %s\n", argv[1]);
  printf("-----------------------------------------------------\n");

  path = argv[1];
  if (argc >= 2) {
    path = argv[1];
  } else {
    path = ".";
  }
  if (nftw(path, display_info, 20, 0) == -1) {
    perror("nftw");
    exit(EXIT_FAILURE);
  }

  for (;;) {
    numRead = read(inotifyFd, buf, BUF_LEN);
    if (numRead == 0) {
      printf("read() from inotify fd returned 0!");
      exit(EXIT_FAILURE);
    }
    if (numRead == -1) {
      printf("errExit: read");
      exit(EXIT_FAILURE);
    }
    for (p = buf; p < buf + numRead;) {
      event = (struct inotify_event *)p;
      printInotifyEvent(event);
      p += sizeof(struct inotify_event) + event->len;
    }
  }
  exit(EXIT_SUCCESS);
}