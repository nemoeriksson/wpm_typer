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

int main(int argc, char **argv){
    int word_count = 0;
    int total_words = 0;
    int failed_words = 0;
    float total_playtime = 0;

    FILE *fptr; 
    char word_buffer[MAX_WORD_LENGHT];
    int word_goal;
    time_t word_start_time, elapsed_time;

    if (!validate_arguments(argc, argv)){
        fprintf(stderr, "Usage: ./run <filename> <word_goal>");
        return 1;
    }

    sscanf(argv[2], "%d", &word_goal);

    if (!open_words_file(argv[1], &fptr, &word_count)){
        fprintf(stderr, "ERROR: Couldn't open words file");
        return 1;
    }

    char **words = get_word_list(fptr);
    
    srand(time(NULL));

    word *head = generate_words(words, word_count);

    while(total_words < word_goal && total_words < MAX_WORD_STACK_SIZE){
        word_start_time = time(NULL);
        clear();
        printf("\e[1;2m===== %3d / %3d words | %3.1fs | %2d failed =====\n", 
            total_words, word_goal, total_playtime, failed_words);

        printf("\e[0;1m %s \e[2m%s %s\e[0m\n", 
            remove_newline(head->content), 
            word_goal-total_words > 1 ? remove_newline(get_following_word(head, 1)) : "", 
            word_goal-total_words > 2 ? remove_newline(get_following_word(head, 2)) : "");

        printf(">");
        scanf("%s", word_buffer);

        elapsed_time = time(NULL) - word_start_time;
        total_playtime += elapsed_time;
        
        if(!check_correct(word_buffer, head->content)) failed_words++;

        head = head->next;
        total_words++;
    }

    clear();

    printf("\e[1mTOTAL WORDS: \t\t%d\n",        total_words);
    printf("TOTAL TIME: \t\t%.2fs\n",           total_playtime);
    printf("AVERAGE TIME PER WORD: \t%.2fs\n",  total_playtime / total_words);
    printf("FAILED WORDS: \t\t%d\n",            failed_words);
    printf("ACCURACY: \t\t%.2f%%\n",            100 * (1 - (float)failed_words / total_words));
    printf("WORDS PER MINUTE: \t%.2f\n",        total_words/(total_playtime/60));
    printf("\e[0m\n");

    return 0;
}

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
