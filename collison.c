#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "app.h"
#include "player.h"
#include "game.h"
#include "snake.h"

bool collison_detection(Snake *snake) {
    // Checks collison with walls
    if (snake->head->pos.x <= 0 || snake->head->pos.y <= 0 || snake->head->pos.x >= WINDOW_WIDTH - CELL_SIZE || snake->head->pos.y >= WINDOW_HEIGHT - CELL_SIZE) {
        // Collison detected
        return true; 
    }
    // Checks collison with snake body
    for (int i = 0; i < snake->body_length; i++) {
        if (snake->head->pos.x == snake->body[i]->pos.x && snake->head->pos.y == snake->body[i]->pos.y) {
            // Collison detected
        return true; 
        }
    }
    // Checks collison with snake tail
    if (snake->head->pos.x == snake->tail->pos.x && snake->head->pos.y == snake->tail->pos.y) {
        // Collison detected
        return true; 
    }
    // No collison detected
    return false;
}