#include "character-manager.h"

// HTML Helper Functions
void print_html_header() {
    printf(HTML_HEADER);
}

void print_html_footer() {
    printf(HTML_FOOTER);
}

void html_print_character(CharacterStruct *character) {
    printf("<div class='character-details'>\n");
    printf("<h3>%s</h3>\n", character->name);
    printf("<p><strong>Health:</strong> %d</p>\n", character->health);
    printf("<p><strong>Mana:</strong> %d</p>\n", character->mana);
    printf("<p><strong>Strength:</strong> %d</p>\n", character->strength);
    printf("<p><strong>Defense:</strong> %d</p>\n", character->defense);
    printf("</div>\n");
}

void html_print_enemy(EnemiesStruct *enemy) {
    printf("<div class='enemy-details'>\n");
    printf("<h3>%s</h3>\n", enemy->name);
    printf("<p><strong>Health:</strong> %d</p>\n", enemy->health);
    printf("<p><strong>Strength:</strong> %d</p>\n", enemy->strength);
    printf("<p><strong>Defense:</strong> %d</p>\n", enemy->defense);
    printf("</div>\n");
}

 
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
        &spell->cost, &spell->dmg, &spell->effect, &spell->cooldown, &spell->race, &spell->rarity
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
 
void characterTurn(CharacterStruct* character, EnemiesStruct* enemy, SpellsStruct* spells, int spellCount) {
    printf("<div class='turn-options'>\n");
    printf("<h3>%s's Turn</h3>\n", getCharacterName(character));
    printf("<div class='stats'>\n");
    printf("<p>HP: %d/%d | MP: %d/%d</p>\n",
           character->health, character->health,
           getCharacterMana(character), character->mana);
    printf("</div>\n");
    
    printf("<form method='post' action='game.cgi'>\n");
    printf("<input type='hidden' name='character_id' value='%d'>\n", character->id);
    printf("<input type='hidden' name='enemy_id' value='%d'>\n", enemy->id);
    
    printf("<div class='action-buttons'>\n");
    printf("<button type='submit' name='action' value='attack' class='btn'>Attack</button>\n");
    
    if (spellCount > 0 && getCharacterMana(character) > 0) {
        printf("<button type='submit' name='action' value='spell' class='btn'>Cast Spell</button>\n");
    }
    
    printf("<button type='submit' name='action' value='stats' class='btn'>Check Stats</button>\n");
    printf("</div>\n");
    printf("</form>\n");
    printf("</div>\n");
}


void enemyTurn(EnemiesStruct* enemy, CharacterStruct* character) {
    printf("\n%s's turn!\n", getOpponentName(enemy));
    attackEnemy(enemy, character);
}

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
bool isCharacterAlive(CharacterStruct *character) {
    return character->health > 0;
}
 
bool isEnemyAlive(EnemiesStruct *enemy) {
    return enemy->health > 0;
}
 
// Determine who goes first based on speed
bool isCharacterFirst(CharacterStruct* character, EnemiesStruct* enemy) {
    if (character->speed > enemy->speed) {
        return true;
    } else if (enemy->speed > character->speed) {
        return false;
    } else {
        // If speeds are equal, randomly decide who goes first
        return (rand() % 2 == 0);
    }
}
 
 
 
 
 
 
 
 
 
 
 
//-------------------------------------Spells func
 
// Modified ChooseSpell to return selected index
int ChooseSpell(SpellsStruct *spells, int count, CharacterStruct *character) {
    if (count <= 0) {
        fprintf(stderr, "No spells available.\n");
        return -1;
    }

    printf("\nAvailable Spells (Current MP: %d):\n", getCharacterMana(character));
    for (int i = 0; i < count; i++) {
        int cost = getSpellFieldByName(&spells[i], "cost");
        printf("%d: %s (Cost: %d MP)", i, getSpellName(&spells[i]), cost);
        if (cost > getCharacterMana(character)) printf(" - Not enough MP");
        printf("\n");
    }
    
    printf("-1: Cancel\n> ");
    int spellRaw;
    scanf("%d", &spellRaw);
    
    if (spellRaw == -1) return -1;
    if (spellRaw < 0 || spellRaw >= count) {
        printf("Invalid selection.\n");
        return -1;
    }

    SpellsStruct* selected = &spells[spellRaw];
    printf("\nSelected: %s (Cost: %d MP, Damage: %d)\n",
           getSpellName(selected),
           getSpellFieldByName(selected, "cost"),
           getSpellFieldByName(selected, "dmg"));
    
    return spellRaw;
}

int getCharacterMana(CharacterStruct *character) {
    return character->mana;
}

void setCharacterMana(CharacterStruct *character, int newMP) {
    character->mana = newMP;
}

void applySpellToCharacter(CharacterStruct* target, SpellsStruct* spell) {
    int effect = getSpellFieldByName(spell, "effect"); // Positive for healing
    
    if (effect > 0) {
        // Healing spell
        int previous_health = target->health;
        target->health += effect;
        printf("%s casts %s on themselves, healing for %d HP! (HP: %d → %d)\n",
               getCharacterName(target),
               getSpellName(spell),
               effect,
               previous_health,
               target->health);
    }
    else if (effect < 0) {
        // Damaging spell
        int damage = -effect; // Convert to positive for display
        target->health += effect; // effect is negative, so this subtracts
        
        printf("%s casts %s on themselves, taking %d damage! (HP: %d → %d)\n",
               getCharacterName(target),
               getSpellName(spell),
               damage,
               target->health - effect, // original health
               target->health);
        
        // Death check
        if (target->health <= 0) {
            printf("%s has succumbed to their own magic!\n", getCharacterName(target));
        }
    }
    else {
        // Status effect or buff
        printf("%s casts %s on themselves. %s\n",
               getCharacterName(target),
               getSpellName(spell),
               "The magic takes effect."); // Default description
    }
}

void applySpellToEnemy(CharacterStruct* caster, EnemiesStruct* target, SpellsStruct* spell) {
    int damage = getSpellFieldByName(spell, "dmg");
    target->health -= damage;
    printf("%s casts %s on %s for %d damage!\n",
           getCharacterName(caster),
           getSpellName(spell),
           getOpponentName(target),
           damage);
}
 
// Helper function to get spell description
const char* getSpellFieldText(SpellsStruct* spell, const char* field) {
    // Implement your spell description lookup here
    return "The spell shimmers with magical energy."; // Default text
}


 
 
 
 
 
 
 
 
 
 
//-------------------------------------FIGHT LOOP
 

// Modified FightLoop for web
void FightLoop(CharacterStruct* character, EnemiesStruct* enemy, SpellsStruct* spells, int spellCount) {
    print_html_header();
    
    printf("<div class='battle-log'>\n");
    printf("<h2>Battle Begins!</h2>\n");
    
    bool characterFirst = isCharacterFirst(character, enemy);

    while (isCharacterAlive(character) && isEnemyAlive(enemy)) {
        if (characterFirst) {
            characterTurn(character, enemy, spells, spellCount);
            if (!isEnemyAlive(enemy)) break;
            enemyTurn(enemy, character);
            if (!isCharacterAlive(character)) break;
        } else {
            enemyTurn(enemy, character);
            if (!isCharacterAlive(character)) break;
            characterTurn(character, enemy, spells, spellCount);
            if (!isEnemyAlive(enemy)) break;
        }
    }
    
    printf("</div>\n"); // Close battle-log
    
    // Battle result
    if (!isEnemyAlive(enemy)) {
        printf("<h2>Victory! %s has been defeated!</h2>\n", enemy->name);
    } else {
        printf("<h2>Defeat! %s has fallen!</h2>\n", character->name);
    }
    
    printf("<a href='/cgi-bin/game.cgi'>Start New Game</a>\n");
    print_html_footer();
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
