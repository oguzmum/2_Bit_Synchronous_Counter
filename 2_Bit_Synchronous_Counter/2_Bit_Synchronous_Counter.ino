/*
 * 2-Bit Synchronous Counter with 7-Segment Display
 *
 * The Arduino generates the clock pulse for a 2-bit synchronous binary
 * counter built from JK flip-flops. It then reads the resulting counter
 * state back in and shows the current count (0-3) on a 7-segment display.
 *
 * 7-segment layout, segments a-g plus decimal point (dp):
 *   Digit 0: a b c d e f g dp = 1 1 1 1 1 1 0 0
 *   Digit 1: a b c d e f g dp = 0 1 1 0 0 0 0 0
 *   Digit 2: a b c d e f g dp = 1 1 0 1 1 0 1 0
 *   Digit 3: a b c d e f g dp = 1 1 1 1 0 0 1 0
 */

const byte SEGMENT_COUNT = 8;
const byte DIGIT_COUNT = 4;

// Arduino pins driving the 7-segment display, in order: a b c d e f g dp
const byte segmentPins[SEGMENT_COUNT] = {2, 3, 4, 5, 6, 7, 8, 9};

// Segment on/off pattern for every displayable digit (0-3)
const byte digitSegments[DIGIT_COUNT][SEGMENT_COUNT] = {
    {1, 1, 1, 1, 1, 1, 0, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1, 0}, // 2
    {1, 1, 1, 1, 0, 0, 1, 0}, // 3
};

const byte CLOCK_PIN = 13;        // Drives the flip-flops' shared (synchronous) clock input
const byte COUNTER_BIT0_PIN = 11; // Least significant bit (Q0) of the flip-flop counter
const byte COUNTER_BIT1_PIN = 12; // Most significant bit (Q1) of the flip-flop counter

const int CLOCK_PULSE_DELAY_MS = 500;

int readCounterValue(int bit0, int bit1);
void displayDigit(int digit);

void setup()
{
  pinMode(COUNTER_BIT0_PIN, INPUT);
  pinMode(COUNTER_BIT1_PIN, INPUT);

  for (byte i = 0; i < SEGMENT_COUNT; i++)
  {
    pinMode(segmentPins[i], OUTPUT);
  }
  pinMode(CLOCK_PIN, OUTPUT);

  Serial.begin(9600);
}

void loop()
{
  delay(CLOCK_PULSE_DELAY_MS);

  // Pulse the clock high to advance the counter by one step
  digitalWrite(CLOCK_PIN, HIGH);

  int bit0 = digitalRead(COUNTER_BIT0_PIN);
  int bit1 = digitalRead(COUNTER_BIT1_PIN);
  int counterValue = readCounterValue(bit0, bit1);

  Serial.print("Counter value: ");
  Serial.println(counterValue);
  displayDigit(counterValue);

  digitalWrite(CLOCK_PIN, LOW);
}

// Lights up the segments for the given digit (0-3) on the display
void displayDigit(int digit)
{
  for (byte i = 0; i < SEGMENT_COUNT; i++)
  {
    digitalWrite(segmentPins[i], digitSegments[digit][i]);
  }
}

// Combines the two counter output bits into a decimal value (0-3).
// bit1 is the most significant bit.
int readCounterValue(int bit0, int bit1)
{
  return (bit1 << 1) | bit0;
}
