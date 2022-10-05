#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <Tone.h>

// codigo modificado para arduino uno 
// Version original por IsmaliSan https://github.com/IsmailSan/Memory-Game


LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // establece el LCD
Tone speakerpin;
int starttune[] = {NOTE_E4, NOTE_E4, NOTE_C4, NOTE_E4, NOTE_G4, NOTE_G4};
int duration2[] = {100, 200, 100, 200, 100, 400};
int note[] = {NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_B4, NOTE_A4};
int duration[] = {100, 100, 100, 300, 100, 300};
int button[] = {2, 3, 4, 5}; //Input  pins botones
int ledpin[] = {8, 9, 10, 11};  // Input pins LEDS
int turn = 0;  // Contador de turnos
int buttonstate = 0;  // checadores de botones
int randomArray[50]; //almacenar hasta 50 entradas
int inputArray[50];
int highscore = 0;
int MyScore;


void setup() 
{
  lcd.setBacklightPin(3,POSITIVE);
 lcd.setBacklight(LOW); 
  lcd.begin(16, 2);
 lcd.clear();
 lcd.setCursor(0, 1);
 lcd.print("Your Score: 0");
 lcd.setCursor(0, 0);
 lcd.print("High Score: 0");
  Serial.begin(9600);
  speakerpin.begin(12); // La altavoz está en el pin 12

  for(int x=0; x<4; x++)  // Los pines LED son Output
  {
    pinMode(ledpin[x], OUTPUT);
  }
  
  for(int x=0; x<4; x++) 
  {
    pinMode(button[x],INPUT);  // los botones son inputs 
    digitalWrite(button[x], HIGH);  // habilitar pullup interno; los botones comienzan en posición alta; lógica invertida
  }

  randomSeed(analogRead(0)); //Agregado para generar "más aleatoriedad" con randomArray para la función de salida
  for (int thisNote = 0; thisNote < 6; thisNote ++) {
     // play the next note:
     speakerpin.play(starttune[thisNote]);
     // hold the note:
     if (thisNote==0 || thisNote==2)
     {
       digitalWrite(ledpin[0], HIGH);
     }
     if (thisNote==1 || thisNote==3 )
     {
       digitalWrite(ledpin[1], HIGH);
     }
     if (thisNote==4 || thisNote==5)
     {
       digitalWrite(ledpin[2], HIGH);
     }  
     if (thisNote==6)
     {   
       digitalWrite(ledpin[3], HIGH);
     }
     delay(duration2[thisNote]);
     // stop for the next note:
     speakerpin.stop();
     digitalWrite(ledpin[0], LOW);
     digitalWrite(ledpin[1], LOW);
     digitalWrite(ledpin[2], LOW);
     digitalWrite(ledpin[3], LOW);
     delay(25);
    }
  delay(1000);
}
 
void loop() 
{   
  for (int y=0; y<=2; y++)
  {
    //función para generar la matriz que debe coincidir con el jugador
    digitalWrite(ledpin[0], HIGH);
    digitalWrite(ledpin[1], HIGH);
    digitalWrite(ledpin[2], HIGH);
    digitalWrite(ledpin[3], HIGH);
  
    for (int thisNote = 0; thisNote < 6; thisNote ++) {
     // play the next note:
     speakerpin.play(note[thisNote]);
     // hold the note:
     delay(duration[thisNote]);
     // stop for the next note:
     speakerpin.stop();
     delay(25);
    }
    
    digitalWrite(ledpin[0], LOW);
    digitalWrite(ledpin[1], LOW);
    digitalWrite(ledpin[2], LOW);
    digitalWrite(ledpin[3], LOW);
    delay(1000);
  
    for (int y=turn; y <= turn; y++) //untuk sekali random
    { //Limitado por la variable de giro
      Serial.println(""); //Algunas salidas en serie para seguir
      Serial.print("Turn: ");
      Serial.print(y);
      Serial.println("");
      lcd.clear();
 MyScore=y;
 lcd.setCursor(0, 1);  //(Column,Row)
 lcd.print("Your Score: ");
 lcd.setCursor(12, 1); 
 lcd.print(MyScore);
 if(MyScore > highscore) {
 highscore = y;
 }
 lcd.setCursor(0, 0);
 lcd.print("High Score: " + (String)(highscore));
 Serial.print(y);
 Serial.println("");
 delay(1000);
      randomArray[y] = random(1, 5); //Asigna un número aleatorio (1-4) a randomArray[y], siendo y el conteo de turnos
      for (int x=0; x <= turn; x++)
      {
        Serial.print(randomArray[x]);
      
        for(int y=0; y<4; y++)
        {
      
          if (randomArray[x] == 1 && ledpin[y] == 8) 
          {  //declaraciones if para mostrar los valores almacenados en el array
            digitalWrite(ledpin[y], HIGH);
            speakerpin.play(NOTE_G3, 100);
            delay(400);
            digitalWrite(ledpin[y], LOW);
            delay(100);
          }

          if (randomArray[x] == 2 && ledpin[y] == 9) 
          {
            digitalWrite(ledpin[y], HIGH);
            speakerpin.play(NOTE_A3, 100);
            delay(400);
            digitalWrite(ledpin[y], LOW);
            delay(100);
          }
  
          if (randomArray[x] == 3 && ledpin[y] == 10) 
          {
            digitalWrite(ledpin[y], HIGH);
            speakerpin.play(NOTE_B3, 100);
            delay(400);
            digitalWrite(ledpin[y], LOW);
            delay(100);
          }

          if (randomArray[x] == 4 && ledpin[y] == 11) 
          {
            digitalWrite(ledpin[y], HIGH);
            speakerpin.play(NOTE_C4, 100);
            delay(400);
            digitalWrite(ledpin[y], LOW);
            delay(100);
          }
        }
      }
    }
    input();
    
  }
  
  
}
 
 
 
void input() { //Función para permitir la entrada del usuario y verificar la entrada contra del array generado

  for (int x=0; x <= turn;)
  { //Declaración controlada por conteo de turnos

    for(int y=0; y<4; y++)
    {
      
      buttonstate = digitalRead(button[y]);
    
      if (buttonstate == LOW && button[y] == 2)
      { //Checking for button push
        digitalWrite(ledpin[0], HIGH);
        speakerpin.play(NOTE_G3, 100);
        delay(200);
        digitalWrite(ledpin[0], LOW);
        inputArray[x] = 1;
        delay(250);
        Serial.print(" ");
        Serial.print(1);
        if (inputArray[x] != randomArray[x]) { //Verifica el valor ingresado por el usuario y lo compara con
          fail();                              //el valor en el mismo lugar en el array generado
        }                                      //Se llama a la función fail si no coincide
        x++;
      }
       if (buttonstate == LOW && button[y] == 3)
      {
        digitalWrite(ledpin[1], HIGH);
        speakerpin.play(NOTE_A3, 100);
        delay(200);
        digitalWrite(ledpin[1], LOW);
        inputArray[x] = 2;
        delay(250);
        Serial.print(" ");
        Serial.print(2);
        if (inputArray[x] != randomArray[x]) {
          fail();
        }
        x++;
      }

      if (buttonstate == LOW && button[y] == 4)
      {
        digitalWrite(ledpin[2], HIGH);
        speakerpin.play(NOTE_B3, 100);
        delay(200);
        digitalWrite(ledpin[2], LOW);
        inputArray[x] = 3;
        delay(250);
        Serial.print(" ");
        Serial.print(3);
        if (inputArray[x] != randomArray[x]) {
          fail();
        }
        x++;
      }

      if (buttonstate == LOW && button[y] == 5)
      {
        digitalWrite(ledpin[3], HIGH);
        speakerpin.play(NOTE_C4, 100);
        delay(200);
        digitalWrite(ledpin[3], LOW);
        inputArray[x] = 4;
        delay(250);
        Serial.print(" ");
        Serial.print(4);
        if (inputArray[x] != randomArray[x]) 
        {
          fail();
        }
        x++;
      }
    }
  }
  delay(500);
  turn++; //Incrementa el número de turnos, también la última acción antes de volver a iniciar la función de salida
}



void fail() { //Función utilizada si el jugador no logra hacer coincidir la secuencia
 
  for (int y=0; y<=3; y++)
  { //Flashes lights for failure
    
    digitalWrite(ledpin[0], HIGH);
    digitalWrite(ledpin[1], HIGH);
    digitalWrite(ledpin[2], HIGH);
    digitalWrite(ledpin[3], HIGH);
    speakerpin.play(NOTE_G2, 300);
    delay(200);
    digitalWrite(ledpin[0], LOW);
    digitalWrite(ledpin[1], LOW);
    digitalWrite(ledpin[2], LOW);
    digitalWrite(ledpin[3], LOW);
    speakerpin.play(NOTE_C2, 300);
    delay(200);
    lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Game Over");
 }
 delay(2000);
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("High: " + (String) (highscore));
 // Serial.print(y);
 // Serial.println("");
 lcd.setCursor(9, 0);
 lcd.print("You: ");
 lcd.setCursor(14, 0);
 lcd.print(MyScore);
 // if(y > highscore) {
 // highscore = y;
 // }
 lcd.setCursor(0, 1);
 lcd.print("<-Press to play again!");
 {
 // buttonstate = digitalRead(button[y]);
 while (digitalRead(button[0]) > 0 &&digitalRead(button[1]) > 0 && digitalRead(button[2]) > 0 && digitalRead(button[3]) > 0 ){
digitalWrite(ledpin[0], HIGH);
digitalWrite(ledpin[1], HIGH);
digitalWrite(ledpin[2], HIGH);
digitalWrite(ledpin[3], HIGH);
delay(100);
digitalWrite(ledpin[0], LOW);
digitalWrite(ledpin[1], LOW);
digitalWrite(ledpin[2], LOW);
digitalWrite(ledpin[3], LOW);
delay(100);
 }
 delay(1000);
  
  turn = -1; //Restablece el valor de turno para que el juego comience de nuevo sin necesidad de un botón de reinicio
}
}


