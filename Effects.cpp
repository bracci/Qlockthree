/**
  Effects.cpp
  Klasse für diverse Effekte

  @mc       Arduino/UNO
  @autor    Manuel Bracher / manuel.bracher@gmail.com
  @version  1.0
  @created  02.01.15

  Versionshistorie:
  V 1.0:  - Erstellt.
*/

#include "Effects.h"
#include "Staben.h"

// #define DEBUG
#include "Debug.h"

void Effects::showTickerString(const char* str2disp, byte tickerSpeed, eColors color) {
  word matrix [16];

  byte strLength = strlen(str2disp);
  unsigned int bufLen;
  char actChar;
  char lastChar;
  byte offsetV = 2;
  bool finish = false;
  unsigned int i = 0;

  while (!finish) {
    renderer.clearScreenBuffer(matrix);
    lastChar = 'W';
    unsigned int shift = 0; // Schiebekorrektur aufgrund variierender Buchstabenbreite
    for (byte k = 0; k < strLength; k++) {
      actChar = str2disp[k];
      if (actChar == ' ') {
        shift += 3;  //bei einem Space eine Lücke von:
      }
      else {
        shift -= pgm_read_byte_near(&(stabenBig[lastChar - '!'][7]));
        for (byte j = 0; j < 7; j++) {
          matrix[offsetV + j] |= (pgm_read_byte_near(&(stabenBig[actChar - '!'][j])) << (1 - shift + i)) & 0b1111111111100000;
        }
        if (k < (strLength - 1)) {
          shift += 6; // Max. Buchstabenbreite + ein Pixel Lücke
        }
        lastChar = actChar;
      }
    }
    writeToBuffer(matrix, 3 * (10 - tickerSpeed), color);
    bufLen = shift + 15;
    if (i == bufLen) {
      finish = true;
    }
    else {
      i++;
    }
  }
}

/**
   Intro
*/
void Effects::showIntro(eColors color) {
  word matrix [16];

  renderer.clearScreenBuffer(matrix);
  for (int j = 0; j < 11; j++) {
    for (byte i = 0; i < 10; i++) {
      matrix[i] |= 0b1 << (15 - j);
    }
    writeToBuffer(matrix, 5, color);
  }
  for (int j = 0; j < 11; j++) {
    for (int i = 0; i < 10; i++) {
      matrix[i] ^= 0b1 << (5 + j);
    }
    writeToBuffer(matrix, 5, color);
  }
  renderer.clearScreenBuffer(matrix);
  for (int i = 9; i >= 0; i--) {
    matrix[i] |= 0b1111111111100000;
    writeToBuffer(matrix, 5, color);
  }
  for (int i = 0; i < 10; i++) {
    matrix[i] ^= 0b1111111111100000;
    writeToBuffer(matrix, 5, color);
  }
}

/**
   Pulsierender Herz-Effekt
*/
void Effects::showHeart(byte duration, eColors color) {
  word matrix [16];
  for (byte y = 0; y < 3; y++) {
    renderer.clearScreenBuffer(matrix);
    for (byte j = 0; j < 8; j++) {
      matrix[1 + j] |= (pgm_read_word_near(&(effectMasksHeart[0][j])) << 5);
    }
    writeToBuffer(matrix, 11 * duration, color);
    for (byte i = 0; i < 2; i++) {
      renderer.clearScreenBuffer(matrix);
      for (byte z = 0; z < 2; z++) {
        for (byte j = 0; j < 8; j++) {
          matrix[1 + j] |= (pgm_read_word_near(&(effectMasksHeart[z][j])) << 5);
        }
        writeToBuffer(matrix, 4 * duration, color);
      }
    }
  }
  renderer.clearScreenBuffer(matrix);
  for (byte j = 0; j < 8; j++) {
    matrix[1 + j] |= (pgm_read_word_near(&(effectMasksHeart[0][j])) << 5);
  }
  writeToBuffer(matrix, 14 * duration, color);
}

/**
   Feuerwerk-Effekt
*/
void Effects::showFireWork(byte posX, eColors color) {
  word matrix [16];

  for (byte i = 9; i >= 3; i--) {
    renderer.clearScreenBuffer(matrix);
    ledDriver.setPixelInScreenBuffer(posX, i, matrix);
    writeToBuffer(matrix, 7, color);
  }

  for (byte i = 0; i <= 2; i++) {
    renderer.clearScreenBuffer(matrix);
    for (byte j = 0; j < 10; j++) {
      matrix[j] |= (pgm_read_word_near(&(effectMasksFireWork[i][j])) << (10 - posX)) & 0b1111111111100000;
    }
    writeToBuffer(matrix, 3 + round(10 * i / 3), color);
  }
  for (byte i = 0; i <= 10; i++) {
    renderer.clearScreenBuffer(matrix);
    for (byte j = 0; j < 10 - i; j++) {
      matrix[j + i] |= (pgm_read_word_near(&(effectMasksFireWork[3 + i % 3][j])) << (10 - posX)) & 0b1111111111100000;
    }
    writeToBuffer(matrix, 20, color);
  }
}

/**
   Kerzen-Effekt
*/
void Effects::showCandle(eColors color) {
  word matrix [16];
  for (byte k = 0; k < 5; k++) {
    for (int j = -4; j < 4; j++) {
      renderer.clearScreenBuffer(matrix);
      for (byte i = 5; i < 10; i++) {
        matrix[i] |= (pgm_read_word_near(&(effectMasksCandle[5][i])) << 5);
      }
      for (byte i = 0; i < 5; i++) {
        matrix[i] |= (pgm_read_word_near(&(effectMasksCandle[4 - abs(j % 4)][i])) << 5);
      }
      writeToBuffer(matrix, 10, color);
    }
  }
}

/**
   Bitmap
*/
void Effects::showBitmap(byte bitmapIdx, byte duration, eColors color) {
  word matrix [16];
  renderer.clearScreenBuffer(matrix);
  for (byte i = 0; i < 10; i++) {
    for (byte j = 0; j < 11; j++) {
      matrix[i] |= ((pgm_read_word_near(&(bitmaps[bitmapIdx - BITMAP_MIN][j])) >> i) & 0x0001) << (15 - j);
    }
  }
  writeToBuffer(matrix, 15 * duration, color);
}

/**
   Bitmap-Effekt
*/
void Effects::showAnimatedBitmap(byte animatedBitmap, byte duration, eColors color) {
  switch (animatedBitmap) {
    case ANI_BITMAP_CHAMPGLASS:
      for (byte i = 0; i < 6; i++) {
        showBitmap(BITMAP_CHAMPGLASS1 + i % 2, duration, color);
      }
      break;
    case ANI_BITMAP_CHRISTTREE:
      for (byte i = 0; i < 4; i++) {
        showBitmap(BITMAP_CHRISTTREE1 + i % 2, duration, color);
      }
      break;
    case ANI_BITMAP_SMILEY_WINK:
      showBitmap(BITMAP_SMILEY, 2 * duration, color);
      showBitmap(BITMAP_SMILEY_WINK, duration, color);
      showBitmap(BITMAP_SMILEY, duration, color);
      break;
    default:
      ;
  }
}

void Effects::writeToBuffer(word aMatrix[], unsigned int aDuration, eColors color)
{
#if defined(RGB_LEDS) || defined(RGBW_LEDS)
  ledDriver.writeScreenBufferToMatrix(aMatrix, true, color);
  delay(aDuration * RGB_SPEED_CORRECTION);
#else
  for (int i = 0; i < aDuration; i++) {
    ledDriver.writeScreenBufferToMatrix(aMatrix, true, color);
  }
#endif
}


