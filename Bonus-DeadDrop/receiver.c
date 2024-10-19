#include"util.h"
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>


#define BUFF_SIZE 262144


int main(int argc, char **argv)
{
  // [Bonus] TODO: Put your covert channel setup code here

  // Allocate a buffer using huge page
  // See the handout for details about hugepage management
  void *buf= mmap(NULL, BUFF_SIZE, PROT_READ | PROT_WRITE, MAP_POPULATE |
      MAP_ANONYMOUS | MAP_PRIVATE | MAP_HUGETLB, -1, 0);

  if (buf == (void*) - 1) {
    perror("mmap() error\n");
    exit(EXIT_FAILURE);
  }
  // The first access to a page triggers overhead associated with
  // page allocation, TLB insertion, etc.
  // Thus, we use a dummy write here to trigger page allocation
  // so later access will not suffer from such overhead.
  *((char *)buf) = 1; // 


  printf("Please press enter.\n");

  char text_buf[2];
  fgets(text_buf, sizeof(text_buf), stdin);

  printf("Receiver now listening.\n");

  bool listening = true;

  uint8_t tmp = 0;
  uint8_t * buf_ptr = (uint8_t *) buf;
  uint64_t misses = 0;

  for(int i = 0; i < BUFF_SIZE; i+=8){
    (void)measure_one_block_access_time((uint64_t) &buf_ptr[i]);
  }

  #define WINDOW 256
  uint32_t misses_arr[WINDOW];

  while (listening) {

    // [Bonus] TODO: Put your covert channel code here
    for(int j = 0; j < WINDOW; j++){
      for(int i = 0; i < BUFF_SIZE; i+=1){
        uint32_t access_time =  measure_one_block_access_time((uint64_t) &buf_ptr[i]);
        if(access_time > 31 && access_time < 100){
          misses++;
        }
      }
      misses_arr[j] = misses;
      misses = 0;
    }
    break;
  }
  
  uint64_t sum = 0;
  printf("Receiver finished.\n");
  for(int i = 0; i < WINDOW; i++){
    sum += misses_arr[i];
  }
  uint64_t avg = sum / WINDOW;

  uint8_t received = 0;

  if(avg >= 100000){
    received = 1;
  }
  printf("%d \n", received);

  return 0;
}
