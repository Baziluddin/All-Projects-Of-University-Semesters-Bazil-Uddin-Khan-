#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
    #include <conio.h>
    #include<windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
    char getch() {
        struct termios oldt, newt;
        char ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
#endif
#define Max_word_length 50
#define Max_category_words 100000
#define MAX_GUESSES 26  
char guessed_letters[MAX_GUESSES];  
int guess_count = 0;  


#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define YELLOW "\033[33m"

typedef struct {
	int*body ;
}man;

int total_h ;
man hangman ;
void intitialize(){
	
	hangman.body = (int*)calloc(5,sizeof(int));
	for (int i = 0 ; i < 5 ; i++){
		hangman.body[i] =1;
	}
}

void totalx(){
	for (int i = 0 ; i < 5 ; i++){
		total_h +=hangman.body[i] ;
	}
}
void choice ();

void cls () {
    #ifdef _WIN32
        system("cls");
    #else 
        system("clear");
    #endif 
}

void saveGameRecords(const char *word, const char *result, int Score)
{
    FILE *file = fopen("game_records.txt","a");
    if(file == NULL)
    {
        printf("Error in opening the file. \n");
        return;
    }
    fprintf(file, "Word : %s , Result : %s, Score : %d \n",word,result,Score);
    fclose(file);
}

void showGameRecord()
{
    cls(); 
    FILE *file = fopen("game_records.txt","r");
    if(file == NULL)
    {
        printf("Game records file not found !! \n");
        return;
    }

    printf("--Game Records-- \n");
    char line[256];
    while(fgets(line,sizeof(line),file))
    {
        printf("%s",line);
    }
    fclose(file);
    printf("\n");
}

int read_word_file(const char *filename, char *word) {
    FILE *dictionary = fopen(filename, "r");
    if (dictionary == NULL) {
        printf("Error: Could not open file.\n");
        return -1; // Return error code for failure
    }

    srand((unsigned int)time(0));
    int random_index = rand() % Max_category_words;
    for (int i = 0; i <= random_index; i++) {
        if (fscanf(dictionary, " %49s", word) != 1) {
            fclose(dictionary);
            printf("Error: Unable to read word from file.\n");
            return -1; 
        }
    }

    fclose(dictionary);
    return 0; 
}

const char *programming_words[] = {
    "variable", "function", "compiler", "debugger", "algorithm", 
    "syntax", "library", "recursion", "polymorphism", "array", 
    "pointer", "inheritance", "encapsulation", "framework", "module", 
    "constructor", "destructor", "repository", "branch", "commit",
    "merge", "dependency", "optimization", "object", "interface",
    "exception", "loop", "condition", "execution", "parameter",
    "binary", "floating", "virtual", "network", "protocol",
    "database", "stack", "queue", "hashmap", "encryption"
};
const int programming_count = sizeof(programming_words) / sizeof(programming_words[0]);

const char *sports_words[] = {
    "soccer", "cricket", "baseball", "tennis", "badminton", 
    "hockey", "basketball", "rugby", "marathon", "cycling",
    "swimming", "gymnastics", "archery", "boxing", "karate",
    "wrestling", "golf", "volleyball", "surfing", "snowboarding",
    "skiing", "sprinting", "skating", "rowing", "kayaking",
    "triathlon", "fencing", "bowling", "snooker", "darts",
    "sailing", "shooting", "equestrian", "handball", "softball",
    "polo", "motorsport", "weightlifting", "paragliding"
};
const int sports_count = sizeof(sports_words) / sizeof(sports_words[0]);

const char *animal_words[] = {
    "elephant", "tiger", "kangaroo", "giraffe", "panda", 
    "dolphin", "parrot", "lion", "rhinoceros", "penguin", 
    "zebra", "cheetah", "hippopotamus", "crocodile", "chameleon",
    "ostrich", "peacock", "koala", "platypus", "armadillo", 
    "jaguar", "gorilla", "flamingo", "salamander", "hedgehog",
    "otter", "octopus", "shark", "whale", "sparrow", 
    "eagle", "owl", "bat", "fox", "wolf",
    "bear", "moose", "buffalo", "antelope", "leopard"
};
const int animal_count = sizeof(animal_words) / sizeof(animal_words[0]);

const char *physics_words[] = {
    "gravity", "force", "momentum", "energy", "work",
    "power", "velocity", "acceleration", "mass", "density",
    "friction", "wave", "frequency", "amplitude", "thermodynamics",
    "quantum", "relativity", "electricity", "magnetism", "circuit",
    "voltage", "current", "resistance", "particle", "atom",
    "nucleus", "photon", "entropy", "field", "motion",
    "optics", "lens", "refraction", "diffraction", "vacuum"
};
const int physics_count = sizeof(physics_words) / sizeof(physics_words[0]);

const char *maths_words[] = {
    "algebra", "geometry", "calculus", "trigonometry", "equation",
    "matrix", "vector", "integral", "derivative", "probability",
    "statistics", "angle", "theorem", "coordinate", "dimension",
    "function", "sequence", "series", "variable", "ratio",
    "logarithm", "exponent", "prime", "factor", "graph",
    "area", "volume", "perimeter", "radius", "diameter"
};
const int maths_count = sizeof(maths_words) / sizeof(maths_words[0]);

void pick_random_word(const char *category_words[], int count, char *word) {
    srand((unsigned int)time(NULL));
    int random_index = rand() % count;
    strcpy(word, category_words[random_index]);
}

void initialize_display(const char *word, char *display) {
    int length = strlen(word);
    for (int i = 0; i < length; i++) {
        display[i] = '_';
    }
    display[length] = '\0';
}

void show_hangman(int attempt) {
    const char *hangman[6] = {
        "  O\n /|\\\n / \\\n",       // 0 attempts left
        "  O\n /|\\\n / \n",        // 1 attempt left
        "  O\n /|\\\n\n",           // 2 attempts left
        "  O\n /|\n\n",             // 3 attempts left
        "  O\n  |\n\n",             // 4 attempts left
        "  O\n\n\n"                 // 5 attempts left
    };
  printf(YELLOW "Hangman:\n%s" RESET, hangman[5 - attempt]); 
  
  
}

int is_repeated(char guess) {
    for (int i = 0; i < guess_count; i++) {
        if (guessed_letters[i] == guess) {
            return 1;  
        }
    }
    return 0;  
}


int letter(const char* word) {
    int letter[127] = {0};
    for (int i = 0; i < strlen(word); i++) {
    	if ((int)word != 137){
        letter[(int)word[i]]++;
		}
    }
    int letter_count = 0;
    for (int i = 0; i < 127; i++) {
        if (letter[i] != 0) {
            letter_count++;
        }
    }
    return letter_count;
}

void hint(const char* word, char* display, int* correct) {
    int length = strlen(word);
    int random;

  
    do {
        random = rand() % length;
    } while (display[random] != '_');

   
    display[random] = word[random];
    (*correct)++;
}

int hint_a = 1;
int score = 0;
void hangman_game(const char *category_words[], int word_count, int n, int choice) {
    char word[Max_word_length];
    char display[Max_word_length];
    char guess;
    int attempts = 5;
    int correct = 0;
    int repeat = 3;
    int save_repeat_count = 1;
    guess_count = 0;  
    memset(guessed_letters, 0, sizeof(guessed_letters));  //it is used to clear the arr 


    if (n == 1) {
        pick_random_word(category_words, word_count, word);
    } else if (n == 0) {
        if (read_word_file("dictionary.txt", word) != 0) {
            printf("Error fetching word from file.\n");
            return;
        }
    }
    
    initialize_display(word, display);
    int length = strlen(word);
    int word_letter_num = letter(word);
    int save_letter_length = letter(display);
    printf("Let's play Hangman!\n");

    while (attempts > 0 && correct < length) {
        cls();
        show_hangman(attempts);
        
if (attempts == 1 && hint_a == 1 && 
    word_letter_num > 2 && (word_letter_num - letter(display)) > 1) {

    char hint_c;
do {
    printf("Do you want a hint? (Y: yes, N: no): ");
    scanf(" %c", &hint_c);

   
    if (hint_c == 'Y' || hint_c == 'y') {
        hint_a = 0 ;
        break; 
    } else if (hint_c == 'N' || hint_c == 'n') {
    	hint_a = 0 ;
    	printf("\nNo hint used. Good luck!\n");
    	printf("\nProcessing.........\n");
		#ifdef _WIN32
    Sleep(3000);  
	#else
    sleep(3);     
	#endif
	cls();
	show_hangman(attempts);
        break; 
    } else {
        printf("\nInvalid input. Please enter Y (yes) or N (no).\n");
        		#ifdef _WIN32
    Sleep(3000);  
	#else
    sleep(3);     
	#endif
	cls();
        
    }
} while (1); 

    if (hint_c == 'Y' || hint_c == 'y') {
        hint(word, display, &correct); 
        show_hangman(attempts);
        printf("Hint used! Revealing a letter...\n");
        printf("Word after hint: %s\n", display); 
        printf("\nProcessing.........\n");
    fflush(stdout);
#ifdef _WIN32
    Sleep(3000);  
#else
    sleep(3);     
#endif
cls();
show_hangman(attempts);
    } else {
        
    }
}


        printf("Word: %s\n", display);
        switch (choice)
        {
        case 1: printf("Theme : Programming \n"); break;
        case 2: printf("Theme : Sports \n"); break;
        case 3: printf("Theme : Animals \n"); break;
        case 4: printf("Theme : Physics \n"); break;
        case 5: printf("Theme : Maths \n"); break;
        case 6: printf("Theme : Entire Dictionary \n"); break;

        
        default:
            break;
        }
        printf("Enter your guess: ");
        scanf(" %c", &guess);

if (is_repeated(guess)) {
    repeat--;
    printf("\nRepeated letters are not allowed! You have %d repeat chances left.\n", repeat);
    if (repeat == 0) {
        break;  
    }
} else {
    guessed_letters[guess_count++] = guess; 

    if (strchr(word, guess) == NULL) {  
        
        attempts--;
        printf("\nIncorrect! Attempts left: %d\n", attempts);
    } else {  
        
        for (int i = 0; i < length; i++) {
            if (word[i] == guess && display[i] == '_') {
                display[i] = guess;  
                correct++; 
              printf("\nCorrect Guess attempts avalible are : %d\n", attempts);   
            }
        }
    }
}


        if (correct == length){
            score++;
            printf("\nCorrect!\n");
        }

        int r = 1;
        while (r ==1 ){
        	printf("\npress Enter to proceed:\n ");
        	char proceed;
        	proceed = getch();
        	if (proceed == 13){
        		r = 0;
			}else {
				printf("\nwrong press try again........\n") ; 
				                #ifdef _WIN32
        			Sleep(2000); 
    			#else
        			sleep(2); 
    			#endif
    			cls();
		}
		}
//        char proceed;
//        proceed = getch();
//        
//        if (proceed == 'N' || proceed == 'n') {
//            printf("Game Over. The word was: %s\n", word);
//            return;
//        }
    }
    
    if (correct == length) {
    	cls();
        show_hangman(attempts);
        printf("\nCongratulations! You guessed the word: %s\n", word);
        saveGameRecords(word,"Win",score);
    } else if (repeat == 0) {
        show_hangman(attempts);
        cls();
        printf("\nYou failed as you did not adhere to the warning. The word was %s. Try again next time.\n", word);
        saveGameRecords(word,"Loss",score);
    } else {
    	cls();
        show_hangman(0);
        printf("\nYou failed! The word was: %s. Try again next time.\n", word);
        saveGameRecords(word,"Loss",score);
    }

    printf("Your Score is : %d \n",score);
char restart;
do {
    printf("\nDo you want to restart? (Y/N): ");
    scanf(" %c", &restart);
} while (restart != 'Y' && restart != 'y' && restart != 'N' && restart != 'n'); // use AND (&&) here

if (restart == 'Y' || restart == 'y') {
    score = 0;  
    printf("\nRestarting the game...\n");
    fflush(stdout);
    #ifdef _WIN32
        Sleep(3000); 
    #else
        sleep(3); 
    #endif
    cls();
    hangman_game(category_words, word_count, n, choice);  
} else {
    score = 0;
    printf("\nGame Over. The Score is reset to 0. \n");
    printf("\nEnding the game...\n");
    #ifdef _WIN32
        Sleep(3000); 
    #else
        sleep(3); 
    #endif
    cls();
}
}

void choice (){
     int choice,options;
    int q = 0;
    int exit = 1;
    do
    {
        cls();
        printf("----- HANGMAN GAME -------\n");
        printf("1. Play \n");
        printf("2. Exit \n");
        printf("3. Show Game Records \n");
        printf("Select Any of these option : ");
        scanf("%d",&options);
        switch (options)
        {
            case 1:
            	do {
                cls();
                printf("Choose a category:\n");
                printf("1. Programming\n");
                printf("2. Sports\n");
                printf("3. Animals\n");
                printf("4. Physics\n");
                printf("5. Maths\n");
                printf("6. Entire dictionary\n");
                printf("Enter your choice (1-6): ");
                scanf("%d", &choice);

                switch (choice) {
                    case 1:
                        printf("\nYou chose Programming!\n");
                        q = 0;
                        hangman_game(programming_words, programming_count, 1, choice);
                        break;
                    case 2:
                        printf("\nYou chose Sports!\n");
                        q = 0;
                        hangman_game(sports_words, sports_count, 1, choice);
                        break;
                    case 3:
                        printf("\nYou chose Animals!\n");
                        q = 0;
                        hangman_game(animal_words, animal_count, 1, choice);
                        break;
                    case 4:
                        printf("\nYou chose Math!\n");
                        q = 0;
                        hangman_game(maths_words, maths_count, 1, choice); 
                        break;
                    case 5:
                        printf("\nYou chose Physics!\n");
                        q = 0;
                        hangman_game(physics_words, physics_count, 1, choice);  
                        break;
                    case 6:  
                        printf("\nYou chose a random word from dictionary!\n");
                        q = 0;
                        hangman_game(NULL, 0, 0, choice);  
                        break;
                    default:
                        printf("\nInvalid choice! Please restart the game and choose a valid category.\n");
                        printf("\nEnter again.........");
                        #ifdef _WIN32
        					Sleep(3000); 
    					#else
        					sleep(3); 
    					#endif
                        q=1;
						break;
						
						
                }
                if (q == 0){
					break ; 
				}
            }while (q = 1);
                break;
            
            case 2:
            	
//            	int timeout = 10;
//            	clock_t start_time = clock();  
//  				while ((clock() - start_time) / CLOCKS_PER_SEC < timeout){
//                printf("Exiting.....\n");
//  } INCORRECT usage will only repeat till time is up better to use :

//int timeout = 10;
//clock_t start_time = clock();
//while ((clock() - start_time) / CLOCKS_PER_SEC < timeout) {
//    printf("Exiting.....\r"); // Use \r to overwrite on the same line
//    fflush(stdout);          // Ensure the output is updated immediately
//
//    #ifdef _WIN32
//        Sleep(1000);         // Sleep for 1000 milliseconds (1 second) on Windows
//    #else
//        sleep(1);            // Sleep for 1 second on Linux/Unix
//    #endif
//} best to use :
    printf("Exiting.....\n");
	fflush(stdout);
    #ifdef _WIN32
        Sleep(3000); 
    #else
        sleep(3); 
    #endif

	cls();
                exit = 0;
                break;

case 3:
    showGameRecord();
    char x;
    int n = 1;
    while (n == 1) {
        printf("Press enter to exit the record: ");
        x = getch();
        if (x == 13) {  
            cls();  
            n = 0;  
            break;  
        } else {
            printf("\nWrong input. Please press Enter........\n");
                #ifdef _WIN32
        			Sleep(2000); 
    			#else
        			sleep(2); 
    			#endif
    			cls();
        }
    }
    break;


            default:
                printf("\nInvalid Option. \n");
                printf("\nRestarting.........\n");
                    #ifdef _WIN32
        				Sleep(3000); 
    				#else
        				sleep(3); 
    				#endif
                break;
                
            }
            
    }while (exit);
    
}

int main() {
   choice();

    return 0;
}
