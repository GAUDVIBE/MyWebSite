#ifndef CHARACTER_MANAGER_H_
#define CHARACTER_MANAGER_H_

#include <stdio.h>
#include <stdlib.h>
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

// Function Prototypes
int read_csv_generic(const char *filename, void *data, int max_items, size_t struct_size, void (*parser)(char *, void *));
void parse_character(char *line, void *data);
void parse_enemy(char *line, void *data);
void parse_boss(char *line, void *data);
void displayCharacter(const char *title, void *characters, int count, size_t size, void (*printFunc)(void *));
void printCharacter(void *character);
void printEnemy(void *enemy);
void printBoss(void *boss);
int getCharacterFieldByName(const CharacterStruct *character, const char *fieldName);
int getOpponentFieldByName(const EnemiesStruct *enemy, const char *fieldName);
const char *getCharacterName(const CharacterStruct *character);
const char *getOpponentName(const EnemiesStruct *enemy);
void attackCharacter(CharacterStruct *attacker, EnemiesStruct *defender);
void attackEnemy(EnemiesStruct *attacker, CharacterStruct *defender);
int isCharacterAlive(CharacterStruct *character);
int isEnemyAlive(EnemiesStruct *enemy);
int RandomNumb(int numb_max);
void disableEcho();
void enableEcho(); 

#endif // CHARACTER_MANAGER_H_