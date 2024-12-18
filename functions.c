#include "functions.h"

char *remove_newline(char *original){
    char *result = malloc(sizeof(char) * MAX_WORD_LENGHT);

    for (int i = 0; i < strlen(original); i++){
        if (original[i] == '\n'){
            result[i] = '\0';
            break;
        }
        result[i] = original[i];
    }

    return result;
}

char *get_following_word(word *head, int offset){
    for(int i = 0; i < offset; i++){
        if(!head->next){
            return "";
        }
        head = head->next;
    }

    return head->content;
}

int check_correct(char *user_input, char *word){
    if(strlen(user_input) < strlen(word)-1) return 0;

    for(int i = 0; i < strlen(user_input); i++){
        if(word[i] != user_input[i]) {
            return 0;
        };
    }
    return 1;
}

word *generate_words(char **words, int word_count){
    static int word_stack_size = 0;
    
    if (word_stack_size > MAX_WORD_STACK_SIZE) return NULL;
    
    word *new_word = malloc(sizeof(word));

    new_word->content = malloc(sizeof(char) * MAX_WORD_LENGHT);
    strcpy(new_word->content, words[rand()%word_count]);

    word_stack_size++;
    new_word->next = generate_words(words, word_count);

    return new_word;
}

int validate_arguments(int argc, char **argv){
    return argc == 3 && argv[1] && argv[2];
}

int open_words_file(char *filename, FILE **fptr, int *length){
    *fptr = fopen(filename, "r");

    if(!*fptr) return 0;

    char buffer[MAX_WORD_LENGHT];

    while (fgets(buffer, MAX_WORD_LENGHT, *fptr) != NULL){
        (*length)++;
    }

    fseek(*fptr, 0, SEEK_SET);

    return 1;
}

char **get_word_list(FILE *fptr){
    char **words = NULL;
    char buffer[MAX_WORD_LENGHT];
    int word_count = 0;

    if (fptr){
        words = malloc(sizeof(char*) * MAX_WORD_COUNT);

        while (fgets(buffer, MAX_WORD_LENGHT, fptr) != NULL){
            words[word_count] = malloc(sizeof(char) * MAX_WORD_LENGHT);
            strcpy(words[word_count], buffer);
            word_count++;
        }
    }

    fclose(fptr);

    return words;
}
