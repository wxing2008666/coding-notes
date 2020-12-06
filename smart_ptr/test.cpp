#include "memplumber.h"
#include <stdio.h>

class MyClass {
    int x;
};

int main(int argc, const char* argv[]) {

    // start collecting mem allocations info
    MemPlumber::start();

    // init 2 objects
    int* num = new int(100);
    MyClass* myClass = new MyClass();

    // init 1 array of 10 objects
    MyClass* myClassArr = new MyClass[10];

    // run memory leak test in verbose mode
    size_t memLeakCount;
    uint64_t memLeakSize;
    MemPlumber::memLeakCheck(memLeakCount, memLeakSize, true);

    // print memory leak results
    printf("Number of leaked objects: %d\nTotal amount of memory leaked: %d[bytes]\n", (int)memLeakCount, (int)memLeakSize);

    return 0;
}