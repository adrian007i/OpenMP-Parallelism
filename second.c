#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
 

int arrayss(){
    int a[4] = {1,2,3,4};
    int b[4] = {1,2,3,4};
    int c[4];
    int id;

    #pragma omp parallel num_threads (4)
    {
        
        id = omp_get_thread_num();
        c[id] = a[id] + b[id];
        printf("c[%d]=%d\n" , id, c[id]);
    }
    return 0; 
}

int for_loop (){ 

    #pragma omp parallel  num_threads(4)
    {
        int i;
        #pragma omp for
            for (i=1; i < 11 ; i++){ 
                printf("Hello %d from thread %d\n",i, omp_get_thread_num()); 

            }
    }  
    return 1;
}

int alternate_for_loop (){ 

    #pragma omp parallel  num_threads(4)
    {
        int i;
        printf("hello world\n");
        #pragma omp for
            for (i=1; i < 4 ; i++){ 
                printf("Hello %d from thread %d\n",i, omp_get_thread_num()); 

            }
    }  
    return 1;
}

int reduction_clause (){ 

    int arr[] = {1,2,3,4,5,6,7,8,9,10};
    int sum = 0;

    int i;
 
    #pragma omp parallel for num_threads(4) 
        
        for (i=0; i < 10 ; i++){ 
            sum = sum + arr[i] ; 
            printf("Running on thread %d --- looking at value %d \n", omp_get_thread_num(), arr[i]);
            // printf("Current Sum %d \n", sum);
        }

    printf("%d\n", sum); 
    return 1; 

}  

int for_loop_nowait (){ 

    #pragma omp parallel  num_threads(3)
    {
        int i;
        #pragma omp for  nowait
            for (i=1; i < 5 ; i++){ 
                printf("Hello %d from thread %d\n",i, omp_get_thread_num());  
            }

        // nowait implies we can execute this block without waiting for the for loop to finish
       printf("Hello from outside the for loop\n");       
    }  
    return 1;
}  

int barrier (){ 

    #pragma omp parallel   
    {
        printf("Step 1 \n"); 
            #pragma omp barrier
        printf("Step 2 \n"); 
             
    }  
    return 1;
}

int static_scheduling (){ 

    int arr[] = {1,2,3,4,5,6,7,8,9,10};  
    int i;
    // static assigns threads almost equally to task, static is the default
    #pragma omp parallel for num_threads(4) schedule(dynamic)
        
        for (i=0; i < 10 ; i++){  
            printf("Thread %d --- Index %d --- Value %d \n", omp_get_thread_num(), i, arr[i]);
        }
 
    return 1; 

}

int sections()
{
    int result1, result2, result3;

    #pragma omp parallel
    { 

        #pragma omp sections
        {
            #pragma omp section
                {
                    // Section 1: This code will be executed by one thread
                    printf("Section 1 is executed by Thread %d\n", omp_get_thread_num());
                    result1 = 1 + 2;
                }

            #pragma omp section
                {
                    // Section 2: This code will be executed by another thread
                    printf("Section 2 is executed by Thread %d\n", omp_get_thread_num());
                    result2 = 3 * 4;
                }

            #pragma omp section
                {
                    // Section 3: This code will be executed by yet another thread
                    printf("Section 3 is executed by Thread %d\n", omp_get_thread_num());
                    result3 = 5 - 1;
                }
        }
    }

    // Print the results computed in each section
    printf("Results: %d, %d, %d\n", result1, result2, result3);

    return 0;
}

int montecarlo_serial() {
    double start = omp_get_wtime();
    int num_points = 10000000; 
    int points_inside_circle = 0; 
    double x, y;

    // Seed the random number generator
    srand(omp_get_wtime());

    int local_points_inside_circle = 0; // Local counter for each thread
 
    for (int i = 0; i < num_points; i++) {
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;

        // Check if the point is inside the quarter circle
        if (x * x + y * y <= 1.0) {
            local_points_inside_circle++;
        }
    }
 

    // Estimate Pi using the Monte Carlo method
    double pi_estimate = 4.0 * local_points_inside_circle / num_points;

    printf("Estimated Pi: %f\n", pi_estimate); 
    printf("Time of Execution: %f\n", omp_get_wtime() - start );
    return 0;
}

int montecarlo_parallel() {
    double start = omp_get_wtime();
    int num_points = 10000000; 
    int points_inside_circle = 0; 
    double x, y;

    // Seed the random number generator
    srand(omp_get_wtime());

    int local_points_inside_circle = 0; // Local counter for each thread
    
    #pragma omp parallel  reduction(+ : local_points_inside_circle)  
    {
        for (int i = 0; i < num_points / 8; i++) {
            x = (double)rand() / RAND_MAX;
            y = (double)rand() / RAND_MAX;

            // Check if the point is inside the quarter circle
            if (x * x + y * y <= 1.0) {
                local_points_inside_circle++;
            } 
        }
    }

    // Estimate Pi using the Monte Carlo method
    double pi_estimate = 4.0 * local_points_inside_circle / num_points;

    printf("Estimated Pi: %f\n", pi_estimate);
    printf("Time of Execution: %f\n", omp_get_wtime() - start );
    return 0;
}

int montecarlo_parallel_for() {
    double start = omp_get_wtime();
    int num_points = 10000000; 
    int points_inside_circle = 0; // Counter for points inside the quarter circle
    double x, y;

    // Seed the random number generator
    srand(omp_get_wtime());

    int local_points_inside_circle = 0; // Local counter for each thread 

        // Each thread generates a portion of the random points
    #pragma omp parallel for  reduction(+: local_points_inside_circle) 
        for (int i = 0; i < num_points; i++) {
            // Generate random coordinates in the square [0, 1] x [0, 1]
            x = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
            y = ((double)rand() / RAND_MAX) * 2.0 - 1.0;

            // Check if the point is inside the quarter circle
            if (x * x + y * y <= 1) {
                local_points_inside_circle++;
            }
        }

    // Estimate Pi using the Monte Carlo method
    double pi_estimate = 4.0 * local_points_inside_circle / num_points;

    printf("Estimated Pi: %f\n", pi_estimate); 
    printf("Time of Execution: %f\n", omp_get_wtime() - start );
    return 0;
} 


 

int main(){
    montecarlo_serial();  
    montecarlo_parallel();   
    montecarlo_parallel_for();
}



 
// gcc -fopenmp first.c
// ./a.out