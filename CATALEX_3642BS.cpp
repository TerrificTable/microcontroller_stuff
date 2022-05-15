#include <SPI.h>
#include <Wire.h>
#include <Arduino.h>
#include <TM1637Display.h>



#define CLK             4
#define DIO             0


// GND ->  G
// VCC -> 5V
// DIO -> D3
// CLK -> D2


//     A
//    ---
// F |   | B
//   | G |
//    ---
// E |   | C
//   |   |
//    ---
//     D
const uint8_t SEG_DONE[]    = {
    SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
    SEG_G | SEG_E | SEG_D | SEG_C,                   // o
    SEG_C | SEG_E | SEG_G,                           // n
    SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
};

TM1637Display segDisplay(CLK, DIO);


void setup() {
    Serial.begin(115200);
    delay(100);

    Serial.println("");
    segDisplay.setBrightness(7);
}

void loop() {
    srand(time(0));
    switch (rand() % 5) {
        case 0:
            segDisplay.showNumberDec(14, false, 2, 1);
            break;
        case 1:
            segDisplay.showNumberDec(-12);
            break;
        case 2:
            segDisplay.showNumberHexEx(0xd1, 0, true);
            break;
        case 3:
            segDisplay.showNumberDec(3, true);
            break;
        case 4:
            segDisplay.showNumberDec(rand() % 9, false);
            break;
        case 5:
            segDisplay.setSegments(SEG_DONE);
            break;
    }

    // dont use `delay()` except you dont want to do anything while delay() isnt over
    delay(60000); // Wait 1min
}
