#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "aboot-tiny.h"

/*---------------------------------------------------------------------------*/
static FILE *firmware_file;
static void *firmware_data;
static size_t firmware_size;
static const char *device_name;
static int baud_rate = 115200;
static bool reboot_after_completed;
static bool from_memory;
static pthread_mutex_t aboot_mutex;
static pthread_cond_t aboot_cond;
static volatile bool aboot_stopped;
static int rc = 0;
/*---------------------------------------------------------------------------*/
static void
aboot_tiny_callback(const aboot_tiny_message_t *msg, void *ctx)
{
  (void)ctx;

  switch(msg->event) {
  case ABOOT_TINY_EVENT_INIT:
    if(msg->u.message) {
      printf("%s\n", msg->u.message);
    }
    if(!msg->error && !aboot_tiny_start(device_name, baud_rate)) {
      break;
    }
    pthread_mutex_lock(&aboot_mutex);
    aboot_stopped = true;
    pthread_cond_signal(&aboot_cond);
    pthread_mutex_unlock(&aboot_mutex);
    break;

  case ABOOT_TINY_EVENT_START:
    if(msg->u.message) {
      printf("%s\n", msg->u.message);
    }
    if(!msg->error) {
      if(from_memory) {
        if(!aboot_tiny_download_data(firmware_data, firmware_size, reboot_after_completed)) {
          break;
        }
      } else {
        if(!aboot_tiny_download_file(firmware_file, firmware_size, reboot_after_completed)) {
          break;
        }
      }
    }
    aboot_tiny_stop();
    break;

  case ABOOT_TINY_EVENT_DOWNLOAD:
    if(msg->u.message) {
      printf("%s\n", msg->u.message);
    }
    if(!msg->error) {
      break;
    }
    aboot_tiny_stop();
    break;

  case ABOOT_TINY_EVENT_STOP:
    if(msg->u.message) {
      printf("%s\n", msg->u.message);
    }
    pthread_mutex_lock(&aboot_mutex);
    aboot_stopped = true;
    pthread_cond_signal(&aboot_cond);
    pthread_mutex_unlock(&aboot_mutex);
    break;

  case ABOOT_TINY_EVENT_EXIT:
  case ABOOT_TINY_EVENT_LOG:
    if(msg->u.message) {
      printf("%s", msg->u.message);
    }
    break;

  case ABOOT_TINY_EVENT_PROGRESS:
    printf("PROGRESS: %d\n", msg->u.progress);
    break;

  case ABOOT_TINY_EVENT_STATUS:
    printf("STATUS: %s\n", msg->u.status);
    if(msg->error || !strcmp(msg->u.status, ABOOT_TINY_STATUS_FAILED)) {
      rc = -1;
      aboot_tiny_stop();
    } else if(!strcmp(msg->u.status, ABOOT_TINY_STATUS_SUCCEEDED)) {
      rc = 0;
      aboot_tiny_stop();
    }
    break;

  default:
    printf("Unknown event: %d\n", msg->event);
    break;
  }
}
/*---------------------------------------------------------------------------*/
int
main(int argc, char *argv[])
{
  off_t base;
  size_t size;
  int opt;

  base = 0;
  size = 0;

  while((opt = getopt(argc, argv, "gmb:s:d:r:")) != -1) {
    switch(opt) {
    case 'b':
      base = strtoull(optarg, NULL, 0);
      break;

    case 's':
      size = strtoull(optarg, NULL, 0);
      break;

    case 'r':
      baud_rate = strtol(optarg, NULL, 0);
      break;

    case 'd':
      device_name = optarg;
      break;

    case 'g':
      reboot_after_completed = true;
      break;

    case 'm':
      from_memory = true;
      break;

    default:            /* '?' */
      fprintf(stderr, "Usage: %s [-b base] [-s size] [-r baud] [-g] [-m] [-d device] filename\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  if(optind >= argc) {
    fprintf(stderr, "Usage: %s [-b base] [-s size] [-r baud] [-g] [-m] [-d device] filename\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  const char *filename = argv[optind];
  struct stat st;

  if(stat(filename, &st) == -1) {
    perror("stat");
    exit(EXIT_FAILURE);
  }

  if(size == 0) {
    size = st.st_size;
  } else {
    assert(base + size <= st.st_size);
  }

  firmware_file = fopen(filename, "rb");
  if(!firmware_file) {
    perror("fopen");
  }
  fseek(firmware_file, base, SEEK_SET);
  firmware_size = size;
  if(from_memory) {
    firmware_data = malloc(size);
    if(!firmware_data) {
      fprintf(stderr, "Out of memory\n");
      return -1;
    }
    if(fread(firmware_data, 1, size, firmware_file) != size) {
      fprintf(stderr, "Can not read enough data from file\n");
      return -1;
    }
  }

  /* Initialize mutex and condition variable objects */
  pthread_mutex_init(&aboot_mutex, NULL);
  pthread_cond_init(&aboot_cond, NULL);

  /*
   * Lock mutex and wait for signal.  Note that the pthread_cond_wait
   * routine will automatically and atomically unlock mutex while it waits.
   * Also, note that if aboot_stopped is true before this routine is run by
   * the waiting thread, the loop will be skipped to prevent pthread_cond_wait
   * from never returning.
   */
  pthread_mutex_lock(&aboot_mutex);
  aboot_stopped = false;
  aboot_tiny_register_cb(aboot_tiny_callback, NULL);
  if(aboot_tiny_init()) {
    rc = EXIT_FAILURE;
    goto cleanup;
  }
  while(!aboot_stopped) {
    pthread_cond_wait(&aboot_cond, &aboot_mutex);
  }

cleanup:
  fclose(firmware_file);
  if(firmware_data) {
    free(firmware_data);
  }
  pthread_mutex_unlock(&aboot_mutex);
  pthread_mutex_destroy(&aboot_mutex);
  pthread_cond_destroy(&aboot_cond);
  if(aboot_tiny_exit()) {
    rc = EXIT_FAILURE;
  }

  return rc;
}
/*---------------------------------------------------------------------------*/
