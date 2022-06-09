#include <Arduino.h>
#include <Adafruit_SSD1306.h>




#define SDA             D2
#define SCL             D1

#define OLED_RESET      0
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println("\nStarting...");
  
    oled_init();
}


void drawBall(int x, int y, int size, uint16_t color, int filled = 1) {
    display.drawCircle(x, y, size, color);
    if (filled)
        display.fillCircle(x, y, size, color);

    display.display();
}



void loop() {

    static int posX         = SCREEN_WIDTH  / 2;
    static int posY         = SCREEN_HEIGHT / 2;
    static int directionX   = 1;
    static int directionY   = 1;
    static int size         = 4;


    // DRAW BOUNCING BALL
    // =================================
    display.clearDisplay();
    drawBall(posX, posY, size, 1);


    if (posX >= (SCREEN_WIDTH  - (size / 2))) directionX = 2;
    else if (posX <= (0 - (size / 2))) directionX = 1;
    if (posY >= (SCREEN_HEIGHT - (size / 2))) directionY = 2;
    else if (posY <= (0 - (size / 2))) directionY = 1;

    // DOWN / LEFT
    if (directionX == 1) posX++;
    if (directionY == 1) posY++;

    // UP / RIGHT
    if (directionX == 2) posX--;
    if (directionY == 2) posY--;


    display.display();
    // =================================

    
}
