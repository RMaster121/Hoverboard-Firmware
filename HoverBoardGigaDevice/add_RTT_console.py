Import("env")
import sys
import socket
import threading

# --- Funkcja klienta RTT ---
def run_rtt_client(*args, **kwargs):
    # Pobieramy argumenty przekazane w definicji zadania
    target_name = kwargs.get("name", "Unknown")
    target_ip = "hoverboard"  # <--- TWOJE IP ORANGE PI
    
    # Rozróżniamy porty na podstawie nazwy zadania
    if "Master" in target_name:
        port = 9090
    else:
        port = 9091

    print(f"\n--- Łączenie z RTT {target_name} ({target_ip}:{port}) ---\n")

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(5)
    
    try:
        s.connect((target_ip, port))
        print(f"✅ POŁĄCZONO! (Ctrl+C aby wyjść)\n")
        s.settimeout(None) # Wyłączamy timeout dla transmisji
    except Exception as e:
        print(f"❌ BŁĄD POŁĄCZENIA: {e}")
        print("   (Sprawdź czy OpenOCD na Orange Pi działa i czy RTT jest włączone)")
        return

    # Wątek do odbierania danych (Orange Pi -> PC)
    def recv_thread():
        while True:
            try:
                data = s.recv(1024)
                if not data: break
                # Wypisz bez buforowania
                sys.stdout.write(data.decode(errors="ignore"))
                sys.stdout.flush()
            except:
                break
        print("\n\n--- Rozłączono ---")
        s.close()

    t = threading.Thread(target=recv_thread, daemon=True)
    t.start()

    # Pętla główna - wysyłanie danych (Klawiatura PC -> Orange Pi)
    try:
        # Na Windowsie sys.stdin w tym trybie może być problematyczny, 
        # więc to prosta pętla blokująca, żeby okno się nie zamknęło.
        while t.is_alive():
            t.join(1)
    except KeyboardInterrupt:
        print("\nZamykanie...")
        s.close()

# --- Rejestracja zadań w PlatformIO ---

env.AddCustomTarget(
    name="Monitor_Master",
    dependencies=None,
    actions=run_rtt_client,
    title="Monitor RTT (Master)",
    description="Podgląd logów z Mastera (Port 9090)"
)

env.AddCustomTarget(
    name="Monitor_Slave",
    dependencies=None,
    actions=run_rtt_client,
    title="Monitor RTT (Slave)",
    description="Podgląd logów ze Slave'a (Port 9091)"
)