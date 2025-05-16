#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "character-manager.h"

#define MAX_CHARACTERS 100

const char *PLAYERS_FILE_PATH = "../game_data/players.csv";
const char *ENEMIES_FILE_PATH = "../game_data/enemies.csv";
const char *SPELLS_FILE_PATH = "../game_data/spells.csv";

// Function to get POST data
void get_post_data(char *post_data) {
    char *content_length = getenv("CONTENT_LENGTH");
    if (content_length != NULL) {
        int length = atoi(content_length);
        fread(post_data, 1, length, stdin);
        post_data[length] = '\0';
    }
}

int main() {
    // Print HTML header
    print_html_header();
    
    // Load game data
    CharacterStruct characters[MAX_CHARACTERS];
    EnemiesStruct enemies[MAX_CHARACTERS];
    SpellsStruct spells[MAX_CHARACTERS];
    
    int characters_count = read_csv_generic(PLAYERS_FILE_PATH, characters, MAX_CHARACTERS, sizeof(CharacterStruct), parse_character);
    int enemy_count = read_csv_generic(ENEMIES_FILE_PATH, enemies, MAX_CHARACTERS, sizeof(EnemiesStruct), parse_enemy);
    int spell_count = read_csv_generic(SPELLS_FILE_PATH, spells, MAX_CHARACTERS, sizeof(SpellsStruct), parse_spell);

    // Check for POST data (form submission)
    char post_data[1024] = {0};
    get_post_data(post_data);
    
    if (strlen(post_data) > 0) {
        // Parse form data
        int character_id = -1, enemy_id = -1;
        char choice[20] = {0};
        
        // Simple form parsing (for demo - use a proper library in production)
        char *token = strtok(post_data, "&");
        while (token != NULL) {
            if (strstr(token, "character_id=")) {
                character_id = atoi(token + 13);
            } else if (strstr(token, "enemy_id=")) {
                enemy_id = atoi(token + 9);
            } else if (strstr(token, "choice=")) {
                strcpy(choice, token + 7);
            }
            token = strtok(NULL, "&");
        }
        
        // Handle battle
        if (character_id >= 0 && enemy_id >= 0) {
            FightLoop(&characters[character_id], &enemies[enemy_id], spells, spell_count);
            print_html_footer();
            return 0;
        }
    }
    
    // No POST data - show character selection
    printf("<h1>Choose Your Character</h1>\n");
    printf("<form method='post' action='game.cgi'>\n");
    printf("<select name='character_id'>\n");
    
    for (int i = 0; i < characters_count; i++) {
        printf("<option value='%d'>%s</option>\n", i, getCharacterName(&characters[i]));
    }
    
    printf("</select>\n");
    
    // Random enemy selection
    int enemy_id = RandomNumb(enemy_count - 1);
    printf("<input type='hidden' name='enemy_id' value='%d'>\n", enemy_id);
    
    printf("<input type='submit' value='Start Battle'>\n");
    printf("</form>\n");
    
    print_html_footer();
    return 0;
}
