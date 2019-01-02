#define waterPin D5    //  Connect WATER Meter, dont forget Ground also
////////////////////////////////////////////////////
int waterPerPulse =              1 ;            // (gallons) Water Meter's Pulse Value
int interval      =              1   ;            // (min) Publish Interval in MINUTES
unsigned long eepromInterval =  60 * 60000;        //8  * 60 * 60000 (hours) How often to Save Totalizer Values to EEPROM milliseconds
#define DEBOUNCE_DELAY           100             // (ms)  min time for Debounce, set similar (but higher) to Water Meter Pulse Width Setting.
/////////////////////////////////////////////////////////////////////////////////////
char msg[256]                           ;        // used for snprintf for Publish.
/////////////////////////////////////////////////////////////////////////////////////
unsigned long previousPublish =     0  ;
unsigned long  waterTotal =         0  ;   //  Saved to EEPROM
float waterFlowRate =               0  ;   //  Calculated by dividing the Gallons during Interval  By Interval (500 Gallons / 5 minutes = 100 GPM)
volatile bool newData =           true ;   // Flag set by Interrupts
volatile int waterCountOneInterval = 0 ;   // Flushed after every Interval / Publish
volatile uint32_t lastWaterIntTime = 0 ;
unsigned long previousEEPROM =       0 ;
/////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  pinMode(waterPin, INPUT_PULLUP);
// NOTE :  IT'S MORE COMMON TO USE AN EXTERNAL 4.7k PULLUP RESISTOR AND  pinMode(waterPin, INPUT) INSTEAD.  IE Not INPUT_PULLUP .  But this is fine for bench testing
  attachInterrupt(waterPin, waterInterrupt, FALLING);
//if the EEPROM value isn't non-written (i.e. the default value ) then get it
    if ((EEPROM.get(0, waterTotal)) !=4294967295) ;
      {   EEPROM.get(0, waterTotal);
      }
 }
void loop() {
  if (millis() - previousPublish >= (interval * 60000) )
  {
  // It's time to Publish the data,
    waterTotal = waterTotal + (waterCountOneInterval * waterPerPulse) ;
    waterFlowRate = (waterCountOneInterval * waterPerPulse) / interval  ;
    publishDataToCloud();    //publishDataToCloud
    printToSerial();        // Print all info to serial
// Photon returns here after printToSerial
    waterCountOneInterval = 0 ;  // Flushed after every Interval / Publish, since the Interval has ellapsed.
    previousPublish = millis(); //commented this out, because if we are clearing here we will never have to correct interval to write to eeprom. clear after that instead
    newData = false;            // Turn Off the Publish Flag, since we just pubilished.
  if (millis() - previousEEPROM >= eepromInterval )
    {      // Write to EEPROM every HOUR, Day, etc. cannot make the same at the publish interval
    EEPROM.put(0, waterTotal);
    previousEEPROM = millis();
    }
  }
}
void waterInterrupt() {           // Interrupt for Water Meter
  if (millis() - lastWaterIntTime >= DEBOUNCE_DELAY )
    {
    waterCountOneInterval++;    // Tracks the # of Pulses from Meter for this time interval ONLY
    newData = true;             // Set Global Flag because interrupt has fired
    lastWaterIntTime = millis();
    }
}
void printToSerial() {
  Serial.println("++++++++++++++++");
  Serial.print("WATER Intur: ");    Serial.println(waterCountOneInterval);
  Serial.print("WATER Flow: ");    Serial.println(waterFlowRate, 0);
  Serial.print("TOTALIZER:");    Serial.println(waterTotal);
  Serial.println("----------------");
  return;
}
void publishDataToCloud()
{
//lu is for unsigned long, f is for float
snprintf(msg, sizeof(msg), "{\"waterTotal\":%lu, \"waterFlowRate\":%f}", waterTotal, waterFlowRate);
Particle.publish("Database", msg, PRIVATE, NO_ACK);
 return;
 }
