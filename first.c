#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int hello_world (){
    #pragma omp parallel  num_threads(4)
        printf("hello world\n");

    return 1;
}

int addition (){
    int x=0;

    #pragma omp parallel  num_threads(10)
    {
        x = x + 1;
        printf("%d\n", x);
    } 
    
    return 1;
}
 

int sync_directive (){
    int x=0;

    #pragma omp parallel  num_threads(10)
    {
        #pragma omp critical 
        {
            x = x + 1;
            printf("%d\n", x);
        }
        
    } 
    
    return 1;
}

int thread_id (){ 

    #pragma omp parallel  num_threads(10)
    {
        printf("Hello from thread %d\n", omp_get_thread_num()); 
    }  
    return 1;
}

int for_loop (){ 

    #pragma omp parallel  num_threads(5)
    {
        int i;
        #pragma omp for
            for (i=1; i < 11 ; i++){
                printf("Eandom Multiplication %d\n", rand() / 4); 
                printf("Hello from thread %d\n", omp_get_thread_num()); 

            }
    }  
    return 1;
}



int main(){ 
    // hello_world ();
    for_loop();
}



// make first.py 
// ./first


// gcc -fopenmp first.c
// ./a.out