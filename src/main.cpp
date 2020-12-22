#include <Arduino.h>
#include "Motor.h"
#include <BluetoothSerial.h>

BluetoothSerial bt;

Motor *motor;

void setup()
{
    Serial.begin(115200);

    bt.begin("IPHONE IS DEAD");
    bt.setPin("3228");

    motor = new Motor(13, 0);
}

unsigned long prevMillis = 0, nowMillis = 0;
int maxValue = 0;

int commandIndex = 0;
char command[32] = {};

void loop()
{
    // КОД ДЛЯ ГИТАРЫ
    /*int value = analogRead(33);
    maxValue = max(maxValue, value);

    nowMillis = millis();

    if (nowMillis - prevMillis >= 25) {
        analogWrite(13, maxValue * 2);
        maxValue = 0;
        prevMillis = nowMillis;
    }*/
    // УРА!!!!!!!!!апжыоилвкпаршгжщывфщшжрпшщрыавщшзнпрзщшгнйкызщшнрпзщшнйыкршщпшрщозфывкзшщрпзшщроыкфпшрщззшыщкрфпзшрщкыуф

    while (bt.available())
    {
        char c = bt.read();
        command[commandIndex] = c;
        commandIndex++;
        //Serial.print(c);
        if (c == '\0' || commandIndex == 32)
        {
            if (command[0] == 'P')
            {
                int percent = 0;
                for (int i = 1; i < commandIndex - 1; i++)
                {
                    char d = command[i];
                    if (d >= '0' && d <= '9')
                    {
                        percent *= 10;
                        percent += (int)(d - '0');
                    }
                }
                motor->writePercents(percent);
            }
            else if (command[0] == 'N')
            {
                int octave = 0;
                char d = command[2];
                if (d >= '0' && d <= '9')
                {
                    octave = (int)(d - '0');
                }
                note_t note;
                char noteChar = command[1];
                switch (noteChar)
                {
                case 'A':
                    note = NOTE_A;
                    break;
                case 'B':
                    note = NOTE_B;
                    break;
                case 'C':
                    note = NOTE_C;
                    break;
                case 'D':
                    note = NOTE_D;
                    break;
                case 'E':
                    note = NOTE_E;
                    break;
                case 'F':
                    note = NOTE_F;
                    break;
                case 'G':
                    note = NOTE_G;
                    break;
                default:
                    return;
                }
                
                if (command[3] == '+')
                    note = (note_t)(note + 1);

                motor->note(note, octave);
            }
            commandIndex = 0;
        }
    }
    motor->update();
}