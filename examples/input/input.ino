#include <TCA6424A.h>

TCA6424 ioex;

#define TCA6424A_SDA  17
#define TCA6424A_SCL  18

void setup() {
    Serial.begin(115200);
    delay(2000);

    Wire.begin(TCA6424A_SDA, TCA6424A_SCL);
    ioex.attach(Wire);
    ioex.polarity(TCA6424A::Polarity::ORIGINAL_ALL);
    ioex.direction(TCA6424A::Direction::IN_ALL);
}

void loop() {
    Serial.println(ioex.read(), BIN);
    delay(1000);
}
