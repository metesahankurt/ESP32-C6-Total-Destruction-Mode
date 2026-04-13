"""
ESP32 Hack Demo - Python Dinleyici
===================================
Sunum için: ESP32-C6'nın yayınladığı web sayfasını kontrol eder.
"SIL" komutu geldiğinde kurban_klasor'u kalıcı olarak siler.

Kullanım:
    pip install requests
    python python_dinleyici.py

NOT: Yalnızca kontrollü sunum ortamında kullanılacak şekilde tasarlanmıştır.
"""

import requests
import os
import shutil
import time

# ============================================================
# AYARLAR — Sunumdan önce düzenlenmesi gereken kısım
# ============================================================

# ESP32'nin IP adresini buraya yaz (ESP32 seri monitörden öğrenebilirsin)
ESP32_IP = "192.168.4.1"
ESP32_URL = f"http://{ESP32_IP}/durum"

# Silinecek klasörün tam yolu
KLASOR_YOLU = r"C:\kurban_klasor"

KONTROL_ARALIGI = 1  # Kaç saniyede bir kontrol edilsin
# ============================================================


def klasoru_sil(yol):
    """Klasörü ve içindekileri kalıcı olarak siler."""
    shutil.rmtree(yol)


def main():
    print("=" * 50)
    print("  ESP32 HACK DEMO — SİNYAL DİNLEYİCİ")
    print("=" * 50)
    print(f"ESP32 adresi : {ESP32_URL}")
    print(f"Hedef klasör : {KLASOR_YOLU}")
    print()

    if not os.path.exists(KLASOR_YOLU):
        print("[HATA] Hedef klasör bulunamadı! Yolu kontrol et.")
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
                print("[✓] HEDEF YOK EDİLDİ! Klasör artık mevcut değil.")
                print("    Sunum tamamlandı.")
                break

            else:
                print(f"[...] Bekleniyor... (ESP32 durumu: '{durum}')", end="\r")

        except requests.exceptions.ConnectionError:
            print("[!] ESP32'ye bağlanılamıyor, tekrar deneniyor...", end="\r")
        except requests.exceptions.Timeout:
            print("[!] Bağlantı zaman aşımı, tekrar deneniyor...", end="\r")
        except Exception as e:
            print(f"[!] Beklenmeyen hata: {e}", end="\r")

        time.sleep(KONTROL_ARALIGI)


if __name__ == "__main__":
    main()
