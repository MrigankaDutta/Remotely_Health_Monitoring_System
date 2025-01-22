#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define BLYNK_PRINT Serial


char auth[] = "vbivaDe2JTrh83Sv3ODCP8vWgEef****";

char ssid[] = "Mriganka Dutta"; // WiFi SSID
char pass[] = "common1234"; // WiFi Password


PulseOximeter pox;
float BPM, SpO2;
uint32_t tsLastReport = 0;

#define REPORTING_PERIOD_MS 1000
//int LED = 2;

/*const double VCC = 3.3;             // NodeMCU on board 3.3v vcc
const double R2 = 8500;            // series resistor
const double adc_resolution = 4095; // 12-bit adc

const double A = 0.001129148;   // thermistor equation parameters
const double B = 0.000234125;
const double C = 0.0000000876741; 
double ntc_analog_value;
*/
void onBeatDetected()
{
Serial.println("Beat Detected!");
//digitalWrite(LED, HIGH);
}


void setup()
{
  Serial.begin(115200);
  pinMode(2,OUTPUT);
 
pinMode(19, OUTPUT);
Blynk.begin(auth, ssid, pass);
Serial.print("Initializing Pulse Oximeter..");
/*bool wifi_connection = Blynk.connected;
while(wifi_connection == false) {
    Serial.println("Waiting for the WiFi...");
    delay(100);
    digitalWrite(2, HIGH);
  }
*/
if (!pox.begin())
{
Serial.println("FAILED");
 for (;;);
}
else
{
Serial.println("SUCCESS");

}

pox.setOnBeatDetectedCallback(onBeatDetected);
pox.setIRLedCurrent(MAX30100_LED_CURR_20_8MA);

}
void loop()
{ 
Blynk.run();
pox.update();

/*double Vout, Rth, temperature, adc_value; 
  
  ntc_analog_value = 0;
  int no_off_measurment = 100;

  for(int i =0; i<no_off_measurment; i++)
  {
    ntc_analog_value = ntc_analog_value + analogRead(15);
    delay(10);
    }

  adc_value = ntc_analog_value/no_off_measurment;
  Vout = (adc_value * VCC) / adc_resolution;
  Rth = (VCC * R2 / Vout) - R2;

/*  Steinhart-Hart Thermistor Equation:
 *  Temperature in Kelvin = 1 / (A + B[ln(R)] + C[ln(R)]^3)
 *  where A = 0.001129148, B = 0.000234125 and C = 8.76741*10^-8  */
 // temperature = (1 / (A + (B * log(Rth)) + (C * pow((log(Rth)),3))));   // Temperature in kelvin

 // temperature = temperature - 273.15;  // Temperature in degree celsius

  
if (millis() - tsLastReport > REPORTING_PERIOD_MS)
{
BPM = pox.getHeartRate();
SpO2 = pox.getSpO2();
Serial.print("Heart rate:");
Serial.println(BPM);
Serial.print("SpO2:");
Serial.print(SpO2);
Serial.println(" %");

Serial.println("*********************************");
Serial.println();

Blynk.virtualWrite(V7, BPM);
Blynk.virtualWrite(V8, SpO2);

tsLastReport = millis();

while(WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi has been disconnected");
    digitalWrite(2, HIGH);
    delay(100);
    digitalWrite(2, LOW);
    delay(100);
}
/*Serial.print("Temperature = ");
Serial.print(temperature);
Serial.println("°c");
delay(100);
Serial.println("=================================");
*/
}
}