#include <iostream>
#include <list>
#include <functional>
#include <stdlib.h>
#include <cstring>
#include <pthread.h>
#include <chrono>
// int user_main(int argc, char **argv);

/* Demonstration on how to pass lambda as parameter.
 * "&&" means r-value reference. You may read about it online.
 */
// void demonstration(std::function<void()> && lambda) {
//   lambda();
// }

typedef struct threads
{
  int start;
  int end;
  std::function<void(int)> lambda;
}thread_args;

typedef struct threads1
{
  int low1;
  int low2;
  int high1;
  int high2;
  std::function<void(int,int)> lambda;
}thread_args1;

void* thread_function(void* ptr)
{
  thread_args* temp = (thread_args*) ptr;
  for(int i=temp->start; i<temp->end; i++)
  {
    temp->lambda(i);
  }
  return NULL;
}

void* thread_function1(void* ptr)
{
  thread_args1* temp = (thread_args1*) ptr;
  for(int i=temp->low1; i<temp->high1;i++)
  {
    for(int j=temp->low2;j<temp->high2;j++)
    {
      temp->lambda(i,j);
    }
  }
  return NULL;
}


void parallel_for(int low, int high, std::function<void(int)> lambda, int numThreads)
{
  auto start_time = std::chrono::high_resolution_clock::now();
  pthread_t tid[numThreads];
  thread_args args[numThreads];
  int chunk = high/numThreads;
  
  for(int i=0;i<numThreads;i++)
  {
    args[i].start = i*chunk;
    // printf("hi\n");
    args[i].end = (i+1)*chunk;
    args[i].lambda = lambda; 
    // pthread_create(&tid[i], NULL,thread_function,(void *)&args[i]);
    if (pthread_create(&tid[i], NULL, thread_function, (void*)&args[i]) != 0)
    {
      std::cerr << "Error creating thread " << tid[i] << std::endl;
      std::exit(EXIT_FAILURE); // Exit the program on error
    }
  }
  for(int i=0; i<numThreads;i++)
  {
    // pthread_join(tid[i],NULL);
    if (pthread_join(tid[i], NULL) != 0)
    {
      std::cerr << "Error joining thread " << tid[i] << std::endl;
      std::exit(EXIT_FAILURE); // Exit the program on error
    }
    auto end_time = std::chrono::high_resolution_clock::now(); // Record end time
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Time taken for thread "<< std::to_string(tid[i]) << " is: " << duration.count() << " microseconds" << std::endl;
  }
  auto end_time = std::chrono::high_resolution_clock::now(); // Record end time
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
  std::cout << "Time taken for parallel_for "<< " is: " << duration.count() << " microseconds" << std::endl;

}

void parallel_for(int low1, int high1,int low2, int high2,  std::function<void(int,int)> lambda, int numThreads)
{
  auto start_time = std::chrono::high_resolution_clock::now();
  pthread_t tid[numThreads][numThreads];
  thread_args1 args[numThreads][numThreads];
  int chunk = high1/numThreads;
  
  for(int i=0;i<numThreads;i++)
  {
    
    for(int j=0; j<numThreads; j++)
    {
      args[i][j].low1 = i*chunk;
      args[i][j].high1 = (i+1)*chunk;
      args[i][j].low2 = j*chunk;
      args[i][j].high2 = (j+1)*chunk; 
      args[i][j].lambda = lambda;
      // pthread_create(&tid[i][j], NULL,thread_function1,(void *)&args[i][j]);
      if (pthread_create(&tid[i][j], NULL, thread_function1, (void*)&args[i][j]) != 0)
      {
        std::cerr << "Error creating thread (" << tid[i][j]<< ")" << std::endl;
        std::exit(EXIT_FAILURE); // Exit the program on error
      }
    }
    
  }
  for(int i=0; i<numThreads;i++)
  {
    auto end_time = std::chrono::high_resolution_clock::now(); // Record end time;
    for(int j=0; j<numThreads;j++)
    {
    // pthread_join(tid[i][j],NULL);
      if (pthread_join(tid[i][j], NULL) != 0)
      {
        std::cerr << "Error joining thread (" << tid[i][j]<< ")" << std::endl;
        std::exit(EXIT_FAILURE); // Exit the program on error
      }
    end_time = std::chrono::high_resolution_clock::now(); // Record end time
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Time taken for thread "<< std::to_string(tid[i][j]) << " is: " << duration.count() << " microseconds" << std::endl;
    }
    
  }
  auto end_time = std::chrono::high_resolution_clock::now(); // Record end time
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
  std::cout << "Time taken for parallel_for "<< " is: " << duration.count() << " microseconds" << std::endl;


}


