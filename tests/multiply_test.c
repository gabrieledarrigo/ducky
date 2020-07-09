#include "../include/tap.h"
#include "../src/multiply.h"

int main() {
    plan(1);
    int a = 2;
    int b = 2;

    ok(multiply(a, b) == 4, "2 * 2 = 4");
    done_testing();
}
