/*
  jsnsr04t.cpp - Library for using ultrasonic sensor driver JSN-SR-04T.
  Created by Overflaw, August 28, 2023
  Released into the public domain.
*/
#include "jsnsr04t.h"

JsnSr04T::JsnSr04T(int echoPin, int triggerPin, int logLevel)
{
    _echoPin = echoPin;
    _triggerPin = triggerPin;
    _logLevel = logLevel;
}

void JsnSr04T::begin(Stream &serial)
{
    _serial = &serial;
    Log.begin(_logLevel, _serial);
    pinMode(_triggerPin, OUTPUT);
    pinMode(_echoPin, INPUT);    
    Log.verbose(F("Set pin %d as trigger pin : OUTPUT"CR), _triggerPin);
    Log.verbose(F("Set pin %d as echo pin : INPUT"CR), _triggerPin);
}

int JsnSr04T::readDistance()
{
    this->initiatesPulse();    
    long echoDurationUs = pulseIn(_echoPin, HIGH);
    Log.verbose(F("Duration measured on echo pin : %dus"CR), echoDurationUs);
    return this->calculateDistance(echoDurationUs);
}

void JsnSr04T::initiatesPulse()
{
    Log.verbose(F("Create pulse : %dus low then %dus high"CR), JSNSR04T_PULSE_LOW_US, JSNSR04T_PULSE_HIGH_US);
    digitalWrite(_triggerPin, LOW);
    _pulseLow.start(JSNSR04T_PULSE_LOW_US, AsyncDelay::MICROS);
    while(!_pulseLow.isExpired()) { }
    digitalWrite(_triggerPin, HIGH);
    _pulseHigh.start(JSNSR04T_PULSE_HIGH_US, AsyncDelay::MICROS);
    while(!_pulseHigh.isExpired()) { }
    digitalWrite(_triggerPin, LOW);
}

int JsnSr04T::calculateDistance(long echoDurationUs)
{    
    int distance = (int) echoDurationUs / METER_CONVERSION_COEFFICIENT;
    Log.notice(F("Distance calculated : %d cm"CR), distance);
    return distance;
}
