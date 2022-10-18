//thermometer - Measuring temperature, record temperature fluctuations

int lm35Pin = A0;

int temp = 0;
int tempMin = 100;
int tempMax = -100;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i <= 9; i++) {
    int signVal = analogRead(lm35Pin);
    temp += (signVal * 5 * 100) / 1024;
    signVal = 0;
    delay(80);
  }
  temp = temp / 10;
  
  if (temp > tempMax) {
    tempMax = temp;
  }
  if (temp < tempMin) {
    tempMin = temp;
  }
 
  Serial.print("Temp:");
  Serial.print(temp);
  Serial.print(" Cels,");
  Serial.print(" Min:");
  Serial.print(tempMin);
  Serial.print(" Max:");
  Serial.println(tempMax);
  temp = 0;
  delay(500);
}
