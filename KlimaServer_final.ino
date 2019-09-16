#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#include <IRremote.h>
#include <DHT.h>
#define DHTTYPE DHT22
#define DHTPIN 36

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   // Fizicka adresa MAC
byte ip[] = { 192, 168, 1, 135 };                      // Adresa
byte gateway[] = { 192, 168, 1, 1 };                   // Pristup internetu preko routera
byte subnet[] = { 255, 255, 255, 0 };                  // Podmrezna maska
EthernetServer server(80);                             // Server port     

String readString;
boolean isreqline = true;
DHT dht(DHTPIN, DHTTYPE);
IRsend irsend;
File myFile;

int khz = 38;
unsigned int  powerOn[147] = {4350,4300, 550,1600, 550,1600, 550,1600, 550,1550, 550,550, 550,500, 550,1600, 550,500, 600,500, 550,500, 550,550, 550,500, 550,1600, 550,1600, 550,500, 550,1600, 550,500, 550,550, 550,500, 550,550, 550,500, 550,500, 550,1600, 550,1600, 550,1600, 550,1600, 550,1550, 600,1550, 550,1600, 550,1600, 550,500, 550,550, 550,500, 550,550, 550,500, 550,500, 550,550, 550,500, 550,550, 550,1550, 550,550, 550,1600, 550,500, 550,1600, 550,500, 550,550, 550,500, 550,550, 500,550, 550,500, 550,550, 550,500, 550,550, 550,500, 550,500, 550,550, 550,500, 550,550, 550,500, 550,500, 600,500, 550,500, 550,550, 550,500, 550,500, 600,1550, 550,550, 550,1600, 550,500, 550,500, 550,550, 550,1600, 550};
unsigned int  powerOf[147] = {4350,4300, 550,1600, 550,1600, 550,1550, 550,1600, 550,550, 550,500, 550,1600, 550,500, 550,550, 550,500, 550,550, 550,500, 550,1600, 550,1600, 550,500, 550,1600, 550,500, 550,550, 550,500, 550,500, 550,550, 550,500, 550,1600, 550,1600, 550,1600, 550,1550, 550,1600, 550,1600, 550,1600, 550,1600, 550,500, 550,550, 550,500, 550,500, 550,550, 550,500, 550,550, 550,500, 550,500, 550,1600, 550,550, 550,1550, 550,550, 550,1600, 550,500, 550,500, 550,550, 550,500, 550,550, 550,1550, 550,550, 550,500, 550,550, 550,1600, 550,1550, 550,1600, 550,550, 550,500, 550,500, 550,550, 550,500, 550,550, 550,500, 550,500, 550,550, 550,500, 550,550, 550,1550, 550,550, 550,1600, 550,1550, 550,550, 550};        
unsigned int  tempUp[147] = {4350,4300, 550,1600, 550,1600, 550,1600, 600,1500, 550,550, 550,500, 550,1600, 550,550, 500,550, 550,500, 550,550, 550,500, 550,1600, 550,1600, 550,550, 500,1600, 550,550, 500,550, 550,500, 550,550, 500,550, 550,500, 550,1600, 550,1600, 550,1600, 550,1600, 550,1550, 550,1600, 550,1600, 550,1600, 550,550, 500,550, 550,500, 550,500, 600,500, 550,550, 500,550, 550,550, 500,550, 550,1550, 550,550, 550,1600, 550,1550, 550,550, 550,500, 550,550, 550,500, 550,500, 600,500, 550,500, 550,550, 550,500, 550,550, 550,500, 550,500, 550,550, 550,500, 550,550, 550,500, 550,500, 550,550, 550,500, 550,550, 550,500, 550,500, 550,1600, 550,1600, 550,550, 500,550, 550,500, 550,550, 550,1550, 550};  
unsigned int  tempDown[147] = {4350,4300, 550,1600, 550,1600, 550,1600, 550,1600, 550,500, 550,500, 550,1600, 550,550, 550,500, 550,500, 550,550, 550,500, 550,1600, 550,1600, 550,550, 500,1600, 550,550, 550,500, 550,500, 550,550, 550,500, 550,550, 550,1550, 550,1600, 550,1600, 550,1600, 550,1600, 550,1550, 550,1600, 550,1600, 550,550, 550,500, 550,500, 550,550, 550,500, 550,550, 550,500, 550,500, 550,550, 550,1600, 550,550, 500,1600, 550,500, 550,1600, 550,500, 600,500, 550,500, 550,550, 550,500, 550,550, 550,500, 550,500, 550,550, 550,500, 550,550, 550,500, 550,500, 550,550, 550,500, 550,550, 550,500, 550,500, 550,550, 550,500, 550,550, 550,1550, 550,550, 550,1600, 550,550, 500,500, 550,550, 550,1600, 550};  
unsigned int  fan[147] = {4300,4350, 550,1550, 600,1550, 550,1600, 550,1600, 550,500, 550,550, 550,1600, 550,500, 550,500, 550,550, 550,500, 550,550, 550,1550, 550,1600, 550,550, 550,1550, 550,550, 550,500, 550,550, 550,500, 550,550, 500,550, 550,1600, 550,1600, 500,1600, 550,1600, 550,1600, 550,1600, 550,1600, 550,1550, 550,550, 550,500, 550,550, 550,500, 550,500, 600,500, 550,500, 550,550, 550,500, 550,1600, 550,500, 550,1600, 550,550, 550,1550, 550,550, 550,500, 550,550, 550,500, 550,500, 550,1600, 550,550, 550,500, 550,500, 600,500, 550,500, 550,550, 550,500, 550,500, 600,500, 550,500, 550,550, 550,500, 550,550, 550,500, 550,500, 550,550, 550,500, 550,1600, 550,500, 550,550, 550,500, 550,1600, 550};  
unsigned int  mode[147] = {4350,4300, 550,1600, 500,1600, 550,1600, 550,1600, 550,500, 550,550, 550,1600, 550,500, 550,500, 550,550, 550,500, 550,550, 550,1550, 550,1600, 550,550, 550,1550, 600,500, 550,500, 550,550, 550,500, 550,550, 550,500, 550,1600, 550,1600, 550,1550, 550,1600, 550,1600, 550,1600, 550,1600, 550,1550, 600,500, 550,500, 550,550, 550,500, 550,550, 550,500, 550,500, 550,550, 550,500, 550,1600, 550,500, 550,1600, 550,550, 550,1550, 550,550, 550,500, 550,550, 550,500, 550,500, 550,1600, 550,550, 550,500, 550,550, 550,500, 550,500, 550,1600, 550,550, 550,500, 550,500, 550,550, 550,500, 550,550, 550,500, 550,500, 550,550, 550,500, 550,550, 550,1550, 550,550, 550,500, 550,550, 550,500, 550}; 
unsigned int  timer[179] = {4350,4300, 550,1600, 550,1600, 550,1550, 550,1600, 550,550, 550,500, 550,1600, 550,500, 550,550, 550,500, 550,500, 550,550, 550,1600, 550,1550, 550,550, 550,1600, 550,500, 550,500, 550,550, 550,500, 550,550, 550,1550, 600,500, 550,1600, 550,1600, 500,1600, 550,1600, 550,1600, 550,1600, 550,500, 550,1600, 550,500, 550,550, 550,500, 550,550, 550,500, 550,500, 550,550, 550,1600, 550,1550, 550,550, 550,1600, 550,500, 550,1600, 550,500, 550,550, 550,500, 550,500, 550,550, 550,1600, 550,500, 550,500, 600,500, 550,500, 550,550, 550,1550, 550,550, 550,500, 550,550, 550,500, 550,550, 550,500, 550,500, 550,550, 550,500, 550,550, 550,500, 550,500, 550,1600, 550,1600, 550,500, 550,550, 550,500, 550,1600, 550,1600, 550,1600, 550,1550, 550,1600, 550,1600, 550,1600, 550,500, 550,1600, 550,1600, 550,500, 550,550, 550,500, 550,500, 550,1600, 550};

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // Pokretanje Ethernet veze i servera:
  dht.begin();
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP()); // Ispisuje IP adresu servera
  pinMode(53, OUTPUT);
  digitalWrite(53, HIGH);
  Serial.println("Inicijalizacija SD kartice:");

  if (!SD.begin(4))
  {
    Serial.println("ERROR - Neuspjesna inicijalizacija SD kartice!");
    return;
  }
  Serial.println("SUCCESS - Uspjesna inicijalizacija SD kartice.");
}


void loop() {
  // Slusaj na dolazak klijenata
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // Http zahtjev zavrsava sa praznom linijom
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        //Cita znak po znak http zahtjeva
        if ((isreqline) && readString.length() < 100) {
          //Spremi znakove u string
          readString += c;
        }
        if (c == '\n') isreqline = false;

        //Ako je http zahtjev zavrsio
        if (c == '\n' && currentLineIsBlank) {
          currentLineIsBlank = true;

          Serial.println(readString); //Ispisi na Serial monitor
          if (readString.indexOf("cmd=test") > 0) send_JSON(client);

          else
          {

            send_Web_page(client);

            if (readString.indexOf("?cmd=AC_ON") > 0){
              irsend.sendRaw(powerOn, 147, khz);

            }
            if (readString.indexOf("?cmd=AC_OFF") > 0){
              irsend.sendRaw(powerOf, 147, khz);

            }
            if (readString.indexOf("cmd=MODE") > 0){
              irsend.sendRaw(mode, 147, khz);
            }

            if (readString.indexOf("cmd=TEMP_UP") > 0){
              irsend.sendRaw(tempUp, 147, khz);
            }
            if (readString.indexOf("cmd=TEMP_DOWN") > 0){
              irsend.sendRaw(tempDown, 147, khz);
            }
            if (readString.indexOf("cmd=FAN") > 0){
              irsend.sendRaw(fan, 147, khz);
            }
            if (readString.indexOf("cmd=TIMER_UP") > 0){
              irsend.sendRaw(timer, 179, khz);
            }
            if (readString.indexOf("cmd=TIMER_DOWN") > 0){
              irsend.sendRaw(timer, 179, khz);
            }

          }
          //Ocisti string za sljedece citanje
          readString = "";
          isreqline = true;
          break;
        }
        if (c == '\n') {
          // Zapocni novu liniju
          currentLineIsBlank = true;
        }

        else if (c != '\r'){
          // Imamo znak na kraju linije
          currentLineIsBlank = false;
        }
      }
    }
    // Daj internet pregledniku vremena da primi podatke
    delay(1);
    // zatvori vezu
    client.stop();
  }
}
static void send_JSON(EthernetClient client) {
  int t = dht.readTemperature();
  int h = dht.readHumidity();
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");  // JSON response type
  client.println("Connection: close");               // close connection after response
  client.println();
  // open JSON
  client.println("{");
  // result header
  client.println("\"res\":");
  client.println("[");
  client.println("{");
  // temperature
  client.print("\"temp\":");
  client.print("\"");
  client.print(t);
  client.print("\"");
  client.println(",");
  // humidity
  client.print("\"humi\":");
  client.print("\"");
  client.print(h);
  client.println("\"");
  client.println("}");
  client.println("]");
  // close json
  client.println("}");
}

static void send_Web_page(EthernetClient client){

  int t = dht.readTemperature();
  int h = dht.readHumidity();
  client.println("HTTP/1.1 200 OK"); //Pošalji standardni http odgovor
  client.println("Content-Type: text/html");
  client.println();
  client.print("<HTML>");
  client.print("<HEAD>");
  client.print("<style>");

  myFile = SD.open("klima.css"); // Otvori dokument klima.css
  if (myFile)
  {
    Serial.println("klima.css:");

    // cita dokument sve dok nedojde do kraja:
    while (myFile.available())
    {
      client.write(myFile.read());

    }
    // zatvori dokument:
    myFile.close();
  }
  else
  {
    // Ako se dokument nije otvorio ispisi gresku :
    Serial.println("Greska otvaranja klima.css");
  }

  client.print("</style>");
  client.print("<meta name='apple-mobile-web-app-capable' content='yes' />");
  client.print("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
  client.print("<TITLE>Climate Control</TITLE>");
  client.print("</HEAD>");
  client.print("<BODY>");
  client.print("<div id=\"container\">");
  client.print("<H1>Climate Control</H1>");
  client.print("<hr />");
  client.print("<div class=\"th\">");
  client.print("<H2>Temperature:");
  client.print(t);
  client.print("&#8451");
  client.print("</H2>");
  client.print("<H2>Humidity: ");
  client.print(h);
  client.print("%");
  client.print("</H2>");
  client.print("</div>");
  client.print("<br />");
  client.print("<br />");
  client.print("<div class=\"FirstButtons\">");
  client.print("<a href=\"/?cmd=AC_ON\"\"> ON </a>");
  client.print("<a href=\"/?cmd=FAN\"\">FAN</a><br />");
  client.print("</div>");
  client.print("<br />");
  client.print("<br />");
  client.print("<br />");
  client.print("<div class=\"SecondButton\">");
  client.print("<a href=\"/?cmd=TEMP_UP\"\">TEMP +</a>");
  client.print("</div>");
  client.print("<br />");
  client.print("<br />");
  client.print("<br />");
  client.print("<div class=\"ThirdButtons\">");
  client.print("<a href=\"/?cmd=TIMER_UP\"\">TIMER +</a>");
  client.print("<a href=\"/?cmd=MODE\"\">MODE</a>");
  client.print("<a href=\"/?cmd=TIMER_DOWN\"\">TIMER -</a>");
  client.print("</div>");
  client.print("<br />");
  client.print("<br />");
  client.print("<br />");
  client.print("<div class=\"SecondButton\">");
  client.print("<a href=\"/?cmd=TEMP_DOWN\"\">TEMP +</a>");
  client.print("</div>");
  client.print("<div id=\"linebottom\">");
  client.print("<hr />");
  client.print("</div>");
  client.print("<div id=\"dno\">Sva prava pridrzana <br /><b>Kristijan Cotic&#169;</b></div>");
  client.print("<br />");
  client.print("</div>");
  client.print("</BODY>");
  client.print("</HTML>");
}
