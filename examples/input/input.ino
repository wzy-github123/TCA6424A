#include <TCA6424A.h>

TCA6424A ioex;

void setup() {
    Serial.begin(115200);
    delay(2000);

    Wire.begin();
    ioex.attach(Wire);
    ioex.polarity(TCA6424A::Polarity::ORIGINAL_ALL);
    ioex.direction(TCA6424A::Direction::IN_ALL);
}

void loop() {
    Serial.println(ioex.read(), BIN);
    delay(1000);
}
