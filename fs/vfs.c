#include <kernel/task.h>
#include <fs/file.h>

extern void * sys_call(int number, int arg0, int arg1);

struct FileNode {
    struct FileNode *parent_node;
    char name[64];
    void *buffer;
    struct FileNode *children_nodes[16];
    int open_files;
};

void append_byte(struct File *file, char c);
char read_byte(struct File *file);
void add_child(struct FileNode *main_node, char *name, void *buffer);
struct FileNode *find_node(struct FileNode *main_node, char *name);
void rewind_file(struct File *file);
int strcmp(char *src, char *dst);
void strcpy(char *src, char *dst);

struct File *open_file(struct FileNode *node);

struct FileNode rootnode = {0, "/", 0, {0}, 0};

int _start(){

    int j = 0;
    int command = 0;
    struct Message *message = 0;

    while (1) {
        message = sys_call(3, 0, 0);
        *(char *)(0xB8C00) = ' ';
        if(!message) {continue;}
        if(message->content < 10){
            command = (int) message->content;
        } else {
            switch (command){
                case 1:
                    char c = read_byte(message->content);
                    sys_call(1, 1, c);
                    break;
                case 2:
                    struct Message *written_byte = 0;
                    while(!written_byte){
                        written_byte = sys_call(3, 0, 0);
                    }
                    append_byte(message->content, (char) written_byte->content);
                    *(char *)(0xB8060) = (int) written_byte->content;
                    written_byte = 0;
                    break;
                case 3:
                    struct FileNode *found_node = find_node(&rootnode, message->content);
                    if(!found_node) break;
                    struct File *new_file = open_file(found_node);
                    sys_call(1, 1, (int) new_file);
                    break;
                case 4:
                    char name[64] = {0};
                    struct Message *buffer = 0;
                    while(!buffer){
                        buffer = sys_call(3, 0, 0);
                    }
                    strcpy(message->content, name);
                    add_child(&rootnode, name, buffer->content);
                    break;
                case 5:
                    rewind_file(message->content);
                    break;
            }   command = 0;
        }
    }
}

int strcmp(char *src, char *dst){
    for(int i = 0; src[i]; ++i){
        if(src[i] != dst[i]) return 0;
    }
    return 1;
}

void strcpy(char *src, char *dst){
    while(*dst++ = *src++);
}
void add_child(struct FileNode *main_node, char *name, void *buffer){

    int i = 0;
    for(; i < 16; ++i){
        if(main_node->children_nodes[i] == 0) break;
    }
    if (i == 16) return ;
    main_node->children_nodes[i] = sys_call(4, sizeof(struct FileNode), 0);
    main_node->children_nodes[i]->parent_node = main_node;
    main_node->children_nodes[i]->buffer = buffer;
    strcpy(name, main_node->children_nodes[i]->name);
    main_node->children_nodes[i]->open_files = 0;
}

struct FileNode *find_node(struct FileNode *main_node, char *name){
    for(int i = 0; i < 16; ++i){
        if(!main_node->children_nodes[i]) continue;
        if(strcmp(main_node->children_nodes[i]->name, name)) {return main_node->children_nodes[i];}
    }
}

struct File *open_file(struct FileNode *node){
    struct File *new_file = sys_call(4, sizeof(struct File), 0);
    new_file->buffer = node->buffer;
    new_file->index = 0;
    node->open_files++;
    return new_file;
}


void *close_file(struct FileNode *node, struct File *file){
    node->open_files--;
}

void append_byte(struct File *file, char c){
    file->buffer[file->index] = c;
    file->index++;
}

char read_byte(struct File *file){
    if(!file) return 0;
    if(file->buffer[file->index]){
        return (file->buffer[file->index++]);
    }
    return 0;
}

void rewind_file(struct File *file){
    file->index = 0;
}
