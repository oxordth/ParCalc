import os
from multiprocessing import Pool
import time
def search_text_in_file(file_path, search_text):
    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            content = file.read()
            count = content.count(search_text)
            return file_path, count
    except Exception as e:
        print(f"Ошибка при чтении файла {file_path}: {e}")
        return file_path, 0
def search_text_in_files_single_thread(search_text, directory):
    results = {}
    if not os.path.isdir(directory):
        raise ValueError("Указанный путь не является директорией.")

    # Проходимся по всем файлам в указанном каталоге
    for root, _, files in os.walk(directory):
        for file_name in files:
            file_path = os.path.join(root, file_name)
            try:
                with open(file_path, 'r', encoding='utf-8') as file:
                    content = file.read()
                    count = content.count(search_text)
                    if count > 0:
                        results[file_path] = count
            except Exception as e:
                print(f"Ошибка при чтении файла {file_path}: {e}")

    return results
def search_text_in_files(search_text, directory):
    results = {}
    if not os.path.isdir(directory):
        raise ValueError("Указанный путь не является директорией.")

    pool = Pool()  # Создаем пул процессов

    file_paths = []
    # Создаем список файлов для обработки
    for root, _, files in os.walk(directory):
        for file_name in files:
            file_paths.append(os.path.join(root, file_name))

    # Распараллеливаем поиск текста в файлах
    search_results = pool.starmap(search_text_in_file, [(file_path, search_text) for file_path in file_paths])

    pool.close()
    pool.join()

    # Собираем результаты
    for file_path, count in search_results:
        if count > 0:
            results[file_path] = count

    return results

if __name__ == "__main__":
    search_text = input("Введите текст для поиска: ")
    directory = "D:\\gen"

    try:
        start_time = time.time()
        search_results = search_text_in_files_single_thread(search_text, directory)
        elapsed_time = time.time() - start_time
        print(f"Однопоточный поиск: {elapsed_time:.2f} секунд ({elapsed_time * 1000:.2f} миллисекунд)")
        print("Результаты поиска в одном потоке:")
        for file_path, count in search_results.items():
            print(f"Файл: {file_path}, количество вхождений: {count}")
        start_time = time.time()
        search_results = search_text_in_files(search_text, directory)
        elapsed_time = time.time() - start_time
        print(f"Многопоточный поиск: {elapsed_time:.2f} секунд ({elapsed_time * 1000:.2f} миллисекунд)")
        print("Результаты поиска в многопотоке:")
        for file_path, count in search_results.items():
            print(f"Файл: {file_path}, количество вхождений: {count}")
    except ValueError as ve:
        print(ve)
