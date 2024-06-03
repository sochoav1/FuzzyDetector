float dc;
float voltage; // Variable para almacenar el voltaje medido
int sensorValue;

// Parámetros PID
float Kp = 2.0; // Constante Proporcional
float Ki = 0.5; // Constante Integral
float Kd = 1.0; // Constante Derivativa

float setPoint = 3000; // Voltaje deseado en mV (3000V)
float previousError = 0;
float integral = 0;

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
}

void loop() {
  // Leer el valor analógico del pin A0 y mapearlo a un rango de 0 a 5000 mV
  sensorValue = analogRead(A0);
  voltage = (sensorValue / 1023.0) * 5000;

  // Calcular el error
  float error = setPoint - voltage;

  // Proporcional
  float P = Kp * error;

  // Integral
  integral += error;
  float I = Ki * integral;

  // Derivativo
  float derivative = error - previousError;
  float D = Kd * derivative;

  // Calcular la salida del PID
  float output = P + I + D;

  // Ajustar el ciclo de trabajo del PWM basado en el control PID
  OCR1A = constrain(OCR1A + output, 0, ICR1);
  dc = ((float)OCR1A / (float)ICR1) * 100;

  // Imprimir el ciclo de trabajo y el voltaje medido por el puerto serie
  Serial.print("Voltaje: ");
  Serial.print(voltage);
  Serial.print(" mV, Ciclo de Trabajo: ");
  Serial.print(dc);
  Serial.println(" %");

  // Actualizar el error previo
  previousError = error;

  delay(1000); // Esperar 1 segundo antes de la siguiente lectura
}
