#include <stdio.h>
#include <stdlib.h>
 
int main(void) {

  printf("%d\n", 4 + rand()%(5 - 4 + 1));
  
  return 0;
}