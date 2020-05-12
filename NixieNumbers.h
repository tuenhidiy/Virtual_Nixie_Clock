/*
  NixieNumbers.h - ESP32 library for PxMatrix displays and clear Nixie Numbers in small and big format.

*/
#ifndef NixieNumbers_h
#define NixieNumbers_h

#ifndef PxMatrix_h
#include <PxMatrix.h>
#endif

#include  "Nixie_20x31.h"
#include  "Nixie_32x61.h"

#define NUMBER_WIDTH_S      20    // Small Nixie Number Width
#define NUMBER_HEIGHT_S     31    // Small Nixie Number Height

#define NUMBER_WIDTH_B      32    // Big Nixie Number Width
#define NUMBER_HEIGHT_B     61    // Big Nixie Number Height

class NixieNumbers
{
  public:
    NixieNumbers(PxMATRIX* display);
    void drawNumber(int x = 0, int y = 0, const uint16_t image[] = Nixie_20x31_0, int imageWidth = 20, int imageHeight = 31);
    void clearNumber(int x = 0, int y = 0, int imageWidth = 20, int imageHeight = 31);
  private:
    PxMATRIX* _display;
};

#endif
