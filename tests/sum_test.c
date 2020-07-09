#include "../include/tap.h"
#include "../src/sum.h"

int main () {
    plan(2);
    int a = 1;
    int b = 2;

    ok(sum(a, b) == 3, "1 + 2 = 3");
    ok(1 < 2, "1 is < than 2");
    done_testing();
}