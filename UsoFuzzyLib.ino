#include <Fuzzy.h>

Fuzzy *fuzzy = new Fuzzy();

float dc;
float voltage; // Variable para almacenar el voltaje medido
int sensorValue;

void setup()
{
    // Configuración inicial del Arduino
    TCCR1B = 0x18; // Deshabilitar el temporizador
    TCCR1A = 0xA2;

    ICR1 = 1600 - 1;
    OCR1A = (int)(ICR1 * 0.25);
    TCNT1 = 0x0;

    pinMode(9, OUTPUT); // OC1a
    TCCR1B |= 1;        // Prescale=1, Habilitar el temporizador

    pinMode(A0, INPUT);
    Serial.begin(9600);

    // Definir conjuntos difusos y funciones de membresía
    FuzzySet *low = new FuzzySet(0, 0, 1.5, 2.5);
    FuzzySet *medium = new FuzzySet(2.0, 2.5, 3.5, 4.0);
    FuzzySet *high = new FuzzySet(3.5, 4.5, 5, 5);

    // Crear una entrada difusa para el voltaje
    FuzzyInput *input = new FuzzyInput(1);
    input->addFuzzySet(low);
    input->addFuzzySet(medium);
    input->addFuzzySet(high);
    fuzzy->addFuzzyInput(input);

    // Crear una salida difusa para el ajuste del PWM
    FuzzyOutput *output = new FuzzyOutput(1);
    FuzzySet *pwmDecrease = new FuzzySet(-50, -50, -25, 0);
    FuzzySet *pwmMaintain = new FuzzySet(-10, 0, 0, 10);
    FuzzySet *pwmIncrease = new FuzzySet(0, 25, 50, 50);
    output->addFuzzySet(pwmDecrease);
    output->addFuzzySet(pwmMaintain);
    output->addFuzzySet(pwmIncrease);
    fuzzy->addFuzzyOutput(output);

    // Definir reglas difusas
    FuzzyRuleAntecedent *ifLow = new FuzzyRuleAntecedent();
    ifLow->joinSingle(low);
    FuzzyRuleConsequent *thenIncreasePWM = new FuzzyRuleConsequent();
    thenIncreasePWM->addOutput(pwmIncrease);

    FuzzyRuleAntecedent *ifMedium = new FuzzyRuleAntecedent();
    ifMedium->joinSingle(medium);
    FuzzyRuleConsequent *thenMaintainPWM = new FuzzyRuleConsequent();
    thenMaintainPWM->addOutput(pwmMaintain);

    FuzzyRuleAntecedent *ifHigh = new FuzzyRuleAntecedent();
    ifHigh->joinSingle(high);
    FuzzyRuleConsequent *thenDecreasePWM = new FuzzyRuleConsequent();
    thenDecreasePWM->addOutput(pwmDecrease);

    FuzzyRule *fuzzyRule1 = new FuzzyRule(1, ifLow, thenIncreasePWM);
    fuzzy->addFuzzyRule(fuzzyRule1);
    FuzzyRule *fuzzyRule2 = new FuzzyRule(2, ifMedium, thenMaintainPWM);
    fuzzy->addFuzzyRule(fuzzyRule2);
    FuzzyRule *fuzzyRule3 = new FuzzyRule(3, ifHigh, thenDecreasePWM);
    fuzzy->addFuzzyRule(fuzzyRule3);
}

void loop()
{
    // Leer el valor analógico del pin A0 y mapearlo a un rango de 0 a 5000
    sensorValue = analogRead(A0);
    voltage = (sensorValue / 1023.0) * 5.0; // Leer el voltaje en un rango de 0 a 5V

    // Control difuso
    fuzzy->setInput(1, voltage);
    fuzzy->fuzzify();

    // Obtener el ajuste del PWM
    float deltaPWM = fuzzy->defuzzify(1);

    // Ajustar el ciclo de trabajo del PWM basado en el control difuso
    OCR1A = constrain(OCR1A + deltaPWM, 0, ICR1);
    dc = ((float)OCR1A / (float)ICR1) * 100;

    // Imprimir el ciclo de trabajo y el voltaje medido por el puerto serie
    Serial.print("Voltaje: ");
    Serial.print(voltage);
    Serial.print(" V, Ciclo de Trabajo: ");
    Serial.print(dc);
    Serial.println(" %");

    delay(1000); // Esperar 1 segundo antes de la siguiente lectura
}
