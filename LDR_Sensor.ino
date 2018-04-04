//swicth case
int statebefore = 0;
int statenow = 1;

// SD card
#include <SD.h>
#include <SPI.h>
int CS_PIN = 10;
File file;

// cahaya variable
const int ldrPin = A0;

// suhu variable
int ThermistorPin = A1;
int Vo;
float R1 = 10000;
float logR2, R2, T, Tc, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

//water variable
const int hygrometer = A2;
int valueHumid;

// relay variable
int relayPinOut = 2;

// suhu func
double SuhuCounter(int Vo) {
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15;
  return Tc;
}

// SD func
void initializeSD()
{
  Serial.println("Initializing SD card...");
  pinMode(CS_PIN, OUTPUT);

  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
}

int createWriteFile(char filename[])
{
  file = SD.open(filename, FILE_WRITE);

  if (file)
  {
    Serial.println("File created successfully.");
    return 1;
  } else
  {
    Serial.println("Error while creating file.");
    return 0;
  }
}

int writeToFile(String text)
{
  if (file)
  {
    file.println(text);
    Serial.println("Writing to file: ");
    Serial.println(text);
    return 1;
  } else
  {
    Serial.println("Couldn't write to file");
    return 0;
  }
}

void closeFile()
{
  if (file)
  {
    file.close();
    Serial.println("File closed");
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // cahaya
  pinMode(ldrPin, INPUT);
  // relay
  pinMode(relayPinOut, OUTPUT);
  // SD
  initializeSD();
}

void loop() {
  
  // cahaya
  int ldrStatus = analogRead(ldrPin);
  int ldrStatusCalibrate = map(ldrStatus, 1023, 0, 100, 0);

  // suhu  
  Vo = analogRead(ThermistorPin);
  double suhuNowC = SuhuCounter(Vo);
  
  // water
  valueHumid = analogRead(hygrometer);    //Read analog value 
  valueHumid = constrain(valueHumid,400,1023);  //Keep the ranges!
  valueHumid = map(valueHumid,400,1023,100,0);  //Map value : 400 will be 100 and 1023 will be 0
  
  
  Serial.print("Soil humidity: "); Serial.print(valueHumid); Serial.println(" %");
  Serial.print("Temperature: "); Serial.print(suhuNowC); Serial.println(" C");   
  Serial.print("Cahaya : "); Serial.println(ldrStatusCalibrate);
  Serial.print("Statenow :"); Serial.println(statenow);
  Serial.print("Statebefore :"); Serial.println(statebefore);

  if (ldrStatusCalibrate >= 20 && valueHumid <= 15){
    statenow = 0;
    if (statenow != statebefore){
      Serial.println("|||||||||||||||||||||||||||||||||||Change State||||||||||||||||||||||||||||||||||||||");
      createWriteFile("logFile.txt");
      String a = String(suhuNowC) + ", waktu sekarang: , state sekarang : " + statenow + ", state sebelumnya : " + statebefore;
      writeToFile(a);
      closeFile(); 
      statebefore = statenow;
    }
  } else if (ldrStatusCalibrate >= 20 && valueHumid >= 80) {
    statenow = 1;
    if (statenow != statebefore){
      Serial.println("|||||||||||||||||||||||||||||||||||Change State||||||||||||||||||||||||||||||||||||||");
      createWriteFile("logFile.txt");
      String a = String(suhuNowC) + ", waktu sekarang: , state sekarang : " + statenow + ", state sebelumnya : " + statebefore;
      writeToFile(a);
      closeFile(); 
      statebefore = statenow;
    }
  } else if (ldrStatusCalibrate <= 7 && valueHumid >= 80) {
    statenow = 2;
    if (statenow != statebefore){
      Serial.println("|||||||||||||||||||||||||||||||||||Change State||||||||||||||||||||||||||||||||||||||");
      createWriteFile("logFile.txt");
      String a = String(suhuNowC) + ", waktu sekarang: , state sekarang : " + statenow + ", state sebelumnya : " + statebefore;
      writeToFile(a);
      closeFile(); 
      statebefore = statenow;
    }
  } else if (ldrStatusCalibrate <= 7 && valueHumid <= 15) {
    statenow = 3;
    if (statenow != statebefore){
      Serial.println("|||||||||||||||||||||||||||||||||||Change State||||||||||||||||||||||||||||||||||||||");
      createWriteFile("logFile.txt");
      String a = String(suhuNowC) + ", waktu sekarang: , state sekarang : " + statenow + ", state sebelumnya : " + statebefore;
      writeToFile(a);
      closeFile(); 
      statebefore = statenow;
    }
  }
  
  switch (statenow) {
    case 0:
      digitalWrite(relayPinOut, LOW);
      break;
    case 1:
      digitalWrite(relayPinOut, HIGH);
      break;
    case 2:
      digitalWrite(relayPinOut, HIGH);
      break;
    case 3:
      digitalWrite(relayPinOut, HIGH);
      break;
    default:
      statenow = 1;
      break;
  }

  delay(500);
}
