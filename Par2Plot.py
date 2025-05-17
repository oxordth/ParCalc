import matplotlib.pyplot as plt

def plot_data(filename,  label):
    with open(filename, 'r') as file:
        datas = file.readlines()
        Y = []
        for date in datas:
            Y.append(float(date))
    plt.plot(Y, label=label)

def main():
    plot_data(r'C:\Users\mukhi\source\repos\Par2Lab\Par2Lab\no_parallel.txt', '12 потоков')
    plot_data(r'C:\Users\mukhi\source\repos\Par2Lab\Par2Lab\parallel.txt',  '6 потоков')
    
    plt.xlabel('Эксперимент')
    plt.ylabel('Время (с)')
    plt.title('Время выполнения программы')
    plt.legend()
    plt.show()

if __name__ == "__main__":
    main()