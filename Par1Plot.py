import matplotlib.pyplot as plt

def plot_data(filename, label, start_row, end_row):
    with open(filename, 'r') as file:
        datas = file.readlines()[start_row:end_row]
        data = [float(date.strip()) for date in datas]
    return data

def main():
    fig, axs = plt.subplots(1, 2, figsize=(12, 6))

    # Plotting first 5 rows in the first subplot
    data1 = plot_data(r'C:\Users\mukhi\source\repos\Par2Lab\Par2Lab\no_parallel.txt', 'Однопоточный режим', 0, 5)
    axs[0].plot(data1, label='Однопоточный режим')
    data2 = plot_data(r'C:\Users\mukhi\source\repos\Par2Lab\Par2Lab\parallel.txt', 'Многопоточный режим', 0, 5)
    axs[0].plot(data2, label='Многопоточный режим')
    axs[0].set_title('Первые 5 строк')
    axs[0].set_xlabel('Номер эксперимента')
    axs[0].set_ylabel('Время (с)')
    axs[0].legend()

    # Plotting second 5 rows in the second subplot
    data3 = plot_data(r'C:\Users\mukhi\source\repos\Par2Lab\Par2Lab\no_parallel.txt', 'Однопоточный режим', 5, 10)
    axs[1].plot(data3, label='Однопоточный режим')
    data4 = plot_data(r'C:\Users\mukhi\source\repos\Par2Lab\Par2Lab\parallel.txt', 'Многопоточный режим', 5, 10)
    axs[1].plot(data4, label='Многопоточный режим')
    axs[1].set_title('Следующие 5 строк')
    axs[1].set_xlabel('Номер эксперимента')
    axs[1].set_ylabel('Время (с)')
    axs[1].legend()

    plt.suptitle('Зависимость количества вычислений от времени')
    plt.show()

if __name__ == "__main__":
    main()
