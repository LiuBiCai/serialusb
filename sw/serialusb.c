/*
 Copyright (c) 2015 Mathieu Laurendeau <mat.lau@laposte.net>
 License: GPLv3
 */

#include <proxy.h>
#include <signal.h>
#include <stddef.h>

static void terminate(int sig) {
  proxy_stop();
}

int main(int argc, char * argv[]) {

  (void) signal(SIGINT, terminate);
  (void) signal(SIGTERM, terminate);

  char * port = NULL;

  if (argc > 1) {
    port = argv[1];
  }

  int ret = proxy_init(port);
  if (port != NULL && ret == 0) {

    proxy_start();
  }

  proxy_clean();

  return ret;
}
