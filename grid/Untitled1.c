#include<stdio.h>
#include <sys/time.h>
#include<time.h>
struct timeval tp_start;
main()
{
     // 	gettimeofday(&tp_start,NULL);
	    srand(tp_start.tv_sec);
        int n = rand();
        printf("%d",n);
        getch();
}
