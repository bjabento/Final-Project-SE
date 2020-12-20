#include <DHT.h>
#define DHTTYPE DHT11
#define DHTPIN 7  
DHT dht(DHTPIN, DHTTYPE);
Servo s1;

int map1;
int n = 30;
int pos = 0;
float hum, temp;

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(10, OUTPUT);
  dht.begin();
  s1.attach (10);

}

void loop() {
  //Servo
  s1.write(pos);
  //Leitura Humidade
 hum = dht.readHumidity();
 //Leitura temperatura
 temp = dht.readTemperature();
  //Verifica se houve erros
  if (isnan(hu) || isnan(te)) {
    Serial.println(F("Falha ao ler o sensor"));
    return;
  }
  else {
    Serial.print("Humidade: ");
    Serial.print(hum);
    Serial.print(" %t");
    Serial.print("Temperatura: ");
    Serial.print(temp);
    Serial.println(" ºC");
    while ( hum < 20)
    {
      //Verificar melhor posicao - POR FAZER
      pos = 90;
      s1.write(pos);
    }
    }
  }
}
