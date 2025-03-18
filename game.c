#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <dirent.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_FIELD_LENGTH 256
#define MAX_FIELDS 10

typedef struct {
    char fields[MAX_FIELDS][MAX_FIELD_LENGTH];
    int field_count;
} CSVLine;

// Function for reading a CSV line and store it into a struct csv_line
void parse_csv_line(char* line, CSVLine* csv_line) {
    char* token;
    char* rest = line;
    csv_line->field_count = 0;

    while ((token = strtok_r(rest, ",", &rest)) && csv_line->field_count < MAX_FIELDS) {
        strncpy(csv_line->fields[csv_line->field_count], token, MAX_FIELD_LENGTH - 1);
        csv_line->fields[csv_line->field_count][MAX_FIELD_LENGTH - 1] = '\0'; // s'assurer que la chaîne est terminée
        csv_line->field_count++;
    }
}

// Function for write a struct CSVLine into CSV
void write_csv_line(FILE* file, CSVLine* csv_line) {
    for (int i = 0; i < csv_line->field_count; i++) {
        fprintf(file, "%s", csv_line->fields[i]);
        if (i < csv_line->field_count - 1) {
            fprintf(file, ",");
        }
    }
    fprintf(file, "\n");
}

const char *GetFilePathGameData(int file_numb) {
    DIR *dirp;
    struct dirent *direntp;
    static char result[LINE_MAX];
    
    dirp = opendir("game_data");
    
    if (dirp == NULL) {
        perror("");
        return NULL;
    } else {
        for (int i = 0; i < file_numb; i++) {
            direntp = readdir(dirp);
            if (direntp == NULL) break; 
        }
        if (direntp != NULL) {
            snprintf(result, LINE_MAX, "game_data/%s", direntp->d_name);
        } else {
            result[0] = '\0';
        }
        closedir(dirp);
    }
    return result;
}

/*const char *ReturnGameDataFolder(short int file_id, short int x, short int y) {
    const char *file_path = GetFilePathGameData(file_id);
    if (!file_path) {
        return NULL;
    }

    FILE *fp = fopen(file_path, "r");
    if (!fp) {
        printf("File not found: %s\n", file_path);
        return NULL;
    }

    static char result[MAX_FIELD_LENGTH];
    char line[MAX_LINE_LENGTH];
    int current_line = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (current_line == x) {
            CSVLine csv_line;
            parse_csv_line(line, &csv_line);
            if (y < csv_line.field_count) {
                strncpy(result, csv_line.fields[y], MAX_FIELD_LENGTH);
                result[MAX_FIELD_LENGTH - 1] = '\0';
                fclose(fp);
                printf("Path: %s\n", file_path);
                return result;
            }
        }
        current_line++;
    }

    fclose(fp);
    return NULL;
}
*/

const char* ReturnGameDataFolder(short int file_id, short int x, short int y) {
    const char* file_path = GetFilePathGameData(file_id);
    if (!file_path) {
        return NULL;
    }

    FILE* fp = fopen(file_path, "r");
    if (!fp) {
        printf("File not found: %s\n", file_path);
        return NULL;
    }

    char* result = (char*)malloc(MAX_FIELD_LENGTH);
    if (!result) {
        printf("Memory allocation failed\n");
        fclose(fp);
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    int current_line = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (current_line == x) {
            CSVLine csv_line;
            parse_csv_line(line, &csv_line);
            if (y < csv_line.field_count) {
                strncpy(result, csv_line.fields[y], MAX_FIELD_LENGTH - 1);
                result[MAX_FIELD_LENGTH - 1] = '\0';
                fclose(fp);
                //printf("Path: %s\n", file_path);
                return result;
            }
        }
        current_line++;
    }

    fclose(fp);
    free(result); // Free memory if no valid result
    return NULL;
}

int FunctionStoreGameData(const char *data, short int file_id, short int coord_x, short int coord_y) {
    const char *file_path = GetFilePathGameData(file_id);
    if (!file_path) {
        return EXIT_FAILURE;
    }

    FILE *file = fopen(file_path, "r");
    if (!file) {
        perror("ERROR opening file");
        return EXIT_FAILURE;
    }

    FILE *temp_file = fopen("game_data/temp.csv", "w");
    if (!temp_file) {
        perror("ERROR opening temp file");
        fclose(file);
        return EXIT_FAILURE;
    }

    char line[MAX_LINE_LENGTH];
    int current_row = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        
        CSVLine csv_line;
        parse_csv_line(line, &csv_line);

        if (current_row == coord_x && coord_y < csv_line.field_count) {
            strncpy(csv_line.fields[coord_y], data, MAX_FIELD_LENGTH - 1);
            csv_line.fields[coord_y][MAX_FIELD_LENGTH - 1] = '\0';
        }

        write_csv_line(temp_file, &csv_line);
        current_row++;
    }

    fclose(file);
    fclose(temp_file);

    if (remove(file_path) != 0) {
        perror("ERROR removing original file");
        return EXIT_FAILURE;
    }

    if (rename("game_data/temp.csv", file_path) != 0) {
        perror("ERROR remane temp file");
        return EXIT_FAILURE;
    }
    printf("Storage successful\n");

    return EXIT_SUCCESS;
}

const char *CSV_reader(int file_ID, int coord_x, int coord_y) {
    
    if (file_ID > 9 || file_ID < 3) {
        printf("\nERROR: There is no such file!\n");
        return 0;
    }

    //printf("ID: %d x: %d y: %d\nLoading...\n", file_ID, coord_x, coord_y);
    const char *ask = ReturnGameDataFolder(file_ID, coord_x, coord_y);
    /*
    if (ask) {
        printf("Result: %s\n", ask);
    } else {
        printf("ERROR CSV_reader: Data not found.\n");
    }
    */
    return ask;
}
    
int CSV_writer(char to_store, int file_ID, int coord_x, int coord_y){ //__Storage function call
    
    printf("Storing: %s\n", &to_store);
    int result = FunctionStoreGameData(&to_store, file_ID, coord_x, coord_y);
    if (result == 0) {
        printf("Stored successfully\n");
    } else {
        printf("ERROR: Storage failed\n");
    }

    return 0;
}

char getYesNoResponse() {
    char input[MAX_INPUT];
    char response;

    while (1) {
        printf("y/n ?");
        // Read input scanf
        if (scanf("%9s", input) == 1) {
            // Flush the input buffer to avoid unwanted effects
            while (getchar() != '\n');

            // Convert in lower caracter
            for (int i = 0; input[i]; i++) {
                input[i] = tolower((unsigned char)input[i]);
            }

            if (strcmp(input, "y") == 0) {
                response = 'y';
                break;
            } else if (strcmp(input, "n") == 0) {
                response = 'n';
                break;
            } else {
                printf("\nInvalide input. Please awnser by y/n.\n");
            }
        } else {
            // Manage error
            printf("ERROR_reading_input.\n");
            clearerr(stdin);  // Remove error
        }
    }

    return response;
}

const char *ModifyData(int file_id, int x, int y) {

    // "CSV's ID: 3 test ; 4 races ; 5 classes ; 6 players ; 7 spells ; 8 bosses ; 9 enemies"

    const char *result = CSV_reader(file_id, x, y);
    printf("result: %s\n", result);

    /* "Function call to modify result data"
    printf("Want to modify this data ?");
    char input_keyboard = getYesNoResponse();
    

    if (input_keyboard == 'y') 
    {
        char new_data[LINE_MAX];
        
        printf("Enter a new data: ");
        scanf("%s", new_data);
        FunctionStoreGameData(new_data, file_id, x, y);
    } else {
        printf("You say: NO.\n");
    }
    */

    return result;
}

int RandomNumb(int numb_max) {
    
    srand(time(NULL));
    return 1+rand()%numb_max;
}

int randOpponentAction() {

    srand(time(NULL));
   
    int random_choice = rand() % 2;
    random_choice = 0; // force to do attack until we make spellFunction
    
    return random_choice;
}

const char * playerSelection(const char * Player, int folder, int yname) {
    
    // HAVE TO CODE "show all the players"
    printf("Please select an id :");
    int xinput;
    scanf("%d",&xinput);
    printf("%d\n", xinput);
    Player = CSV_reader(folder, xinput ,yname);
    printf("Your Player is : %s\n", Player);

    return Player;
}

void attackFunction(const char * entity, int shooter_folder, int xshooter, int yshooter_strenght, int target_folder, int xtarget, int ytarget_hp) {
    printf("\n - %s do an attack ! -\n", entity);
    // read the strenght of the shooter
    const char *shooter_strenght_amount = CSV_reader(shooter_folder, xshooter, yshooter_strenght);
    int shooter_strenght = atoi(shooter_strenght_amount); // Convert
    printf("\nShooter strenght is %d\n", shooter_strenght);
     // read target HP
    const char *target_hp_amount = CSV_reader(target_folder, xtarget, ytarget_hp);
    int target_hp = atoi(target_hp_amount); // Convert
    printf("Target HP %d\n", target_hp);
    // Calcul new target's HP
    target_hp -= shooter_strenght;
    printf("Target HP after attack %d\n", target_hp);
    // write new target HP in CSV
    char new_target_hp[MAX_FIELD_LENGTH];
    snprintf(new_target_hp, MAX_FIELD_LENGTH, "%d", target_hp);
    FunctionStoreGameData(new_target_hp, target_folder, xtarget, ytarget_hp);
return ;    
}

int main() {

// fight system

    int player_folder = 6; // players.csv
    int opponent_folder = 9; // ennemies.csv
    int spells_folder = 7; // spells.csv
    
    int xplayer = 1; // is the player we choose on players.csv
    int xopponent; // is generated randomly

    int yname = 1;// name coord on CSV
    int yhp = 2; // hp coord on CSV
    int ymp = 3;
    int yattack = 4;// strength coord on CSV

    //Player selection
    const char *Player = playerSelection(Player, player_folder, yname);
    
    // randomize ennemie
    xopponent = RandomNumb(12);
    const char *Opponent = CSV_reader(opponent_folder, xopponent ,yname);
    printf("\n - Your opponent is : %s -\n", Opponent);

    // Fight loop (while)
    // Action selection
    char action [100] ;
    printf("\nWhat do u want to do ? : ");
    scanf("%10s", action);
    printf("You decided to : %s\n", action);

    if (strcmp(action, "attack") == 0) {
    attackFunction(Player, player_folder, xplayer, yattack, opponent_folder, xopponent, yhp);
    }
    else if (strcmp(action, "spell") == 0) { // 'have to code this
        // const char *result = CSV_reader(folder, xplayer, yattack); 
        // printf("RESULT : %s", result);
        printf("Action not done yet...\n");
    }
    else {
        printf("Unknown action. (%s)\n", action);
    }

    // Opponent turn
    printf("\n - Opponnent Turn -\n");

    int numb = randOpponentAction();

    if (numb == 0) {
       
        attackFunction(Opponent ,opponent_folder, xopponent, yattack, player_folder, xplayer, yhp);

    } else {
       
        printf("do spell\n");
    }




return 0;
}