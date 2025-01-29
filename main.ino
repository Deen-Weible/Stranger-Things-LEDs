#include "FastLED.h"
// #include "eeprom.h"

// ███████╗████████╗██████╗  █████╗ ███╗   ██╗ ██████╗ ███████╗██████╗     ████████╗██╗  ██╗██╗███╗   ██╗ ██████╗ ███████╗
// ██╔════╝╚══██╔══╝██╔══██╗██╔══██╗████╗  ██║██╔════╝ ██╔════╝██╔══██╗    ╚══██╔══╝██║  ██║██║████╗  ██║██╔════╝ ██╔════╝
// ███████╗   ██║   ██████╔╝███████║██╔██╗ ██║██║  ███╗█████╗  ██████╔╝       ██║   ███████║██║██╔██╗ ██║██║  ███╗███████╗
// ╚════██║   ██║   ██╔══██╗██╔══██║██║╚██╗██║██║   ██║██╔══╝  ██╔══██╗       ██║   ██╔══██║██║██║╚██╗██║██║   ██║╚════██║
// ███████║   ██║   ██║  ██║██║  ██║██║ ╚████║╚██████╔╝███████╗██║  ██║       ██║   ██║  ██║██║██║ ╚████║╚██████╔╝███████║
// ╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝       ╚═╝   ╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝ ╚═════╝ ╚══════╝
//				Arduino program to control 2812B led strips based on the show, using alphabetical order					//

// Variables, times are in ms
// Note that arduino times are... iffy
const char inputString[] = "+=+= run-. boo.. /right here/-. spooky-. no candy-. /peter dum/-. sosososos-. help me-.../lets go avs/-. /spooky sentence/-. /happy halloween/-. ";
// const char inputString[] = "-";

int alphabetSize = 26;
int letterDelay = 2250; // accurate to the show :)
int offTime = 1600;     // also, accurate to the show. i'm very proud of myself
const int periodDelay = 3500;

// Can swap for fast time using "/"
int fastLetterDelay = 1200;
int fastOffTime = 800;
int fastRandomDelayMin = -100;
int fastRandomDelayMax = 200;

int flickerTimes = 15;
int flickerDelay = 300;

const bool periodResets = false;

// random delay
const bool randomDelayEnabled = true;
int randomDelayMin = -300;
int randomDelayMax = 500;

// Alphabet for locating letter numbers
char Alphabet[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z'};

// int LedNumbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26};
// temp
int LedNumbers[] = {
    2, 5, 8, 11, 14, 18, 22, 26, 35, 43, 46,
    50, 53, 56, 59, 62, 65, 68, 71, 74, 77, 80, 83, 86, 89, 92, 95};

// char Alphabet[] = {
//     'Z', 'Y', 'X','W','V','U','T','S','R', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'H', 'G', 'F','E','D','C','B','A'
// };

#define NUM_LEDS 100
#define DATA_PIN 4

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup()
{
    Serial.begin(115200);
    FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS).setRgbw(RgbwDefault()); // ignore error, vscode issue
    FastLED.setBrightness(255);                                                    // Set global brightness to 100%
    delay(2000);                                                                   // If something ever goes wrong this delay will allow upload.
}

// DO NOT CHANGE //
int randletterDelay = 2250;
int randOffTime = 2500;
int randPeriodDelay = 5000;

void swapInts(int &varOne, int &varTwo)
{
    int tempVar = varOne;
    varOne = varTwo;
    varTwo = tempVar;
}

void loop()
{
    // Loop over each letter in the input string and run with their place in the alphabet
    for (char letter : inputString)
    {
        // delay if letter is period

        if (letter == ' ')
        {
            return;
        }
        if (letter == '.')
        {
            Serial.println("delaying for period");
            delay(randPeriodDelay);
        };
        if (letter == '-')
        {
            Serial.println("activate ze flicker");
            flickerlights();
            // speed up/slow things down
        };
        if (letter == '+')
        {
            sweepEff();
        }
        if (letter == '+')
        {
            sweepEff(true);
        }
        if (letter == '/')
        {
            swapInts(letterDelay, fastLetterDelay);
            swapInts(offTime, fastOffTime);

            swapInts(randomDelayMax, fastRandomDelayMax);
            swapInts(randomDelayMin, fastRandomDelayMin);

            Serial.println("Swapping");

            // debug
            Serial.println(letterDelay);
            Serial.println(offTime);
            Serial.println(randomDelayMax);
            Serial.println(randomDelayMin);
        }

        // Update random delays
        if (randomDelayEnabled)
        {
            randletterDelay = letterDelay + random(randomDelayMin, randomDelayMax);
            randOffTime = offTime + random(randomDelayMin, randomDelayMax);
            randPeriodDelay = periodDelay + random(randomDelayMin, randomDelayMax);
        };

        letter = toupper(letter);

        // loop over each letter and get its position in alphabet, light position.
        for (int i = 0; i < sizeof(Alphabet); i++)
        {
            if (letter == Alphabet[i])
            {
                reset();
                writeled(LedNumbers[i]);
                FastLED.show();
                delay(randletterDelay);
                break;
            };
        };

        // offtime if enabled.
        if (offTime > 0)
        {
            reset();
            delay(randOffTime);
        };
    };
};

void writeled(int ledToWrite)
{
    Serial.println(ledToWrite);
    leds[ledToWrite] = CHSV(random(0, 255), 255, 255);
};

void offled(int ledToWrite)
{
    leds[ledToWrite] = CRGB::Black;
};

void reset()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        offled(i);
    }

    FastLED.show();
};

int randDelay = 0;

void sweepEff(bool reversed = false)
{
    if (reversed)
    {
        for (int i = NUM_LEDS; i > 0; i--)
        {
            if (i != 100)
            {
                offled(i + 1);
            }

            writeled(i);
            delay(8);
            FastLED.show();
        }
    }

    for (int i = 0; i < NUM_LEDS; i++)
    {
        if (i != 0)
        {
            offled(i - 1);
        }

        writeled(i);
        delay(8);
        FastLED.show();
    }
    reset();
}

void flickerlights()
{
    flickerTimes = random(20, 30);

    for (int i = 0; i < flickerTimes; i++)
    {
        randDelay = flickerDelay + random(-100, 150);
        for (int i = 0; i < NUM_LEDS; i++)
        {
            writeled(i);
        }
        for (int i = 1; i < 255; i *= 4)
        {
            FastLED.setBrightness(i);
            FastLED.show();
            delay(10);
        }

        for (int i = 255; i > 5; i -= 10)
        {
            FastLED.setBrightness(i);
            FastLED.show();
            delay(2);
        }

        reset();
        delay(randDelay / 3);
    };

    FastLED.setBrightness(255);
};
