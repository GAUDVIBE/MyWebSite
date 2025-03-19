#include <stdio.h>
#include <stdbool.h>

#include "character-manager.h"

#define MAX_CHARACTERS 100

const char *PLAYERS_FILE_PATH = "./game_data/players.csv"; 
const char *ENEMIES_FILE_PATH = "./game_data/enemies.csv"; 
const char *BOSSES_FILE_PATH = "./game_data/bosses.csv";

int main() {

    /*
    CharacterStruct characters[MAX_CHARACTERS];
    int characters_count = read_csv_generic(PLAYERS_FILE_PATH, characters, MAX_CHARACTERS, sizeof(CharacterStruct), parse_character);
    displayCharacter("CSV PLAYERS", characters, characters_count, sizeof(CharacterStruct), printCharacter);

    EnemiesStruct enemies[MAX_CHARACTERS];
    int enemy_count = read_csv_generic(ENEMIES_FILE_PATH, enemies, MAX_CHARACTERS, sizeof(EnemiesStruct), parse_enemy);
    displayCharacter("CSV ENEMIES", enemies, enemy_count, sizeof(EnemiesStruct), printEnemy);

    BossesStruct bosses[MAX_CHARACTERS];
    int boss_count = read_csv_generic(BOSSES_FILE_PATH, bosses, MAX_CHARACTERS, sizeof(BossesStruct), parse_boss);
    displayCharacter("CSV BOSSES", bosses, boss_count, sizeof(BossesStruct), printBoss);
    */
      
    
    // Array to store characters loaded from the CSV file
    CharacterStruct characters[MAX_CHARACTERS];
    EnemiesStruct enemies[MAX_CHARACTERS];
    BossesStruct bosses[MAX_CHARACTERS];
    

    // Read the CSV file and parse the data
    int characters_count = read_csv_generic(PLAYERS_FILE_PATH, characters, MAX_CHARACTERS, sizeof(CharacterStruct), parse_character);
    int enemy_count = read_csv_generic(ENEMIES_FILE_PATH, enemies, MAX_CHARACTERS, sizeof(EnemiesStruct), parse_enemy);
    // int bosses_count 

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

    // Access specific fields of the first character
    if (enemy_count > enemieRaw && enemieRaw >= 0) {
        int health = getOpponentFieldByName(&enemies[enemieRaw], "health");
        int mana = getOpponentFieldByName(&enemies[enemieRaw], "mana");
        const char *name = getOpponentName(&enemies[enemieRaw]);
  
        // Print the enemy retrieved values
        printf("\nDetails of the enemy:\n");
        printf("Name: %s, Health: %d, Mana: %d\n", name, health, mana); //adapt for enemy struct
    } else {
        printf("No characters loaded.\n");
    }
    
    
    
    //--------------------------------FIGHT LOOP

   

    // Determine who goes first based on initiative
    printf("yo\n");
    bool characterFirst = isCharacterFirst(&characters[characterRaw], &enemies[enemieRaw]);
    
    while (isCharacterAlive(&characters[characterRaw]) && isEnemyAlive(&enemies[enemieRaw])) {
        if (characterFirst) {
            // Character's turn
            printf("%s's turn!\n", getCharacterName(&characters[characterRaw]));
            attackCharacter(&characters[characterRaw], &enemies[enemieRaw]);
    
            // Check if the enemy is defeated
            if (!isEnemyAlive(&enemies[enemieRaw])) {
                printf("%s has been defeated!\n", getOpponentName(&enemies[enemieRaw]));
                break;
            }
    
            // Enemy's turn
            printf("%s's turn!\n", getOpponentName(&enemies[enemieRaw]));
            attackEnemy(&enemies[enemieRaw], &characters[characterRaw]);
    
            // Check if the character is defeated
            if (!isCharacterAlive(&characters[characterRaw])) {
                printf("%s has been defeated!\n", getCharacterName(&characters[characterRaw]));
                break;
            }
            } else {
            // Enemy's turn
            printf("%s's turn!\n", getOpponentName(&enemies[enemieRaw]));
            attackEnemy(&enemies[enemieRaw], &characters[characterRaw]);
    
            // Check if the character is defeated
            if (!isCharacterAlive(&characters[characterRaw])) {
                printf("%s has been defeated!\n", getCharacterName(&characters[characterRaw]));
                break;
            }
    
            // Character's turn
            printf("%s's turn!\n", getCharacterName(&characters[characterRaw]));
            attackCharacter(&characters[characterRaw], &enemies[enemieRaw]);
    
            // Check if the enemy is defeated
            if (!isEnemyAlive(&enemies[enemieRaw])) {
                printf("%s has been defeated!\n", getOpponentName(&enemies[enemieRaw]));
                break;
            }
        }
    }



    return 0;
}












