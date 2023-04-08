#include <Arduino.h>
#include <LiquidCrystal.h>
#include <TimerMs.h>

#define BUTTON_JUMP 7

LiquidCrystal lcd(13, 12, 11, 10,  9,  8);

byte word_s[8] = { B00000, B00000, B00000, B01110, B01000, B01110, B00010, B01110 };
byte word_t[8] = { B00000, B00000, B00000, B01110, B00100, B00100, B00100, B00100 };
byte word_a[8] = { B00000, B00000, B00000, B01110, B01010, B01010, B01110, B01010 };
byte word_r[8] = { B00000, B00000, B00000, B01110, B01010, B01110, B01100, B01010 };
byte word_c[8] = { B00000, B00000, B00000, B01110, B01000, B01000, B01000, B01110 };
byte word_o[8] = { B00000, B00000, B00000, B01110, B01010, B01010, B01010, B01110 };
byte word_e[8] = { B00000, B00000, B00000, B01110, B01000, B01110, B01000, B01110 };
byte game_dino[8] = { B00111, B00111, B10010, B10111, B11111, B11110, B01010, B01111 };
byte game_cactus[8] = { B00100, B00101, B10101, B10101, B10101, B10111, B11100, B00100 };

TimerMs update_timer(500, true);

int jump_time = 0;
int score = 0;
int location[16] = {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

bool jump_check = false;
bool game_starting = false;


void game_menu() {
  if (update_timer.tick()) {
    lcd.clear();

    int message_start[] = { 1, 2, 3, 4, 2 };

    lcd.createChar(1, word_s);
    lcd.createChar(2, word_t);
    lcd.createChar(3, word_a);
    lcd.createChar(4, word_r);

    lcd.setCursor(5, 0);

    for (int i = 0; i < sizeof(message_start)/sizeof(message_start[0]); i++) {
      lcd.write(message_start[i]);
    }
  }
}

void game_over() {
  if (update_timer.tick()) {
    lcd.clear();

    int message_score[] = { 1, 2, 3, 4, 5 };

    lcd.createChar(1, word_s);
    lcd.createChar(2, word_c);
    lcd.createChar(3, word_o);
    lcd.createChar(4, word_r);
    lcd.createChar(5, word_e);

    lcd.setCursor(5, 0);

    for (int i = 0; i < sizeof(message_score)/sizeof(message_score[0]); i++) {
      lcd.write(message_score[i]);
    }

    lcd.setCursor(0, 1);
    lcd.print(score);
  }
}

void initialize_game() {
  if (update_timer.tick()) {
    lcd.clear();

    lcd.createChar(1, game_dino);
    lcd.createChar(2, game_cactus);

    // cactus generation
    int check = rand() % 2;
    if (check == 1 && location[12] != 2) {
      location[15] = 2;
    }

    // updating array
    for (int i = 0; i < sizeof(location)/sizeof(location[0]); i++) {
      if (location[i] == 2) {
        if (location[i-1] != 1) location[i-1] = 2;
        else game_starting = false;
        location[i] = 0;
      }
    }

    // update display
    for (int i = 0; i < sizeof(location)/sizeof(location[0]); i++) {
      lcd.setCursor(i, 1);
      if (location[i] == 2) lcd.write(2);
    }

    // handling dino jump
    if (jump_check == false) {
      lcd.setCursor(3, 1);
      lcd.write(1);
    }
    else if (jump_time < 3) {
      lcd.setCursor(3, 0);
      lcd.write(1);
      jump_time++;
    }
    else {
      if (location[3] == 2) game_starting = false;
      location[3] = 1;
      lcd.setCursor(3, 1);
      lcd.write(1);
      jump_time = 0;
      jump_check = false;
    }
    
    score++;
  }
}

void game_start() {
  lcd.clear();
  for (int i = 0; i < sizeof(location)/sizeof(location[0]); i++) location[i] = 0;
  location[3] = 1;
  score = 0;
  game_starting = true;
  initialize_game();
  delay(1000);
}

void dino_jump() {
  if (jump_time == 0) {
    jump_check = true;
    location[3] = 0;
  }
}


void setup() {
  lcd.begin(16, 2);
  pinMode(BUTTON_JUMP, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  boolean check_jump = digitalRead(BUTTON_JUMP);

  if (game_starting == false && score == 0) game_menu();
  if (game_starting == false && score != 0) game_over();

  if (game_starting == true) initialize_game();

  if (!check_jump && game_starting == false) game_start();
  if (!check_jump && game_starting == true) dino_jump();
}

