#include <kernel/task.h>
#include <fs/file.h>

void append_byte(struct File *file, char c);
char read_byte(struct File *file);

int _start(){
    struct Message *message;
    int command = 0;
    int j = 0;
    while (1) {
        asm (
            "mov $2, %%edi\n"
            "mov $2, %%eax\n"
            "int $128\n"
            "mov %%eax, %0\n"
            :"=r"(message) : :"eax");
        if (message->content == 3) {
            *(char *)(0xB8060) = 'R';
            command = 1;
        }
        else if (message->content == 4) {
            *(char*)(0xB8062) = 'W';
            command = 2;
        } else {
            switch (command) {
                case 1:
                    if (!message) break;
                    *(char *)(0xB8050) = j + 0x30;
                    int c = read_byte(message->content);
                    asm (
                    "mov %0, %%esi\n"
                    "mov $2, %%edi\n"
                    "mov $1, %%eax\n"
                    "int $128\n"
                    : :"r"(c) :);
                    command = 0;
                    break;
                case 2:
                    if(message)
                        *(char *)(0xB8066) = 0x30 + message->sender_id;
                    break;
                default:
                    command = 0;
                    break;
            }
        }
        j++;
    }
}

void append_byte(struct File *file, char c){
    file->buffer[file->index] = c;
    file->index++;
}

char read_byte(struct File *file){
    if(file->buffer[file->index]){
        return (file->buffer[file->index++]);
    }
    return 0;
}
