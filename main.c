#include <stdio.h>
#include <stdbool.h>

#include "character-manager.h"

#define MAX_CHARACTERS 100

const char *PLAYERS_FILE_PATH = "./game_data/players.csv"; 
const char *ENEMIES_FILE_PATH = "./game_data/enemies.csv"; 
const char *BOSSES_FILE_PATH = "./game_data/bosses.csv";
const char *SPELLS_FILE_PATH = "./game_data/spells.csv";

int main() {

    /*
    CharacterStruct characters[MAX_CHARACTERS];
    int characters_count = read_csv_generic(PLAYERS_FILE_PATH, characters, MAX_CHARACTERS, sizeof(CharacterStruct), parse_character);
    displayCSV("CSV PLAYERS", characters, characters_count, sizeof(CharacterStruct), printCharacter);

    EnemiesStruct enemies[MAX_CHARACTERS];
    int enemy_count = read_csv_generic(ENEMIES_FILE_PATH, enemies, MAX_CHARACTERS, sizeof(EnemiesStruct), parse_enemy);
    displayCSV("CSV ENEMIES", enemies, enemy_count, sizeof(EnemiesStruct), printEnemy);

    BossesStruct bosses[MAX_CHARACTERS];
    int boss_count = read_csv_generic(BOSSES_FILE_PATH, bosses, MAX_CHARACTERS, sizeof(BossesStruct), parse_boss);
    displayCSV("CSV BOSSES", bosses, boss_count, sizeof(BossesStruct), printBoss);
    */
      
    
    // Array to store Structures loaded from the CSV file
    CharacterStruct characters[MAX_CHARACTERS];
    EnemiesStruct enemies[MAX_CHARACTERS];
    BossesStruct bosses[MAX_CHARACTERS];
    
    SpellsStruct spells[MAX_CHARACTERS];
    
    // Read the CSV file and parse the data
    int characters_count = read_csv_generic(PLAYERS_FILE_PATH, characters, MAX_CHARACTERS, sizeof(CharacterStruct), parse_character);
    int enemy_count = read_csv_generic(ENEMIES_FILE_PATH, enemies, MAX_CHARACTERS, sizeof(EnemiesStruct), parse_enemy);
    // int bosses_count 
    int spell_count = read_csv_generic(SPELLS_FILE_PATH, spells, MAX_CHARACTERS, sizeof(SpellsStruct), parse_spell);


    // Check if data was loaded successfully
    if (characters_count < 0) {
      fprintf(stderr, "Error reading CSV file.\n");
      return 1;
    }
  
    // Display all characters
    printf("Choose a character :\n");
    for (int i = 0; i < characters_count; i++) {
        //printCharacter(&characters[i]); // Pass the address of each character
        printf("%d : %s\n", i, getCharacterName(&characters[i]));
    } 
    int characterRaw;
    
    //disableEcho();
    scanf("%d", &characterRaw);
    //enableEcho();
    printf("You choose ID : %d.\n", characterRaw);
    // Access specific fields of the first character
    if (characters_count > characterRaw && characterRaw >= 0) {
        int health = getCharacterFieldByName(&characters[characterRaw], "health");
        int mana = getCharacterFieldByName(&characters[characterRaw], "mana");
        const char *name = getCharacterName(&characters[characterRaw]);
  
        // Print the character retrieved values
        printf("\nDetails of the character:\n");
        printf("Name: %s, Health: %d, Mana: %d\n", name, health, mana);
    } else {
      printf("No characters loaded.\n");
    }

    // Random opponent
    int enemieRaw = RandomNumb(12);
    const char *opponent = getOpponentName(&enemies[enemieRaw]);
    printf("\nYour enemy is : %s\n", opponent);

    // Access specific fields of the first enemie
    if (enemy_count > enemieRaw && enemieRaw >= 0) {
        int health = getOpponentFieldByName(&enemies[enemieRaw], "health");
        int mana = getOpponentFieldByName(&enemies[enemieRaw], "mana");
        const char *name = getOpponentName(&enemies[enemieRaw]);
  
        // Print the enemie retrieved values
        printf("\nDetails of the enemy:\n");
        printf("Name: %s, Health: %d, Mana: %d\n", name, health, mana); //adapt for enemy struct
    } else {
        printf("No characters loaded.\n");
    }
    
    
    
    //--------------------------------FIGHT LOOP
    
    FightLoop(&characters[characterRaw], &enemies[enemieRaw]);
   
   

    return 0;
}






















