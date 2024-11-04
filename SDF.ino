#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <AD9833.h>

#define FSYNC 9

#define FREQUMAS 6
#define FREQUMENOS 7
#define SIGNALMAS 4
#define SIGNALMENOS 5

LiquidCrystal_I2C lcd(0x27,16,2); // utilizo el 0x27 porq es mi direccion de memoria

AD9833 AD(10, 11, 13);
float tension=5;
float frequencia=1;
float atenuador=1;
float segundos=0;
int i=0;
float freka=0;

int frequ=0;
int signal=0;


uint8_t button_estate = HIGH;
int   VRx = 0;
int   VRy = 0;
int   mapX, mapXnegado, mapY, mapYnegado, btnRead, btn_push;
int line[3]={0,1,2}, line_bk[2],index;
int caract[20]={0,1,2,3}, caract_bk[20];

void setup() {  

  lcd.init();// Inicializacion el LCD
  lcd.backlight(); // Enciende la luz del fondo.

  Wire.begin(); // Inicia el puerto I2C

  AD.begin();

  lcd.clear();
  lcd.setCursor(0,3);
  lcd.print("Compilando Prog");
  delay(4000);

  lcd.clear();
  lcd.setCursor(0,3);
  lcd.print("Generando senal");
  delay(3000);

  lcd.clear();
  lcd.setCursor(0,3);
  lcd.print("Terminando....");
  delay(2000);
  lcd.clear();

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
}

void loop() {

  leeJoystick();
  joystick();

  switch(frequ){
    case 0:
      atenuador = map(analogRead(A1),0,1023,1,5);
      frequencia = map(analogRead(A0),0,1023,1,100);
      frequencia = frequencia + atenuador;
      AD.setFrequency(frequencia, 0);
      break;

    case 1:
      atenuador = map(analogRead(A1),0,1023,1,50);
      frequencia = map(analogRead(A0),0,1023,100,1000);
      frequencia = frequencia + atenuador;
      AD.setFrequency(frequencia, 0);
      break;

    case 2:
      atenuador = map(analogRead(A1),0,1023,1,500);
      frequencia = map(analogRead(A0),0,1023,1000,10000);
      frequencia = frequencia + atenuador;
      AD.setFrequency(frequencia, 0);
      break;

    case 3:
      atenuador = map(analogRead(A1),0,1023,1,5000);
      frequencia = map(analogRead(A0),0,1023,10000,100000);
      frequencia = frequencia + atenuador;
      AD.setFrequency(frequencia, 0);
      break;
  }

  switch(signal){
    case 0:
      AD.setWave(AD9833_SINE);
      break;

    case 1:
      AD.setWave(AD9833_TRIANGLE);
      break;

    case 2:
      AD.setWave(AD9833_SQUARE1);
      break;
  }
  
  display();
}

void display(){
  lcd.setCursor(0,0);
  lcd.print("F: ");
  if(frequencia <= 1000){
    lcd.print(frequencia);
    lcd.print("Hz");
  }
  if(frequencia >= 1000){
    freka = frequencia / 1000;
    lcd.print(freka);
    lcd.print("KHz");
  }

  lcd.setCursor(0,1);
  lcd.print("SENAL: ");
  if(signal == 0){
    lcd.print("SENO");
  }
  if(signal == 1){
    lcd.print("TRIA");
  }
  if(signal == 2){
    lcd.print("CUAD");
  }

  delay(1000); // La informacion se actualiza cada 1 segundo
  lcd.clear();
}

uint8_t flancoSubida(){
  uint8_t valor_nuevo = digitalRead(A0);
  uint8_t result = button_estate != valor_nuevo && valor_nuevo == 1;
  button_estate = valor_nuevo;
  return result;
}

int leeJoystick(){
  VRx = analogRead(A2); 
  VRy = analogRead(A3);
  mapX = map(VRx, 1023, 0, 0, 7);
  mapXnegado = map(VRx, 0, 1023, 0, 7); // izquierda-derecha [1-2]
  mapY = map(VRy, 0, 1023, 0, 7);
  mapYnegado = map(VRy, 1023, 0, 0, 7); // arriba - abajo    [3-4] 
  btn_push = flancoSubida();      // push-Enter        [ 5 ]
                                  // Sin mover Joystick[ 0 ]
  if(mapX == 0){btnRead = 1;}
  else if(mapXnegado == 0) {btnRead = 2;}
       else if(mapYnegado == 0) {btnRead = 3;}
            else if(mapY == 0) {btnRead = 4;}
                 else if(btn_push == 1) {btnRead = 5;}
                      else btnRead = 0;                  
  return btnRead;                      
}

void joystick(){
  switch(btnRead){
  case 4:          
    if(frequ >= 0 && frequ <= 4){
      delay(400);
      frequ+=1;   
    }
    lcd.clear();    //HACIA ARRIBA
  break;
  case 3:
    if(frequ >= 1 && frequ <= 3){
      delay(400);
      frequ-=1;
    }
    lcd.clear();  // HACIA ABAJO
  break;           
  case 2:
    if(signal >= 0 && signal <= 3){
      delay(400);
      signal+=1;   
    }
    lcd.clear();       // HACI LA DERECHA
  break;
  case 1:
    if(signal >= 1 && signal <= 2){
      delay(400);
      signal-=1;
    }
    lcd.clear();      //HACIA LA IZQUIERDA
  break;
  case 5:
  break;
  case 0:
  break;  
  }
}
