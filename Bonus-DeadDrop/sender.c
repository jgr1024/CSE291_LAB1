#include"util.h"
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>

// [Bonus] TODO: define your own buffer size
#define BUFF_SIZE 262144
//#define BUFF_SIZE TODO

int main(int argc, char **argv)
{
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
  *((char *)buf) = 1; // dummy write to trigger page allocation


  // [Bonus] TODO:
  // Put your covert channel setup code here

  printf("Please type a message.\n");

  bool sending = true;
  char text_buf[128];
  fgets(text_buf, sizeof(text_buf), stdin);

  uint8_t tmp = 0;

  uint8_t * buf_ptr = (uint8_t *) buf;
  uint32_t idx = string_to_int(text_buf) * 128; 
  
  //while (sending) {
    // [Bonus] TODO:
    // Put your covert channel code here
    for(int i = 0; i < 1000000000; i++){
      tmp = buf_ptr[idx];  
    }
    //break;
  //}
  
  //printf("Sender finished.\n");
  return 0;
}
