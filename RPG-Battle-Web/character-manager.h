#ifndef CHARACTER_MANAGER_H
#define CHARACTER_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>  // For bool type

#define MAX_LINE 1024
#define MAX_NAME_LEN 50

// HTML helper macros
#define HTML_HEADER "Content-type: text/html\n\n" \
                    "<!DOCTYPE html>\n" \
                    "<html>\n" \
                    "<head>\n" \
                    "<title>RPG Battle</title>\n" \
                    "<style>\n" \
                    "body { font-family: Arial, sans-serif; margin: 20px; }\n" \
                    ".battle-log { border: 1px solid #ccc; padding: 10px; margin-bottom: 20px; }\n" \
                    "form { margin: 10px 0; }\n" \
                    "</style>\n" \
                    "</head>\n" \
                    "<body>\n"

#define HTML_FOOTER "</body>\n</html>\n"

// Structure for Characters
typedef struct CharacterStruct {
    int id;
    char name[MAX_NAME_LEN];
    int health;
    int mana;
    int strength;
    int intelligence;
    int defense;
    int resistance;
    int speed;
    int luck;
    int rarity;
} CharacterStruct;

// Structure for Enemies
typedef struct EnemiesStruct {
    int id;
    char name[MAX_NAME_LEN];
    int health;
    int mana;
    int strength;
    int intelligence;
    int defense;
    int resistance;
    int speed;
    int luck;
} EnemiesStruct;

// Structure for Bosses
typedef struct BossesStruct {
    int id;
    char name[MAX_NAME_LEN];
    int health;
    int mana;
    int strength;
    int intelligence;
    int defense;
    int resistance;
    int speed;
    int luck;
    int rarity;
} BossesStruct;

// Structure for Spells
typedef struct SpellsStruct {
    int id;
    char name [MAX_NAME_LEN];
    int cost;
    int dmg;
    int effect;
    int cooldown;
    int race;
    int rarity;
} SpellsStruct;

// Function Prototypes

void print_html_header();
void print_html_footer();
void html_print_character(CharacterStruct *character);
void html_print_enemy(EnemiesStruct *enemy);
void html_print_spell(SpellsStruct *spell);
bool isCharacterFirst(CharacterStruct* character, EnemiesStruct* enemy);
void characterTurn(CharacterStruct* character, EnemiesStruct* enemy, SpellsStruct* spells, int spellCount);
void FightLoop(CharacterStruct* character, EnemiesStruct* enemy, SpellsStruct* spells, int spellCount);
bool isCharacterAlive(CharacterStruct *character);
bool isEnemyAlive(EnemiesStruct *enemy);

int read_csv_generic(const char *filename, void *data, int max_items, size_t struct_size, void (*parser)(char *, void *));
void parse_character(char *line, void *data);
void parse_enemy(char *line, void *data);
void parse_boss(char *line, void *data);
void parse_spell(char *line, void *data);

void displayCSV(const char *title, void *characters, int count, size_t size, void (*printFunc)(void *));
void printCharacter(void *character);
void printEnemy(void *enemy);
void printBoss(void *boss);
void printSpell(void *spell);

int getCharacterFieldByName(const CharacterStruct *character, const char *fieldName);
int getOpponentFieldByName(const EnemiesStruct *enemy, const char *fieldName);
int getSpellFieldByName(const SpellsStruct *character, const char *fieldName);
const char *getCharacterName(const CharacterStruct *character);
const char *getOpponentName(const EnemiesStruct *enemy);
const char *getSpellName(const SpellsStruct *spell);


void enemyTurn(EnemiesStruct* enemy, CharacterStruct* character);
void attackCharacter(CharacterStruct *attacker, EnemiesStruct *defender);
void attackEnemy(EnemiesStruct *attacker, CharacterStruct *defender);

void applyDamageToEnemy(EnemiesStruct* enemy, int damage);

int ChooseSpell(SpellsStruct *spells, int count, CharacterStruct* character);
int getCharacterMana(CharacterStruct* character);
void setCharacterMana(CharacterStruct* character, int newMP);
void applySpellToCharacter(CharacterStruct* target, SpellsStruct* spell);
void applySpellToEnemy(CharacterStruct* caster, EnemiesStruct* target, SpellsStruct* spell);
const char* getSpellFieldText(SpellsStruct* spell, const char* field);



void performCharacterTurn(CharacterStruct* character, EnemiesStruct* enemy);
void performEnemyTurn(EnemiesStruct* enemy, CharacterStruct* character);

int RandomNumb(int numb_max);
void disableEcho();
void enableEcho(); 

#endif // CHARACTER_MANAGER_H_
