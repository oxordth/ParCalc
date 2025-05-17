import os
import random
import string

def generate_random_text(length):
    # Генерируем случайную строку заданной длины из букв латинского алфавита
    return ''.join(random.choices(string.ascii_lowercase, k=length))

def generate_files(directory, num_files):
    if not os.path.isdir(directory):
        raise ValueError("Указанный путь не является директорией.")

    for i in range(num_files):
        file_name = f"file_{i+1}.txt"
        file_path = os.path.join(directory, file_name)
        text_length = random.randint(100000, 10000000) # Генерируем случайную длину текста для каждого файла
        text = generate_random_text(text_length)
        try:
            with open(file_path, 'w', encoding='utf-8') as file:
                file.write(text)
        except Exception as e:
            print(f"Ошибка при создании файла {file_path}: {e}")

if __name__ == "__main__":
    directory = "D:\\gen"
    num_files = 3200

    try:
        os.makedirs(directory, exist_ok=True) # Создаем директорию, если ее нет
        generate_files(directory, num_files)
        print(f"Создано {num_files} файлов в директории {directory}")
    except ValueError as ve:
        print(ve)
