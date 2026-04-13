/*
  ESP32-C6 Hack Demo — Web Sunucusu
  ==================================
  Sunum sırasında bir web sayfası yayınlar.
  /durum endpoint'i varsayılan olarak "BEKLE" döndürür.
  /tetikle endpoint'ine girilince "SIL" moduna geçer.

  Bağlantı:
    - ESP32-C6 ve bilgisayar aynı Wi-Fi ağında olmalı
    - IP adresi seri monitörden öğrenilir

  Kütüphane: Arduino IDE -> Board: "ESP32C6 Dev Module"
*/

#include <WiFi.h>
#include <WebServer.h>

// ============================================================
// Wi-Fi Bilgileri — Kendi ağını yaz
// ============================================================
const char* ssid     = "WIFI_ADI";
const char* password = "WIFI_SIFRESI";
// ============================================================

WebServer server(80);

// Durum değişkeni: "BEKLE" veya "SIL"
String durum = "BEKLE";

// ---- Endpoint: /durum ----
// Python bu endpoint'i sürekli sorgular
void handleDurum() {
  server.send(200, "text/plain", durum);
}

// ---- Endpoint: /tetikle ----
// Sen telefonundan bu adrese girince durum "SIL" olur
void handleTetikle() {
  durum = "SIL";
  server.send(200, "text/html",
    "<html><body style='font-family:sans-serif; text-align:center; margin-top:80px;'>"
    "<h1 style='color:red;'>&#x1F4A5; HEDEF YOK EDİLİYOR!</h1>"
    "<p>Python betiği 'SIL' sinyalini aldı.</p>"
    "<p>Klasör birkaç saniye içinde silinecek.</p>"
    "</body></html>"
  );
  Serial.println("[!] SIL komutu gönderildi!");
}

// ---- Endpoint: /sifirla ----
// Sunumu tekrar yapmak istersen durumu sıfırlar
void handleSifirla() {
  durum = "BEKLE";
  server.send(200, "text/html",
    "<html><body style='font-family:sans-serif; text-align:center; margin-top:80px;'>"
    "<h1 style='color:green;'>&#x2705; Sıfırlandı</h1>"
    "<p>Durum tekrar 'BEKLE' olarak ayarlandı.</p>"
    "</body></html>"
  );
  Serial.println("[*] Durum sıfırlandı: BEKLE");
}

// ---- Ana Kontrol Paneli ----
void handleRoot() {
  String html = "<html><head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>body{font-family:sans-serif;text-align:center;background:#111;color:#0f0;padding:40px;}";
  html += "h1{color:#0f0;} .btn{display:inline-block;padding:20px 40px;margin:10px;border:none;border-radius:8px;font-size:20px;cursor:pointer;text-decoration:none;}";
  html += ".red{background:#c0392b;color:white;} .green{background:#27ae60;color:white;}</style></head>";
  html += "<body><h1>&#x1F4BB; ESP32 HACK DEMO</h1>";
  html += "<p>Mevcut Durum: <strong>" + durum + "</strong></p>";
  html += "<a href='/tetikle' class='btn red'>&#x1F4A5; SİL KOMUTU GÖNDER</a><br>";
  html += "<a href='/sifirla' class='btn green'>&#x1F504; Sıfırla (BEKLE)</a>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n[*] ESP32-C6 Hack Demo Başlatılıyor...");
  Serial.print("[*] Wi-Fi'ye bağlanılıyor: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  // Bağlantı bekleniyor
  int deneme = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    deneme++;
    if (deneme > 30) {
      Serial.println("\n[HATA] Wi-Fi bağlantısı kurulamadı!");
      return;
    }
  }

  Serial.println("\n[✓] Wi-Fi bağlandı!");
  Serial.print("[✓] IP Adresi: ");
  Serial.println(WiFi.localIP());
  Serial.println("[*] Tarayıcıdan bu IP'ye gir ve 'SİL' butonuna bas.\n");

  // Endpoint'leri tanımla
  server.on("/",        handleRoot);
  server.on("/durum",   handleDurum);
  server.on("/tetikle", handleTetikle);
  server.on("/sifirla", handleSifirla);

  server.begin();
  Serial.println("[✓] Web sunucusu başlatıldı. Hazır!\n");
}

void loop() {
  server.handleClient();
}
