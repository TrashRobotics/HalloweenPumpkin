#include "sound.h"
#include <Servo.h>
#include <FastLED.h>

#define SPEAKER_PIN 3   // пин динамика 
#define LED_PIN     6   // пин светодиодной матрицы
#define SERVO_PIN   9   // пин сервы

#define LED_COLOR_ORDER GRB       // параметры светодиодной матрицы
#define LED_CHIPSET     WS2812    //
#define LED_NUM_LEDS    8         //
#define LED_BRIGHTNESS  70        // яркость светодиодов
#define LED_FRAMES_PER_SECOND 30  // количество всполохов пламени за секунду

#define SERVO_BASE_POS  90  // начальная позиция сервы в градусах
#define SERVO_MIN_POS   45  // минимальная и максимальные позиции сервы в градусах
#define SERVO_MAX_POS   135 //
#define SERVO_CHANGE_PERIOD   3000  // период изменения положения сервы в мс

#define SPEAKER_SPEAK_PERIOD  5000  // частота воспроизведения звука в мс
#define TIMER2_SCALER   3     // 3  // несущая частота ШИМ = 62.5 кГц => 62.5 / 8 = 7.8 ~ 8 кГц
                              // можно уменьшать, тогда звуковая дорожка будет ускоряться 
CRGB leds[LED_NUM_LEDS];
Servo servo;

uint32_t servoChangeTimer;        // таймер, определяющий, когда серве нужно повернуться
uint8_t servoTargetPos;           // целевая позиция сервы
volatile uint8_t timer2ScalerCount = 0; // счетчик промжуточных циклов таймера2
volatile uint16_t soundCount = 0; // счетчик звуковых данных   
volatile bool isSpeaking = false; // флаг: говорит ли динамик
uint32_t speakerTimer = millis(); // таймер засекающий: как часто нужо воспроизводить звук

void setup() {
  pinMode(SPEAKER_PIN, OUTPUT);   
  digitalWrite(SPEAKER_PIN, LOW);
  
  // инициализация Timer2
  cli(); // отключить глобальные прерывания
  TCCR2A = 0x00; // установить TCCR2A регистр в 0
  TCCR2B = 0x00;
  // включить прерывание Timer2 overflow:
  TIMSK2 = (1 << TOIE2);
  // При CS20 == 1: clk_io/1 (без делителя частоты) => (16000000/1)/255(тики таймера) = 62500 Гц
  // 62.5 кГц - частота прерываний при переполнении, т.е. для 8 кГц звука нужен дополнительный прескейлер
  // в качестве которого выступает TIMER2_SCALER.
  // 62.5 кГц - несущая частота ШИМ на ножке SPEAKER_PIN. Ниже ее делать не желательно, иначе будут шумы, писки, скрипы и прочее.
  TCCR2B |= (1 << CS20);
  TCCR2A |= (1 << WGM21) | (1 << WGM20);  // fast pwm
  sei();  // включить глобальные прерывания
  digitalWrite(SPEAKER_PIN, LOW);
  
  delay(300); 
  servo.attach(SERVO_PIN);  // привязываем серву к выводу
  servo.write(SERVO_BASE_POS); // устанавливаем его в начальную позицию 90 градусов
  delay(300); 
  FastLED.addLeds<LED_CHIPSET, LED_PIN, LED_COLOR_ORDER>(leds, LED_NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(LED_BRIGHTNESS);
  
  servoChangeTimer = millis();
}

void loop()
{
  if (millis() > speakerTimer + SPEAKER_SPEAK_PERIOD) {   // вызов звука не должен пересекаться с изменением положения сервы
    redLed();   // зажигаем светодиоды красным
    delay(300);
    isSpeaking = true;
    speakerTimer = millis();
    while(isSpeaking);   // блокируем серву и светодиоды, пока динамик говорит. Иначе перегруз по току будет
    digitalWrite(3, LOW);
    delay(300);
  }
  
  if(!isSpeaking) digitalWrite(3, LOW); // отключаем динамик
  
  if (millis() > servoChangeTimer + SERVO_CHANGE_PERIOD) {   // изменяет положение сервы, если прошло достаточно времени
    // slowChangeServoPos(random(SERVO_MIN_POS, SERVO_MAX_POS);   // перестают работать светодиоды
    servoTargetPos = random(SERVO_MIN_POS, SERVO_MAX_POS);  // устанавливаем случайную позицию из допустимого диапазона
    servoChangeTimer = millis();
  }  
  servo.write(yieldServoStep(servoTargetPos, 2)); // каждую итерацию делает шаг в 1 градус
  
  Fire2012(); // run simulation frame
  FastLED.show(); // display this frame
  FastLED.delay(1000 / LED_FRAMES_PER_SECOND);
}


// Взято, как пример отсюда (MIT License): https://github.com/FastLED/FastLED/tree/master/examples/Fire2012
// Taken as an example from here (MIT License): https://github.com/FastLED/FastLED/tree/master/examples/Fire2012
// 
// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
//// 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  45

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 200

void Fire2012()
{
// Array of temperature readings at each simulation cell
  static uint8_t heat[LED_NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < LED_NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / LED_NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= LED_NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < LED_NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      pixelnumber = j;
      leds[pixelnumber] = color;
    }
}

void redLed()
{
  for(uint8_t i = 0; i < LED_NUM_LEDS; i++) {   
    leds[i] = CRGB::Black;
    leds[i] = CRGB::Red;  //красный
    FastLED.delay(20);
  }
}

void slowChangeServoPos(uint8_t pos)    // медленное изменение положения сервы (блокирующее)
{
  static uint8_t lastpos = SERVO_BASE_POS;
  if ((pos - lastpos) >= 0){
    for(uint8_t i = lastpos; i < pos; i++) {   
      servo.write(i);
      delay(20);
    }
  }
  else {
    for(uint8_t i = lastpos; i > pos; i--) {   
      servo.write(i);
      delay(20);
    }
  }
  lastpos = pos;
}

uint8_t yieldServoStep(uint8_t pos, uint8_t step)  // генератор положения сервы для медленного движения (неблокирующий)
{
  static uint8_t currentPos = SERVO_BASE_POS;
  int16_t diff = (int16_t)pos - currentPos; // определяем расстояние до новой позиции
  if ((diff == 0) || (abs(diff) < step)) return currentPos;
  
  currentPos += (diff / abs(diff)) * step;  // определяем направление через знак и добавляем шаг по напралению движения
  return currentPos;
}

ISR(TIMER2_OVF_vect)
{
  if(timer2ScalerCount >= TIMER2_SCALER){
    if(isSpeaking){
      analogWrite(3, pgm_read_byte(&sound_data[soundCount]));
      soundCount++;
      if (soundCount >= SOUND_LENGTH) {
        isSpeaking = false;
        soundCount = 0;
      }
    }
    timer2ScalerCount = 0;
  }
  timer2ScalerCount++;
}
