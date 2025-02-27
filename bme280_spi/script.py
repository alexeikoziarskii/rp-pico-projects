import serial
import time
import matplotlib.pyplot as plt

def record_data(port, baudrate, duration, humidity_file, pressure_file, temperature_file):
    ser = serial.Serial(port, baudrate, timeout=1)
    
    with open(humidity_file, 'w') as h_file, \
         open(pressure_file, 'w') as p_file, \
         open(temperature_file, 'w') as t_file:
        
        start_time = time.time()
        first_timestamp = None
        print(f"Начало записи на {duration} секунд...")
        
        humidity_data, pressure_data, temperature_data = [], [], []
        timestamps = []
        
        while time.time() - start_time < duration:
            data = ser.readline()
            if data:
                decoded_data = data.decode('utf-8', errors='ignore').strip()
                print(f"{decoded_data}")

                current_time = time.time()
                humidity, pressure, temperature = map(float, decoded_data.split(' '))
                
                if first_timestamp is None:
                    first_timestamp = current_time
                        
                elapsed_time = current_time - first_timestamp
                
                h_file.write(f"{elapsed_time} {humidity}\n")
                h_file.flush()
                p_file.write(f"{elapsed_time} {pressure}\n")
                p_file.flush()
                t_file.write(f"{elapsed_time} {temperature}\n")
                t_file.flush()
                        
                humidity_data.append(humidity)
                pressure_data.append(pressure)
                temperature_data.append(temperature)
                timestamps.append(elapsed_time)
    
    ser.close()
    print("Запись завершена.")
    
    plot_data(timestamps, humidity_data, "Влажность", "blue")
    plot_data(timestamps, pressure_data, "Давление", "red")
    plot_data(timestamps, temperature_data, "Температура", "green")

def plot_data(timestamps, data_list, label, color):
    plt.figure(figsize=(10, 5))
    plt.plot(timestamps, data_list, linestyle='-', color=color)
    plt.xlabel('Время (секунды)')
    plt.ylabel(label)
    plt.title(f'График {label}')
    plt.grid(True, linestyle='--', linewidth=0.5)
    plt.savefig(f"D:\\rp-pico-projects\\bme280_spi\\{label}.png")
    #plt.show()

if __name__ == "__main__":
    port = "COM3"  # Укажите ваш порт
    baudrate = 115200  # Укажите скорость
    duration = int(input("Введите время записи (в секундах): "))
    
    file_humidity = "D:\\rp-pico-projects\\bme280_spi\\humidity.txt"
    file_pressure = "D:\\rp-pico-projects\\bme280_spi\\pressure.txt"
    file_temperature = "D:\\rp-pico-projects\\bme280_spi\\temperature.txt"
    record_data(port, baudrate, duration, file_humidity, file_pressure, file_temperature)
