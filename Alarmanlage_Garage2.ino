#include "U8glib.h"
//#include "EEPROM.h"
#include "SoftwareSerial.h"
U8GLIB_PCD8544 u8g(12, 11, 10, 9, 8);
SoftwareSerial myserial (18, 19); // RX, TX Verbindung von SIM <-> Arduino


const uint8_t rook_bitmap[] U8G_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x03, 0xC0, 0x00, 0x00,
  0x00, 0x00, 0x7F, 0xFE, 0x00, 0x00,
  0x00, 0x03, 0xFF, 0xFF, 0xC0, 0x00,
  0x00, 0x0F, 0xFF, 0xFF, 0xF0, 0x00,
  0x00, 0x1F, 0xFF, 0xFF, 0xF8, 0x00,
  0x00, 0x7F, 0xFF, 0xFF, 0xFE, 0x00,
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
  0x01, 0x7F, 0xFF, 0xFF, 0xFF, 0x80,
  0x03, 0x3F, 0xFF, 0xFF, 0xFF, 0xC0,
  0x06, 0x1F, 0xFF, 0xFF, 0xFF, 0xC0,
  0x04, 0x0F, 0xFF, 0xFF, 0xFF, 0xE0,
  0x08, 0x07, 0xFF, 0xFF, 0xFF, 0xF0,
  0x08, 0x00, 0x1F, 0xFF, 0xFF, 0xF0,
  0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF8,
  0x01, 0x80, 0x00, 0x03, 0xFF, 0xF8,
  0x07, 0xE0, 0x00, 0x01, 0xFF, 0xFC,
  0x1C, 0x30, 0x0F, 0xC0, 0x1F, 0xFC,
  0x3C, 0x18, 0x10, 0x20, 0x00, 0x1C,
  0x18, 0x04, 0x08, 0x00, 0x00, 0x0C,
  0x0C, 0x06, 0x04, 0x00, 0x00, 0x04,
  0x07, 0x03, 0x00, 0x00, 0x00, 0x0C,
  0x03, 0x81, 0x80, 0x1F, 0x00, 0x0E,
  0x00, 0x81, 0x00, 0xE0, 0xE0, 0x0E,
  0x00, 0xC0, 0x01, 0xC0, 0x18, 0x0E,
  0x00, 0x60, 0x07, 0x01, 0xFF, 0x1C,
  0x00, 0x20, 0x0C, 0x20, 0xAA, 0x1C,
  0x00, 0x30, 0x10, 0x30, 0x00, 0x3C,
  0x00, 0x10, 0x30, 0x1F, 0xFC, 0x7C,
  0x00, 0x18, 0x10, 0x1F, 0xFC, 0xFC,
  0x00, 0x3C, 0x10, 0x1F, 0xFF, 0xFC,
  0x00, 0x04, 0x18, 0x1F, 0xFF, 0xF8,
  0x00, 0x06, 0x08, 0x1F, 0xFF, 0xF8,
  0x00, 0x0F, 0x04, 0x1F, 0xFF, 0xF8,
  0x00, 0x01, 0x86, 0x1F, 0xFF, 0xF0,
  0x00, 0x00, 0x03, 0x30, 0x0F, 0xF0,
  0x00, 0x00, 0x01, 0x00, 0x0F, 0xE0,
  0x00, 0x00, 0x00, 0xC0, 0x07, 0xC0,
  0x01, 0x00, 0x00, 0x40, 0x07, 0xC0,
  0x01, 0x80, 0x60, 0x00, 0x0F, 0x80,
  0x00, 0x83, 0xE0, 0x00, 0x0F, 0x00,
  0x00, 0x7F, 0xFE, 0x00, 0x1E, 0x00,
  0x00, 0x1F, 0xFF, 0xFF, 0xF8, 0x00,
  0x00, 0x0F, 0xFF, 0xFF, 0xF0, 0x00,
  0x00, 0x03, 0xFF, 0xFF, 0xC0, 0x00,
  0x00, 0x00, 0x7F, 0xFE, 0x00, 0x00,
  0x00, 0x00, 0x01, 0xE0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};








int INIT = 1; //
int TOR3 = 0;
int TOR4 = 0;
int MOTION = 0;
int counter = 0;
int SEC = 15, offtime = 15, openstate = 0;
int a = 16;
bool ABORT_INTERRUPT = false;
int sendingsms = 0;
int longtimemotion = 0;
int uptime = 0;
unsigned long timereset, timetocount, requesttime, hourcount;
String network;


// STATES
const int INITIAL_STATE = 1000;
const int TOR_3_OPEN_STATE = 2001;
const int TOR_4_OPEN_STATE = 2002;
const int ABORT_STATE = 3000;

int current_state = INITIAL_STATE;


// Config
const long SHOW_WELCOME_LOG = 1000;
const int ALARMDAUER = 2000;

const int BEWEGUNGSMELDER_HW_PIN = 5;
const int TOR_4_HW_PIN = 6;
const int TOR_3_HW_PIN = 7;


const int STATUS_LED_GREEN = 14;
const int STATUS_LED_RED = 15;
const int SIRENE = 4;

/*
   Show welcome Logo on initialze
*/
void showDemo() {

  if (INIT == 1) {
    u8g.firstPage();
    do {
      u8g.drawBitmapP(16, 0, 6, 48, rook_bitmap);
    } while (u8g.nextPage());

    delay(SHOW_WELCOME_LOG);

    INIT = 0;
  }
}


/**
   Setup hook
*/
void setup() {

  pinMode(7, INPUT);
  pinMode(6, INPUT);
  pinMode(5, INPUT);
  pinMode(2, INPUT);
  pinMode(4, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);

  // ISR = Interrupt service routine
  attachInterrupt(0, abort_alarm_interrupt_service_routine, HIGH);



  Serial.begin(9600); // Verbindung zwischen Arduino <-> Rechner
  Serial.println("SIM800 AT CMD Test");
  myserial.begin(9600);
  requesttime = millis();
  hourcount = millis();



}
/**
   Digital Read
   1 = N/A
   2 = N/A
   3 = N/A
   4 = N/A
   5 = N/A
   6 = N/A
   7 = N/A
   8 = N/A
   9 = N/A
*/


bool is_on(int digital_pin){
  return (digitalRead(digital_pin) == HIGH);
}

bool is_off(int digital_pin){
  return (digitalRead(digital_pin) == LOW);
}

void set_on(int digital_pin){
  digitalWrite(digital_pin, HIGH);
}

void set_off(int digital_pin){
  digitalWrite(digital_pin, LOW);
}

/**
   Loop hook

*/
void loop() {
  if (myserial.available())
    Serial.write(myserial.read()); // Ausgabe auf Rechner  LOG
  if (Serial.available())
    myserial.write(Serial.read());

  // initial demo
  showDemo();


  // Totmanschalter
  if (!ABORT_INTERRUPT) {
    if (is_on(TOR_3_HW_PIN)) {
      TOR3 = 1;
      openstate = 1;
    }
    else TOR3 = 0;

    if (is_on(TOR_4_HW_PIN)) {
      TOR4 = 1;
      openstate = 1;
    }
    else TOR4 = 0;

    if (is_on(BEWEGUNGSMELDER_HW_PIN)) {
      MOTION = 1;
      if (millis() - timetocount > 1000) {
        counter++;
        longtimemotion++;
        timetocount = millis();
      }
      timereset = millis();

    }
    else {
      MOTION = 0;
      timetocount = millis();
    }


    if ((counter > 0) && (millis() - timereset > 20000)) counter = 0;




    screen();

    if (TOR3 == 1) ALARMOFFTIMER();
    if (TOR4 == 1) ALARMOFFTIMER();
    if (counter >= 4) ALARMOFFTIMER();



  }



  if (ABORT_INTERRUPT) {
    set_on(STATUS_LED_GREEN);

    if ((is_off(TOR_3_HW_PIN)) && (is_off(TOR_4_HW_PIN))) {
      TOR3 = 0;
      TOR4 = 0;
      ABORT_INTERRUPT = false;
      set_off(STATUS_LED_GREEN);

    }
  }

  screen();
  offtime = 15;
  if ((openstate == 1) && (ABORT_INTERRUPT == false)) SCHARFSCHALTUNG();
  if (millis() - requesttime > 15000) {

    network = get_network();

  }
  if (millis() - hourcount > 60000) {
    uptime++;
    hourcount = millis();
  }

}





void screen() {
  if (!ABORT_INTERRUPT) {
    u8g.firstPage();
    do {
      u8g.setFont(u8g_font_5x7);
      if (TOR3 == 1) {
        u8g.drawStr(0, 7, "TOR3 AUF");
        u8g.drawStr(0, 48, "ALARM");
      }

      if (TOR3 == 0) u8g.drawStr(0, 7, "TOR3 ZU");

      if (TOR4 == 1) {
        u8g.drawStr(0, 15, "TOR4 AUF");
        u8g.drawStr(0, 48, "ALARM");
      }
      if (TOR4 == 0) u8g.drawStr(0, 15, "TOR4 ZU");

      if (MOTION == 1) {
        u8g.drawStr(0, 22, "Bewegung");
        u8g.drawStr(0, 30, "erkannt");
      }
      if ((counter == 1) && (TOR3 == 0) && (TOR4 == 0)) u8g.drawStr(0, 48, ".");
      if ((counter == 2) && (TOR3 == 0) && (TOR4 == 0)) u8g.drawStr(0, 48, "..");
      if ((counter == 3) && (TOR3 == 0) && (TOR4 == 0)) u8g.drawStr(0, 48, "...");
      if (counter >= 4) u8g.drawStr(0, 48, "ALARM");

      if (a < 16) {
        u8g.setPrintPos(73, 48);
        u8g.print(SEC);
      }
      int net = network.toInt();

      if (net == 0) u8g.drawLine(72, 6, 72, 6);

      if (net == 20) {
        u8g.drawLine(72, 6, 72, 6);
        u8g.drawLine(74, 6, 74, 5);
      }

      if (net == 40) {
        u8g.drawLine(72, 6, 72, 6);
        u8g.drawLine(74, 6, 74, 5);
        u8g.drawLine(76, 6, 76, 4);
      }
      if (net == 60) {
        u8g.drawLine(72, 6, 72, 6);
        u8g.drawLine(74, 6, 74, 5);
        u8g.drawLine(76, 6, 76, 4);
        u8g.drawLine(78, 6, 78, 3);
      }

      if (net == 99) {
        u8g.drawLine(72, 6, 72, 6);
        u8g.drawLine(74, 6, 74, 5);
        u8g.drawLine(76, 6, 76, 4);
        u8g.drawLine(78, 6, 78, 3);
        u8g.drawLine(80, 6, 80, 2);
      }


      u8g.drawLine (69, 6, 69, 1);
      u8g.drawLine(68, 1, 70, 1);
      u8g.drawLine(67, 1, 68, 3);
      u8g.drawLine(71, 1, 70, 3);
      u8g.setFont(u8g_font_4x6);
      u8g.setPrintPos(56, 7);
      u8g.print(net);
      if (net > 10) u8g.drawStr(50, 15, "congstar");

      if (sendingsms == 1) {
        u8g.drawLine (60, 25, 60, 20);
        u8g.drawLine (59, 21, 61, 21);
        u8g.drawLine(58, 22, 62, 22);
        u8g.drawLine(57, 23, 63, 23);
        u8g.drawStr(38, 32, "sending sms");
      }

      u8g.setPrintPos (35, 48);
      u8g.print(longtimemotion);
      u8g.setPrintPos (42, 25);
      u8g.print("uptime:");
      u8g.setPrintPos(71, 25);
      u8g.print(uptime);

    } while (u8g.nextPage());
  }

  if (ABORT_INTERRUPT) {
    u8g.firstPage();
    do {
      u8g.setFont(u8g_font_5x7);
      u8g.drawStr ( 26, 22, "ALARM");
      u8g.drawStr ( 10, 30, "abgeschalten");
      u8g.drawStr ( 12, 38, "bis Tore zu");
      u8g.setFont(u8g_font_4x6);
      u8g.drawStr(0, 48, "motion:");
      u8g.setPrintPos (28, 48);
      u8g.print(longtimemotion);
      u8g.setPrintPos (42, 6);
      u8g.print("uptime:");
      u8g.setPrintPos(71, 6);
      u8g.print(uptime);
    } while (u8g.nextPage());
  }


}



void ALARMOFFTIMER() {

  for ( a = 0; a <= 15; a++) {
    if (ABORT_INTERRUPT) break;
    if (a > 9) set_on(STATUS_LED_RED);
    screen();
    SEC--;
    delay (1000);
  }
  set_off(STATUS_LED_RED);
  SEC = 15;
  if (!ABORT_INTERRUPT) ALARM();
}



void abort_alarm_interrupt_service_routine() {
  ABORT_INTERRUPT = true;
  a = 16;
  counter = 0;

}

void ALARM() {
  sendingsms = 1;
  screen();
  myserial.print("AT+CMGF=1\r");
  delay(1000);
  //  myserial.print("AT+CMGS=\"+491759858616\"\r");
  delay(1000);
  if (TOR3 == 1) myserial.println("ALARM!!! Garage TOR 3 offen");
  if (TOR4 == 1) myserial.println("ALARM!!! Garage TOR 4 offen");
  if (counter >= 4) myserial.println("ALARM!!! Garage Bewegungsmelder");
  myserial.print("\r");
  delay(1000);
  myserial.println((char)26);
  myserial.println();
  delay(2000);

  /////
  myserial.print("AT+CMGF=1\r");
  delay(1000);
  //  myserial.print("AT+CMGS=\"+491777114557\"\r");
  delay(1000);
  if (TOR3 == 1) myserial.println("ALARM!!! Garage TOR 3 offen");
  if (TOR4 == 1) myserial.println("ALARM!!! Garage TOR 4 offen");
  if (counter >= 4) myserial.println("ALARM!!! Garage Bewegungsmelder");
  myserial.print("\r");
  delay(1000);
  myserial.println((char)26);
  myserial.println();
  delay(2000);
  sendingsms = 0;
  screen();

 
    set_on(SIRENE);
    delay(ALARMDAUER);
    set_off(SIRENE);
   

 
}


void SCHARFSCHALTUNG() {
  set_on(SIRENE);
  delay(50);
  set_off(SIRENE);
  delay(175);
  set_on(SIRENE);
  delay(50);
  set_off(SIRENE);
  delay(100);
  openstate = 0;

}

/**
 * Die Funktion baut eine Netzwerkverbindung auf und führt 
 * ein AT Kommando Request aus. Bei einem Timeout wir ein
 * leerer String zurückgegeben.
 *
 * @param command -  Das auszuführende AT command Request 
 * @return 
 * - das Ergebnis(response) einen AT Command
 * - empty string "" bei Netzwerktimeout
 */
String getCommandResult(String command) {

  String response = "";

  myserial.println(command);


  for (unsigned long start = millis(); millis() - start < 1000;) {
    while (myserial.available())
    {
      response = myserial.readString();
      break;
    }
  }

  return response;
}

int get_network() {

  requesttime = millis();

  String buff = "";
  //buff = "+CSQ: 16,0";
  unsigned int result, index1, index2, timeout = 0;


  buff = getCommandResult("AT+CSQ");



  if (buff == "")
  {
    return 0;
  }
  Serial.println("***START***");
  Serial.println(buff);
  Serial.println("***END***");
  //String network_status;
  //_____________________________________________________
  //Remove sent "AT Command" from the response string.
  index1 = buff.indexOf("\r");
  buff.remove(0, index1 + 2);

  buff.trim();
  //_____________________________________________________

  //_____________________________________________________

  index1 = buff.indexOf(":"); //3
  index2 = buff.indexOf(","); //7
  buff = buff.substring(index1 + 1, index2);
  buff.trim();
  result = buff.toInt();


  if (result == 99)
  {
    //not known or not detectable
    return 0;
  }
  else if (result >= 2 && result <= 9)
  {
    //Signal Quality = Marginal
    return 20;
  }
  else if (result >= 10 && result <= 14)
  {
    //Signal Quality = OK
    return 40;
  }
  else if (result >= 15 && result <= 19)
  {
    //Signal Quality = Good
    return 60;
  }
  else if (result >= 20 && result <= 31)
  {
    //Signal Quality = Excellent
    return 99;
  }

  return 0;

}

int get_network2() {

  requesttime = millis();

  String buff;
  //buff = "+CSQ: 16,0";
  unsigned int result, index1, index2, timeout = 0;

  myserial.println("AT+CSQ");

  // 2000 Millis
  for (unsigned long start = millis(); millis() - start < 1000;) {
    while (myserial.available())
    {
      buff = myserial.readString();
      timeout = 1;
      break;
    }
  }
  /**
    AT+CSQ
    +CSQ: 12,0

    OK

  */

  if (timeout == 0)
  {
    return 0;
  }
  Serial.println("***START***");
  Serial.println(buff);
  Serial.println("***END***");
  //String network_status;
  //_____________________________________________________
  //Remove sent "AT Command" from the response string.
  index1 = buff.indexOf("\r");
  buff.remove(0, index1 + 2);

  buff.trim();
  //_____________________________________________________

  //_____________________________________________________

  index1 = buff.indexOf(":"); //3
  index2 = buff.indexOf(","); //7
  buff = buff.substring(index1 + 1, index2);
  buff.trim();
  result = buff.toInt();


  if (result == 99)
  {
    //not known or not detectable
    return 0;
  }
  else if (result >= 2 && result <= 9)
  {
    //Signal Quality = Marginal
    return 20;
  }
  else if (result >= 10 && result <= 14)
  {
    //Signal Quality = OK
    return 40;
  }
  else if (result >= 15 && result <= 19)
  {
    //Signal Quality = Good
    return 60;
  }
  else if (result >= 20 && result <= 31)
  {
    //Signal Quality = Excellent
    return 99;
  }

  return 0;

}
