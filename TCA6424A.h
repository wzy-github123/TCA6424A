#pragma once
#ifndef ARDUINO_TCA6424A_H
#define ARDUINO_TCA6424A_H

#include <Arduino.h>
#include <Wire.h>

namespace TCA6424A {

namespace Reg {
    enum : uint8_t {
        INPUT_PORT_0 = 0x00,
        INPUT_PORT_1,
        INPUT_PORT_2,
        OUTPUT_PORT_0 = 0x04,
        OUTPUT_PORT_1,
        OUTPUT_PORT_2,
        POLARITY_INVERSION_PORT_0 = 0x08,
        POLARITY_INVERSION_PORT_1,
        POLARITY_INVERSION_PORT_2,
        CONFIGURATION_PORT_0 = 0x0C,
        CONFIGURATION_PORT_1,
        CONFIGURATION_PORT_2,
    };
}

namespace Port {
    enum Port : uint8_t {
        P00,
        P01,
        P02,
        P03,
        P04,
        P05,
        P06,
        P07,
        P10,
        P11,
        P12,
        P13,
        P14,
        P15,
        P16,
        P17,
        P20,
        P21,
        P22,
        P23,
        P24,
        P25,
        P26,
        P27,
    };
}  // namespace Port

namespace Level {
    enum Level : uint8_t { L, H };
    enum LevelAll : uint32_t { L_ALL = 0x00000000, H_ALL = 0x00FFFFFF };
}  // namespace Level

namespace Polarity {
    enum Polarity : uint8_t { ORIGINAL, INVERTED };
    enum PolarityAll : uint32_t { ORIGINAL_ALL = 0x00000000, INVERTED_ALL = 0x00FFFFFF };
}  // namespace Polarity

namespace Direction {
    enum Direction : uint8_t { OUT, IN };
    enum DirectionAll : uint32_t { OUT_ALL = 0x00000000, IN_ALL = 0x00FFFFFF };
}  // namespace Direction

template <typename WireType = TwoWire>
class TCA6424A {
    union Ports {
        uint32_t w;
        uint8_t b[3];
    };
    static constexpr uint8_t TCA6424_ADDRESS_GND = 0x22;
    static constexpr uint8_t TCA6424_ADDRESS_VCC = 0x23;
    static constexpr uint8_t BASE_I2C_ADDR = TCA6424_ADDRESS_GND;

    WireType* wire {nullptr};
    uint8_t addr {BASE_I2C_ADDR};
    Ports input {0x000000};
    Ports output {0xFFFFFF};
    Ports pol {0x000000};
    Ports dir {0xFFFFFF};
    uint8_t status {0x00};
    uint8_t auto_increment {0x80};

public:
    void attach(WireType& wire, uint8_t i2c_addr = BASE_I2C_ADDR) {
        this->wire = &wire;
        this->addr = i2c_addr;
    }

    uint32_t read() {
        read_bytes(this->addr, Reg::INPUT_PORT_0 | auto_increment, this->input.b, 3);
        return this->input.w;
    }
    Level::Level read(const Port::Port port) {
        uint32_t v = read();
        return (v & (1 << port)) ? Level::H : Level::L;
    }

    bool write(const uint32_t value) {
        this->output.w = value;
        return write_impl();
    }
    bool write(const Port::Port port, const Level::Level level) {
        if (level == Level::H) {
            this->output.w |= (1 << port);
        } else {
            this->output.w &= ~(1 << port);
        }
        return write_impl();
    }

    bool polarity(const uint16_t value) {
        this->pol.w = value;
        return polarity_impl();
    }
    bool polarity(const Port::Port port, const Polarity::Polarity pol) {
        if (pol == Polarity::INVERTED) {
            this->pol.w |= (1 << port);
        } else {
            this->pol.w &= ~(1 << port);
        }
        return polarity_impl();
    }

    bool direction(const uint16_t value) {
        this->dir.w = value;
        return direction_impl();
    }

    bool direction(const Port::Port port, const Direction::Direction dir) {
        if (dir == Direction::IN) {
            this->dir.w |= (1 << port);
        } else {
            this->dir.w &= ~(1 << port);
        }
        return direction_impl();
    }

    uint8_t i2c_error() const {
        return status;
    }

private:
    bool write_impl() {
        return write_bytes(this->addr, Reg::OUTPUT_PORT_0 | auto_increment, this->output.b, 3);
    }

    bool polarity_impl() {
        return write_bytes(this->addr, Reg::POLARITY_INVERSION_PORT_0 | auto_increment, this->pol.b, 3);
    }

    bool direction_impl() {
        return write_bytes(this->addr, Reg::CONFIGURATION_PORT_0 | auto_increment, this->dir.b, 3);
    }

    int8_t read_bytes(const uint8_t dev, const uint8_t reg, uint8_t* data, const uint8_t size) {
        wire->beginTransmission(dev);
        wire->write(reg);
        wire->endTransmission();
        wire->requestFrom(dev, size);
        int8_t count = 0;
        while (wire->available()) data[count++] = wire->read();
        return count;
    }

    bool write_bytes(const uint8_t dev, const uint8_t reg, const uint8_t* data, const uint8_t size) {
        wire->beginTransmission(dev);
        wire->write(reg);
        for (uint8_t i = 0; i < size; i++) wire->write(data[i]);
        status = wire->endTransmission();
        return (status == 0);
    }
};

}  // namespace TCA6424A

using TCA6424A = TCA6424A::TCA6424A<>;

#endif // ARDUINO_TCA6424A_H
