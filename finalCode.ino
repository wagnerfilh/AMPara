#include <OneWire.h>
#include <DallasTemperature.h>
//Carrega a biblioteca LiquidCrystal
#include <LiquidCrystal.h>
 
//Define os pinos que serão utilizados para ligação ao display
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//Temperatura 
#define ONE_WIRE_BUS 13
DeviceAddress sensor1;
// Setup a oneWire
OneWire oneWire(ONE_WIRE_BUS);
float temperatura;
DallasTemperature sensors(&oneWire);

//https://wiki.keyestudio.com/KS0429_keyestudio_TDS_Meter_V1.0
#include <GravityTDS.h>
GravityTDS gravityTds;
#define TdsSensorPin A1
#define VREF 5.0 // analog reference voltage(Volt) of the ADC
#define SCOUNT 30 // sum of sample point
int analogBuffer[SCOUNT]; // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0, copyIndex = 0;
float mediaVoltagem = 0, valorTds = 0;

//SENSOR DE PH https://www.electroniclinic.com/arduino-libraries-download-and-projects-they-are-used-in-project-codes/
float calibration_value = 23.34; //valor para calibrar os valores do pH
int phval = 0;
unsigned long int avgval;
int buffer_arr[10], temp;
float ph_act;

void setup()
{
  //CARREGA LCD
  lcd.begin(16, 2);
  ///
  Serial.begin(115200);
    gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
    gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
    gravityTds.begin();  //initialization
  sensors.begin();
  if (!sensors.getAddress(sensor1, 0))
{
  Serial.println("Sensores nao encontrados !");
  }
    
  
}
void loop()
{
  //TEMPERATURA
  sensors.requestTemperatures();
  temperatura = sensors.getTempCByIndex(0);
  Serial.print("Celsius: ");
  //Printa no terminal a temperatura
  Serial.print(sensors.getTempCByIndex(0));
  Serial.println(sensors.getTempFByIndex(0));
  // CÓDIGO DA DOCUMENTAÇÃO DO SENSOR DE TDS
  gravityTds.setTemperature(temperatura);  // set the temperature and execute temperature compensation
  gravityTds.update();  //sample and calculate
  valorTds = gravityTds.getTdsValue();  // then get the value
  
  //SENSOR DE PH
  for (int i = 0; i < 10; i++)
  {
    buffer_arr[i] = analogRead(A0);
    delay(30);
  }
  for (int i = 0; i < 9; i++)
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buffer_arr[i] > buffer_arr[j])
      {
        temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
  }
  avgval = 0;
  for (int i = 2; i < 8; i++)
    avgval += buffer_arr[i];
  float volt = (float)avgval * 5.0 / 1024 / 6; //CALIBRA VOLTAGEM DO SENSOR DE PH
  volt-=1.5;
  ph_act = -5.70 * volt + calibration_value;//PH ATUAL
  ph_act=ph_act/2;
  Serial.print("pH Val: ");
  Serial.println(ph_act);
  delay(1000);
  printarTela();
}



void printarTelaBoasVindas()
{
  lcd.setCursor(0,0);
  lcd.print("Bem vindo a");
  lcd.setCursor(0,1);
  lcd.print("AMPara");
  delay(5000);
}
void printarTela()
{
  //TEMPERATURA
  lcd.setCursor(0,0);
  lcd.print("TEMPERATURA ");
  delay(3000);
  lcd.clear();
  delay(500);
  lcd.print("Valor normal:");
  lcd.setCursor(0,1);
  lcd.print("20 *C - 28 *C");
  delay(4000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Valor atual");
  lcd.setCursor(0,1);
  lcd.print(temperatura);
  lcd.print(" *C");
  delay(3000);
  lcd.clear();
  //TDS
  lcd.setCursor(0,0);
  lcd.print("TDS ");
  delay(3000);
  lcd.clear();
  delay(500);
  lcd.print("Valor normal:");
  lcd.setCursor(0,1);
  lcd.print("0 PPM - 200 PPM");
  delay(4000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Valor atual");
  lcd.setCursor(0,1);
  lcd.print(valorTds);
  lcd.print(" ppm");
  delay(3000);
  lcd.clear();
  //PH
  lcd.setCursor(0,0);
  lcd.print("PH ");
  delay(3000);
  lcd.clear();
  delay(500);
  lcd.print("Valor normal:");
  lcd.setCursor(0,1);
  lcd.print("6.5 PH - 7.5 PH");
  delay(4000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Valor atual");
  lcd.setCursor(0,1);
  lcd.print(ph_act);
  lcd.print(" pH");
  delay(3000);
  lcd.clear();
}
