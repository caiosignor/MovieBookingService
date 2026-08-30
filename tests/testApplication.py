import os
import socket
import subprocess
import threading
import time

HOST = "127.0.0.1"
MOVIE = "Inception"
THEATER = "IMAX"
SEAT = "A1 A2 A3"
THREAD_COUNT = 10
START_SIGNAL = threading.Event()

def get_free_port():
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        sock.bind((HOST, 0))
        return sock.getsockname()[1]
    
PORT = get_free_port()

def client_session(client_id):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.settimeout(2.5)
    sock.bind(("127.0.0.1", 20000 + client_id))

    START_SIGNAL.wait()

    session_id = f"session-{client_id}"
    commands = [
        f"{session_id} SELECT_MOVIE {MOVIE}",
        f"{session_id} SELECT_THEATER {THEATER}",
        f"{session_id} BOOK {SEAT}",
    ]

    responses = []
    for command in commands:
        sock.sendto(command.encode("utf-8"), (HOST, PORT))
        try:
            data, _ = sock.recvfrom(4096)
            response = data.decode("utf-8", errors="ignore").strip()
            print(f"client id {client_id} received from server {response}")
        except socket.timeout:
            response = "TIMEOUT"
        responses.append(response)

    sock.close()
    return responses

def trigger_thread():
    time.sleep(0.5)
    START_SIGNAL.set()

def main():
    process = subprocess.Popen(
        [os.path.join("..","build","bin", "BookingServiceExecutable"), "--host", HOST, "--port", str(PORT)],
        cwd=os.getcwd(),
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    )

    try:
        time.sleep(0.5)

        results = []
        workers = []

        for i in range(THREAD_COUNT):
            thread = threading.Thread(target=lambda idx=i: results.append(client_session(idx)))
            workers.append(thread)
            thread.start()

        trigger = threading.Thread(target=trigger_thread)
        trigger.start()

        for thread in workers:
            thread.join(timeout=15)
        trigger.join(timeout=5)

        successful = 0
        failed = 0

        for responses in results:
            last_response = responses[-1] if responses else ""
            if last_response == "BOOKED":
                successful += 1
            else:
                failed += 1

        print(f"successful={successful}")
        print(f"failed={failed}")

        assert successful == 1, f"Expected only 1 successful booking, got {successful}"
        assert failed == THREAD_COUNT - 1, f"Expected {THREAD_COUNT - 1} failed bookings, got {failed}"

    finally:
        process.terminate()
        try:
            process.wait(timeout=5)
        except subprocess.TimeoutExpired:
            process.kill()
            process.wait(timeout=5)


if __name__ == "__main__":
    main()
