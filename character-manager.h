
#ifndef CHARACTER_MANAGER_H_
#define CHARACTER_MANAGER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#define MAX_LINE 1024
#define MAX_NAME_LEN 100
#define MAX_FIELDS 10
#define MAX_FIELD_LENGTH 256

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

void attackCharacter(CharacterStruct *attacker, EnemiesStruct *defender);
void attackEnemy(EnemiesStruct *attacker, CharacterStruct *defender);
int isCharacterAlive(CharacterStruct *character);
int isEnemyAlive(EnemiesStruct *enemy);
int isCharacterFirst(CharacterStruct* character, EnemiesStruct* enemy);
void applyDamageToEnemy(EnemiesStruct* enemy, int damage);

int ChooseSpell(SpellsStruct *spells, int count, CharacterStruct* character);
void castSpell(CharacterStruct* caster, EnemiesStruct* target, SpellsStruct* spell);
int getCharacterMana(CharacterStruct* character);
void setCharacterMana(CharacterStruct* character, int newMP);

void FightLoop(CharacterStruct* character, EnemiesStruct* enemy, SpellsStruct* spells, int spellCount);
void performCharacterTurn(CharacterStruct* character, EnemiesStruct* enemy);
void performEnemyTurn(EnemiesStruct* enemy, CharacterStruct* character);

int RandomNumb(int numb_max);
void disableEcho();
void enableEcho(); 

#endif // CHARACTER_MANAGER_H_
