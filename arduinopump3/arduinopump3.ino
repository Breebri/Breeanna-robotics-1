  #include "DHT.h"
  #include <SD.h>
  #include <DFRobot_DS1307.h>

  const int DHTpin = 3;
  #define DHTTYPE DHT11
  DHT dht(DHTpin, DHTTYPE);

  DFRobot_DS1307 DS1307; 



  File myFile;
  File logfile = SD.open("log.csv", FILE_WRITE);
  const int CSPin = 10;

  

  const int moistpin = A0;

  const int photopin = A1;
  const int solarpin = A2;

  

  int pumppin = 5;
float TimeBuff()
{
  if(DS1307.eSquareWave_1Hz == DS1307.getSqwPinMode())
  {
    Serial.print("SQW/OUT pin: 1Hz | ");
  }
  uint16_t getTimeBuff[7] = {0};
  DS1307.getTime(getTimeBuff);
  char outputarr[128];
  sprintf(outputarr, "time: %d/%d/%d-%d, %d:%d:%d,\r\n",
        getTimeBuff[6],
        getTimeBuff[5],
        getTimeBuff[4],
        getTimeBuff[3],
        getTimeBuff[2],
        getTimeBuff[1],
        getTimeBuff[0]
        );
  float time6 = getTimeBuff[6];
  float time5 = getTimeBuff[5];
  float time4 = getTimeBuff[4];
  float time3 = getTimeBuff[3];
  float time2 = getTimeBuff[2];
  float time1 = getTimeBuff[1];
  float time0 = getTimeBuff[0];

}
  


void setup() 
{
  pinMode(moistpin,INPUT);
  pinMode(photopin, INPUT);
  pinMode(solarpin, INPUT);
  pinMode(pumppin, OUTPUT);
  digitalWrite(pumppin, LOW);

  Serial.print("Initialising SD card...");
  pinMode(CSPin, OUTPUT);

  if (!SD.begin(CSPin))
  {
    Serial.println("Initialisation failed.");
    return;
  }

  Serial.println("Initialisation done.");

  Serial.println("Setting up datasheet...");
  if(logfile)
  {
    string title = "Date","time", "Solar", "Humidity", "Temp", "Moisture";
    logfile.print(title);
    logfile.println();
    logfile.close();
  }

  Serial.println("Done");



  rtcup();

  Serial.begin(9600);
  dht.begin();
}

void rtcup()
{
  Serial.begin(9600);
  while(!(DS1307.begin()))
  {
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }

  Serial.println("Begin ok!");
  DS1307.start();

  DS1307.setSqwPinMode(DS1307.eSquareWave_1Hz);
}


void conversion(float sensorValue)
{
  float voltage = (float)sensorValue * 5 / 1023;
  float mv = voltage*1000;
  Serial.println("mv");
  delay(500);
}

void sdread()
{
  myFile = SD.open("test.txt");
  if (myFile)
  {
    Serial.println("test.txt:");

    while (myFile.available())
    {
      Serial.write(myFile.read());
    }
    myFile.close();
  } 
  else
  {
    Serial.println("error opening test.text");
  }
}

void solarread(float &sol, float &photo)
{
  sol = analogRead(solarpin);

  photo = analogRead(photopin);
}

float humidread()
{
  float humid = dht.readHumidity();
  Serial.print("Humidity = ");
  Serial.print((float)humid,1);
  Serial.print("%");

  return humid;
}

float tempread()
{
  float temp = dht.readTemperature();
  Serial.print("Temperature = ");
  Serial.print((float)temp, 1);
  Serial.print("C");

  return temp;
}

float moistread()
{
  float init = analogRead(moistpin);
  float moist = init/10.23;
  Serial.print(moist);

  return moist;
}

void loop()
{
  float TimeBuff;
  TimeBuff = TimeBuff();
  if(c==4)
  {
    float solar;
    solar = solarread(float &sol, float &photo);

    float humid;
    humid = humidread();

    float temp;
    temp = tempread();

    float moist;
    moist = moistread();

    infoLog(solarread(float &sol, float &photo), humidread(), tempread(), moistread());
  }
 }

void infoLog(float solar,float humid, float temp, float moist)
{
  logfile = SD.open("log.csv", FILE_WRITE);

  if(DS1307.eSquareWave_1Hz == DS1307.getSqwPinMode())
  {
    Serial.print("SQW/OUT pin: 1Hz | ");
  }

  uint16_t getTimeBuff[7] = {0};
  DS1307.getTime(getTimeBuff);
  char outputarr[128];
  sprintf(outputarr, "time: %d/%d/%d-%d, %d:%d:%d,\r\n",
        getTimeBuff[6],
        getTimeBuff[5],
        getTimeBuff[4],
        getTimeBuff[3],
        getTimeBuff[2],
        getTimeBuff[1],
        getTimeBuff[0]
        );

  delay(1000);

  if(logfile)
  {
    Serial.println("logging data...");
    Serial.print(outputarr);
    logfile.print(outputarr);
    Serial.print(solar);
    logfile.print(solar);
    Serial.print(",");
    logfile.print(",");  
    Serial.print(humid);
    logfile.print(humid);
    Serial.print(",");
    logfile.print(",");
    Serial.print(temp);
    logfile.print(temp);
    Serial.print(",");
    logfile.print(",");
    Serial.print(moist);
    logfile.print(moist);
    Serial.print(",");
    logfile.print(",");

    Serial.println();
    logfile.println();

    logfile.close();
    Serial.println("done.");
  }
  else
  {
    Serial.println("error opening text file");
  }
}

void gopump()
{
  float moist;
  moist = moistread();
  if(moist < 50){digitalWrite(pumppin, HIGH);}
  else{digitalWrite(pumppin, LOW);}

  int b = getTimeBuff[2];
    // else if(b == 4)
    // {
    //   digitalWrite(dirpin, HIGH); 
    // }
} 

void schedule()
{

  while(getTimeBuff[5] >=10 || getTimeBuff[5]<=3) //summer
  {
    int i = getTimeBuff[6];
    if(i==4||i==8||i==12||i==16||i==20||i==24||i==28||i==31){gopump();}
  }

  while(getTimeBuff[5] >=4 || getTimeBuff[5] <=9) //winter
  {
    int a = getTimeBuff[6];
    if(a==10||a==20||a==30){gopump();}
  }
}

