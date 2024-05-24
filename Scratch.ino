float dc;
float voltage; // Variable para almacenar el voltaje medido
int sensorValue;

void setup() {
  // Configuración inicial del Arduino
  TCCR1B = 0x18; // Deshabilitar el temporizador
  TCCR1A = 0xA2;

  ICR1 = 1600 - 1;
  OCR1A = (int) (ICR1 * 0.25);
  TCNT1 = 0x0;

  pinMode(9, OUTPUT);  // OC1a
  TCCR1B |= 1; // Prescale=1, Habilitar el temporizador

  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void loop() {
  // Leer el valor analógico del pin A0 y mapearlo a un rango de 0 a 5000
  sensorValue = analogRead(A0);
  voltage = (sensorValue / 1023.0) * 5000;

  // Control difuso
  float error = 3000 - voltage; // Error con respecto al voltaje deseado de 3000V
  float deltaPWM = fuzzyControl(error);

  // Ajustar el ciclo de trabajo del PWM basado en el control difuso
  OCR1A = constrain(OCR1A + deltaPWM, 0, ICR1);
  dc = ((float)OCR1A / (float)ICR1) * 100;
  
  // Imprimir el ciclo de trabajo y el voltaje medido por el puerto serie
  Serial.print("Voltaje: ");
  Serial.print(voltage);
  Serial.print(" V, Ciclo de Trabajo: ");
  Serial.print(dc);
  Serial.println(" %");

  delay(1000);
}

// Función de control difuso
float fuzzyControl(float error) {
  // Definir conjuntos difusos y funciones de membresía
  float low = max(0, min(1, (2000 - error) / 2000.0));
  float medium = max(0, min(1, (error - 2000) / 2000.0, (4000 - error) / 2000.0));
  float high = max(0, min(1, (error - 4000) / 1000.0));
  
  // Evaluar las reglas difusas
  float pwmLow = low * 50; // Aumentar PWM
  float pwmMedium = medium * 0; // Mantener PWM
  float pwmHigh = high * -50; // Disminuir PWM

  // Agregar resultados ponderados
  float deltaPWM = (pwmLow + pwmMedium + pwmHigh) / (low + medium + high + 0.0001); // Para evitar división por cero

  return deltaPWM;
}
