# 🚨 ESP32-C6 - Total Destruction Mode

![ESP32-C6](https://img.shields.io/badge/MCU-ESP32--C6-red)
![Platform](https://img.shields.io/badge/Platform-Windows-0078D4)
![Language](https://img.shields.io/badge/Language-Python%20%2B%20Arduino-blue)
![License](https://img.shields.io/badge/License-Educational%20Only-important)
![Status](https://img.shields.io/badge/Status-Demo%20Project-yellow)

## About

A **destructive security demonstration project** showcasing ESP32-C6 microcontroller capabilities for educational and authorized lab testing. This project demonstrates remote system manipulation through Wi-Fi connectivity using two operational modes: Station mode (existing network) and AP mode (hotspot creation).

**⚠️ CRITICAL WARNING: This code was NOT designed to run on your own computer!**

This project is designed to cause a target Windows computer to **completely lose all data** using the ESP32-C6 module. It should only be used in **controlled lab environments** and on **test machines**.

---

## ⚡ Danger Warning

This code performs the following operations:

1. **Deletes boot data** - `bcdedit /deleteall` - Computer cannot find Windows
2. **Deletes Registry keys** - `HKEY_LOCAL_MACHINE\SYSTEM` and `HKEY_LOCAL_MACHINE\SOFTWARE`
3. **Deletes system drivers** - All `.sys` files are removed
4. **Triggers system crash** - `svchost.exe` and other critical processes are terminated

**Result:** Target computer **cannot run without an operating system**, all data is lost.

---

## 📋 Project Structure

```
esp32_hack_demo/
├── README.md                          (This file)
├── ESP32Station/                      (Station Mode - Connected to Existing Wi-Fi)
│   ├── ESP32Station.ino              (Arduino code - Station mode)
│   ├── dinleyici_station.py          (PC listener running on desktop)
│   └── build/
└── ESP32Wifi/                         (Hotspot (AP) Mode)
    ├── ESP32Wifi.ino                 (Arduino code - AP mode)
    ├── dinleyici_wifi.py             (PC listener running on desktop)
    └── build/
```

---

## 🔧 Hardware Requirements

- **Microcontroller:** ESP32-C6 Dev Module
- **Built-in RGB LED:** GPIO 8 (Adafruit NeoPixel compatible)
- **Connection:** USB-C (for programming)
- **Power:** 3.3V, 500mA

### ESP32-C6 Specifications

| Feature | Value |
|---------|-------|
| Processor | Xtensa 32-bit @ 160 MHz |
| RAM | 512 KB |
| Flash | 4 MB |
| Wi-Fi | 802.11b/g/n (2.4 GHz) |
| Bluetooth | 5.0 |
| GPIO | 22 pins |
| USB | USB-OTG (Type-C) |
| Built-in LED | GPIO 8 (WS2812B) |

---

## 📦 Software Requirements

### Arduino IDE Setup

1. **Add ESP32 to Board Manager:**
   - Arduino IDE → Preferences
   - Add to "Additional Boards Manager URLs":
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Tools → Board Manager → Search "esp32" and install

2. **Board Selection:**
   - Tools → Board → "ESP32C6 Dev Module"

3. **Required Libraries:**
   - Tools → Manage Libraries
   - Search "Adafruit NeoPixel" and install

### Python Requirements

```bash
pip install requests pyinstaller
```

---

## 🚀 Usage

### Mode 1: Station Mode (Connected to Existing Wi-Fi)

This mode connects the ESP32 to the lab's existing Wi-Fi network.

#### ESP32 Side

1. Open [ESP32Station/ESP32Station.ino](ESP32Station/ESP32Station.ino)
2. Update Wi-Fi credentials:
   ```cpp
   const char* ssid     = "FiberHGW_ZTE4FE";
   const char* password = "Trabzon61";
   ```
3. Set static IP (based on router's subnet):
   ```cpp
   IPAddress local_IP(192, 168, 1, 253);
   IPAddress gateway(192, 168, 1, 1);
   IPAddress subnet(255, 255, 255, 0);
   ```
4. Upload and verify IP from Serial Monitor (9600 baud)

#### PC Side

1. Open [ESP32Station/dinleyici_station.py](ESP32Station/dinleyici_station.py)
2. Review the code (understand what it does for security)
3. Open PowerShell/CMD **AS ADMINISTRATOR**
4. Run:
   ```bash
   python dinleyici_station.py
   ```
5. Program waits for "DELETE" signal
6. Connect phone to same Wi-Fi and visit `192.168.1.253`
7. Press button and destruction occurs

**Create Executable:**
```bash
pyinstaller --onefile -w dinleyici_station.py
# .exe file created in dist/ folder
```

---

### Mode 2: AP (Hotspot) Mode

ESP32 creates its own Wi-Fi hotspot (`HACK_DEMO` / `12345678`).

#### ESP32 Side

1. Open [ESP32Wifi/ESP32Wifi.ino](ESP32Wifi/ESP32Wifi.ino)
2. Upload
3. Observe LED status

#### PC Side

1. Connect PC's Wi-Fi to `HACK_DEMO` network (Password: `12345678`)
2. Open [ESP32Wifi/dinleyici_wifi.py](ESP32Wifi/dinleyici_wifi.py)
3. Set target folder path:
   ```python
   KLASOR_YOLU = r"C:\target_folder"
   ```
4. Run **AS ADMINISTRATOR**:
   ```bash
   python dinleyici_wifi.py
   ```
5. Visit `192.168.4.1/tetikle` in web browser → folder gets deleted

---

## 🎨 LED Status Indicators

The RGB LED on ESP32 shows status:

| Color | Meaning |
|-------|---------|
| 🟦 Blue | Startup, ready |
| 🟩 Green | Wi-Fi connected |
| 🟨 Yellow | Searching for computer connection |
| 🔴 Red | "DELETE" command received, destruction started |

---

## 🔌 Network Architecture

### Station Mode
```
┌─────────────────────────────────────────┐
│           Lab Wi-Fi Router              │
│         (192.168.1.1)                   │
└──────────┬──────────────────┬───────────┘
           │                  │
    ESP32 (253)         PC (DHCP)
    [Green LED]         [Listener Running]
           │                  │
           └──────Wifi────────┘
           Control Signals
```

### AP Mode
```
┌──────────────────────────────┐
│  ESP32 Hotspot (AP Mode)     │
│  SSID: HACK_DEMO             │
│  IP: 192.168.4.1             │
└──────────┬───────────────────┘
           │
           ├─ PC (192.168.4.2)
           │  [Listener Running]
           │
           └─ Phone/Tablet
              [Web Interface]
```

---

## 💻 API Endpoints

### Station Mode

| Endpoint | Method | Returns | Description |
|----------|--------|---------|-------------|
| `/durum` | GET | `BEKLE` / `SIL` | Poll computer status |

### AP Mode

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/` | GET | Home page (HTML web interface) |
| `/durum` | GET | Return current status |
| `/tetikle` | GET | Send "DELETE" command, start destruction |

---

## 📧 HTTP Request Examples

### Poll Status
```bash
curl http://192.168.1.253/durum
> BEKLE
```

### Trigger Destruction (AP Mode)
```bash
curl http://192.168.4.1/tetikle
> DELETE COMMAND RECEIVED
```

### Python Requests
```python
import requests

# Station mode
response = requests.get("http://192.168.1.253/durum")
print(response.text)  # BEKLE or SIL

# AP mode trigger
requests.get("http://192.168.4.1/tetikle")
```

---

## 🔍 Troubleshooting

### "Cannot connect to Wi-Fi"
- **Solution:** Verify SSID and password are correct
- Try even if network doesn't use WPA2
- Check IP from Serial Monitor

### "Python cannot reach ESP32"
- Are computer and ESP32 on same network?
- Disable firewall and antivirus
- Is IP address correct? `ping 192.168.1.253`

### "Running as Administrator"
- Python scripts MUST run as **administrator**
- Otherwise you'll get `WinError 5`

### "LED not lighting up"
- Is GPIO 8 pin correct?
- Is Adafruit NeoPixel library installed?
- Test LED manually:
  ```cpp
  void setup() {
    led.begin();
    ledRenk(255, 0, 0);  // RED
  }
  ```

---

## 🛡️ Security Notes

1. **NEVER run this on your own computer under any circumstances!**
2. **Only deploy on test/lab machines**
3. **Keep network isolated** - Disconnect from internet
4. **Add firewall rules** - Prevent unauthorized access
5. **Restrict physical access** - Nobody can access ESP32
6. **Backup all data** - Before any testing

---

## 📚 References

- **ESP32-C6 Datasheet:** https://www.espressif.com/sites/default/files/documentation/esp32-c6_datasheet_en.pdf
- **Arduino-ESP32:** https://github.com/espressif/arduino-esp32
- **Adafruit NeoPixel:** https://www.adafruit.com/product/5672
- **WebServer Library:** Built-in with Arduino ESP32

---

## 📝 License

This project is designed for **educational and research purposes** only. Any misuse is solely the responsibility of the user.

---

## 👨‍💻 Development

Code Components:

### Arduino Libraries
- `WiFi.h` - Wi-Fi connectivity
- `WebServer.h` - HTTP server
- `Adafruit_NeoPixel.h` - RGB LED control

### Python Modules
- `requests` - HTTP requests
- `os` - System commands
- `shutil` - File operations
- `ctypes` - Administrator control

---

## ⚠️ Final Warning

**THIS CODE MAY ONLY BE USED IN AUTHORIZED TEST ENVIRONMENTS!**

No responsibility is accepted for any damage caused. Use at your own risk.

```
╔════════════════════════════════════════════════════════╗
║  WARNING: Backup All Data Before Running This Code!    ║
╚════════════════════════════════════════════════════════╝
```

---

**Date:** April 2026 | **Version:** 1.0 | **Platform:** ESP32-C6 / Windows
