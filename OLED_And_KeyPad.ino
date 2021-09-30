#include <Keypad.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h>

#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'*', '7', '4', '1'},
  {'\n', '8', '5', '2'},
  {'#', '9', '6', '3'},
  {'D', 'C', 'B', 'A'},
};

byte rowPins[ROWS] = {6, 5, 4, 3};
byte colPins[COLS] = {9, 10, 11, 12};

char teks[128];

int posisi = 0;
int posisiSementara = 0;

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
  // initialize OLED display with address 0x3C for 128x64
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  delay(2000);         // wait for initializing
  oled.clearDisplay(); // clear display
  oled.display();
}

void loop() {
  // INPUT FROM KEYPAD
  char tombol = keypad.getKey();

  if (tombol != NO_KEY) {
    int n = sizeof(teks) / sizeof(*teks);
    switch (tombol) {
      case '\n':
        masukanData(teks, tombol, posisi);
        posisi++;
        posisiSementara++;
        break;
      case 'D':
        if (posisi < posisiSementara) {
          posisi++;
        }
        break;
      case '#':
        if (posisi > 0) {
          posisi--;
        }
        break;
      case '*':
        int index;
        if (posisi != 0) {
          for (index = posisi - 1; index < n; index++) {
            int temp = teks[index];
            teks[index] = teks[index + 1];
            teks[index + 1] = temp;
          }
        }
        if (posisi > 0) {
          posisiSementara--;
          posisi--;
        }
        break;
      default:
        masukanData(teks, tombol, posisi);
        posisi++;
        posisiSementara++;
        break;
    }
    // OLED
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(WHITE);
    oled.setCursor(0, 0);
    oled.println(teks);
    oled.display();
  }
}

char* masukanData(char arr[], char x, int pos) {
  pos++;
  int panjang = strlen(arr);
  int index;
  for (index = panjang; index >= pos; index--) {
    arr[index] = arr[index - 1];
  }
  arr[pos - 1] = x;
  return arr;
}
