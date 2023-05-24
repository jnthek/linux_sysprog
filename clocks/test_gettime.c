#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
struct timespec mytime;
int res;
double unixtime;
res = clock_gettime(CLOCK_REALTIME, &mytime);
printf ("UNIX timestamp is %ld\n", mytime.tv_sec);
printf ("UNIX timestamp (ms) is %ld\n", mytime.tv_nsec);
unixtime = (double)mytime.tv_sec + ((double)mytime.tv_nsec/1000);
printf ("Full UNIX timestamp is %lf\n", unixtime);
return 0;
}
