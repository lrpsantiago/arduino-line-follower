#include <EEPROM.h>
#include "MotorPair.h"
#include "LdrSensor.h"
#include "PidController.h"
#include "Led.h"

#define SENSORS_COUNT 5
#define SENSOR_RIGHT_END A0
#define SENSOR_RIGHT A1
#define SENSOR_CENTER A2
#define SENSOR_LEFT A3
#define SENSOR_LEFT_END A4

#define S_INDEX_LE 0
#define S_INDEX_L 1
#define S_INDEX_C 2
#define S_INDEX_R 3
#define S_INDEX_RE 4

#define BUZZER_PIN 13
#define CALIBRATING 0
#define FOLLOWING_LINE 1
#define TURNING_L 2
#define TURNING_R 3
#define ROTATING_L 5
#define ROTATING_R 6

Led* leds[5];
PidController* pidControl;
MotorPair* motors;
int lastFrameTime;
float stateTime = 5;
byte state = CALIBRATING;
byte sensorPins[SENSORS_COUNT] = { SENSOR_LEFT_END, SENSOR_LEFT, SENSOR_CENTER, SENSOR_RIGHT, SENSOR_RIGHT_END };
LdrSensor* sensors[SENSORS_COUNT];
float threshold[SENSORS_COUNT] = { 0.5, 0.5, 0.5, 0.5, 0.5 }; //{ 0.88, 0.73, 0.68, 0.52, 0.3 };
bool currentSensorValues[SENSORS_COUNT] = { false, false, true, false, false };
bool lastSensorValues[SENSORS_COUNT] = { false, false, true, false, false };
bool programStarts = true;
float spd = 0.5;

void setup()
{
    pidControl = new PidController(1, 0, 0);
    motors = new MotorPair(1, 2, 3, 4, 5, 6, true);
    motors->setBalance(-0.95);

    for (int i = 0; i < 5; i++)
    {
        leds[i] = new Led(8 + i);
    }

    for (int i = 0; i < SENSORS_COUNT; i++)
    {
        sensors[i] = new LdrSensor(sensorPins[i]);
    }

    pinMode(BUZZER_PIN, OUTPUT);

    lastFrameTime = millis();

    Serial.begin(9600);
}

void loop()
{
    auto currentFrameTime = millis();
    auto delta = float(currentFrameTime - lastFrameTime) / 1000;
    stateTime -= delta;

    if (state != CALIBRATING)
    {
        for (auto i = 0; i < SENSORS_COUNT; i++)
        {
            auto value = sensors[i]->getNormalizedValue();
            currentSensorValues[i] = value <= threshold[i];

            Serial.print(currentSensorValues[i] ? "1" : "0");
            Serial.print(" (");
            Serial.print(value);
            Serial.print(")\t");

            if (currentSensorValues[i])
            {
                leds[i]->turnOn();
            }
            else
            {
                leds[i]->turnOff();
            }
        }

        Serial.println();
    }

    switch (state)
    {
        case CALIBRATING: calibrating(); break;
        case FOLLOWING_LINE: followingLine(); break;
        case TURNING_L: turningLeft(); break;
        case TURNING_R: turningRight(); break;
        case ROTATING_L: rotatingLeft(); break;
        case ROTATING_R: rotatingRight(); break;
        default: break;
    }

    for (auto i = 0; i < SENSORS_COUNT; i++)
    {
        lastSensorValues[i] = currentSensorValues[i];
    }

    lastFrameTime = currentFrameTime;
}

void calibrating()
{
    if (programStarts)
    {
        Serial.println("Starting calibration...");
        digitalWrite(BUZZER_PIN, HIGH);
        delay(100);
        digitalWrite(BUZZER_PIN, LOW);
        programStarts = false;
    }

    for (auto i = 0; i < SENSORS_COUNT; i++)
    {
        auto value = sensors[i]->getValue();
        sensors[i]->calibrate(value);
    }

    if (stateTime <= 0)
    {
        Serial.println("End of calibration!");
        digitalWrite(BUZZER_PIN, HIGH);
        delay(100);
        digitalWrite(BUZZER_PIN, LOW);
        delay(50);
        digitalWrite(BUZZER_PIN, HIGH);
        delay(100);
        digitalWrite(BUZZER_PIN, LOW);
        state = FOLLOWING_LINE;
    }
}

void followingLine()
{
    //detect 90º turns
    //detect acute turns
    //detect crossroad

    if (checkCurrentSensors(1, 1, 1, 0, 0))
    {
        state = TURNING_L;
    }
    else if (checkCurrentSensors(0, 0, 1, 1, 1))
    {
        state = TURNING_R;
    }
    else
    {
        auto error = calculateError();
        auto normalizedError = float(error) / 4;
        auto pid = pidControl->calculatePid(normalizedError);

        motors->setDirection(pid);
    }

    motors->updateMovement();
}

void turningLeft()
{
    if (checkCurrentSensors(0, 0, 1, 0, 0)
        || checkCurrentSensors(0, 0, 0, 1, 0)
        || checkCurrentSensors(0, 1, 0, 0, 0))
    {
        state = FOLLOWING_LINE;
        return;
    }

    motors->turnLeft(spd);
    motors->updateMovement();
}

void turningRight()
{
    if (checkCurrentSensors(0, 0, 1, 0, 0)
        || checkCurrentSensors(0, 0, 0, 1, 0)
        || checkCurrentSensors(0, 1, 0, 0, 0))
    {
        state = FOLLOWING_LINE;
        return;
    }

    motors->turnRight(spd);
    motors->updateMovement();
}

void rotatingLeft()
{
    if (checkCurrentSensors(0, 0, 1, 0, 0)
        || checkCurrentSensors(0, 0, 0, 1, 0)
        || checkCurrentSensors(0, 1, 0, 0, 0))
    {
        state = FOLLOWING_LINE;
        return;
    }

    motors->rotateLeft(spd);
    motors->updateMovement();
}


void rotatingRight()
{
    if (checkCurrentSensors(0, 0, 1, 0, 0)
        || checkCurrentSensors(0, 0, 0, 1, 0)
        || checkCurrentSensors(0, 1, 0, 0, 0))
    {
        state = FOLLOWING_LINE;
        return;
    }

    motors->rotateRight(spd);
    motors->updateMovement();
}

bool checkCurrentSensors(const int& leftEnd, const int& left, const int& center, const int& right, const int& rightEnd)
{
    return currentSensorValues[S_INDEX_LE] == bool(leftEnd)
           && currentSensorValues[S_INDEX_L] == bool(left)
           && currentSensorValues[S_INDEX_C] == bool(center)
           && currentSensorValues[S_INDEX_R] == bool(right)
           && currentSensorValues[S_INDEX_RE] == bool(rightEnd);
}

bool checkLastSensors(const int& leftEnd, const int& left, const int& center, const int& right, const int& rightEnd)
{
    return lastSensorValues[S_INDEX_LE] == bool(leftEnd)
           && lastSensorValues[S_INDEX_L] == bool(left)
           && lastSensorValues[S_INDEX_C] == bool(center)
           && lastSensorValues[S_INDEX_R] == bool(right)
           && lastSensorValues[S_INDEX_RE] == bool(rightEnd);
}

int calculateError()
{
    if (checkCurrentSensors(0, 0, 1, 0, 0))
    {
        return 0;
    }
    else if (checkCurrentSensors(0, 0, 1, 1, 0))
    {
        return 1;
    }
    else if (checkCurrentSensors(0, 0, 0, 1, 0))
    {
        return 2;
    }
    else if (checkCurrentSensors(0, 0, 0, 1, 1))
    {
        return 3;
    }
    else if (checkCurrentSensors(0, 0, 0, 0, 1))
    {
        return 4;
    }
    else if (checkCurrentSensors(0, 1, 1, 0, 0))
    {
        return -1;
    }
    else if (checkCurrentSensors(0, 1, 0, 0, 0))
    {
        return -2;
    }
    else if (checkCurrentSensors(1, 1, 0, 0, 0))
    {
        return -3;
    }
    else if (checkCurrentSensors(1, 0, 0, 0, 0))
    {
        return -4;
    }
}
