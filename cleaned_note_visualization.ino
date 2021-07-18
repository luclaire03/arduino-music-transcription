#include<string.h>
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h



// SCREEN-SPECIFIC PARAMETERS
int X_LIMIT = 127;
int Y_LIMIT = 159;

// CHANGE THESE TO BE MORE APPROPRIATE FOR YOUR SCREEN
int MARGIN = 5; 
int BTWN_LINES = 10;
int X_RAD = 3;
int Y_RAD = 4;
int DOT_RAD = 1;
int STEM_LENGTH = 25;


// EXAMPLE
char* note = "F4";

int center_x;
int center_y = MARGIN * 2; 
int x_offset = 0;
int x_dot;

char* old_note;
float bpm_in_ms;
float left_over = 0.0;

char* note_type;
bool sharp = false;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); //initialize serial!
  tft.init(); //initialize the screen
  tft.setRotation(2); //set rotation for our layout
  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  // draw two staffs
  draw_staff(x_offset);
  draw_staff(60);
}

void loop() {
  draw_note(note, 1000, 60); //dummy values
}

void draw_note(char* note, int duration, int bpm) {
  if (center_y >= Y_LIMIT - MARGIN) {
    x_offset = BTWN_LINES * 6;
    center_y = MARGIN * 2;
  }
  
  bpm_in_ms = (1000*bpm)/60;
  sharp = false;

  if (note[1] == '#'){
    
    char new_note[3];
    new_note[0] = note[0];
    new_note[1] = note[strlen(note) - 1];
    *(new_note+2) = '\0';
    note = new_note;
    *(note+2) = '\0';
    Serial.println(note);
    sharp = true;
  }

  if (strcmp(note, "C4") == 0) {
    center_x = (X_LIMIT - BTWN_LINES * 6) - x_offset; // 67
    x_dot = X_LIMIT - BTWN_LINES * 6 + Y_RAD;
  } else if (strcmp(note, "D4") == 0) {
    center_x = (X_LIMIT - int(BTWN_LINES * 5.5)) - x_offset; // 72
    x_dot = center_x;
  } else if (strcmp(note, "E4") == 0) {
    center_x = (X_LIMIT - BTWN_LINES * 5) - x_offset; // 77
    x_dot = X_LIMIT - BTWN_LINES * 5 + Y_RAD;
  } else if (strcmp(note, "F4") == 0) {
    center_x = (X_LIMIT - int(BTWN_LINES * 4.5)) - x_offset; // 82
    x_dot = center_x;
  } else if (strcmp(note, "G4") == 0) {
    center_x = (X_LIMIT - BTWN_LINES * 4) - x_offset;
    x_dot = X_LIMIT - BTWN_LINES * 4 + Y_RAD;
  } else if (strcmp(note, "A4") == 0) {
    center_x = (X_LIMIT - int(BTWN_LINES * 3.5)) - x_offset;
    x_dot = center_x;
  } else if (strcmp(note, "B4") == 0) {
    center_x = (X_LIMIT - BTWN_LINES * 3) - x_offset;
    x_dot = X_LIMIT - BTWN_LINES * 3 + Y_RAD;
  } else if (strcmp(note, "C5") == 0) {
    center_x = (X_LIMIT - int(BTWN_LINES * 2.5)) - x_offset;
    x_dot = center_x;
  } else if (strcmp(note, "D5") == 0) {
    center_x = (X_LIMIT - BTWN_LINES * 2) - x_offset;
    x_dot = X_LIMIT - BTWN_LINES * 2 + Y_RAD;
  } else if (strcmp(note, "E5") == 0) {
    center_x = (X_LIMIT - int(BTWN_LINES * 1.5)) - x_offset;
    x_dot = center_x;
  } else if (strcmp(note, "F5") == 0) {
    center_x = (X_LIMIT - BTWN_LINES) - x_offset; // 117
    x_dot = X_LIMIT - BTWN_LINES + Y_RAD;
  }

  Serial.println(center_x);

  left_over = 0.0;


  // determine note type
  if (duration < bpm_in_ms + 100 && duration > bpm_in_ms - 100) {
    note_type = "quarter";
  } else if (duration < (bpm_in_ms * 0.5 + 100) && duration > (bpm_in_ms * 0.5 - 100)) {
    note_type = "eighth";
  } else if (duration < (bpm_in_ms * 1.5 + 100) && duration > (bpm_in_ms * 1.5 - 100)) {
    note_type = "dotted quarter";
  } else if (duration < bpm_in_ms * 2 + 100 && duration > bpm_in_ms * 2 - 100) {
    note_type = "half";
  } else if (duration > bpm_in_ms * 2 + 100 && duration < bpm_in_ms * 3 - 100) {
    note_type = "half";
    left_over = duration - (bpm_in_ms * 2);
  } else if (duration < bpm_in_ms * 3 + 100 && duration > bpm_in_ms * 3 - 100) {
    note_type = "dotted half";
  } else if (duration > bpm_in_ms * 3 + 100 && duration < bpm_in_ms * 4 - 100) {
    note_type = "dotted half";
    left_over = duration - (bpm_in_ms * 3);
  } else if (duration < bpm_in_ms * 4 + 100 && duration > bpm_in_ms * 4 - 100) {
    note_type = "whole";
  } else if (duration > bpm_in_ms * 4 + 100) {
    note_type = "sustained";
    left_over = duration - (bpm_in_ms * 4);
  }

  Serial.println(note_type);

  if (sharp) {
    draw_sharp(center_x, center_y);
    center_y += int(Y_RAD * 2.5);
  }

  // QUARTER NOTE OR EIGHTH NOTE: DURATION SHOULD BE EQUAL TO BPM (PENDING UNIT CONVERSION)
  if (strcmp(note_type, "quarter") == 0 || strcmp(note_type, "eighth") == 0 || strcmp(note_type, "eighth") == 0 || strcmp(note_type, "dotted quarter") == 0) {
    tft.fillEllipse(center_x, center_y, X_RAD, Y_RAD, TFT_GREEN);
  } else { // HALF NOTE OR WHOLE NOTE
    tft.drawEllipse(center_x, center_y, X_RAD, Y_RAD, TFT_GREEN);
  }

  Serial.println("body drawn");

  if (strcmp(note_type, "whole") != 0 && strcmp(note_type, "sustained") != 0) { // ANYTHING BUT WHOLE - NEEDS A STEM
    if (atoi(&note[1]) <= 4) {
      tft.drawLine(center_x, center_y + Y_RAD, center_x + STEM_LENGTH, center_y + Y_RAD, TFT_GREEN);
      // EIGHTH NOTE: ADD LITTLE FLAG
      if (strcmp(note_type, "eighth") == 0) {
        tft.drawLine(center_x + STEM_LENGTH, center_y + Y_RAD, center_x + int(STEM_LENGTH * 0.8), center_y + int(Y_RAD * 2.5), TFT_GREEN);
      }
    } else {
      tft.drawLine(center_x, center_y - Y_RAD, center_x - int(STEM_LENGTH * 4.5), center_y - Y_RAD, TFT_GREEN);
      // EIGHTH NOTE: ADD LITTLE FLAG
      if (strcmp(note_type, "eighth") == 0) {
        tft.drawLine(center_x - STEM_LENGTH, center_y - Y_RAD, center_x - int(STEM_LENGTH * 0.8), center_y + int(Y_RAD * 0.5), TFT_GREEN);
      }
    }
    if (strcmp(note_type, "dotted quarter") == 0 || strcmp(note_type, "dotted half") == 0) {
      tft.fillCircle(x_dot, center_y + (Y_RAD * 2), DOT_RAD, TFT_GREEN);
      center_y += Y_RAD;
    }
  }

  center_y += (Y_RAD * 4);
  if (left_over != 0.0) {
    draw_note(note, left_over, bpm); 
  }
}

void draw_staff(int x_offset) {
    tft.drawLine((X_LIMIT - BTWN_LINES) - x_offset, MARGIN, (X_LIMIT - BTWN_LINES) - x_offset, Y_LIMIT - MARGIN, TFT_GREEN);
    tft.drawLine((X_LIMIT - BTWN_LINES * 2) - x_offset, MARGIN, (X_LIMIT - BTWN_LINES * 2) - x_offset, Y_LIMIT - MARGIN, TFT_GREEN);
    tft.drawLine((X_LIMIT - BTWN_LINES * 3) - x_offset, MARGIN, (X_LIMIT - BTWN_LINES * 3) - x_offset, Y_LIMIT - MARGIN, TFT_GREEN);
    tft.drawLine((X_LIMIT - BTWN_LINES * 4) - x_offset, MARGIN, (X_LIMIT - BTWN_LINES * 4) - x_offset, Y_LIMIT - MARGIN, TFT_GREEN);
    tft.drawLine((X_LIMIT - BTWN_LINES * 5) - x_offset, MARGIN, (X_LIMIT - BTWN_LINES * 5) - x_offset, Y_LIMIT - MARGIN, TFT_GREEN);
}

// change as necessary, keep these ratios
void draw_sharp(int center_x, int center_y) {
  tft.drawLine(center_x + 1, center_y - 4, center_x + 2, center_y + 4, TFT_GREEN);
  tft.drawLine(center_x - 2, center_y - 4, center_x - 1, center_y + 4, TFT_GREEN);
  tft.drawLine(center_x - 4, center_y - 2, center_x + 4, center_y - 1, TFT_GREEN);
  tft.drawLine(center_x - 4, center_y + 1, center_x + 4, center_y + 2, TFT_GREEN);
}
