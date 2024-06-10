#include <Adafruit_CircuitPlayground.h>

#define NUMPIXELS 10 // Number of NeoPixels on the CPE
#define WIN_SCORE 21

int targetPosition = -1; // Position of the target LED
int currentPosition = 0; // Current position of the spinning LED
bool gameActive = false; // Game state
int playerScores[2] = {0, 0}; // Scores for two players
int currentPlayer = 0; // Which player's turn it is

void setup() {
  CircuitPlayground.begin(); // Initialize the Circuit Playground
  Serial.begin(9600);
  Serial.println("Reset");
}

void loop() {
  if (CircuitPlayground.leftButton()) {
    startGame();
  }

  if (gameActive && CircuitPlayground.rightButton()) {
    int score = checkScore();
    updateScore(score);
    switchPlayer();
    gameActive = false; // Stop the game until restarted
    delay(500); // Debouncing delay
  }

  if (gameActive) {
    spinLEDs();
  }
}

void startGame() {
  fillLEDs(255, 0, 0); // All LEDs turn red briefly
  delay(500);
  clearLEDs();
  targetPosition = random(NUMPIXELS); // Set a random target position
  gameActive = true; // Start spinning the LED
}

void spinLEDs() {
  clearLEDs();
  CircuitPlayground.setPixelColor(currentPosition, 0, 255, 0); // Green LED spins
  currentPosition = (currentPosition + 1) % NUMPIXELS; // Move to the next LED
  delay(100);
}

int checkScore() {
  if (currentPosition == targetPosition) {
    return 3; // Exact match
  } else if (abs(currentPosition - targetPosition) == 1 || abs(currentPosition - targetPosition) == (NUMPIXELS - 1)) {
    return 1; // One position off
  }
  return 0; // No points
}

void updateScore(int score) {
  playerScores[currentPlayer] += score;
  Serial.print("Player ");
  Serial.print(currentPlayer + 1);
  Serial.print(" Score: ");
  Serial.println(playerScores[currentPlayer]);
  if (playerScores[currentPlayer] >= WIN_SCORE) {
    Serial.print("Player ");
    Serial.print(currentPlayer + 1);
    Serial.println(" Wins!");
    gameActive = false;
  }
}

void switchPlayer() {
  currentPlayer = (currentPlayer + 1) % 2; // Toggle between player 1 and 2
}

void fillLEDs(byte r, byte g, byte b) {
  for (int i = 0; i < NUMPIXELS; i++) {
    CircuitPlayground.setPixelColor(i, r, g, b);
  }
}

void clearLEDs() {
  fillLEDs(0, 0, 0);
}
