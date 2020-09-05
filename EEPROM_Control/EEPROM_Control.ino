#define ADDR_BUS_WIDTH  13
#define DATA_BUS_WIDTH  8

// Control lines
#define CE  46 
#define OE  45
#define WE  44

const uint8_t IO[8] = { 36, 37, 38, 39, 40, 41, 42, 43 }; // Data bus pins
const uint8_t ADDR[ADDR_BUS_WIDTH] = { 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34 }; // Address bus pins
char cmd[32]; // Buffer for storing the data
boolean cmdExecuted = false; // Tracker for tracking when the command is ready to execute
uint16_t userAddr = 0; // Variable for the user entered address
uint8_t userData = 0; // Variable for the user entered data

void setup() {
  /* Set pinmodes */

  // Control lines
  pinMode(CE, OUTPUT);
  pinMode(OE, OUTPUT);
  pinMode(WE, OUTPUT);

  // Address lines
  for (uint8_t i = 0; i < ADDR_BUS_WIDTH; i++) {
    pinMode(ADDR[i], OUTPUT);
  }

  // Initialize control lines to high
  digitalWrite(CE, HIGH);
  digitalWrite(OE, HIGH);
  digitalWrite(WE, HIGH);

  Serial.begin(9600);
}

void loop() {
  // Variables for the address
  uint16_t addr_high = 0;
  uint16_t addr_low = 0;
  
  // Only execute the command after the full command has been received
  if (Serial.available() > 0) {
    // Read in the command
    Serial.readBytesUntil('\n', cmd, 32);
    cmdExecuted = true;
  }
  Serial.flush();
  
  // Execute the proper command
  if (cmdExecuted == true) {
    // Write command
    if (cmd[0] == 'w') {
      //Serial.println("Writing to memory");
      // Calculate out the address byte
      addr_high = ((0x1000*toHex(cmd[2]))+(0x0100*toHex(cmd[3])));
      addr_low = ((0x0010*toHex(cmd[4]))+(toHex(cmd[5])));
      userAddr = addr_high + addr_low;
      // Calculate out the data value
      userData = (0x10*toHex(cmd[7]))+toHex(cmd[8]);
      // Write the value to memory
      writePROMByte(userAddr, userData);
    }
    else if (cmd[0] == 'r') {
      //Serial.println("Reading from memory");
      // Calculate out the address byte
      addr_high = ((0x1000*toHex(cmd[2]))+(0x0100*toHex(cmd[3])));
      addr_low = ((0x0010*toHex(cmd[4]))+(toHex(cmd[5])));
      userAddr = addr_high + addr_low;
      // Return the data to the serial port
      Serial.print(userAddr);
      Serial.print(": ");
      Serial.println(readPROMByte(userAddr));
    }
    else {
      Serial.println("Invalid command");
    }
    // Reset the command executed variable
    cmdExecuted = false;
  }
}

// Function to set the IO lines to either an input or an output
void setDataIO(uint8_t setVal) {
  if (setVal == INPUT || setVal == OUTPUT)
    for (uint8_t i = 0; i < DATA_BUS_WIDTH; i++)
      pinMode(IO[i], setVal);
}

// Function to set the value of the address
void setAddr(uint16_t addr) {
  for (uint8_t i = 0; i < ADDR_BUS_WIDTH; i++) {
    int addrBitVal = (addr & (1 << i)) > 0;
    digitalWrite(ADDR[i], addrBitVal);
  }
}

// Function to write a byte to the EEPROM
void writePROMByte(uint16_t addr, uint8_t val) {
  digitalWrite(OE, HIGH);
  setDataIO(OUTPUT);
  setAddr(addr);

  // Send the byte to the data bus
  for (uint8_t i = 0; i < DATA_BUS_WIDTH; i++) {
    int bitVal = (val & (1 << i)) > 0;
    digitalWrite(IO[i], bitVal);
  }

  // Commit the data to memory
  digitalWrite(CE, LOW);
  delay(10);
  digitalWrite(WE, LOW);
  delay(10);
  digitalWrite(WE, HIGH);
  delay(10);
  digitalWrite(CE, HIGH);
  delay(10);
}

// Function to read a byte from the EEPROM
byte readPROMByte(uint16_t addr) {
  byte val = 0;

  setDataIO(INPUT);
  setAddr(addr);

  digitalWrite(CE, LOW);
  digitalWrite(OE, LOW);
  delay(10);

  // Read data from the data bus
  for (uint8_t i = 0; i < DATA_BUS_WIDTH; i++) {
    int dataBit = digitalRead(IO[i]);
    val += (dataBit << i);
  }

  digitalWrite(OE, HIGH);
  digitalWrite(CE, HIGH);
  delay(10);

  return val;
}

// Function to convert an ASCII character to a hexadecimal number (Ex: A0 becomes 0xA0)
uint8_t toHex(char val) {
  return ((val <= '9') ? val - '0' : val - '7');
}

