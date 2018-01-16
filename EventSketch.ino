#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <SPI.h>

#define trigPin 2
#define echoPin 3
/*
char ssid[] = "OnePlus3";     //  SSID sieci WiFi 
char pass[] = "olaolaola";    // hasło
int status = WL_IDLE_STATUS;     // monitor połączenia
WiFiClient client;
String sendDist;
String sendVelocity;

unsigned long lastConnectionTime = 0;            // Ostatni czas połączenia z serwerem
const unsigned long postingInterval = 1L * 600L; // Interwał czasowy co który będzie nawiązywane połączenie z serwerem
float velocity;
unsigned long timeVelocityStart = 0L * 1000L;
unsigned long timeVelocityStop = 0L * 1000L;
long lastdystans = 0;*/
int incoming = 0;

void setup()  {
  
  Serial.begin (9600);
  SPI.begin();
  // Wersja firmware WiFiShield
  //  Serial.println("the version is: ");
  //  Serial.println(WiFi.firmwareVersion());

  
  // Adres MAC Arduino
  //printMacAddress();

  // Listowanie dostępnych sieci bezprzewodowych
  //Serial.println("Wyszukiwanie dostępnych sieci bezprzewodowych");
  //listNetworks();

  // Nazwiązywanie połączenia z wybraną siecią
  //Serial.print("Próba połączenia z siecią bezprzewodową: ");
  //Serial.println(ssid);
 /* status = WiFi.begin(ssid, pass);

  // Błąd połączenia:
  if ( status != WL_CONNECTED) { 
    //Serial.println("Nie udało się ustanowić połączenia z siecią WiFi!");
    while(true);
  } 
  // Udane połączenie:
  else {
    //Serial.println("Połączono z siecią!");
  }*/
  pinMode(trigPin, OUTPUT); // Ustawienie Trig pin dla sensora odległości
  pinMode(echoPin, INPUT); // Ustawienie Echo pin dla sensora odległości
  pinMode(5, OUTPUT); // Ustawienie pinu sterującego dla beepera

  // Sygnalizacja poprawnej inicjalizacji Arduino (3 krótkie sygnały dźwiękowe)
  initialbeep();
  
  delay(3000);
  //Serial.println("Odczyt danych: ");
} 

void loop()  {

  long czas, dystans;
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  czas = pulseIn(echoPin, HIGH);
  dystans = czas / 58;


  incoming = Serial.read();
  if(incoming != -1){
    Serial.print(dystans);
    Serial.println();
  }


  
  if(dystans <= 10){
    beep(dystans * 4);
  }
/*
  timeVelocityStop = millis();
  velocity = abs((float)lastdystans - (float)dystans)/((float)timeVelocityStop - (float)timeVelocityStart)*1000.0;
  //Serial.println((float)timeVelocityStop - (float)timeVelocityStart);
  timeVelocityStart = timeVelocityStop;
  lastdystans = dystans;
  //Serial.print("Vel:");
  //Serial.println(velocity);
  
  // Wypisanie przychodzących wiadomości przy połączeniu z serwerem 
  while (client.available()) {
    char c = client.read();
    //Serial.write(c);
  }

  // Po upływie interwału przesyłana jest kolejna porcja informacji:
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest(dystans, velocity);
  }
  */
  delay(100);
}

void beep(unsigned char delayms){
  analogWrite(5, 20);      // Nadanie sygnału na pin beepera (wartości od 1- 254 zmieniają ton)
  delay(delayms);          // Oczekiwanie w milisekundach
  analogWrite(5, 0);       // Wyłączenie beepera
  delay(delayms);          // Dodatkowy przestój   
}  

void initialbeep(){
  analogWrite(5, 30); 
  delay(150);      
  analogWrite(5, 0);     
  delay(30);
  analogWrite(5, 120); 
  delay(50);      
  analogWrite(5, 0);     
  delay(30);
  analogWrite(5, 30); 
  delay(150);      
  analogWrite(5, 0);     
  delay(30);
}
/*
void printMacAddress() {
  // MAC adres
  byte mac[6];                     

  // Wypisywanie adresu MAC:
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
}
*/
/*
void listNetworks() {
  // Skan pobliskich sieci Wifi:
  Serial.println("** Skanuję sieci bezprzewodowe **");
  byte numSsid = WiFi.scanNetworks();

  // Wypisanie odnalezionych sieci:
  Serial.print("Liczba dostępnych sieci bezprzewodowych: ");
  Serial.println(numSsid);

  // Wypisanie numeru i nazwy sieci bezprzewodowych:
  for (int thisNet = 0; thisNet<numSsid; thisNet++) {
    Serial.print(thisNet);
    Serial.print(") ");
    Serial.print(WiFi.SSID(thisNet));
    Serial.print("\tSiła sygnału: ");
    Serial.print(WiFi.RSSI(thisNet));
    Serial.print(" dBm");
    Serial.print("\tSzyfrowanie: ");
    Serial.println(WiFi.encryptionType(thisNet));
  }
}
*/
/*
void httpRequest(long dist, float velocity) {
  // Zamykanie jakichkolwiek połączeń, przed próbą nawiązania kolejnego
  client.stop();

  // Działanie po udanym połączeniu:  
  if (client.connect("apps.agencja-art.pl", 80)) {
    
      //Serial.println("Przesył danych...");

    sendDist = String(dist);
    sendVelocity = String(velocity, 2);
    unsigned int len = 5 + sendDist.length() + 5 + sendVelocity.length();
    // Wysłanie POST request:
    client.println("POST /piotrek/arduino/save.php HTTP/1.1");
    client.println("Host: apps.agencja-art.pl");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(len);
    client.println();
    client.print("dist=");
    client.print(sendDist);
    client.print("&vel=");
    client.println(sendVelocity);

    // Zliczanie czasu ustalenia połączenia:
    lastConnectionTime = millis();
  } else {
    // W przypadku braku połączenia:
    //Serial.println("Nieudane połączenie z serwerem");
  }
}
*/
