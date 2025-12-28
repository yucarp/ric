#pragma once
#include <drivers/terminal.h>

int x = 0;
void kprintf(char * str){
    char *n = str;
    do{
        *((char *)(0xB8000) + x +((int)n - (int) str) * 2) = str[n - str];
    }while (*(n++) != 0);
    x += (int) n - (int) str;
}
