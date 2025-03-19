#include "character-manager.h"



//---------------------------------------------CSV parsing

int read_csv_generic(const char *filename, void *data, int max_items, size_t struct_size, void (*parser)(char *, void *)) {
    if (!filename || !data || max_items <= 0 || struct_size == 0 || !parser) {
        fprintf(stderr, "Invalid arguments passed to read_csv_generic\n");
        return -1;
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    char line[MAX_LINE];
    int count = 0;

    // Skip the header line
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        return 0;
    }

    while (fgets(line, sizeof(line), file) && count < max_items) {
        parser(line, (char *)data + count * struct_size);
        count++;
    }

    fclose(file);
    return count;
}

void parse_character(char *line, void *data) {
    CharacterStruct *character = (CharacterStruct *)data;
    char *token = strtok(line, ",");

    if (token) character->id = atoi(token);

    token = strtok(NULL, ",");
    if (token) strncpy(character->name, token, MAX_NAME_LEN - 1);
    character->name[MAX_NAME_LEN - 1] = '\0';

    int *fields[] = {
        &character->health, &character->mana, &character->strength, &character->intelligence, 
        &character->defense, &character->resistance, &character->speed, &character->luck, &character->rarity
    };

    for (int i = 0; i < sizeof(fields) / sizeof(fields[0]); i++) {
        token = strtok(NULL, ",");
        if (token == NULL) break;
        *fields[i] = atoi(token);
    }
}

void parse_enemy(char *line, void *data) {
    EnemiesStruct *enemy = (EnemiesStruct *)data;
    char *token = strtok(line, ",");

    if (token) enemy->id = atoi(token);

    token = strtok(NULL, ",");
    if (token) strncpy(enemy->name, token, MAX_NAME_LEN - 1);
    enemy->name[MAX_NAME_LEN - 1] = '\0';

    int *fields[] = {
        &enemy->health, &enemy->mana, &enemy->strength, &enemy->intelligence, 
        &enemy->defense, &enemy->resistance, &enemy->speed, &enemy->luck
    };

    for (int i = 0; i < sizeof(fields) / sizeof(fields[0]); i++) {
        token = strtok(NULL, ",");
        if (token == NULL) break;
        *fields[i] = atoi(token);
    }
}

void parse_boss(char *line, void *data) {
    BossesStruct *boss = (BossesStruct *)data;
    char *token = strtok(line, ",");

    if (token) boss->id = atoi(token);

    token = strtok(NULL, ",");
    if (token) strncpy(boss->name, token, MAX_NAME_LEN - 1);
    boss->name[MAX_NAME_LEN - 1] = '\0';

    int *fields[] = {
        &boss->health, &boss->mana, &boss->strength, &boss->intelligence, 
        &boss->defense, &boss->resistance, &boss->speed, &boss->luck, &boss->rarity
    };

    for (int i = 0; i < sizeof(fields) / sizeof(fields[0]); i++) {
        token = strtok(NULL, ",");
        if (token == NULL) break;
        *fields[i] = atoi(token);
    }
}




//-----------------------------------------------Print

void displayCharacter(const char *title, void *characters, int count, size_t size, void (*printFunc)(void *)) {
    if (count > 0) {
        printf("\n - %s -\n", title);
        for (int i = 0; i < count; i++) {
            void *character = (char *)characters + i * size;
            printFunc(character);
        }
    } else {
        printf("Missing data or error reading\n");
    }
}

void printCharacter(void *character) {
    CharacterStruct *c = (CharacterStruct *)character;
    printf("ID: %d, Name: %s, Health: %d, Mana: %d, Strength: %d, Intelligence: %d, Defense: %d, Resistance: %d, Speed: %d, Luck: %d, Rarity: %d\n",
           c->id, c->name, c->health, c->mana, c->strength, c->intelligence, c->defense, c->resistance, c->speed, c->luck, c->rarity);
}

void printEnemy(void *enemy) {
    EnemiesStruct *e = (EnemiesStruct *)enemy;
    printf("ID: %d, Name: %s, Health: %d, Mana: %d, Strength: %d, Intelligence: %d, Defense: %d, Resistance: %d, Speed: %d, Luck: %d\n",
           e->id, e->name, e->health, e->mana, e->strength, e->intelligence, e->defense, e->resistance, e->speed, e->luck);
}

void printBoss(void *boss) {
    BossesStruct *b = (BossesStruct *)boss;
    printf("ID: %d, Name: %s, Health: %d, Mana: %d, Strength: %d, Intelligence: %d, Defense: %d, Resistance: %d, Speed: %d, Luck: %d, Rarity: %d\n",
           b->id, b->name, b->health, b->mana, b->strength, b->intelligence, b->defense, b->resistance, b->speed, b->luck, b->rarity);
}




//---------------------------------Structure parsing


// Getter function for CharacterStruct fields using field names
int getOpponentFieldByName(const EnemiesStruct *enemy, const char *fieldName) {
    if (strcmp(fieldName, "id") == 0) {
        return enemy->id;
    } else if (strcmp(fieldName, "health") == 0) {
        return enemy->health;
    } else if (strcmp(fieldName, "mana") == 0) {
        return enemy->mana;
    } else if (strcmp(fieldName, "strength") == 0) {
        return enemy->strength;
    } else if (strcmp(fieldName, "intelligence") == 0) {
        return enemy->intelligence;
    } else if (strcmp(fieldName, "defense") == 0) {
        return enemy->defense;
    } else if (strcmp(fieldName, "resistance") == 0) {
        return enemy->resistance;
    } else if (strcmp(fieldName, "speed") == 0) {
        return enemy->speed;
    } else if (strcmp(fieldName, "luck") == 0) {
        return enemy->luck;
    } else {
        fprintf(stderr, "Invalid field name: %s\n", fieldName);
        return -1; // Indicate an error
    }
}

// Getter function for EnemiesStruct fields using field names
int getCharacterFieldByName(const CharacterStruct *character, const char *fieldName) {;
    if (strcmp(fieldName, "id") == 0) {
        return character->id;
    } else if (strcmp(fieldName, "health") == 0) {
        return character->health;
    } else if (strcmp(fieldName, "mana") == 0) {
        return character->mana;
    } else if (strcmp(fieldName, "strength") == 0) {
        return character->strength;
    } else if (strcmp(fieldName, "intelligence") == 0) {
        return character->intelligence;
    } else if (strcmp(fieldName, "defense") == 0) {
        return character->defense;
    } else if (strcmp(fieldName, "resistance") == 0) {
        return character->resistance;
    } else if (strcmp(fieldName, "speed") == 0) {
        return character->speed;
    } else if (strcmp(fieldName, "luck") == 0) {
        return character->luck;
    } else if (strcmp(fieldName, "rarity") == 0) {
        return character->rarity;
    } else {
        fprintf(stderr, "Invalid field name: %s\n", fieldName);
        return -1; // Indicate an error
    }
}

// Getter function for the name field (returns a string)
const char *getCharacterName(const CharacterStruct *character) {
    return character->name;
}

const char *getOpponentName(const EnemiesStruct *enemy) {
    return enemy->name;
}







//--------------------------------------Fight Loop

// Function to simulate an attack
void attackCharacter(CharacterStruct *attacker, EnemiesStruct *defender) {
    printf("%s attacks %s!\n", attacker->name, defender->name);
    defender->health -= attacker->strength;
    printf("%s takes %d damage! %s's health is now %d.\n", defender->name, attacker->strength, defender->name, defender->health);
}

void attackEnemy(EnemiesStruct *attacker, CharacterStruct *defender) {
    printf("%s attacks %s!\n", attacker->name, defender->name);
    defender->health -= attacker->strength;
    printf("%s takes %d damage! %s's health is now %d.\n", defender->name, attacker->strength, defender->name, defender->health);
}

// Function to check if a character is still alive
int isCharacterAlive(CharacterStruct *character) {
    return character->health > 0;
}

int isEnemyAlive(EnemiesStruct *enemy) {
    return enemy->health > 0;
}

// Determine who goes first based on speed
int isCharacterFirst(CharacterStruct* character, EnemiesStruct* enemy) {
    if (character->speed > enemy->speed) {
        return 1;
    } else if (enemy->speed > character->speed) {
        return 0;
    } else {
        // If speeds are equal, randomly decide who goes first
        return (rand() % 2 == 0);
    }
}




















//--------------------------------------Features

// Function to disable terminal echo
void disableEcho() {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty); // Get current terminal attributes
    tty.c_lflag &= ~ECHO;          // Disable echo
    tcsetattr(STDIN_FILENO, TCSANOW, &tty); // Apply changes immediately
}

// Function to enable terminal echo
void enableEcho() {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty); // Get current terminal attributes
    tty.c_lflag |= ECHO;           // Enable echo
    tcsetattr(STDIN_FILENO, TCSANOW, &tty); // Apply changes immediately
}

int RandomNumb(int numb_max) {
    
    srand(time(NULL));
    return 1+rand()%numb_max;
}
