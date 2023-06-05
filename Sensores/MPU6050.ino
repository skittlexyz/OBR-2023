#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  
  Serial.println("Teste do sensor GY-521 (MPU-6050)");
  Serial.println("==================================");
  
  mpu.initialize();
  
  if (mpu.testConnection()) {
    Serial.println("Conexão bem-sucedida com o sensor.");
    Serial.println("==================================");
  } else {
    Serial.println("Falha na conexão com o sensor!");
    while (1);
  }
}

void loop() {
  int16_t ax, ay, az, gx, gy, gz;
  
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  Serial.print("Acelerômetro (g): ");
  Serial.print("X = ");
  Serial.print(ax / 16384.0);
  Serial.print(", Y = ");
  Serial.print(ay / 16384.0);
  Serial.print(", Z = ");
  Serial.print(az / 16384.0);
  Serial.println();
  
  Serial.print("Giroscópio (graus/seg): ");
  Serial.print("X = ");
  Serial.print(gx / 131.0);
  Serial.print(", Y = ");
  Serial.print(gy / 131.0);
  Serial.print(", Z = ");
  Serial.print(gz / 131.0);
  Serial.println("====================================================");
  Serial.println();
  
  delay(1000); // Aguarda 1 segundo entre cada leitura
}
