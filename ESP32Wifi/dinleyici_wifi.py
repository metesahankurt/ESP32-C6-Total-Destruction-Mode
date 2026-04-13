"""
ESP32 Hack Demo - AP (Hotspot) Modu Dinleyici
===============================================
ESP32 kendi Wi-Fi'sini açar (HACK_DEMO).
PC bu ağa bağlanır, Python ESP32'ye 192.168.4.1 üzerinden ulaşır.

Kullanım:
    1. PC'nin Wi-Fi'sini HACK_DEMO ağına bağla
    2. pip install requests
    3. python dinleyici_wifi.py
    -- veya --
    pyinstaller --onefile dinleyici_wifi.py
"""

import requests
import os
import shutil
import time

# ============================================================
# AYARLAR
# ============================================================
ESP32_IP  = "192.168.4.1"            # AP modunda ESP32 her zaman bu IP'de
ESP32_URL = f"http://{ESP32_IP}/durum"

# Silinecek klasörün yolu — lab bilgisayarına göre düzenle
KLASOR_YOLU = r"C:\kurban_klasor"

KONTROL_ARALIGI = 1
# ============================================================


def klasoru_sil(yol):
    shutil.rmtree(yol)


def main():
    print("=" * 50)
    print("  ESP32 HACK DEMO — AP MOD DİNLEYİCİ")
    print("=" * 50)
    print(f"ESP32 adresi : {ESP32_URL}")
    print(f"Hedef klasör : {KLASOR_YOLU}")
    print()

    if not os.path.exists(KLASOR_YOLU):
        print("[HATA] Hedef klasör bulunamadı! Yolu kontrol et.")
        input("\nÇıkmak için Enter'a bas...")
        return

    print("[HAZIR] Sinyal bekleniyor... Klasör şu an güvende.\n")

    while True:
        try:
            response = requests.get(ESP32_URL, timeout=2)
            durum = response.text.strip()

            if durum == "SIL":
                print("[!] SİL SİNYALİ ALINDI!")
                print("[*] Klasör siliniyor...")
                klasoru_sil(KLASOR_YOLU)
                print("[✓] HEDEF YOK EDİLDİ!")
                print("    Sunum tamamlandı.")
                input("\nÇıkmak için Enter'a bas...")
                break
            else:
                print(f"[...] Bekleniyor... (ESP32: '{durum}')", end="\r")

        except requests.exceptions.ConnectionError:
            print("[!] ESP32'ye bağlanılamıyor — HACK_DEMO ağında mısın?", end="\r")
        except requests.exceptions.Timeout:
            print("[!] Zaman aşımı, tekrar deneniyor...                  ", end="\r")
        except Exception as e:
            print(f"[!] Hata: {e}                                        ", end="\r")

        time.sleep(KONTROL_ARALIGI)


if __name__ == "__main__":
    main()
