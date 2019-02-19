#include "EnginePair.h"
#include "Calibration.h"

#define SENSORS_COUNT 3
#define SENSOR_RIGHT A0
#define SENSOR_CENTER A1
#define SENSOR_LEFT A2
#define LED_RIGHT A3
#define LED_CENTER A4
#define LED_LEFT A5
#define BUZZER_PIN 2
#define CALIBRATING 0
#define WANDERING 1
#define TURNING_L 2
#define TURNING_R 3
#define ROTATING_L 5
#define ROTATING_R 6

EnginePair* engines;
int lastFrameTime;
float stateTime = 5;
int state = CALIBRATING;
int sensorPins[3] = { SENSOR_RIGHT, SENSOR_CENTER, SENSOR_LEFT };
Calibration* sensorCalibrations[3];
bool currentSensorValues[3] = { false, true, false };
bool lastSensorValues[3] = { false, true, false };
bool programStarts = true;
float spd = 0.5f;

void setup()
{
    engines = new EnginePair(4, 5, 6, 8, 9, 10, true);
    engines->setBalance(0.05);

    for (int i = 0; i < SENSORS_COUNT; i++)
    {
        sensorCalibrations[i] = new Calibration();
    }

    pinMode(SENSOR_RIGHT, INPUT);
    pinMode(SENSOR_CENTER, INPUT);
    pinMode(SENSOR_LEFT, INPUT);
    pinMode(LED_LEFT, OUTPUT);
    pinMode(LED_CENTER, OUTPUT);
    pinMode(LED_RIGHT, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    lastFrameTime = millis();

    Serial.begin(9600);
}

void loop()
{
    auto currentFrameTime = millis();
    auto delta = float(currentFrameTime - lastFrameTime) / 1000;
    stateTime -= delta;

    if (state == CALIBRATING)
    {
        if (programStarts)
        {
            Serial.println("Starting calibration...");
            digitalWrite(BUZZER_PIN, HIGH);
            delay(100);
            digitalWrite(BUZZER_PIN, LOW);
            programStarts = false;
        }

        calibration();

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
            state++;
        }

        lastFrameTime = currentFrameTime;
        return;
    }

    for (auto i = 0; i < SENSORS_COUNT; i++)
    {
        auto value = analogRead(sensorPins[i]);
        currentSensorValues[i] = value < sensorCalibrations[i]->getNormalizedValue(0.8f);

        Serial.print(currentSensorValues[i] ? "1" : "0");
        Serial.print(" (");
        Serial.print(value);
        Serial.print(")\t");
    }

    Serial.println();

    digitalWrite(LED_RIGHT, currentSensorValues[0] ? HIGH : LOW);
    digitalWrite(LED_CENTER, currentSensorValues[1] ? HIGH : LOW);
    digitalWrite(LED_LEFT, currentSensorValues[2] ? HIGH : LOW);

    switch (state)
    {
        case WANDERING: wandering(); break;
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

void calibration()
{
    for (auto i = 0; i < SENSORS_COUNT; i++)
    {
        auto value = analogRead(sensorPins[i]);

        if (value < sensorCalibrations[i]->getMin())
        {
            sensorCalibrations[i]->setMin(value);
        }

        if (value > sensorCalibrations[i]->getMax())
        {
            sensorCalibrations[i]->setMax(value);
        }
    }
}

void wandering()
{
    auto wasRightBlack = lastSensorValues[0];
    auto wasCenterBlack = lastSensorValues[1];
    auto wasLeftBlack = lastSensorValues[2];
    auto isRightBlack = currentSensorValues[0];
    auto isCenterBlack = currentSensorValues[1];
    auto isLeftBlack = currentSensorValues[2];

    if (!isLeftBlack && isCenterBlack && !isRightBlack)
    {
        engines->forward(spd);
    }
    else if (!isLeftBlack && !isCenterBlack && isRightBlack)
    {
        if ((!wasLeftBlack && wasCenterBlack && !wasRightBlack)
            || (!wasLeftBlack && !wasCenterBlack && wasRightBlack))
        {
            engines->slightTurnRight(spd);
        }
        else
        {
            state = ROTATING_R;
        }
    }
    else if (isLeftBlack && !isCenterBlack && !isRightBlack)
    {
        if ((!wasRightBlack && wasCenterBlack && !wasLeftBlack)
            || (wasLeftBlack && !wasCenterBlack && !wasRightBlack))
        {
            engines->slightTurnLeft(spd);
        }
        else
        {
            state = ROTATING_L;
        }
    }
    else if (!isLeftBlack && isCenterBlack && isRightBlack)
    {
        state = TURNING_R;
    }
    else if (isLeftBlack && isCenterBlack && !isRightBlack)
    {
        state = TURNING_L;
    }
    else if ((isLeftBlack && isCenterBlack && isRightBlack)
             && (!wasLeftBlack && wasCenterBlack && !wasRightBlack))
    {
        engines->forward(spd);

        currentSensorValues[0] = wasRightBlack;
        currentSensorValues[1] = wasCenterBlack;
        currentSensorValues[2] = wasRightBlack;
    }
    else
    {
        if ((wasLeftBlack && wasCenterBlack && !wasRightBlack)
            || (wasLeftBlack && !wasCenterBlack && !wasRightBlack))
        {
            state = ROTATING_L;
        }
        else if ((!wasLeftBlack && wasCenterBlack && wasRightBlack)
                 || (!wasLeftBlack && !wasCenterBlack && wasRightBlack))
        {
            state = ROTATING_R;
        }
        else
        {
            engines->backward(spd / 2);
        }
    }

    engines->updateMovement();
}

void turningLeft()
{
    auto isRightBlack = currentSensorValues[0];
    auto isCenterBlack = currentSensorValues[1];
    auto isLeftBlack = currentSensorValues[2];

    if ((!isLeftBlack && isCenterBlack && !isRightBlack)
        || (!isLeftBlack && !isCenterBlack && isRightBlack)
        || (isLeftBlack && !isCenterBlack && !isRightBlack))
    {
        state = WANDERING;
        return;
    }

    engines->turnLeft(spd);
    engines->updateMovement();
}

void turningRight()
{
    auto isRightBlack = currentSensorValues[0];
    auto isCenterBlack = currentSensorValues[1];
    auto isLeftBlack = currentSensorValues[2];

    if ((!isLeftBlack && isCenterBlack && !isRightBlack)
        || (!isLeftBlack && !isCenterBlack && isRightBlack)
        || (isLeftBlack && !isCenterBlack && !isRightBlack))
    {
        state = WANDERING;
        return;
    }

    engines->turnRight(spd);
    engines->updateMovement();
}

void rotatingLeft()
{
    auto isRightBlack = currentSensorValues[0];
    auto isCenterBlack = currentSensorValues[1];
    auto isLeftBlack = currentSensorValues[2];

    if ((!isLeftBlack && isCenterBlack && !isRightBlack)
        || (!isLeftBlack && !isCenterBlack && isRightBlack)
        || (isLeftBlack && !isCenterBlack && !isRightBlack))
    {
        state = WANDERING;
        return;
    }

    engines->rotateRight(spd);
    engines->updateMovement();
}


void rotatingRight()
{
    auto isRightBlack = currentSensorValues[0];
    auto isCenterBlack = currentSensorValues[1];
    auto isLeftBlack = currentSensorValues[2];

    if ((!isLeftBlack && isCenterBlack && !isRightBlack)
        || (!isLeftBlack && !isCenterBlack && isRightBlack)
        || (isLeftBlack && !isCenterBlack && !isRightBlack))
    {
        state = WANDERING;
        return;
    }

    engines->rotateRight(spd);
    engines->updateMovement();
}
