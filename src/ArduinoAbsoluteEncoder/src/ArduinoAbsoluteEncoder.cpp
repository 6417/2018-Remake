/*
*    Grundinformationen zum Encoder
*    ------------------------------
*    
*    Der Code ist für den AbsEncoderI2C V2 abgestimmt.
*        
*        
*    I2C Infos:
*        Slaveadresse:
*            Die Slaveadresse wird über den Drehschalter auf dem Print eingestellt.
*            Nach der Änderung der Adresse muss der Sensor neugestartet werden.
*            Neustart kann per Knopfdruck auf dem Sensor durchgeführt werden (Reset auf dem Arduino)
*        
*        Register:
*            Adresse | Lesen | Schreiben | Beschreibung 
*            --------+-------+-----------+----------------
*             0x00   | JA    | NEIN      | RETURN_ABS_POSITION  -> Auslesen der absoluten Sensorposition. 1 bytes Wertebereich 0 - 127
*             0x01   | JA    | JA        | SET_HOME             -> Setzen oder auslesen der Home Position. 1 bytes Wertebereich 0 - 127. 
*                    |       |           |                         Wenn Wert > 127, dann wird automatisch 128 subtrahiert,
*                    |       |           |                         um in den Wertebereich zu gelangen.
*             0x02   | JA    | NEIN      | RETURN_REL_POSITION  -> Auslesen der relativen Position zur Home Position. 1 bytes Wertebereich 0 - 127
*             0x10   | JA    | NEIN      | RETURN_CURRENT_ERROR -> Auslesen der aktuell aufgetretenen Fehlermeldung. 1 bytes (Siehe Fehlermeldungen)
*             0x11   | JA    | NEIN      | RETURN_LAST_ERROR    -> Auslesen des letzten Fehlers. 1 bytes (Siehe Fehlermeldungen)
*             0x12   | NEIN  | JA        | CLEAR_ERROR          -> Löscht beide Fehlerregister.
*             0x20   | JA    | NEIN      | RETURN_VERSION       -> Auslesen der aktuellen Softwareversion. 1 bytes Wertebereich 0 - 255
*             0x30   | JA    | NEIN      | RETURN_ALL_POSITIONS -> Auslesen aller Positionen. (3 bytes): ABS,HOME,REL
*             0x31   | JA    | NEIN      | RETURN_ALL_REGISTERS -> Auslesen aller lesbaren Register. (6 bytes): ABS,HOME,REL,ERR1,ERR2,VERS
*             
*    Fehlermeldungen:
*        Sollten Fehler während dem Betrieb erkannt werden, so wird der Fehlercode 3 mal über die LED rot 
*        blinkend ausgegeben. Die anzahl an Blinkzyklen entspricht dem Fehlercode.
*        Zwischen den 3 Wiederholungen gibts eine kurze Pause.
*        
*        Die Fehler verschwinden automatisch und es sind immer nur die neusten zwei Fehler vorhanden.
*        Die Fehlerregister können über I2C gelöscht werden. (Siehe I2C Infos)
*        
*        Fehlercode  | Bedeutung
*        ------------+--------------------------
*         0          | Kein Fehler vorhanden
*         2          | BAD_I2C_REGISTER_REQUEST    -> Es wurde versucht ein Register zu lesen, welches nicht vorhanden ist.
*                    |                                Es wurde versucht auf ein Register zu schreiben, welches WRITE ONLY ist.
*         3          | BAD_I2C_REGISTER_ACCESS     -> Es wurde versucht auf ein Register zu schreiben, welches nicht vorhanden ist.
*                    |                                Es wurde versucht auf ein Register zu schreiben, welches READ ONLY ist.
*         4          | I2C_TO_MUCH_DATA_RECIEVED   -> Es wurden zu viele bytes an ein beschreibbares Register gesendet.
*         5          | I2C_TO_LESS_DATA_RECIEVED   -> Es wurden zu wenige bytes an ein beschreibbares Register gesendet. (wird noch nirgends verwendet)
*
*
*    Status LED:
*        Über die LED werden diverse Informationen dargestellt.
*        
*        Farben:    
*            Grün:    Die grüne LED signalisiert Datenaustausch über den I2C Bus. 
*                    Die LED ist dabei 100ms eingeschaltet nachdem die Kommunikation stattgefunden hat.
*                    
*                    Wenn sie blinkt, werden Daten in einem grossen Zeitintervall ausgetauscht.
*                    Leuchtet sie konstannt, wird der Bus stark frequentiert.
*                    
*                    | Signal:
*                    |          |<-- 100ms -->|          |<-- 100ms +               ---->|
*                    |                                  
*                    | Grün     |-------------|          |-------------------------------|
*                    |          |             |          |                               |
*                    | 0  ------|             |----------|                               |---------
*                    |          
*                    |          |                        |      |       |   |      
*                    | (I2C Kommunikation)             (I2C)  (I2C)   (I2C)(I2C)
*                    
*            Blau:   Die Blaue LED signalisiert blinkend, dass sich der Encoder in der Home Position befindet.
*                    Die LED leuchtet, sobald sich der Encoder +- 5 Ticks um die Home Position befindet.
*                    Die LED blinkt dabei in einem 200ms Takt.
*                    
*            Orange: Die Orange LED signalisiert, dass seit 5 sekunden keine I2C kommunikation stattgefunden hat.
*                    Die LED verändert dabei konstant die helligkeit und zeigt so den IDLE modus. (Sensor läuft, wird aber nicht verwendet)
*                    
*            Rot:    Fehler werden durch ein rotes Blinksignal dargestellt.
*                    Die anzahl der Blinkzyklen entspricht dem Fehlercode.
*                    Der gleiche Fehler wird 3 mal nacheinander dargestellt und wird jeweils durch eine kleine Pause voneinander getrennt.
*                    
*                    | Signal:
*                    |          |----    Fehlercode 2     ----|                  |---- 1. Wiederholung     ----|
*                    |
*                    |          |<-300ms->|<-300ms->|<-300ms->|<--   600ms    -->|<-300ms->|<-300ms->|<-300ms->|
*                    |                                                                            
*                    | Rot      |---------|         |---------|                  |---------|         |---------|
*                    |          |         |         |         |                  |         |         |         |
*                    | 0  ------|         |---------|         |------------------|         |---------|         |------
*                    |          
*                    |          |                         
*                    |  (Fehler ist aufgetreten: code 2)            
*/


// Aktiviere die Serielle Schnittstelle mit debug Ausgaben.
// Durch das Aktivieren der debug Funktion, müssen beide Jumper an 
// RX und TX entfernt werden. 
// Es können dadurch auch keine korrekten Positionsangaben ausgelesen werden.
// Nach der Fehlersuche muss der debugmodus entfernt werden, 
// da ansonnsten das Encodersignal gestört werden kann.
#define DEBUG
#ifdef DEBUG
// Baudrate für die Konsole
 #define SERIAL_BAUD 115200
#endif

// Es kann zwischen zwei modis gewechselt werden.
// USE_TIMED_UPDATE      -> Liest den Sensor zyklisch ein, 
//                          auch wenn die Werte nicht an den Master gesendet werden.
//                          Diese option ist bevorzugt, da die Status LED auf stetige updates des 
//                          Sensors angewiesen ist.
// 
// USE_UPDATE_ON_REQUEST -> Liest den Sensor ein, wenn der Master nach neuen Werten fragt.
#define USE_TIMED_UPDATE
#define USE_UPDATE_ON_REQUEST



#include <Wire.h>
#include <EEPROM.h>
#include "AbsoluteEncoderI2C.h"
#include "AbsoluteEncoder8bit.h"
#include "error.h"

// Das Programm verwendet Timer, darum dürfen keine delays verwendet werden.
// Die Timer sind nicht interrupt gesteuert und werden durch delays daher blockiert.
// Das Programm darf nirgends unterbrochen werden mit delays oder langen while schleifen.
#include "Timer.h"

// Speichere die Version nur als String, 
// wenn sie auch durch Serial ausgegeben werden kann.
// Bitte bei jeder Änderung am code auch die Version ändern.
// ENCODER_SOFTWARE_VERSION_STR und ENCODER_VERSION erhöhen.
//
// ENCODER_VERSION ist der Wert, der durch den i2C Bus ausgelesen werden kann und
// besteht daher nur aus einer Zahl.
#ifdef DEBUG
    const char *ENCODER_HARDWARE_VERSION_STR = "2";
    const char *ENCODER_SOFTWARE_VERSION_STR = "2.0.1";
#endif
#define     ENCODER_VERSION 2
//     Datum:     16.12.2020
//  Autor:  Tim Koelbl, Alex Krieg


// PINS ----------------------------------------------------
  // CAN CHANGE (DEPENDS ON HARDWARE):
    // Drehschalter für die I2C Adresse
    const uint16_t dipSwitchPins[4] {A0, A1, A2, A3};
    
    // LED 
    const byte led_red   = 9;
    const byte led_green = 10;
    const byte led_blue  = 11;
//----------------------------------------------------------


// ZYKLISCHES UPDATEN DES SENSORS --------------------------
#ifdef USE_TIMED_UPDATE
  // CAN CHANGE:
    // Zeit Intervall für das zyklische updaten des Sensors
    const unsigned long encoderUpdateInterval   = 0; // us 
    
  // DO NOT CHANGE:
    // Timer für das zyklische aufrufen des updates für den Sensor
    Timer encoderUpdateTimer(TMicros); // Starte den Timer im Micros mode (schneller Timer)
#endif
//----------------------------------------------------------




// IDLE MODE -----------------------------------------------
  // CAN CHANGE:
    // Schalte nach 5sekunden in den IDLE mode
    const unsigned int idleTimeAfter            = 5000; // ms
    
    // Helligkeit für die farbe Orange
    const byte     idleColorBrightness          = 128;
    
  // DO NOT CHANGE:
    // Timer der automatisch in den LED Idle mode umschaltet wenn die Zeit abgelaufen ist.
    Timer encoderIdleTimer;
    
    // Timer zum dimmen der LED
    Timer idleLedDimTimer;
    
    // true, wenn die Organge LED leuchtet
    volatile bool encoderIsIdle                 = false;
    
    // Helligheitswert für das Dimmen der LED. Wird stetig inkrementiert und 
    // die effektive helligkeit wird durch den sin() von <encoderIdleBrightness> ermittelt.
    float encoderIdleBrightness                 = 0;
//----------------------------------------------------------

// I2C TRAFFIC MODE ----------------------------------------
  // CAN CHANGE:
    // Zeit, nach der die grüne LED ausschalten soll.
    const unsigned int i2cEventRecievedOnTime   = 100; // ms
    
    // Helligkeit für die farbe Grün
    const byte     i2cEventColorBrightness      = 128;
    
  // DO NOT CHANGE:
    // Timer der automatisch nach <i2cEventRecievedOnTime> ms die grüne LED ausschaltet,
    // nachdem der I2C Bus benutzt wurde.
    Timer i2cEventCooldownTimer;
    
    // Speichert ob die Gründe LED eingeschaltet sein soll oder nicht.
    volatile bool i2cEventRecieved              = false;

    volatile bool initializing                  = false;
    
    // callbackfunktion die der Timer ausführt
    void on_i2cEventCooldownTimer();
//----------------------------------------------------------

// ERROR DISPLAY -------------------------------------------
  // CAN CHANGE:
    // Anzahl wiederholungen des Blinkcodes
    const byte errorRepeatAmount                = 3;

    // Blinkintervall des Blinkcodes
    const unsigned int errorBlinkInterval       = 300; // ms

    // Helligkeit für die farbe Rot
    const byte     errorColorBrightness         = 255;
    
  // DO NOT CHANGE:
    // Zählt die aktuellen wiederholungen die bereits ausgeführt wurden
    unsigned int errorRepeatCounter             = 0;
    
    // Timer für das zyklische Blinken des Fehlercodes
    Timer errorBlinkTimer;
    
    // Speichert die zeitliche Position im Blinkcode
    byte errorCode_currentBlinkStep             = 0;
//----------------------------------------------------------

// HOME POS LED MODE ---------------------------------------
  // CAN CHANGE:
    // Tolleanz in dem Bereich um die Home Position, in der die blaue LED blinken soll.
    const byte         homePosDisplayTolerance  = 5; // +- 5 encoder ticks (ca. 28 grad)
    
    // Helligkeit für die farbe lau
    const byte     homePosColorBrightness       = 255;
    
    // Blinkintervall der blauen LED
    const unsigned int homePosBlinkInterval     = 200;
    
  // DO NOT CHANGE:
    // Timer für das zyklische Blinken der blauen Home LED
    Timer homePosBlinkTimer;
    
    // true, wenn die blaue LED leuchtet
    bool homePosBlinkState                      = 0;
//----------------------------------------------------------


// I2C -----------------------------------------------------
  // NO CHANGE, ADD ONLY:
    // Register, auf die der I2C Master zugreifen kann
    // Adressen nicht ändern um kompatibel zu bleiben, nur neue hinzufügen.
    enum Rquest {
        SEND_ERROR                  = 0xff,
        RETURN_ABS_POSITION         = 0x00,
        SET_HOME                    = 0x01,
        RETURN_REL_POSITION         = 0x02,
        RETURN_CURRENT_ERROR        = 0x10,
        RETURN_LAST_ERROR           = 0x11,
        CLEAR_ERROR                 = 0x12,
        RETURN_VERSION              = 0x20,
        INITIALZE_TEST              = 0x40,
	    
	    RETURN_ALL_POSITIONS        = 0x30,
	    RETURN_ALL_REGISTERS        = 0x31
    };
    
  // DO NOT CHANGE:
    // Das aktuell ausgewehlte Register 
    volatile byte registerRequest = 0x00;
    
    // I2C Adresse, diese wird im setup() gesetzt
    uint8_t       address         = 0;

  // NO CHANGE, ADD ONLY:
    // I2C callback funktion um ein Register zu beschreiben
    void receiveEvent();
    
    // I2C callback funktion um ein Register aus zu lesen
    void requestEvent();
    
    // Sendet die absolute Position dem I2C Master zu
    void returnAbsPosition();
    
    // Sendet die Home Position dem I2C Master zu
    void returnHomePosition();
    
    // Sendet die relative Position, zur Home Position, dem I2C Master zu
    void returnRelPosition();
    
    // Sendet die aktuelle Fehlermeldung dem I2C Master zu
    void returnCurrentError();
    
    // Sendet die letzte Fehlermeldung dem I2C Master zu
    void returnLastError();
    
    // Sendet die Softwareversion dem I2C Master zu
    void returnVersion();
	
	// Sendet alle Positionen. (3 bytes): ABS,HOME,REL
	void returnAllPositions();
	
	// Sendet alle lesbaren Register. (6 bytes): ABS,HOME,REL,ERR1,ERR2,VERS
	void returnAllRegisters();

    // returns 0xfe to thest the connection when initialzing
    void initializeTest();

    // returns error code 0xff
    void sendError();
//----------------------------------------------------------

// PIND -> Inputregister for Arduino Nano Pins: 0, 1, 2, 3, 4, 5, 6, 7
// DDRD -> Pinmoderegister for the same pins
Encoder8bit encoder{&PIND,&DDRD};

Timer handleLedTimer;
const unsigned int handleLedInterval = 20; //ms
void handleLed();

#ifdef DEBUG
    void debug_print_version();
#endif

void setup()
{
    pinMode(13,OUTPUT);
    // set pinMode and read address
    for (int i = 0; i < 4; i++)
    {
        pinMode(dipSwitchPins[i], INPUT);
        address |= digitalRead(dipSwitchPins[i]) << i;
    }
    pinMode(led_red     , OUTPUT);
    pinMode(led_green    , OUTPUT);
    pinMode(led_blue    , OUTPUT);
    
    // begin I2C communication
    // Setze slave Adresse 
    Wire.begin(address);
    
    // Setze callback Funktionen
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
    
#ifdef DEBUG
    //begin Serial communication
    Serial.begin(SERIAL_BAUD);
    debug_print_version();
    Serial.println("Encoder8bit Version: "+String(Encoder8bit_version));
    Serial.print(  "Address: ");
    Serial.println(address,BIN);
    Serial.println("Setup completed");
#endif
    
    // Setzt callback Funktion für den Timer
    i2cEventCooldownTimer.onFinished(on_i2cEventCooldownTimer);
    
    // Lese die HomePosition aus dem EEPROM und setze damit die Encoder Home Position
    encoder.setHome(EEPROM.read(SET_HOME)); 
}
Timer blinkTimer;
void loop()
{
    if(blinkTimer.start(300))
    {

      digitalWrite(13,!digitalRead(13));
    }
    // update den Timer
    i2cEventCooldownTimer.update();
    
    // Starte den Timer mit <idleTimeAfter> ms 
    // Der Timer returnt true, wenn die Zeit abgelaufen ist und resetet sich selbst.
    // Beim nächsten durchlauf startet der Timer automatisch wieder.
    // Der timer wird in receiveEvent() oder requestEvent() gestoppt um die Zeit zurückzusetzen da ja Kommunikation 
    // auf dem Bus stattfindet und darum nicht in IDLE mode gewechselt werden soll.
    // Erst wenn receiveEvent() und requestEvent() für <idleTimeAfter> ms nicht mehr aufgerufen wurde,
    // kann nach <idleTimeAfter> ms in den IDLE mode gewechselt werden und die orange LED leuchten lassen.
    if(encoderIdleTimer.start(idleTimeAfter))
    {
        encoderIsIdle = true;
    }
    


#ifdef USE_TIMED_UPDATE

// Wenn <encoderUpdateInterval> == 0 ist, muss man den Timer entfernen.
// 0 bedeutet für den Timer, dass er niemals aufhört. 
// Mit dieser Funktion kann man Zeit messen aber das brauchen wir hier nicht.
#if encoderUpdateInterval == 0

#else
    // Update den Encoder alle <encoderUpdateInterval> ms
    if(encoderUpdateTimer.start(encoderUpdateInterval))
#endif
    {
        encoder.update();
        #ifdef DEBUG
        /*  
            Serial.print(encoder.getPosAbs());
            Serial.print(" ");
            Serial.println(encoder.getPosRel());
        */
        #endif
    }
#endif

	if(handleLedTimer.start(handleLedInterval))
	{
		// Update die LED
		handleLed();
	}
}

// Der I2C Master möchte auf ein Register zugreifen
void receiveEvent()
{
    // Stope den IDLE mode, schalte die orange LED aus
    encoderIsIdle    = false;
    encoderIdleTimer.stop();
    
    // Der I2C Bus wird gerade verwendet, schalte die grüne LED ein
    i2cEventRecieved = true;
    i2cEventCooldownTimer.stop(); // Reset Timer
    // Starte den cooldown Timer der die grüne LED wieder ausschaltet
    i2cEventCooldownTimer.start(i2cEventRecievedOnTime);

    initializing = false;
    
    if (Wire.available())
    {
        Array<byte> buffer = i2c.read();
        // Lese das erste byte
        registerRequest = buffer.buffer[0];
        #ifdef DEBUG
        Serial.println(buffer.size);
        for (int i = 0; i < buffer.size; i++)
            Serial.println(buffer.buffer[i], HEX);
        Serial.print("Selected Register: ");
        Serial.println(registerRequest,HEX);
        #endif
        switch (registerRequest)
        {
            // Setze die Home Position
            case SET_HOME:
                if(buffer.size == 2) // Setzt sie nur, wenn das zweite byte auch mitgesendet wurde.
                                  // Wenn nicht, gibts kein Fehler, da es sein kann, dass der I2C Master nuch aus diesem Register lesen möchte.
                    encoder.setHome(buffer.buffer[1]); 
                    encoder.calculateRelativePos();
                    Serial.print("Set Home to: ");
                    Serial.println(buffer.buffer[1], HEX);
                    // Speichere die Home Position noch im EEPROM damit sie beständig bleibt. 
                    EEPROM.write(SET_HOME, encoder.getHome());
                break;
                
            // Lösche die ERROR Register
            case CLEAR_ERROR:
                ERROR::currentError = 0;
                ERROR::lastError    = 0;
                ERROR::errorOccured = false;
                break;
                
            // Die nachfolgenden Register können nicht mit daten beschrieben werden aber müssen aufgeführt werden, 
            // da ansonnsten im default case ein Fehler geworfen wird, obwohl es diese Register gibt und obwohl nicht sicher steht,
            // ob diese überhaupt beschrieben werden sollen.
            case RETURN_ABS_POSITION: 
            case RETURN_REL_POSITION: 
            case RETURN_CURRENT_ERROR: 
            case RETURN_LAST_ERROR: 
            case RETURN_VERSION:
			case RETURN_ALL_POSITIONS:
			case RETURN_ALL_REGISTERS:
            case INITIALZE_TEST:
                if(buffer.size > 1) // Es wird versucht auf ein READ ONLY Register zu schreiben
                    ERROR::throw__i2c_badRegisterAccess(); // Diese Register sind READ ONLY
                break;
            default:
            {
                // Werfe einen Fehler, da versucht wird, auf ein nicht vorhandenes Register zu zu greifen
                ERROR::throw__i2c_badRegisterAccess();
                break;
            }
        }
    }

    // Wenn hier noch daten vorhanden sind, handelt es sich um einen Fehler
    if(Wire.available())
    {
        ERROR::throw__i2c_toMuchDataRecieved();
        // Leere den Buffer, da es sich um datenmüll handelt.
        // bytes die nicht in das Register geschrieben werden können.
        #ifdef DEBUG
        Serial.print("Remaining data: ");
        #endif
        while(Wire.available())
        {
          #ifndef DEBUG
          Wire.read();
          #else 
          Serial.print(Wire.read());
          Serial.print(", ");
          #endif
        }
        #ifdef DEBUG
        Serial.println("");
        #endif
      }
      if (Exception::error != Exception::ErrorCodes::NO_ERROR)
        registerRequest = SEND_ERROR;
}

// Der I2C Master möchte auf dem Register <registerRequest> Daten auslesen
void requestEvent()
{
    // Stope den IDLE mode, schalte die orange LED aus
    encoderIsIdle    = false;
    encoderIdleTimer.stop();
    
    // Der I2C Bus wird gerade verwendet, schalte die grüne LED ein
    i2cEventRecieved = true;
    i2cEventCooldownTimer.stop(); // Reset Timer
    // Starte den cooldown Timer der die grüne LED wieder ausschaltet
    i2cEventCooldownTimer.start(i2cEventRecievedOnTime);
    
    #ifdef DEBUG
        Serial.print("master requested response on Register: ");
        Serial.println(registerRequest,HEX);
    #endif
    #ifdef USE_UPDATE_ON_REQUEST
        // Update den encoder
        encoder.update();
    #endif
	
	//noInterrupts();
    switch (registerRequest)
    {
        case RETURN_ABS_POSITION:    returnAbsPosition();     break; // Sende dem I2C Master die absolute Position des Encoders. 1 byte Wertebereich 0 - 127
        case INITIALZE_TEST:         initializeTest();        break;
        case SEND_ERROR:             sendError();             break;
        case SET_HOME:               returnHomePosition();    break; // Sende dem I2C Master die Home Position des Encoders. 1 byte Wertebereich 0 - 127 case RETURN_REL_POSITION:    returnRelPosition();     break; // Sende dem I2C Master die relative Position zur Home Position des Encoders. 1 byte Wertebereich 0 - 127 case RETURN_CURRENT_ERROR:   returnCurrentError();    break; // Sende dem I2C Master den aktuellen Fehler. 1 byte case RETURN_LAST_ERROR:      returnLastError();       break; // Sende dem I2C Master den letzten Fehler. 1 byte
        case RETURN_VERSION:         returnVersion();         break; // Sende dem I2C Master die Softwareversion. 1 byte
        case RETURN_ALL_POSITIONS:   returnAllPositions();    break; // Sende dem I2C Master alle Positionen. ABS,HOME,REL
		case RETURN_ALL_REGISTERS:   returnAllRegisters();    break; // Sende dem I2C Master alle lesbaren Register. ABS,HOME,REL,ERR1,ERR2,VERS
		default:
            // Es wird versucht auf ein nicht vorhandenes Register zu zu greifen. Oder:
            // Es wird versucht auf ein WRITE ONLY Register zu zu greifen.
            ERROR::throw__i2c_badRegisterRequest();
    }
	//interrupts();
}

void sendError() 
{
    i2c.write(0xff);
}

// Sende dem I2C Master die absolute Position des Encoders. 1 byte Wertebereich 0 - 127
void returnAbsPosition()
{
    #ifdef DEBUG
        Serial.print("returnAbsPosition:  ");
        Serial.println(encoder.getPosAbs());
    #endif
    i2c.write(encoder.getPosAbs());
}

void initializeTest()
{
    i2c.write(0xfe);
}

// Sende dem I2C Master die relative Position zur Home Position des Encoders. 1 byte Wertebereich 0 - 127
void returnRelPosition()
{
    #ifdef DEBUG
        Serial.print("returnRelPosition:  ");
        Serial.println(encoder.getPosRel());
    #endif
    //if(!encoder.dataIsConsistent())
    //  encoder.recalculate();
    i2c.write(encoder.getPosRel());
}

// Sende dem I2C Master die Home Position des Encoders. 1 byte Wertebereich 0 - 127
void returnHomePosition()
{
    #ifdef DEBUG
        Serial.print("returnHomePosition:  ");
        Serial.println(encoder.getHome());
    #endif
    i2c.write(encoder.getHome());
}

// Sende dem I2C Master den aktuellen Fehler. 1 byte
void returnCurrentError()
{
    #ifdef DEBUG
        Serial.print("returnCurrentError:  ");
        Serial.println(ERROR::currentError);
    #endif
    i2c.write(ERROR::currentError);
}

// Sende dem I2C Master den letzten Fehler. 1 byte
void returnLastError()
{
    #ifdef DEBUG
        Serial.print("returnLastError:  ");
        Serial.println(ERROR::lastError);
    #endif
    i2c.write(ERROR::lastError);
}

// Sende dem I2C Master die Softwareversion. 1 byte
void returnVersion()
{
    #ifdef DEBUG
        Serial.println("returnVersion:  ");
        debug_print_version();
    #endif
    i2c.write(ENCODER_VERSION);
}

// Sendet alle Positionen. ABS,HOME,REL
void returnAllPositions()
{
    byte buffer[3];
    buffer[0] = encoder.getPosAbs();
    buffer[1] = encoder.getHome();
    buffer[2] = encoder.getPosRel();
    Array<byte> data(buffer, 3);
    i2c.write(data);
}
	
// Sendet alle lesbaren Register. ABS,HOME,REL,ERR1,ERR2,VERS
void returnAllRegisters()
{
    byte buffer[6];
    buffer[0] = encoder.getPosAbs();
    buffer[1] = encoder.getHome();
    buffer[2] = encoder.getPosRel();
    buffer[3] = ERROR::currentError;
    buffer[4] = ERROR::lastError;
    buffer[5] = ENCODER_VERSION;
    Array<byte> data(buffer, 6);
    i2c.write(data);
}


void handleLed()
{
    // Abschnitte in dieser Funktion, die weiter unten definiert wurden, sind setzdominant.
    // z.B. IDLE mode ist nicht so wichtig, das er dargestellt wird, wie Fehler.
    
    // Initialisiere für alle Farben eine Helligkeit von 0
    byte red   = 0; 
    byte green = 0;
    byte blue  = 0;
    
    // Wird der Sensor zur Zeit nicht verwendet soll die LED orange dimmen.
    // Das dimmen wird mit dem sin() durchgeführt
    if(encoderIsIdle)
    {
        // Ändere alle 10 ms den winkel (RAD) für den Sinus
        // Verändert die Dimmfrequenz
        //if(idleLedDimTimer.start(10))
        {
            encoderIdleBrightness += 0.02;
            
            // Kontrollierter Overflow 
            // Dient dazu, dass der Sinus keine negativen Werte ausgiebt und dass <encoderIdleBrightness> nicht mit der zeit so gross wird, 
            // dass das Verhalten beeinträchtigt wird.            
            if(encoderIdleBrightness >= 3.141592)
                encoderIdleBrightness = 0;
        }
        
        // Setze die Farben um ein Orange zu bekommen
        red   = idleColorBrightness * sin(encoderIdleBrightness);
        green = idleColorBrightness * sin(encoderIdleBrightness);
    }
    
    // Blinke blau, wenn sich der Encoder im Home Positions Bereich befindet
    if((encoder.getPosRel() <      homePosDisplayTolerance) || 
       (encoder.getPosRel() > (128-homePosDisplayTolerance)))
    {
        // Toggle die LED nach <homePosBlinkInterval>
        if(homePosBlinkTimer.start(homePosBlinkInterval))
        {
            homePosBlinkState = !homePosBlinkState;
        }
        
        // Schalte die eventuell vorhandene orange LED aus,
        // um ein Farbmischen zwischen Orange und blau zu verhindern.
        if(homePosBlinkState)
        { 
            red     = 0;
            green     = 0;
            blue     = homePosColorBrightness;
        }
    }
    else
    {
        // Encoder ist nicht mehr in der Home Position
        // Setzt daher die blaue LED auf 1 damit sie direkt wieder leuchtet,
        // wenn sich der Encoder wieder in der Home Position befindet.
        homePosBlinkState = 1;
        
        // Stoppe den Timer, damit er beim nächsten mal wieder bei 0 beginnt.
        homePosBlinkTimer.stop();
    }

    
    // Besteht gerade I2C Kommunikation, so soll die grüne LED leuchten. 
    // Dies kann sich mit der lauen Home Position LED mischen aber dafür hat man beide Informationen.
    // Der Unterschied ist im ausgeschalteten Blinkzustand der blauen LED ersichtlich.
    if(i2cEventRecieved)
    {
        // Setzt Helligkeit auf <i2cEventColorBrightness>
        green = i2cEventColorBrightness;
    }

    // Wenn ein Fehler aufgetreten ist, soll der Fehler mit einem Blinkcode ausgegeben werden
    if(ERROR::errorOccured)
    {
        // Setzt beide anderen Farben auf 0
        green = 0;
        blue  = 0;
        
        
        if(errorCode_currentBlinkStep % 2 == 0 && errorCode_currentBlinkStep < 2*ERROR::currentError)
        {
            red = errorColorBrightness;
        }
        else
        {
            red = 0;
        }
        
        // Wechsle zum nächsten zeitpunkt in der Animation nach <errorBlinkInterval> ms
        if(errorBlinkTimer.start(errorBlinkInterval))
        {
            errorCode_currentBlinkStep++;
            if(errorCode_currentBlinkStep > 2*ERROR::currentError+1)
            {
                errorCode_currentBlinkStep     = 0;
                errorRepeatCounter++;
                if(errorRepeatCounter >= errorRepeatAmount)
                {
                    ERROR::clear();
                    errorRepeatCounter         = 0;
                    encoderIsIdle            = false;
                }
            }
        }
    }

    if (initializing) 
    {
        red = 255;
        green = 128;
    }

    // Setze  die Helligkeit der LED's
    analogWrite(led_red        , red);
    analogWrite(led_green    , green);
    analogWrite(led_blue    , blue);

    if (initializing)
    {
        delay(500);
    }
}

// Callback funktion für einen Timer
void on_i2cEventCooldownTimer()
{
    // Schalte die grüne LED wieder aus,
    // I2C Kommunikation ist vorbei.
    i2cEventRecieved = false;
}

#ifdef DEBUG
void debug_print_version()
{
    Serial.print("Hardware:  V");
    Serial.println(ENCODER_HARDWARE_VERSION_STR);
    Serial.print("Software:  V");
    Serial.println(ENCODER_SOFTWARE_VERSION_STR);
    Serial.print("Version BIN:  ");
    Serial.println(ENCODER_VERSION);
}    
#endif
