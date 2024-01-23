#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define OLED_RESET -1

// Gemi boyutları
#define SHIP_WIDTH 8
#define SHIP_HEIGHT 8

// Ekran boyutları
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Oyun değişkenleri
int shipX;
int shipY; // Gemimizin konumu
int shipLife = 3;
int shotCount = 3;
int shotX;
int shotY;
int stoneX1; // Engel konumu
int stoneX2;
int lPointX;
int lPointY;
int sPointX;
int sPointY;
int chooseL;
int chooseS;
int score = 0;    // Oyuncu puanı
int stoneY1;
int chooseY1;
int stoneY2;
int chooseY2;
int locationY1[] = {0, 8, 16, 24, 32, 48, 56, 64};
int locationY2[] = {0, 8, 16, 24, 32, 48, 56, 64};
int locationLife[] = {4, 12, 20, 28, 36, 52, 60};
int locationShot[] = {4, 12, 20, 28, 36, 52, 60};

int gecikme;
int can;
int skorArttirma = 128;

int menuPointer  = 10; // menu elemanlari
int pageNumber;
int son = 0; 
int diger = 1; 
volatile boolean tus = false;

int digit1 = 5; // 1. display için Arduino pin numarası
int digit2 = 4; // 2. display için Arduino pin numarası
int digit3 = 3; // 3. display için Arduino pin numarası
int segmentA = 13; // A segmenti için Arduino pin numarası
int segmentB = 12; // B segmenti için Arduino pin numarası
int segmentC = 11; // C segmenti için Arduino pin numarası
int segmentD = 10; // D segmenti için Arduino pin numarası
int segmentE = 9; // E segmenti için Arduino pin numarası
int segmentF = 8; // F segmenti için Arduino pin numarası
int segmentG = 7; // G segmenti için Arduino pin numarası

byte digits[10][7] = {
  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,0,0,1,1}  // 9
};

void setup() {
  // Ekranın başlatılması
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  
  // Gemimizin başlangıç konumunu ayarla
  shipX = 0;
  shipY = 12;
  
  // Rastgele bir yıldız konumu seç
  randomSeed(analogRead(0));
  stoneX1 = 128;
  stoneX2 = 112;
  stoneY2 = 32;
  lPointX = 128;
  lPointY = 12;
  shotX = shipX + 4;
  shotY = shipY;
  sPointX = 120;
  sPointY = 36;


  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A6, OUTPUT);
  pinMode(A7, OUTPUT);
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(6, OUTPUT);
  pinMode(52, INPUT_PULLUP);
  pinMode(53, INPUT);
  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
  pinMode(segmentA, OUTPUT);
  pinMode(segmentB, OUTPUT);
  pinMode(segmentC, OUTPUT);
  pinMode(segmentD, OUTPUT);
  pinMode(segmentE, OUTPUT);
  pinMode(segmentF, OUTPUT);
  pinMode(segmentG, OUTPUT);

  gecikme = 30;
  can = 0;
  pageNumber = 1;
}

void loop() {
  // Ekranın temizlenmesi
  display.clearDisplay();

  int upDownButton = digitalRead(52); // menu butonlari atama islemi
  int selectButton = digitalRead(53);

    if (pageNumber == 1) 
  {
    display.clearDisplay();
    display.setCursor(36, 0);
    display.setTextColor(SSD1306_WHITE);
    display.println("ANA MENU");
    display.setCursor(10, 10);
    display.setTextColor(SSD1306_WHITE);
    display.print("BASLAT");
    display.setCursor(10, 25);
    display.setTextColor(SSD1306_WHITE);
    display.print("ZORLUK SEVIYESI");
    display.setCursor(0, menuPointer);
    display.setTextColor(SSD1306_WHITE);
    display.print(">"); 
    display.display();
    yon();
  }

  secim();

  if (menuPointer == 10 && pageNumber == 1 && selectButton == 1 )
  {
    pageNumber = 2;
  }

  if (pageNumber == 2)
  {
    if (stoneX1 == 128){ //rastgele engel koymak icin if kontrolu
  chooseY1 = random(0,9);
  stoneY1 = locationY1[chooseY1];
  }

  if (stoneX2 == 128) { // itemlerin cakismalarini engellemek icin if kontrolu
  chooseY2 = random(0,9);
  stoneY2 = locationY2[chooseY2];
  if (stoneY1 == stoneY2){
    chooseY2 = random(0,9);
    stoneY2 = locationY2[chooseY2];
    }
  }

  if (lPointX == 128) { // itemlerin cakismalarini engellemek icin if kontrolu
    chooseL = random(0,7);
    lPointY = locationLife[chooseL];
  }

  if (sPointX == 128) {
    chooseS = random(0,7);
    sPointY = locationShot[chooseS];
  }

   if (shotCount == 3) { // atis sayisina gore led yakip sondurme
    digitalWrite(A3, HIGH);
    digitalWrite(A6, HIGH);
    digitalWrite(A7, HIGH);
  }
  if (shotCount == 2) {
    digitalWrite(A3, HIGH);
    digitalWrite(A6, LOW);
    digitalWrite(A7, HIGH);
  }
  if (shotCount == 1) {
    digitalWrite(A3, HIGH);
    digitalWrite(A6, LOW);
    digitalWrite(A7, LOW);
  }
  if (shotCount == 0) {
    digitalWrite(A3, LOW);
    digitalWrite(A6, LOW);
    digitalWrite(A7, LOW);
  }

  gemiCiz();
  
  engel1();
  engel2();
  lifePoint();
  shotPoint();

  display.display(); // ekran güncellemesi

  if (digitalRead(0) == LOW) {
    shot();
  }

  backgroundMovement();

  yeniEngel();

  control();

  if ((stoneX1 <= 8 || stoneX2 <= 8) && (stoneY1 == shipY-2 || stoneY1 == shipY+4 || stoneY2 == shipY-2 || stoneY2 == shipY+4)) { // carpisma kontrolu
    if (shipLife > 0 && shipLife <= 3) {
      shipLife--;
      tone(6, 1000, 1000);
    }
    else if (shipLife == 0) {
    if (stoneY1 == shipY+4){
      stoneX1 = 128;
      can++;
    }
    if (stoneY2 == shipY+4) {
      stoneX2 = 128;
      can++;
    }
    if (can == 3) {
      while(1);
      display.clearDisplay();
      display.display();
      pageNumber = 1;
    }
    }
  }

  if (skorArttirma < 0) { // skorun arttirildigi kisim
    skorArttirma = 128;
    score++;
  }

  if (lPointX <= 8 && (lPointY == shipY-4 || lPointY == shipY+4)){ // can arttirma itemini alirsa can artisi yapan kisim
    lPointX = 128;
    if (shipLife < 3) {
      shipLife++;
    }
    else {
      shipLife = 3;
    }
  }

  if (sPointX <= 8 && (sPointY == shipY-4 || sPointY == shipY+4)){ // atis sayisi arttirma itemini alirsa can artisi yapan kisim
    sPointX = 128;
    if (shotCount < 3) {
      shotCount++;
    }
    else {
      shotCount = 3;
    }
  }

  if ((shotX == stoneX1+4 || shotX == stoneX1+3 || shotX == stoneX1+2 || shotX == stoneX1+1 || shotX == stoneX1 || shotX == stoneX1-1 || shotX == stoneX1-2 || shotX == stoneX1-3 || shotX == stoneX1-4) && (shotY == stoneY1+4 || shotY == stoneY1+3 || shotY == stoneY1+2 || shotY == stoneY1+1 || shotY == stoneY1 || shotY == stoneY1-1 || shotY == stoneY1-2 || shotY == stoneY1-3 || shotY == stoneY1-4)) {
    stoneX1 = 128; // atis buyuk kayaya carparsa engelin yok oldugu kisim    
  }

  if ((shotX == stoneX2+2 || shotX == stoneX2+1 || shotX == stoneX2 || shotX == stoneX2-1 || shotX == stoneX2-2) && (shotY == stoneY2+2 || shotY == stoneY2+1 || shotY == stoneY2 || shotY == stoneY2-1 || shotY == stoneY2-2)) {
    stoneX2 = 128; // atis kucuk kayaya carparsa engelin yok oldugu kisim
  }

  if (shipLife == 3) { // can sayisina ait led yakip sondurme kismi
    digitalWrite(A0, HIGH);
    digitalWrite(A1, HIGH);
    digitalWrite(A2, HIGH);
  }
  if (shipLife == 2) {
    digitalWrite(A0, LOW);
    digitalWrite(A1, HIGH);
    digitalWrite(A2, HIGH);
  }
  if (shipLife == 1) {
    digitalWrite(A0, LOW);
    digitalWrite(A1, LOW);
    digitalWrite(A2, HIGH);
  }
  if (shipLife == 0) {
    digitalWrite(A0, LOW);
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);
  }

  digitalWrite(digit1, LOW);
  digitalWrite(digit2, LOW);
  digitalWrite(digit3, HIGH);
  showDigit(score);
  if (score > 9 && score < 100) {
    int tam = score / 10;
    int elde = score % 10;
    digitalWrite(digit1, LOW);
    digitalWrite(digit2, LOW);
    digitalWrite(digit3, HIGH);
    showDigit(elde);
    delay(10);

    digitalWrite(digit1, LOW);
    digitalWrite(digit2, HIGH);
    digitalWrite(digit3, LOW);
    showDigit(tam);
    delay(10);
  }
  if (score > 99 && score < 999) {
    int yuzluk = score / 100;
    int onluk = (score / 10) % 10;
    int birlik = score % 10;
    digitalWrite(digit1, LOW);
    digitalWrite(digit2, LOW);
    digitalWrite(digit3, HIGH);
    showDigit(birlik);
    delay(10);

    digitalWrite(digit1, LOW);
    digitalWrite(digit2, HIGH);
    digitalWrite(digit3, LOW);
    showDigit(onluk);
    delay(10);

    digitalWrite(digit1, HIGH);
    digitalWrite(digit2, LOW);
    digitalWrite(digit3, LOW);
    showDigit(yuzluk);
    delay(10);
  }

  delay(gecikme); // Oyun hızını kontrol etmek için gecikme
  }

  if (menuPointer == 25 && pageNumber == 1 && selectButton == 1 )
  {
    pageNumber = 3;
  }

  if (pageNumber == 3 )
  {
    display.clearDisplay();
    display.setCursor(20, 0);
    display.setTextColor(SSD1306_WHITE);
    display.print("ZORLUK SEVIYESI");
    display.setCursor(10, 10);
    display.setTextColor(SSD1306_WHITE);
    display.print("ZORLUK 1");
    display.setCursor(10, 25);
    display.setTextColor(SSD1306_WHITE);
    display.print("ZORLUK 2");
    display.setCursor(0, menuPointer);
    display.setTextColor(SSD1306_WHITE);
    display.print(">"); 
    display.display();
    yon();

    if (menuPointer == 10 && pageNumber == 3 && digitalRead(53) == LOW) {
      pageNumber = 2;
    }

    if (menuPointer == 25 && pageNumber == 3 && digitalRead(53) == LOW) {
      gecikme = 18;
      pageNumber = 2;
    }
  }
}

void secim() { // menu methodlari
  int selectButton = digitalRead(53);
  if (selectButton == 1 && menuPointer != 10 && menuPointer != 25)
  {
    pageNumber = 2;
    menuPointer = 10;
  }
}

void yon() { // menu methodlari
  int upDownButton = digitalRead(52);
  if (upDownButton == 0)
  {
    diger = 0;
  }
  if (upDownButton == 1 && diger == 0)
  {
    son = 1;
    tus = true;
  }
  if (son == 1 && tus == true)
  { 
    diger = 1;
    display.clearDisplay();
    display.display();
    menuPointer = menuPointer + 15;
    tus = false; 
    son = 0;

    if (menuPointer > 30)
    {
      menuPointer = 10;
    }
  }
}

/*--------------------------------------------------------------------*/

void gemiCiz() {
  display.fillRect(shipX, shipY, 8, 8, SSD1306_WHITE);
}

void engel1() {
  display.fillCircle(stoneX1, stoneY1, 4, SSD1306_WHITE);
}

void engel2() {
  display.fillCircle(stoneX2, stoneY2, 2, SSD1306_WHITE);
}

void lifePoint() {
  display.drawRect(lPointX, lPointY, 6, 6, SSD1306_WHITE);
}

void shotPoint() {
  display.drawRoundRect(sPointX, sPointY, 8, 8, 5, SSD1306_WHITE);
}

void backgroundMovement() {
  stoneX1 -= 4;
  stoneX2 -= 6;
  lPointX -= 4;
  sPointX -= 4;
  skorArttirma -= 64;
  if (digitalRead(0) == LOW) {
    shotX += 4;
  }
}

void control() {
  if (digitalRead(1) == LOW && shipY > 0) {
    shipY = shipY - 4;
  }
  if (digitalRead(2) == LOW && shipY < 60) {
    shipY = shipY + 4;
  }

  if (shipY == 4) {
    shipY = 4;
  }  
}

void yeniEngel() {
  if (stoneX1 < 0) {
    stoneX1 = 128; // Ekran genişliği (128 piksel) kadar
  }
  if (stoneX2 < 0) {
    stoneX2 = 128; // Ekran genişliği (128 piksel) kadar
  }
  if (lPointX < 0) {
    lPointX = 128;
  }
  if (sPointX < 0) {
    sPointX = 128;
  }
}

void shot() {
  if (shotCount > 0) { // Kontrol, atış sayısının 0'dan büyük olduğundan emin olur
    if (shotCount == 3) { // Kontrol, ekrandaki mermi sayısının 3 olduğundan emin olur
      display.fillRect(shipX + 4, shipY - 5, 1, 5, SSD1306_BLACK);
    }
    // Mermi konumunu güncelle
    shotX += 3;
    // Mermiyi ekrana çiz
    display.drawPixel(shotX, shotY, SSD1306_WHITE);
    // Ekrana güncelle
    display.display();
    // Mermi ekranın dışına çıktı mı kontrol et
    if (shotX > SCREEN_WIDTH) {
      // Mermiyi ekrandan kaldır
      display.drawPixel(shotX, shotY, SSD1306_BLACK);
      // Atış sayısını azalt
      shotCount--;
      // Yeni mermi konumunu ayarla
      shotX = shipX + 4;
      shotY = shipY;
    }
  }
}
void showDigit(int digit) {
  if (digit == 0){
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
    digitalWrite(7, HIGH);
  }
  if (digit == 1){
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
  }
  if (digit == 2){
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(11, HIGH);
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
    digitalWrite(8, HIGH);
    digitalWrite(7, LOW);
  }
  if (digit == 3){
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(7, LOW);
  }
  if (digit == 4){
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
  }
  if (digit == 5){
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
  }
  if (digit == 6){
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
  }
  if (digit == 7){
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
  }
  if (digit == 8){
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
  }
  if (digit == 9){
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
  }
}
