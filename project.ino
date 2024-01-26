#include "Ultrasonic.h" //INCLUSÃO DA BIBLIOTECA NECESSÁRIA PARA FUNCIONAMENTO DO CÓDIGO

//sensor carro 1
const int echoPin_car1 = 7; //PINO DIGITAL UTILIZADO PELO HC-SR04 ECHO(RECEBE)
const int trigPin_car1 = 6; //PINO DIGITAL UTILIZADO PELO HC-SR04 TRIG(ENVIA)
Ultrasonic ultrasonic_car1(trigPin_car1,echoPin_car1); //INICIALIZANDO OS PINOS DO ARDUINO

//sensor carro 2
const int echoPin_car2 = 8; //PINO DIGITAL UTILIZADO PELO HC-SR04 ECHO(RECEBE)
const int trigPin_car2 = 9; //PINO DIGITAL UTILIZADO PELO HC-SR04 TRIG(ENVIA)
Ultrasonic ultrasonic_car2(trigPin_car2,echoPin_car2); //INICIALIZANDO OS PINOS DO ARDUINO

//int trigPin;
int distancia_car1 = 100; //VARIÁVEL DO TIPO INTEIRO
int distancia_car2 = 100; //VARIÁVEL DO TIPO INTEIRO

//botões
const int pinoInterrupcao_botton = 2;

volatile bool cronometro_car1 = false;
volatile bool cronometro_car2 = false;
volatile unsigned long tempo_car1 = 0;
volatile unsigned long tempo_car2 = 0;
float tempo_final_car1 = 0;                 //segundos
float tempo_final_car2 = 0;
const float espaco = 73;                   //centimetros (DECIDIR)
volatile float velocidade_car1 = 0;
volatile float velocidade_car2 = 0;

void setup() {
  //sensor carro 1
  pinMode(echoPin_car1, INPUT); //DEFINE O PINO COMO ENTRADA (RECEBE)
  pinMode(trigPin_car1, OUTPUT); //DEFINE O PINO COMO SAIDA (ENVIA)
  //senro carro 2
  pinMode(echoPin_car2, INPUT); //DEFINE O PINO COMO ENTRADA (RECEBE)
  pinMode(trigPin_car2, OUTPUT); //DEFINE O PINO COMO SAIDA (ENVIA)

  //botões
  pinMode(pinoInterrupcao_botton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinoInterrupcao_botton), timer_init, RISING);
 /* pinMode(pinoInterrupcao_sensor2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinoInterrupcao_sensor2), timer_end, RISING);*/

  Serial.begin(9600);
}

void loop() {
  if (cronometro_car1 || cronometro_car2){
    hcsr04(); // FAZ A CHAMADA DO MÉTODO "hcsr04()"
  }
  if (cronometro_car1 && distancia_car1 < 25){
    timer_end_car1();
  }
  if (cronometro_car2 && distancia_car2 < 15){
    timer_end_car2();
  }
  /*if (!cronometro && distancia_1 < 10 ){                  //COLOCAR A DISTANCIA EM QUE A PILHA PASSA PELO SENSOR 1 AQUI
    timer_init();
  }else if (cronometro && distancia_2 < 10 ){             //COLOCAR A DISTANCIA EM QUE A PILHA PASSA PELO SENSOR 2 AQUI
    timer_end();
  }*/
}

void timer_init() {
  if (!cronometro_car1 && !cronometro_car2) {
    cronometro_car1 = true;
    cronometro_car2 = true;
    tempo_car1 = millis();  // Armazena o tempo atual
    tempo_car2 = millis();
    Serial.println(" ");
  } 
}

void timer_end_car1() {
  if (cronometro_car1) {
    cronometro_car1 = false;
    tempo_final_car1 = millis() - tempo_car1;  // Calcula o tempo decorrido
    tempo_car1 = 0;  // Reinicia o tempo inicial
    
    tempo_final_car1 = tempo_final_car1/1000;  // Convertendo para segundos
    velocidade_car1 = espaco/tempo_final_car1;

    Serial.print(velocidade_car1);
    Serial.println(" cm/s car 1");
  }
}

void timer_end_car2() {
  if (cronometro_car2) {
    cronometro_car2 = false;
    tempo_final_car2 = millis() - tempo_car2;  // Calcula o tempo decorrido
    tempo_car2 = 0;  // Reinicia o tempo inicial
    
    tempo_final_car2 = tempo_final_car2/1000;  // Convertendo para segundos
    velocidade_car2 = espaco/tempo_final_car2;

    Serial.print(velocidade_car2);
    Serial.println(" cm/ car 2");
  }
}

//MÉTODO RESPONSÁVEL POR CALCULAR A DISTÂNCIA 
void hcsr04(){
  digitalWrite(trigPin_car1, LOW); //SETA O PINO 6 COM UM PULSO BAIXO "LOW"
  delayMicroseconds(2); //INTERVALO DE 2 MICROSSEGUNDOS
  digitalWrite(trigPin_car1, HIGH); //SETA O PINO 6 COM PULSO ALTO "HIGH"
  delayMicroseconds(10); //INTERVALO DE 10 MICROSSEGUNDOS
  digitalWrite(trigPin_car1, LOW); //SETA O PINO 6 COM PULSO BAIXO "LOW" NOVAMENTE
  //FUNÇÃO RANGING, FAZ A CONVERSÃO DO TEMPO DE
  //RESPOSTA DO ECHO EM CENTIMETROS, E ARMAZENA
  //NA VARIAVEL "distancia"
  distancia_car1 = (ultrasonic_car1.Ranging(CM)); //VARIÁVEL GLOBAL RECEBE O VALOR DA DISTÂNCIA MEDIDA

  digitalWrite(trigPin_car2, LOW); //SETA O PINO 6 COM UM PULSO BAIXO "LOW"
  delayMicroseconds(2); //INTERVALO DE 2 MICROSSEGUNDOS
  digitalWrite(trigPin_car2, HIGH); //SETA O PINO 6 COM PULSO ALTO "HIGH"
  delayMicroseconds(10); //INTERVALO DE 10 MICROSSEGUNDOS
  digitalWrite(trigPin_car2, LOW); //SETA O PINO 6 COM PULSO BAIXO "LOW" NOVAMENTE
  //FUNÇÃO RANGING, FAZ A CONVERSÃO DO TEMPO DE
  //RESPOSTA DO ECHO EM CENTIMETROS, E ARMAZENA
  //NA VARIAVEL "distancia"
  distancia_car2 = (ultrasonic_car2.Ranging(CM));

  delay(100); //INTERVALO DE 500 MILISSEGUNDOS

  /*if (!cronometro){
    distancia_1 = (ultrasonic_1.Ranging(CM)); //VARIÁVEL GLOBAL RECEBE O VALOR DA DISTÂNCIA MEDIDA
    distancia_2 = 100;
  }else{
    distancia_2 = (ultrasonic_2.Ranging(CM));
    distancia_1 = 100;
  }*/
}

