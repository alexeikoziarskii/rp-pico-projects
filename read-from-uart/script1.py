import serial
import threading
import time
import sys

# Настроим последовательный порт для общения с Raspberry Pi Pico
ser = serial.Serial('COM4', 115200, timeout=1)  # Для Windows, для Linux замените на /dev/ttyUSB0

# Функция для чтения данных от устройства
def read_from_port():
    while True:
        # Читаем данные с порта
        response = ser.readline().decode('ascii').strip()
        if response:
            print(response)

# Функция для отправки команды на Raspberry Pi Pico
def write_to_port():
    while True:
        command = input("")  # Ожидаем ввода от пользователя
        ser.write((command + '\n').encode('ascii'))  # Отправляем команду на устройство
        time.sleep(0.1)  # Задержка для синхронизации

# Запуск потока для чтения данных
read_thread = threading.Thread(target=read_from_port, daemon=True)
read_thread.start()

# Запуск потока для отправки команд
write_thread = threading.Thread(target=write_to_port, daemon=True)
write_thread.start()

# Бесконечный цикл для того, чтобы основной поток продолжал работать
try:
    while True:
        time.sleep(1)  # Этот цикл не завершится, пока программа работает
except KeyboardInterrupt:
    print("\nВыход из программы.")
    ser.close()  # Закрываем порт при завершении
    sys.exit()
