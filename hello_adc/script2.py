import serial
import time
import matplotlib.pyplot as plt

def record_data(port, baudrate, duration, filename):
    ser = serial.Serial(port, baudrate, timeout=1)
    
    with open(filename, 'wb') as f:
        start_time = time.time()
        print(f"Начало записи в {filename} на {duration} секунд...")
        
        data_list = []
        timestamps = []
        
        while time.time() - start_time < duration:
            data = ser.readline()
            if data:
                f.write(data)
                f.flush()
                decoded_data = data.decode('utf-8', errors='ignore').strip()
                print(decoded_data)
                
                try:
                    value = float(decoded_data)
                    data_list.append(value)
                    timestamps.append(time.time() - start_time)
                except ValueError:
                    pass
    
    ser.close()
    print("Запись завершена.")
    plot_data(timestamps, data_list)

def plot_data(timestamps, data_list):
    plt.figure(figsize=(10, 5))
    plt.plot(timestamps, data_list, linestyle='-')
    plt.xlabel('Время (секунды)')
    plt.ylabel('Значение')
    plt.title('График полученных данных')
    plt.grid(True, linestyle='--', linewidth=0.5)
    plt.show()

if __name__ == "__main__":
    port = input("Введите порт (например, COM3 или /dev/ttyUSB0): ")
    baudrate = int(input("Введите скорость (например, 9600): "))
    duration = int(input("Введите время записи (в секундах): "))
    filename = input("Введите имя файла для сохранения данных: ")
    
    record_data(port, baudrate, duration, filename)
