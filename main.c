#include "functions.h"

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
