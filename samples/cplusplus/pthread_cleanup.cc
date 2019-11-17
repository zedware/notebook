/*
 * See man page for the details.
 * See the following link, too.
 * https://computing.llnl.gov/tutorials/pthreads/man/pthread_cleanup_push.txt
 *
 * $ g++ -Wall -pthread pthread_cleanup.cc
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int condition = 1;
int count = 0;
int unlock = 1;

void
cleanup_handler(void *arg)
{
  printf("Called clean-up handler\n");
  if (unlock)
  {
    int rc = pthread_mutex_unlock(&mutex);
    printf("cleanup_handler.rc=%d\n", rc);
  }
}

void *
produce(void *arg)
{
  pthread_cleanup_push(cleanup_handler, NULL);

  while(1)
  {
    pthread_mutex_lock( &mutex );
    while( condition == 1)
      pthread_cond_wait( &cond, &mutex );
    printf("Produced %d\n", count++);
    pthread_mutex_unlock( &mutex );
  }

  pthread_cleanup_pop(0);
  return( 0 );
}

void usage(void)
{
  printf("Usage: ./a.out <unlock_or_not>\n");
  printf("       unlock_or_not is 1 or 0.\n");
  exit(1);  
}

int main(int argc, char *argv[])
{
   if (argc != 2)
     usage();

   if (strcmp(argv[1], "1") == 0)
     unlock = 1;
   else if (strcmp(argv[1], "0") == 0)
     unlock = 0;
   else
     usage();

   pthread_t thd_produce;
   pthread_create(&thd_produce, NULL, &produce, NULL );
   sleep(1);
   pthread_cancel(thd_produce);  
   pthread_join(thd_produce, NULL);
   int rc = pthread_mutex_destroy(&mutex);
   printf("rc=%d\n", rc);
   return 0;
}
