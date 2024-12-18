#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define clear() printf("\033[H\033[J")

#define MAX_WORD_COUNT 3000
#define MAX_WORD_LENGHT 20
#define MAX_WORD_STACK_SIZE 200

typedef struct word{
    char *content;
    struct word *next;
} word;

char *remove_newline(char *original);
char *get_following_word(word *head, int offset);
int check_correct(char *user_input, char *word);
word *generate_words(char **words, int word_count);
int validate_arguments(int argc, char **argv);
int open_words_file(char *filename, FILE **fptr, int *length);
char **get_word_list(FILE *fptr);
