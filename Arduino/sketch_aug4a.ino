const byte OUT_PIN = 2;                 // Pin del sensor
const unsigned long DEBOUNCE_MS = 30;   // Antirrebote (20–50 ms suele andar)
const unsigned long COOLDOWN_MS = 200;  // Ventana mínima entre disparos

bool lastReading = HIGH;                // Lectura anterior (suponiendo sensor activo en LOW)
bool stableState = HIGH;                // Estado estable tras antirrebote
unsigned long lastChangeTime = 0;       // Último instante de cambio
unsigned long lastTriggerTime = 0;      // Último disparo válido

void setup() {
  Serial.begin(9600);
  // Si tu sensor es de colector abierto/salida flotante, conviene el pull-up interno:
  pinMode(OUT_PIN, INPUT_PULLUP);
  // Si tu sensor ya entrega HIGH/LOW firme, podés usar INPUT a secas:
  // pinMode(OUT_PIN, INPUT);
}

void loop() {
  bool reading = !digitalRead(OUT_PIN);

  // Detecto cambio crudo y reinicio el temporizador de antirrebote
  if (reading != lastReading) {
    lastReading = reading;
    lastChangeTime = millis();
  }

  // Cuando pasó el tiempo de antirrebote, acepto el nuevo estado como "estable"
  if (millis() - lastChangeTime > DEBOUNCE_MS) {
    if (stableState != reading) {
      stableState = reading;

      // Disparo sólo en flanco de bajada (HIGH -> LOW)
      if (stableState == LOW) {
        unsigned long now = millis();
        if (now - lastTriggerTime >= COOLDOWN_MS) {
          Serial.println("JUMP");
          lastTriggerTime = now;
        }
      }
    }
  }
}
