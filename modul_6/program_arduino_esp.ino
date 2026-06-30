#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

const char* ssid = "Ajibarang";
const char* password = "celekcelek";
const char* mqtt_server = "broker.hivemq.com";

// Konfigurasi DHT11
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  Serial.println();
  Serial.println("=================================");
  Serial.println("Mulai Proses Booting...");
  
  // 1. TRICK KHUSUS ESP32: Bersihkan cache WiFi yang nyangkut
  WiFi.disconnect(true);
  delay(1000);
  
  // 2. Paksa menjadi mode penerima (Station)
  WiFi.mode(WIFI_STA); 
  
  Serial.print("Mencoba konek ke WiFi: ");
  Serial.println(ssid);
  
  // 3. Mulai menyambung
  WiFi.begin(ssid, password);
  
  // 4. Beri batas waktu (Timeout) agar tidak titik-titik selamanya
  int batas_waktu = 0;
  while (WiFi.status() != WL_CONNECTED && batas_waktu < 20) { 
    delay(500); 
    Serial.print("."); 
    batas_waktu++;
  }

  // Cek apakah sukses atau gagal
  Serial.println();
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("GAGAL KONEK WIFI! Silakan cek Hotspot HP Anda.");
  } else {
    Serial.println(">>> WiFi Berhasil Terhubung! <<<");
    Serial.print("Alamat IP Alat: ");
    Serial.println(WiFi.localIP());
  }

  client.setServer(mqtt_server, 1883);
}

void loop() {
  // Hanya jalankan sistem jika WiFi tersambung
  if (WiFi.status() == WL_CONNECTED) {
    
    // Cek koneksi MQTT
    if (!client.connected()) { 
      Serial.println("Menyambungkan ke MQTT Broker...");
      // Memberi nama alat yang unik secara acak agar tidak bentrok dengan orang lain
      String clientId = "ESP32_Otomasi_";
      clientId += String(random(0xffff), HEX);
      
      if (client.connect(clientId.c_str())) {
        Serial.println("MQTT Sukses Terhubung!");
      } else {
        Serial.print("Gagal konek MQTT, error code: ");
        Serial.println(client.state());
        delay(2000);
        return; // Jangan lanjut baca sensor jika MQTT putus
      }
    }
    client.loop();

    // Baca suhu dan kelembaban
    delay(2000); 
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Cek apakah sensor berhasil terbaca
    if (isnan(h) || isnan(t)) {
      Serial.println("Gagal membaca dari sensor DHT! Cek kabelnya.");
      return;
    }

    // Menggabungkan data menjadi format JSON
    String payload = "{\"suhu\":" + String(t) + ",\"kelembaban\":" + String(h) + "}";
    
    // Kirim ke MQTT
    client.publish("otomasi/sensor_dht", payload.c_str());
    
    Serial.print("Data Terkirim: ");
    Serial.println(payload);
    
  } else {
    // Jika WiFi putus di tengah jalan, tampilkan pesan ini
    Serial.println("Peringatan: Koneksi WiFi Terputus! Mencoba nyambung ulang...");
    WiFi.disconnect();
    WiFi.reconnect();
    delay(3000);
  }
}