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
    ioex.direction(TCA6424A::Direction::OUT_ALL);
    ioex.write(TCA6424A::Level::L_ALL);
}

void loop() {
    for (size_t i = 0; i < 24; ++i) {
        Serial.print("set port high: ");
        Serial.println(i);

        ioex.write(static_cast<TCA6424A::Port::Port>(i), TCA6424A::Level::H);
        Serial.println(ioex.read(), BIN);
        delay(500);
    }

    for (size_t i = 0; i < 24; ++i) {
        Serial.print("set port low: ");
        Serial.println(i);

        ioex.write(static_cast<TCA6424A::Port::Port>(i), TCA6424A::Level::L);
        Serial.println(ioex.read(), BIN);
        delay(500);
    }
}
