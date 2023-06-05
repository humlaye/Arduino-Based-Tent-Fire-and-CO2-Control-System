
#include <MG811.h>

// Pin tanımlamaları
const int CO2SensorPin = A0;  // CO2 sensörü bağlantı pini
const int TemperatureSensorPin = A1;  // Sıcaklık sensörü bağlantı pini
const int IRSensorPin = 6;          // Kızılötesi sensör bağlantı pini
const int WaterMotorPin1 = 7;       // Su motoru 1 bağlantı pini
const int WaterMotorPin2 = 8;       // Su motoru 2 bağlantı pini
const int BuzzerPin = 9;     // Buzzer bağlantı pini
const int MotorPin1 = 10;    // Motor 1 bağlantı pini
const int MotorPin2 = 11;    // Motor 2 bağlantı pini


// Sensör nesnesi oluştur
MG811 co2Sensor(CO2SensorPin);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  // Pin modlarını ayarla
  pinMode(BuzzerPin, OUTPUT);
  pinMode(MotorPin1, OUTPUT);
  pinMode(MotorPin2, OUTPUT);
  pinMode(IRSensorPin, INPUT);
  pinMode(WaterMotorPin1, OUTPUT);
  pinMode(WaterMotorPin2, OUTPUT);
  
  // Sıcaklık sensörünü başlat
  mlx.begin();
  
  // Serial haberleşmesini başlat 
  Serial.begin(9600);
}

void loop() {
  // CO2 seviyesini ölç
  int co2Level = co2Sensor.readCO2();
    // Sıcaklık ölçümünü yap
  float temperature = mlx.readObjectTempC();

  // CO2 seviyesini seri portta görüntüle 
  Serial.print("CO2 Seviyesi: ");
  Serial.println(co2Level);
    // Sıcaklık değerini seri portta görüntüle 
  Serial.print("Sıcaklık: ");
  Serial.println(temperature);

  // Eşik değeri kontrolü yap
  if (co2Level > 1000) {  
    // Buzzer'ı 5 saniye boyunca iki kez çal
    for (int i = 0; i < 2; i++) {
      digitalWrite(BuzzerPin, HIGH);
      delay(5000);
      digitalWrite(BuzzerPin, LOW);
      delay(5000);
    }
    
    // Motorları çalıştır
    digitalWrite(MotorPin1, HIGH);
    digitalWrite(MotorPin2, LOW);
    
    // CO2 seviyesi düşene kadar motorları çalıştır
    while (co2Level > 1000) {
      co2Level = co2Sensor.readCO2();
    }
    
    // Motorları durdur
    digitalWrite(MotorPin1, LOW);
    digitalWrite(MotorPin2, LOW);
  }
  
    // Eşik değeri kontrolü yap
  if (temperature > 30.0) {  
    // Buzzer'ı 5 saniye boyunca iki kez çal
    for (int i = 0; i < 2; i++) {
      digitalWrite(BuzzerPin, HIGH);
      delay(5000);
      digitalWrite(BuzzerPin, LOW);
      delay(5000);
    }
    
    // Kızılötesi sensörü kontrol et
    if (digitalRead(IRSensorPin) == HIGH) {
      // Yangın durumu algılandı, su motorlarını çalıştır
      digitalWrite(WaterMotorPin1, HIGH);
      digitalWrite(WaterMotorPin2, LOW);
    }
    
    // Yangın durumu sona erene kadar bekle
    while (digitalRead(IRSensorPin) == HIGH) {
      
    }
    
    // Yangın durumu sona erdi, su motorlarını durdur
    digitalWrite(WaterMotorPin1, LOW);
    digitalWrite(WaterMotorPin2, LOW);
  }
  
  
  delay(1000);  // Her saniyede bir ölçüm yapılmasını sağlar
}
