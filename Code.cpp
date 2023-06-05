
#include <MG811.h>

// Pin definitions
const int CO2SensorPin = A0;  // CO2 sensor connection pin
const int TemperatureSensorPin = A1;  //Temperature sensor connection pin
const int IRSensorPin = 6;          // Infrared sensor connection pin
const int WaterMotorPin1 = 7;       // Water motor 1 connection pin
const int WaterMotorPin2 = 8;       // Water motor 2 connection pin
const int BuzzerPin = 9;     //Buzzer connection pin
const int MotorPin1 = 10;    // Motor 1 connection pin
const int MotorPin2 = 11;    // Motor 2 connection pin


// Create sensor object
MG811 co2Sensor(CO2SensorPin);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  // Set pin modes
  pinMode(BuzzerPin, OUTPUT);
  pinMode(MotorPin1, OUTPUT);
  pinMode(MotorPin2, OUTPUT);
  pinMode(IRSensorPin, INPUT);
  pinMode(WaterMotorPin1, OUTPUT);
  pinMode(WaterMotorPin2, OUTPUT);
  
  // Start temperature sensor
  mlx.begin();
  
  // start serial communication
  Serial.begin(9600);
}

void loop() {
  // Measure CO2 level
  int co2Level = co2Sensor.readCO2();
    // Temperature measurement
  float temperature = mlx.readObjectTempC();

  // Display CO2 level on serial port
  Serial.print("CO2 level: ");
  Serial.println(co2Level);
    // Display temperature value on serial port
  Serial.print("Sıcaklık: ");
  Serial.println(temperature);

  // check co2 level threshold
  if (co2Level > 1000) {  
    // Ring the buzzer twice for 5 seconds
    for (int i = 0; i < 2; i++) {
      digitalWrite(BuzzerPin, HIGH);
      delay(5000);
      digitalWrite(BuzzerPin, LOW);
      delay(5000);
    }
    
    // start the motors
    digitalWrite(MotorPin1, HIGH);
    digitalWrite(MotorPin2, LOW);
  }else{
    // stop motors
    digitalWrite(MotorPin1, LOW);
    digitalWrite(MotorPin2, LOW);
  }
  
    // check for temperature
  if (temperature > 30.0) {  
    // BRing the buzzer twice for 5 seconds
    for (int i = 0; i < 2; i++) {
      digitalWrite(BuzzerPin, HIGH);
      delay(5000);
      digitalWrite(BuzzerPin, LOW);
      delay(5000);
    }
    
    // Check infrared sensor
    if (digitalRead(IRSensorPin) == HIGH) {
      // Fire condition detected, start water motor
      digitalWrite(WaterMotorPin1, HIGH);
      digitalWrite(WaterMotorPin2, LOW);
    }
  }else{  
    // Fire condition over, stop water motors
    digitalWrite(WaterMotorPin1, LOW);
    digitalWrite(WaterMotorPin2, LOW);
  }
  
  
  delay(1000); 
}
