#pragma once

#include <Arduino.h>
#include <esp32-hal-ledc.h>

enum MotorState {
    DEFAULT_MODE,
    PIANO_MODE,
    GUITAR_MODE
};

class Motor {
private:
    unsigned long notePressMillis = 0;
    uint8_t pin, channel;
    MotorState state = MotorState::DEFAULT_MODE;
    
    void write_raw(int value) {
        ledcWrite(channel, value);
    }

public:
    Motor(uint8_t pin, uint8_t channel) : pin(pin), channel(channel)
    {
        ledcSetup(channel, 5000.0, 12);
        ledcAttachPin(pin, channel);
    }

    void note(note_t note, uint8_t octave)
    {
        state = MotorState::PIANO_MODE;
        notePressMillis = millis();
        ledcWriteNote(channel, note, octave);
    }

    void update()
    {
        if (state == MotorState::PIANO_MODE)
        {
            unsigned long now = millis();
            if (now - notePressMillis >= 500) {
                write_raw(0);
            }
        }
    }

    void write(int value) 
    {
        if (state != MotorState::DEFAULT_MODE)
        {
            setFrequency(5000.0);
            state = MotorState::DEFAULT_MODE;
        }

        ledcWrite(channel, value);
    }

    void writePercents(int percent)
    {
        write((percent*4095)/100);
    }

    void setFrequency(double freq)
    {
        ledcSetup(channel, freq, 12);
    }
};