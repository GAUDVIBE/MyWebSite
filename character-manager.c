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

void parse_spell(char *line,void *data) {
    SpellsStruct *spell = (SpellsStruct *)data;
    char *token = strtok(line, ",");

    if (token) spell->id = atoi(token);

    token = strtok(NULL, ",");
    if (token) strncpy(spell->name, token, MAX_NAME_LEN - 1);
    spell->name[MAX_NAME_LEN - 1] = '\0';

    int *fields[] = {
        &spell->cost, &spell->dmg, &spell->effect, &spell->cooldown, 
        &spell->race, &spell->race, &spell->rarity
    };

    for (int i = 0; i < sizeof(fields) / sizeof(fields[0]); i++) {
        token = strtok(NULL, ",");
        if (token == NULL) break;
        *fields[i] = atoi(token);
    }
}














//-----------------------------------------------Print

void displayCSV(const char *title, void *characters, int count, size_t size, void (*printFunc)(void *)) {
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

void printSpell(void *spell) {
    SpellsStruct *c = (SpellsStruct *)spell;
    printf("ID: %d, Name: %s, Cost: %d, Dmg: %d, Effect: %d, Cooldown: %d, Race: %d, Rarity: %d\n",
           c->id, c->name, c->cost, c->dmg, c->effect, c->cooldown, c->race, c->rarity);
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

// Getter function for SpellsStruct fields using field names
int getSpellFieldByName(const SpellsStruct *spell, const char *fieldName) {;
    if (strcmp(fieldName, "id") == 0) {
        return spell->id;
    } else if (strcmp(fieldName, "cost") == 0) {
        return spell->cost;
    } else if (strcmp(fieldName, "dmg") == 0) {
        return spell->dmg;
    } else if (strcmp(fieldName, "effect") == 0) {
        return spell->effect;
    } else if (strcmp(fieldName, "cooldown") == 0) {
        return spell->cooldown;
    } else if (strcmp(fieldName, "race") == 0) {
        return spell->race;
    } else if (strcmp(fieldName, "rarity") == 0) {
        return spell->rarity;
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

const char *getSpellName(const SpellsStruct *spell) {
    return spell->name;
}












//--------------------------------------Fight Loop function call

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











//-------------------------------------Spells func

 int ChooseSpell(SpellsStruct *spells, int count) {
        // Check if the CSV was structured successfully
        if (count < 0) {
        fprintf(stderr, "[spell_count]Error reading CSV file .\n");
        return 1;
        }

        printf("Choose a spell :\n");
        for (int i = 0; i < count; i++) {
            printf("%d : %s\n", i, getSpellName(&spells[i]));
        } 
        int spellRaw;
        
        //disableEcho();
        scanf("%d", &spellRaw);
        //enableEcho();
        printf("You have enter: %d.\n", spellRaw);
        // Access specific fields of the first character
        if (count > spellRaw && spellRaw >= 0) {
            int cost = getSpellFieldByName(&spells[spellRaw], "cost");
            int dmg = getSpellFieldByName(&spells[spellRaw], "dmg");
            const char *name = getSpellName(&spells[spellRaw]);
    
            // Print the character retrieved values
            printf("\nDetails of the spell:\n");
            printf("%s: cost: %d, dmg: %d\n", name, cost, dmg);
        } else {
        printf("No characters loaded.\n");
        }
    }













//-------------------------------------FIGHT LOOP

void FightLoop(CharacterStruct* character, EnemiesStruct* enemy){
    // Determine who goes first based on initiative
    bool characterFirst = isCharacterFirst(character, enemy);
        
    while (isCharacterAlive(character) && isEnemyAlive(enemy)) {
        if (characterFirst) {
            // Character's turn
            printf("%s's turn!\n", getCharacterName(character));
            attackCharacter(character, enemy);
        
            // Check if the enemy is defeated
            if (!isEnemyAlive(enemy)) {
                printf("%s has been defeated!\n", getOpponentName(enemy));
                break;
            }
        
            // Enemy's turn
            printf("%s's turn!\n", getOpponentName(enemy));
            attackEnemy(enemy, character);
        
            // Check if the character is defeated
            if (!isCharacterAlive(character)) {
                printf("%s has been defeated!\n", getCharacterName(character));
                break;
            }
            } else {
            // Enemy's turn
            printf("%s's turn!\n", getOpponentName(enemy));
            attackEnemy(enemy, character);
        
            // Check if the character is defeated
            if (!isCharacterAlive(character)) {
                printf("%s has been defeated!\n", getCharacterName(character));
                break;
            }
        
            // Character's turn
            printf("%s's turn!\n", getCharacterName(character));
            attackCharacter(character, enemy);
        
            // Check if the enemy is defeated
            if (!isEnemyAlive(enemy)) {
                printf("%s has been defeated!\n", getOpponentName(enemy));
                break;
            }
        }
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

