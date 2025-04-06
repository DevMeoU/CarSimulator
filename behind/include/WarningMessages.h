#ifndef WARNING_MESSAGES_H
#define WARNING_MESSAGES_H

// System warnings
#define WARNING_NONE ""
#define WARNING_SYSTEM_ERROR "WARNING: System error"
#define WARNING_LOW_OIL_PRESSURE "WARNING: Low oil pressure"
#define WARNING_HIGH_ENGINE_TEMP "WARNING: High engine temperature"
#define WARNING_LOW_BATTERY "WARNING: Low battery"
#define WARNING_BRAKE_SYSTEM "WARNING: Brake system error"

// Safety warnings
#define WARNING_SEATBELT_HIGH_SPEED "WARNING: Seat belt not fastened at high speed!"
#define WARNING_LOW_BRAKE_PRESSURE "WARNING: Low brake pressure at high speed!"
#define WARNING_PARKING_WHILE_MOVING "WARNING: Cannot release parking brake while moving!"
#define WARNING_REVERSE_HIGH_SPEED "WARNING: Cannot shift to reverse at high speed!"

// Environmental warnings
#define WARNING_HIGH_ALTITUDE "WARNING: High altitude detected"
#define WARNING_EXTREME_TEMPERATURE "WARNING: Extreme temperature condition"
#define WARNING_SEVERE_WEATHER "WARNING: Severe weather condition"

// Battery and charging warnings
#define WARNING_CHARGING_ERROR "WARNING: Charging system error"
#define WARNING_BATTERY_TEMP_HIGH "WARNING: Battery temperature too high"
#define WARNING_LOW_RANGE "WARNING: Low battery range"

// Gear warnings
#define WARNING_INVALID_GEAR_ACCELERATION "WARNING: Cannot accelerate in Park or Neutral gear"
#define WARNING_INVALID_GEAR_CHANGE "WARNING: Cannot change gear while vehicle is moving"

// Default warning
#define WARNING_UNDEFINED "UNDEFINED WARNING"

#endif // WARNING_MESSAGES_H