/**
 * Get time difference in microseconds
 * */
  
#include <stdio.h>
#include <sys/time.h>

#define MAX_DT 100 // 100 == (1/10) * 1000
#define MIN_DT 33 // 33 == (1/30) * 1000
  
int time_diff(struct timeval x , struct timeval y);
  
int main()
{ 
    int i;
    int max_dt;
    int samples[128], i_samples = 0;
    int diff = 0, total = 0, media;
    int count = 0, tempo = 0;
    for (i=0; i<128; i++)
        samples[i] = 30;

    struct timeval before , after;
    for(;;){
        gettimeofday(&before , NULL);
        count++;
        //Time taking task
        for (i=total=0 ; i <= 128; i++)
        {
            total += samples[i];
            //printf("%d %d %d n",i, i*i, i*i*i);
        }
        
        gettimeofday(&after , NULL);
        diff = time_diff(before , after);
        samples[i_samples] = diff;
        i_samples++;
        if(i_samples == 128){
            i_samples = 0;
        }
        media = total/128;
        if(media < MIN_DT){
            media = MIN_DT;
        }

        if(diff < media){
            usleep((media - diff)*1000);
        }
        tempo += media - diff;

        if(tempo>1000){
            printf("count: %d\n", count);
            break;
        }

        printf("\rmedia: %d\n", media);

    }


 
    printf("Total time elapsed : %d ms" ,  diff);
   
    return 0;
}
 
int time_diff(struct timeval x , struct timeval y)
{
    int x_ms , y_ms , diff;
     
    x_ms = x.tv_sec*1000 + x.tv_usec/1000;
    y_ms = y.tv_sec*1000 + y.tv_usec/1000;
     
    diff = y_ms - x_ms;
     
    return diff;
}