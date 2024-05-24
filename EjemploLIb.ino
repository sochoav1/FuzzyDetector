#include <Fuzzy.h>

Fuzzy *fuzzy = new Fuzzy();

void setup() {
  // Inicializar Serial para depuración
  Serial.begin(9600);

  // Definir el conjunto difuso
  FuzzySet *low = new FuzzySet(0, 0, 1.5, 2.5);
  FuzzySet *medium = new FuzzySet(2.0, 2.5, 3.5, 4.0);
  FuzzySet *high = new FuzzySet(3.5, 4.5, 5, 5);

  // Crear una entrada difusa
  FuzzyInput *input = new FuzzyInput(1);
  input->addFuzzySet(low);
  input->addFuzzySet(medium);
  input->addFuzzySet(high);
  fuzzy->addFuzzyInput(input);

  // Crear una salida difusa (no utilizada en este ejemplo)
  FuzzyOutput *output = new FuzzyOutput(1);
  fuzzy->addFuzzyOutput(output);

  // Definir reglas (no necesarias ya que no controlamos nada)
  FuzzyRuleAntecedent *ifLow = new FuzzyRuleAntecedent();
  ifLow->joinSingle(low);

  FuzzyRuleConsequent *thenNothing = new FuzzyRuleConsequent();
  thenNothing->addOutput(output);

  FuzzyRule *fuzzyRule1 = new FuzzyRule(1, ifLow, thenNothing);
  fuzzy->addFuzzyRule(fuzzyRule1);

  // Configurar el pin A0 como entrada
  pinMode(A0, INPUT);
}

void loop() {
  // Leer el valor del sensor (0 a 5V)
  int sensorValue = analogRead(A0);
  float voltage = (sensorValue / 1023.0) * 5.0;

  // Asignar el valor leído al conjunto difuso
  fuzzy->setInput(1, voltage);

  // Ejecutar la lógica difusa
  fuzzy->fuzzify();

  // Obtener el grado de pertenencia de cada conjunto
  float lowMembership = fuzzy->getFuzzyInput(1)->getFuzzySet("low")->getPertinence();
  float mediumMembership = fuzzy->getFuzzyInput(1)->getFuzzySet("medium")->getPertinence();
  float highMembership = fuzzy->getFuzzyInput(1)->getFuzzySet("high")->getPertinence();

  // Imprimir los grados de pertenencia
  Serial.print("Voltaje: ");
  Serial.print(voltage);
  Serial.print(" V, Low Membership: ");
  Serial.print(lowMembership);
  Serial.print(", Medium Membership: ");
  Serial.print(mediumMembership);
  Serial.print(", High Membership: ");
  Serial.println(highMembership);

  delay(1000); // Esperar 1 segundo antes de la siguiente lectura
}
