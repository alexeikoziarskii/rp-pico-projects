import serial
import time
import csv
import time
import matplotlib.pyplot as plt

filename = "D:\\rp-pico-projects\\hello_adc\\sample.csv"
ser = serial.Serial('COM3', 115200, timeout=1) 

def send_command(command):
    ser.write((command + '\n').encode('utf-8')) 
def read_response():
    response = ser.readline().decode('utf-8').strip() 
    return response

print("input sampling duration(s): ")
timer = int(input())
for i in range(timer, 0, -1):
    send_command("sample")
    time.sleep(1)

    response = read_response()
    if response:
        with open(filename, "a") as file:
            file.write(response + "\n")
        file.close()

numbers = []
with open(filename, mode="r", encoding="utf-8") as file:
    reader = csv.reader(file)
    for row in reader:
        try:
            numbers.append(float(row[0]))  # Преобразуем строку в число
        except ValueError:
            print(f"Ошибка в строке: {row}")
            
plt.figure(figsize=(8, 5))
plt.plot(numbers, marker="o", linestyle="-", color="b", label="Данные из CSV")
    
plt.xlabel("Номер измерения")
plt.ylabel("Значение")
plt.title("График данных из CSV")
plt.legend()
plt.grid(True)
    
plt.show()