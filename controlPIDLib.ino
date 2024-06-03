#include <PID_v1.h>

float dc;
float voltage; // Variable para almacenar el voltaje medido
int sensorValue;

// Parámetros PID
double setPoint = 3000; // Voltaje deseado en mV (3000V)
double input;  // Voltaje actual en mV
double output; // Salida del PID que ajustará el ciclo de trabajo del PWM

// Definición de las constantes del PID
double Kp = 2.0;
double Ki = 0.5;
double Kd = 1.0;

// Crear un objeto PID
PID myPID(&input, &output, &setPoint, Kp, Ki, Kd, DIRECT);

void setup() {
  // Configuración inicial del Arduino
  TCCR1B = 0x18; // Deshabilitar el temporizador
  TCCR1A = 0xA2; // Configuración del Timer-1 en modo Fast PWM

  ICR1 = 1600 - 1;
  OCR1A = (int) (ICR1 * 0.25);
  TCNT1 = 0x0;

  pinMode(9, OUTPUT);  // Configurar el pin 9 como salida (OC1A)
  TCCR1B |= 1; // Prescale=1, habilitar el temporizador

  pinMode(A0, INPUT); // Configurar el pin A0 como entrada
  Serial.begin(9600); // Inicializar la comunicación serie

  // Inicializar el PID
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(0, ICR1); // Limitar la salida del PID a los límites del Timer-1
}

void loop() {
  // Leer el valor analógico del pin A0 y mapearlo a un rango de 0 a 5000 mV
  sensorValue = analogRead(A0);
  input = (sensorValue / 1023.0) * 5000;

  // Ejecutar el PID
  myPID.Compute();

  // Ajustar el ciclo de trabajo del PWM basado en la salida del PID
  OCR1A = output;
  dc = ((float)OCR1A / (float)ICR1) * 100;

  // Imprimir el ciclo de trabajo y el voltaje medido por el puerto serie
  Serial.print("Voltaje: ");
  Serial.print(input);
  Serial.print(" mV, Ciclo de Trabajo: ");
  Serial.print(dc);
  Serial.println(" %");

  delay(1000); // Esperar 1 segundo antes de la siguiente lectura
}
