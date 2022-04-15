#include <stdio.h>
#include <stdbool.h>

int main(){
    char x[100];
    FILE *ptr = fopen("input.txt", "r");
    fgets(x, 100, ptr);
    printf(x);
    return 0;
}
