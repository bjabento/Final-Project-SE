#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <DHT.h>
#define DHTTYPE DHT11
#define DHTPIN 4  //pin temperatura e humidade
//Qauliade do ar
#include <MQUnifiedsensor.h>
#define placa "Arduino UNO"
#define Voltage_Resolution 5
#define pin A0 
#define type "MQ-135"
#define ADC_Bit_Resolution 10 
#define RatioMQ135CleanAir 3.6//RS / R0 = 3.6 ppm 
//Humidade do solo
#define pinSolo A1
 
SoftwareSerial comS(0,1); 

DHT dht(DHTPIN, DHTTYPE);
MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

int vLDR = 0, huSolo = 0;
const int pinAlarme = 5;
float hum, temp, CO, CO2, NH4;
long dist = 0;
volatile bool alarme = false; //Inicializado a false pois o utilizador liga o alarme quando quiser
LiquidCrystal LCD(1, 10, 9, 8, 7, 6);

void setup() {
  Serial.begin(9600);
  //while(!Serial);
  comS.begin(19200);
  pinMode(2, OUTPUT); //pin echo ultrasound
  pinMode(3, INPUT);  //pin trig ultrasound
  //pinMode(7, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(pinSolo, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(10, OUTPUT);
  dht.begin();

  MQ135.setRegressionMethod(1);
  MQ135.init();
  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ135.update();
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }
  MQ135.setR0(calcR0/10);
  Serial.println("  done!.");
  
  if(isinf(calcR0)) {Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0 == 0){Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); while(1);}
  attachInterrupt(digitalPinToInterrupt(pinAlarme), intruso, RISING);
  LCD.begin(16, 2);
}

void loop()
{
  if(alarme != false)
  {
    hum = dht.readHumidity();
    temp = dht.readTemperature();
    //Verifica se houve error
    if (isnan(hum) || isnan(temp)) {
      Serial.println(F("Falha ao ler o sensor"));
      return;
    }

    MQ135.update();

    MQ135.setA(605.18); MQ135.setB(-3.937); // Configurate the ecuation values to get CO concentration
    CO = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

    MQ135.setA(110.47); MQ135.setB(-2.862); // Configurate the ecuation values to get CO2 concentration
    CO2 = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

    MQ135.setA(102.2 ); MQ135.setB(-2.473); // Configurate the ecuation values to get NH4 concentration
    NH4 = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

    huSolo = analogRead(pinSolo);

    vLDR = analogRead(A2);

    qual();
    temper();
    rega();
    lum();
    
    digitalWrite(2, LOW);
    delayMicroseconds(2);
    digitalWrite(2, HIGH);
    delayMicroseconds(10);
    digitalWrite(2, LOW);
  
    dist = (pulseIn(3, HIGH) /29 /2);

    comS.print(CO);
    comS.print(CO2);
    comS.print(NH4);
    comS.print(temp);
    comS.print(huSolo);
    comS.print(vLDR);
     if(dist > 5)
     {
      comS.print("Detetado intruzo");
      LCD.clear();
      LCD.print("Detetado intruzo");
     }
       Serial.print("Luminosidade: ");
  Serial.println(vLDR);
  Serial.print("Humidade: ");
  Serial.print(hum);
  Serial.println("%");
  Serial.print("Temperatura: ");
  Serial.print(temp);
  Serial.println("ºC");
  Serial.print("Humidade do solo: ");
  Serial.print(huSolo);
  Serial.println("|    CO    |    CO2   |    NH4    |");   
  Serial.print("|   "); Serial.print(CO); 
  Serial.print("   |   "); Serial.print(CO2); 
  Serial.print("   |   "); Serial.print(NH4); 
  Serial.println("   |"); 
  Serial.println(dist);
  
     LCD.clear();
    LCD.print(" CO   CO2   NH4");
    LCD.setCursor(0, 1);
    LCD.print("           ");
    LCD.setCursor(0, 1);
    LCD.print(CO);
    LCD.setCursor(5, 1);
    LCD.print(CO2);
    LCD.setCursor(11, 1);
    LCD.print(NH4);
  }
  else
  {
    hum = dht.readHumidity();
    temp = dht.readTemperature();
    //Verifica se houve error
    if (isnan(hum) || isnan(temp)) {
      Serial.println(F("Falha ao ler o sensor"));
      return;
    }

    MQ135.update();

    MQ135.setA(605.18); MQ135.setB(-3.937); // Configurate the ecuation values to get CO concentration
    CO = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

    MQ135.setA(110.47); MQ135.setB(-2.862); // Configurate the ecuation values to get CO2 concentration
    CO2 = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

    MQ135.setA(102.2 ); MQ135.setB(-2.473); // Configurate the ecuation values to get NH4 concentration
    NH4 = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

    huSolo = analogRead(pinSolo);

    vLDR = analogRead(A2);

    qual();
    temper();
    rega();
    lum();

    comS.print(CO);
    comS.print(CO2);
    comS.print(NH4);
    comS.print(temp);
    comS.print(huSolo);
    comS.print(vLDR);

    Serial.print("Luminosidade: ");
  Serial.println(vLDR);
  Serial.print("Humidade: ");
  Serial.print(hum);
  Serial.println("%");
  Serial.print("Temperatura: ");
  Serial.print(temp);
  Serial.println("ºC");
  Serial.print("Humidade do solo: ");
  Serial.print(huSolo);
  Serial.println("|    CO    |    CO2   |    NH4    |");   
  Serial.print("|   "); Serial.print(CO); 
  Serial.print("   |   "); Serial.print(CO2); 
  Serial.print("   |   "); Serial.print(NH4); 
  Serial.println("   |"); 
  Serial.println(" ");
  
     LCD.clear();
    LCD.print(" CO   CO2   NH4");
    LCD.setCursor(0, 1);
    LCD.print("           ");
    LCD.setCursor(0, 1);
    LCD.print(CO);
    LCD.setCursor(5, 1);
    LCD.print(CO2);
    LCD.setCursor(11, 1);
    LCD.print(NH4);
  }
  delay(5000);
}

void qual()
{
  if((CO >= 4) || (CO2 >= 3) || (NH4 >= 4))
  {
    digitalWrite(A4, HIGH);
  }
  else
  {
    digitalWrite(A4, LOW);
  }
}

void temper()
{
  if(temp >= 16)
  {
    digitalWrite(A4, HIGH);
  }
  else
  {
    digitalWrite(A4, LOW);
  }
}

void rega()
{
  if(huSolo >= 1015){
    digitalWrite(A5, HIGH);
  }
  else{
    digitalWrite(A5, LOW);
  }
}

void lum()
{
  if(vLDR <= 300){
    digitalWrite(A3, HIGH);
  }
  else
  {
    digitalWrite(A3, LOW);
  }
}

void intruso()
{
  alarme = !alarme;
}
