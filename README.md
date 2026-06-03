# TCA6424A

Arduino library for [TCA6424A](https://www.ti.com/product/TCA6424A), a 24-bit I2C GPIO expander with configurable direction, output state, and polarity inversion.

## Features

- Supports all 24 GPIOs across port groups P0, P1, and P2
- Read all inputs at once or read a single pin
- Write all outputs at once or control a single pin
- Configure polarity inversion and pin direction per pin or for all pins
- Uses `TwoWire` via `attach()` and supports custom I2C addresses

By default the library uses I2C address `0x22` (`A0 = GND`). You can also pass `0x23` (`A0 = VCC`) to `attach()`.

## Usage

Pins can be addressed with the enum values such as `TCA6424A::Port::P00` through `TCA6424A::Port::P27`.

### Input Example

```cpp
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
```

### Output Example

```cpp
#include <TCA6424A.h>

TCA6424A ioex;

void setup() {
    Serial.begin(115200);
    delay(2000);

    Wire.begin();
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
```

## API

```cpp
void attach(WireType& wire, uint8_t i2c_addr = BASE_I2C_ADDR);
uint32_t read();
Level::Level read(const Port::Port port);
bool write(uint32_t value);
bool write(Port::Port port, Level::Level level);
bool polarity(uint32_t value);
bool polarity(Port::Port port, Polarity::Polarity pol);
bool direction(uint32_t value);
bool direction(Port::Port port, Direction::Direction dir);
uint8_t i2c_error() const;
```

## Enums

```cpp
namespace Port {
    enum Port : uint8_t {
        P00, P01, P02, P03, P04, P05, P06, P07,
        P10, P11, P12, P13, P14, P15, P16, P17,
        P20, P21, P22, P23, P24, P25, P26, P27,
    };
}

namespace Level {
    enum Level : uint8_t { L, H };
    enum LevelAll : uint32_t { L_ALL = 0x00000000, H_ALL = 0x00FFFFFF };
}

namespace Polarity {
    enum Polarity : uint8_t { ORIGINAL, INVERTED };
    enum PolarityAll : uint32_t { ORIGINAL_ALL = 0x00000000, INVERTED_ALL = 0x00FFFFFF };
}

namespace Direction {
    enum Direction : uint8_t { OUT, IN };
    enum DirectionAll : uint32_t { OUT_ALL = 0x00000000, IN_ALL = 0x00FFFFFF };
}
```

## Metadata

- Author: Wenzy
- Repository: https://github.com/wzy-github123/TCA6424A

## License

MIT
