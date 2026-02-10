#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include "driver/i2s.h"

// ------------------ CONFIG ------------------
#define I2S_WS 25   // L/R clock
#define I2S_SD 33   // Serial data
#define I2S_SCK 32  // Bit clock
#define I2S_PORT I2S_NUM_0

// SD Card Pinouts
#define SD_CS 5  // SD card CS
#define SCK 18
#define MISO 19
#define MOSI 23

#define BUTTON_PIN 14  // Push button

const char *ssid = "TECNO POP 7 Pro";
const char *password = "beyond,56";
const char *backendUrl = "https://getsignedurl-387799109589.europe-west1.run.app";  // Your GCF endpoint

// ------------------ COURSE INFO ------------------
const char *COURSE_CODE = "EEE313";
const int COURSE_LEVEL = 300;  // 100, 200, 300, ...

const char *DEVICE_ID = "esp32a1";

// Audio config
#define SAMPLE_RATE 16000
#define BITS_PER_SAMPLE 16
#define I2S_READ_LEN 1024
#define WAV_HEADER_SIZE 44
#define CHANNELS 1

// Globals
File recordingFile;
String currentFilename = "";
bool isRecording = false;
unsigned long silenceStart = 0;
bool sdReady = false;

// ------------------ I2S SETUP ------------------
void setupI2S() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 1024
  };
  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1,
    .data_in_num = I2S_SD
  };
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
}

// ------------------ WAV HEADER ------------------
void writeWavHeader(File file, int sampleRate, int bitsPerSample, int totalSamples) {
  byte header[44];
  int byteRate = sampleRate * bitsPerSample / 8;
  int dataSize = totalSamples * bitsPerSample / 8;

  memcpy(header, "RIFF", 4);
  *((int *)&header[4]) = dataSize + 36;
  memcpy(&header[8], "WAVEfmt ", 8);
  *((int *)&header[16]) = 16;
  *((short *)&header[20]) = 1;
  *((short *)&header[22]) = 1;
  *((int *)&header[24]) = sampleRate;
  *((int *)&header[28]) = byteRate;
  *((short *)&header[32]) = bitsPerSample / 8;
  *((short *)&header[34]) = bitsPerSample;
  memcpy(&header[36], "data", 4);
  *((int *)&header[40]) = dataSize;
  file.write(header, 44);
}

void finalizeWav(File file) {
  file.seek(4);
  uint32_t fileSize = file.size() - 8;
  file.write((byte *)&fileSize, 4);
  file.seek(40);
  uint32_t dataSize = file.size() - 44;
  file.write((byte *)&dataSize, 4);
}

// ------------------ FILENAME ------------------
String nextFilename() {
  int index = 0;
  String filename;
  do {
    filename = "/LEC" + String(index) + ".wav";
    index++;
  } while (SD.exists(filename));
  return filename;
}

// ------------------ UPLOAD ------------------

// helper: URL-encode a filename (simple)
String urlEncode(const String &str) {
  String encoded = "";
  char c;
  char buf[4];
  for (size_t i = 0; i < str.length(); ++i) {
    c = str[i];
    if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
      encoded += c;
    } else if (c == ' ') {
      encoded += '+';
    } else {
      sprintf(buf, "%%%02X", (uint8_t)c);
      encoded += buf;
    }
  }
  return encoded;
}

// ---------- Persist basic upload state on SD (simple JSON) ----------
bool saveUploadStateToSD(const String &sessionUrl, const String &recordingId, const String &objectPath) {
  StaticJsonDocument<512> doc;
  doc["sessionUrl"] = sessionUrl;
  doc["recordingId"] = recordingId;
  doc["objectPath"] = objectPath;

  File f = SD.open("/upload_state.json", FILE_WRITE);
  if (!f) return false;

  serializeJson(doc, f);
  f.close();
  return true;
}

bool loadUploadStateFromSD(String &sessionUrl, String &recordingId, String &objectPath) {
  if (!SD.exists("/upload_state.json")) return false;

  File f = SD.open("/upload_state.json", FILE_READ);
  if (!f) return false;

  StaticJsonDocument<512> doc;
  DeserializationError err = deserializeJson(doc, f);
  f.close();
  if (err) return false;

  sessionUrl = String(doc["sessionUrl"] | "");
  recordingId = String(doc["recordingId"] | "");
  objectPath = String(doc["objectPath"] | "");

  return sessionUrl.length() > 0;
}

void clearUploadStateOnSD() {
  if (SD.exists("/upload_state.json")) SD.remove("/upload_state.json");
}

// ---------- Get signed URL + start resumable session ----------
String getsignedUrlResumable(const String &filename, String &outRecordingId, String &outObjectPath) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return "";
  }

  // Build request URL; prefer POST with JSON body
  String reqUrl = String(backendUrl) + "/getsignedUrl";  // your Cloud Run route

  WiFiClientSecure clientHTTPS;
  clientHTTPS.setInsecure();
  HTTPClient http;

  http.begin(clientHTTPS, reqUrl);
  http.addHeader("Content-Type", "application/json");

  // request JSON {"filename":"...","resumable":true}
  StaticJsonDocument<256> doc;
  doc["filename"] = filename;
  doc["deviceId"] = DEVICE_ID;
  doc["resumable"] = true;
  doc["course_code"] = COURSE_CODE;
  doc["level"] = COURSE_LEVEL;


  String body;
  serializeJson(doc, body);

  // Debug print to confirm that course info is being sent
  Serial.println("📤 Upload metadata:");
  Serial.printf("  Course: %s\n", COURSE_CODE);
  Serial.printf("  Level: %d\n", COURSE_LEVEL);


  int code = http.POST(body);
  if (code != HTTP_CODE_OK && code != HTTP_CODE_CREATED) {
    Serial.printf("getsignedUrlResumable POST failed: %d\n", code);
    Serial.println("Response:");
    Serial.println(http.getString());
    http.end();
    return "";
  }

  String payload = http.getString();
  http.end();

  Serial.println("--- Backend JSON Response ---");
  Serial.println(payload);
  Serial.println("-----------------------------");

  // Parse JSON
  StaticJsonDocument<2048> resp;
  DeserializationError err = deserializeJson(resp, payload);
  if (err) {
    Serial.print("JSON parse failed: ");
    Serial.println(err.c_str());
    Serial.println("Payload:");
    Serial.println(payload);
    return "";
  }

  // Extract the ACTUAL resumable session URL
  String sessionUrl = "";
  if (resp.containsKey("resumableUrl")) sessionUrl = resp["resumableUrl"].as<String>();
  else if (resp.containsKey("uploadUrl")) sessionUrl = resp["uploadUrl"].as<String>();

  if (sessionUrl.length() == 0) {
    Serial.println("ERROR: Backend did not return a resumable session URL.");
    return "";
  }

  // Extract metadata from backend
  outRecordingId = resp["recordingId"] | "";
  outObjectPath = resp["objectPath"] | "";

  Serial.println("Resumable Session URL (ready to upload):");
  Serial.println(sessionUrl);
  Serial.println("Recording ID: " + outRecordingId);
  Serial.println("Object Path: " + outObjectPath);

  // Save state for resuming later
  saveUploadStateToSD(sessionUrl, outRecordingId, outObjectPath);

  return sessionUrl;  // RETURN THE READY-TO-USE resumable URL
}

// ---------- splitUrl() ----------
void splitUrlPreserveQuery(const String &url, String &host, String &path) {
  int idx1 = url.indexOf("://");
  int idx2 = url.indexOf('/', idx1 + 3);

  host = url.substring(idx1 + 3, idx2);

  // MUST keep everything after '/'
  path = url.substring(idx2);
}

#define GCS_CHUNK_SIZE 262144  // 256 KB (MANDATORY for GCS)
#define STREAM_BUF_SIZE 4096   // ESP32-safe


bool uploadFileInChunks(const String &sessionUrl, const String &sdFilePath) {
  File f = SD.open(sdFilePath, FILE_READ);
  if (!f) {
    Serial.println("❌ Failed to open SD file");
    return false;
  }

  uint64_t totalSize = f.size();
  uint64_t offset = 0;

  Serial.printf("📦 File size: %llu bytes\n", totalSize);
  Serial.printf("🧠 Free heap (start): %u\n", ESP.getFreeHeap());

  // Use heap buffer to avoid stack overflow
  uint8_t *buffer = (uint8_t *)malloc(STREAM_BUF_SIZE);
  if (!buffer) {
    Serial.println("❌ Failed to allocate heap buffer");
    f.close();
    return false;
  }

  String host, path;
  splitUrlPreserveQuery(sessionUrl, host, path);
  if (host.length() == 0 || path.length() == 0) {
    Serial.println("❌ Invalid resumable URL (host/path empty)");
    free(buffer);
    f.close();
    return false;
  }

  while (offset < totalSize) {
    uint64_t remaining = totalSize - offset;
    bool isFinalChunk = remaining <= GCS_CHUNK_SIZE;
    uint64_t chunkSize = isFinalChunk ? remaining : GCS_CHUNK_SIZE;

    Serial.printf("⬆ Uploading chunk: offset=%llu size=%llu\n", offset, chunkSize);

    WiFiClientSecure client;
    client.setInsecure();

    if (!client.connect(host.c_str(), 443)) {
      Serial.println("❌ TLS connect failed");
      free(buffer);
      f.close();
      return false;
    }

    // ---- HTTP headers ----
    client.printf("PUT %s HTTP/1.1\r\n", path.c_str());
    client.printf("Host: %s\r\n", host.c_str());
    client.print("Content-Type: application/octet-stream\r\n");
    client.printf("Content-Length: %llu\r\n", chunkSize);
    client.printf("Content-Range: bytes %llu-%llu/%llu\r\n", offset, offset + chunkSize - 1, totalSize);
    client.print("Connection: close\r\n\r\n");

    // ---- Stream body in small buffer blocks ----
    uint64_t sent = 0;
    while (sent < chunkSize) {
      size_t toRead = min((uint64_t)STREAM_BUF_SIZE, chunkSize - sent);
      size_t r = f.read(buffer, toRead);
      if (r == 0) {
        Serial.println("❌ SD read error");
        client.stop();
        free(buffer);
        f.close();
        return false;
      }
      client.write(buffer, r);
      sent += r;
      yield();  // let ESP32 background tasks run
    }

    // ---- Wait for response ----
    unsigned long start = millis();
    while (!client.available()) {
      if (millis() - start > 15000) {
        Serial.println("❌ Server timeout");
        client.stop();
        free(buffer);
        f.close();
        return false;
      }
      delay(1);
    }

    String statusLine = client.readStringUntil('\n');
    statusLine.trim();
    Serial.println("⬅ " + statusLine);

    if (!statusLine.startsWith("HTTP/")) {
      Serial.println("❌ Invalid HTTP response");
      client.stop();
      free(buffer);
      f.close();
      return false;
    }

    int code = statusLine.substring(9, 12).toInt();
    String rangeHeader = "";

    while (client.connected() || client.available()) {
      if (!client.available()) {
        delay(1);
        continue;
      }
      String line = client.readStringUntil('\n');
      line.trim();
      if (line.length() == 0) break;
      if (line.startsWith("Range:")) rangeHeader = line;
    }

    client.stop();

    if (code == 200 || code == 201) {
      Serial.println("✅ Upload complete");
      break;
    } else if (code == 308) {
      if (rangeHeader.length()) {
        uint64_t last = atoll(rangeHeader.substring(rangeHeader.indexOf('-') + 1).c_str());
        offset = last + 1;
        f.seek(offset);
      } else {
        offset += chunkSize;
        f.seek(offset);
      }
    } else {
      Serial.printf("❌ Unexpected HTTP %d\n", code);
      free(buffer);
      f.close();
      return false;
    }
  }

  free(buffer);
  f.close();
  Serial.printf("🧠 Free heap (end): %u\n", ESP.getFreeHeap());
  return true;
}


// ---------------- SD Check ----------------
bool checkSD() {
  if (!SD.begin(SD_CS)) {
    if (sdReady) {
      Serial.println("SD card removed!");
      sdReady = false;
      if (isRecording) stopRecording();
    }
    return false;
  }
  if (!sdReady) {
    Serial.println("SD card ready.");
    sdReady = true;
  }
  return true;
}

// Print in smaller chunks
void printLongString(const String &str) {
  const int chunkSize = 128;
  for (size_t i = 0; i < str.length(); i += chunkSize) {
    Serial.println(str.substring(i, i + chunkSize));
    delay(1);  // give watchdog time
  }
}

// ---------------- Start/Stop ----------------
void startRecording() {
  if (!checkSD()) return;
  currentFilename = nextFilename();
  recordingFile = SD.open(currentFilename, FILE_WRITE);
  if (!recordingFile) {
    Serial.println("File open failed!");
    return; 
  }
  writeWavHeader(recordingFile, SAMPLE_RATE, BITS_PER_SAMPLE, CHANNELS);
  isRecording = true;
  silenceStart = millis();
  Serial.print("Recording started: ");
  Serial.println(currentFilename);
}

// ---------- stopRecording() updated flow ----------
void stopRecording() {
  if (!isRecording) return;

  finalizeWav(recordingFile);
  recordingFile.close();
  isRecording = false;
  Serial.println("Recording stopped & file saved: " + currentFilename);

  // If WiFi not available, skip upload for now
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected; upload deferred.");
    return;
  }

  // Variables to be filled by the backend
  String recordingId, objectPath, sessionUrl;

  // 🔥 Get the short stable upload session URL
  sessionUrl = getsignedUrlResumable(
    currentFilename.substring(1),  // file name only, without '/'
    recordingId,
    objectPath);

  if (sessionUrl.length() == 0) {
    Serial.println("Could not get session URL; upload deferred.");
    return;
  }

  Serial.println("Uploading using sessionUrl (short, stable):");

  // --- Persist session to SD/NVS for future resume ----
  saveUploadStateToSD(sessionUrl, recordingId, objectPath);
  Serial.println("Session URL + recordingId saved to SD");

  delay(50);  // allow system tasks to run

  // 🔥 Upload using sessionUrl, not signedUrl
  bool ok = uploadFileInChunks(sessionUrl, currentFilename);

  if (ok) {
    Serial.println("Upload successful; clearing saved state");
    clearUploadStateOnSD();
  } else {
    Serial.println("Upload failed; state saved for resume.");
  }
}


// ------------------ SETUP ------------------
void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  // if (!SD.begin(SD_CS)) {
  //   Serial.println("SD Card mount failed!");
  //   while (true)
  //     ;
  // }

  SPI.begin(SCK, MISO, MOSI, SD_CS);  // 🔑 critical for ESP32

  if (!checkSD()) {
    Serial.println("SD not ready at boot (will retry)");
  }


  setupI2S();
}

// ---------------- Loop ----------------
void loop() {
  static bool lastButtonState = HIGH;
  bool buttonState = digitalRead(BUTTON_PIN);

  if (lastButtonState == HIGH && buttonState == LOW) {
    if (!isRecording) startRecording();
    else stopRecording();
    delay(200);
  }
  lastButtonState = buttonState;

  if (isRecording && sdReady) {
    int32_t rawBuffer[512];
    size_t bytesRead;
    i2s_read(I2S_PORT, (char *)rawBuffer, sizeof(rawBuffer), &bytesRead, portMAX_DELAY);

    int samplesRead = bytesRead / 4;
    int16_t pcmBuffer[512];
    double sumSq = 0;
    for (int i = 0; i < samplesRead; i++) {
      int16_t sample = (int16_t)(rawBuffer[i] >> 13);
      pcmBuffer[i] = sample;
      sumSq += (double)sample * sample;
    }

    // float rms = sqrt(sumSq / samplesRead);
    // if (rms > 0) { float newGain = targetLevel / rms; gain = gain * (1.0 - agcSpeed) + newGain * agcSpeed; }

    // for (int i = 0; i < samplesRead; i++) {
    //     int32_t boosted = (int32_t)(pcmBuffer[i] * gain);
    //     if (boosted > 32767) boosted = 32767;
    //     if (boosted < -32768) boosted = -32768;
    //     pcmBuffer[i] = (int16_t)boosted;
    // }

    if (recordingFile) recordingFile.write((uint8_t *)pcmBuffer, samplesRead * 2);

    // if (isSilent(pcmBuffer, samplesRead)) {
    //     if (millis() - silenceStart > silenceLimit) {
    //         Serial.println("Auto-stopped (long silence).");
    //         stopRecording();
    //     }
    // } else silenceStart = millis();
  }

  checkSD();
}
