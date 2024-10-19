#include"util.h"
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>


#define BUFF_SIZE 262144

static inline void lfence(){
  asm volatile("lfence");
}


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

  uint32_t received = 0;
  uint32_t access_times[256];

  for(int i = 0; i < 256; i++){
    access_times[i] = 0;
  }

  uint32_t ctr = 0;
  
  // FIll up cache
  for(int j = 0; j < 50000; j++){
    for(int i = 0; i < 32768; i+=8){
      tmp = buf_ptr[i]; 
    }
  }

  //while(listening){
  for(int j = 0; j < 100000; j++){
    for(int i = 0; i < 32768; i+=128){
        uint32_t access = measure_one_block_access_time((uint64_t) &buf_ptr[i]);
        if(access >= 30 && access < 100){
          access_times[ctr]++;
        }
        ctr++;
    }
    ctr = 0;
  }
  
  printf("Receiver finished.\n");
  
  int32_t max = access_times[0];
  int32_t max_idx = 0;
 for(int i = 0; i < 256; i++){
     if(access_times[i] > max){
      max = access_times[i];
      max_idx = i;
     }
  } 
 printf("\n"); 
 
 printf("%d\n", max_idx);
  return 0;
}
