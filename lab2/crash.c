#include <stdio.h>

int main()
{
    // declaration lines
    char name[] = "Jhon";
    int age = 10;
    int height = 190;
    int* p = &height;

    // crash lines (my examples)
    //name[strlen(name) + 1] = ' ';
    p += 0xEFF;
    //name = (char*)1;

    // printf lines
    printf("Hello, my name is %s.\n", name);
    printf("My age is %d.\n", age);
    printf("I am %d cm tall.\n", *p);

    return 0;
}
