/*
  NixieNumbers.cpp - ESP32 library for PxMatrix displays and clear Nixie Numbers in small and big format.

*/

#include "NixieNumbers.h"

NixieNumbers::NixieNumbers(PxMATRIX* display) {
  _display = display;
}

void NixieNumbers::drawNumber(int x, int y, const uint16_t image[], int imageWidth, int imageHeight) {

  uint16_t * buffer = new uint16_t[imageWidth* imageHeight * 2];
  memcpy_P(buffer, image, imageWidth * imageHeight * 2);

  int counter = 0;
  for (int yy = 0; yy < imageHeight; yy++)
  {
    for (int xx = 0; xx < imageWidth; xx++)
    {
      _display->drawPixel(xx + x , yy + y, buffer[counter]);
      counter++;
    }
  }
  free(buffer);
}

void NixieNumbers::clearNumber(int x, int y, int imageWidth, int imageHeight) {

  uint16_t * buffer = new uint16_t[imageWidth * imageHeight * 2];
  //memcpy_P(buffer, image, NUMBER_HEIGHT*NUMBER_WIDTH*2);
  memset(buffer, 0, sizeof(buffer[0]) * imageWidth * imageHeight * 2);

  int counter = 0;
  for (int yy = 0; yy < imageHeight; yy++)
  {
    for (int xx = 0; xx < imageWidth; xx++)
    {
      _display->drawPixel(xx + x , yy + y, buffer[counter]);
      counter++;
    }
  }
  free(buffer);
}
