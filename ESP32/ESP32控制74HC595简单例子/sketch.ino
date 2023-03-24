
#define latchPin 4  //Pin connected to ST_CP(pin 12) of 74HC595 //ST
#define clockPin 2 //Pin connected to SH_CP(pin 11) of 74HC595  //这个pin居然可以没有 SH
#define dataPin 15  //Pin connected to DS(pin 14) of 74HC595

#define dataCtr 5

void SendTo74HC595(uint8_t bits);

int r = 8;
unsigned int sequence[24] = {0xFFFF,0x0000,0x00FF,0x0000,0xFF00,0x0000,0xF0F0,0x0000,
                            0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,
                            0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000,};

unsigned int sequence2[12] = {0xFF000000,0x00FF0000,0x0000FF00,0x000000FF,
                             0xF0000000,0x0F000000,0x00F00000,0x000F0000,
                             0x0000F000,0x00000F00,0x000000F0,0x0000000F,};
 
void setup() {
  Serial.begin(115200);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(dataCtr, OUTPUT);
}

uint64_t cur = 0x000000;
void loop() {
  SendTo74HC595(0x10);
  for(int c = 0; c < 12; c++){
    cur = sequence2[c];
    Serial.println("print value is");
    digitalWrite(latchPin, LOW);
    for (int c = 0; c < 32; c++) {
      digitalWrite(clockPin, LOW);
      Serial.print(cur);
      if (cur & 0x0001)digitalWrite(dataPin, HIGH);
      else digitalWrite(dataPin, LOW);
      digitalWrite(clockPin, HIGH);
      cur >>= 1;
    }
    digitalWrite(latchPin, HIGH);
    delay(800); 
  }
  delay(2000);
}

void SendTo74HC595(uint8_t bits)
{
   Serial.println("print value ctroler");
    uint8_t val = bits;
    digitalWrite(latchPin, LOW);
    for (int c = 0; c < 8; c++) {
      digitalWrite(clockPin, LOW);
      if (val & 0x01)digitalWrite(dataCtr, HIGH);
      else digitalWrite(dataCtr, LOW);
      digitalWrite(clockPin, HIGH);
      val >>= 1;
    }
    digitalWrite(latchPin, HIGH);
    delay(800); 
}