#include <stdio.h>
#include "sum.h"
#include "multiply.h"

int main() {
   int s = sum(1, 2);
   int mul = multiply(2, 3);

   printf("Sum is %i", s);
   printf("Multiply is is %i", mul);
}
