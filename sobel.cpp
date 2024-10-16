#include <opencv2/opencv.hpp>
#include <iostream>
#include <pthread.h>
#include <cmath>
#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace cv;

// Собель X и Y ядра
const int kernelX[3][3] = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}
};

const int kernelY[3][3] = {
    {1, 2, 1},
    {0, 0, 0},
    {-1, -2, -1}
};

// Исходное изображение и результат
Mat image;
Mat result;

// Структура данных для передачи параметров в потоки
struct ThreadData {
    int startRow;
    int endRow;
};

// Функция для применения фильтра Собела к части изображения
void* applySobelFilter(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int gx, gy;

    for (int i = data->startRow; i < data->endRow; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            gx = gy = 0;
            // Apply Sobel kernels, with boundary checks
            for (int k = -1; k <= 1; ++k) {
                for (int l = -1; l <= 1; ++l) {
                    int row = i + k;
                    int col = j + l;
                    // Check boundaries
                    if (row >= 0 && row < image.rows && col >= 0 && col < image.cols) {
                        int pixel = image.at<uchar>(row, col);
                        gx += pixel * kernelX[k + 1][l + 1];
                        gy += pixel * kernelY[k + 1][l + 1];
                    }
                }
            }
            int magnitude = sqrt(gx * gx + gy * gy);
            result.at<uchar>(i, j) = saturate_cast<uchar>(magnitude);
        }
    }
    return nullptr;
}

int main() {
    // Загрузка изображения в градациях серого
    string imagePath = "/Users/jora/Desktop/маи/5 сем/ОС/ЛР4/cheshire_cat.jpg";  // Укажи путь к изображению
    image = imread(imagePath, IMREAD_GRAYSCALE);

    if (image.empty()) {
        cerr << "Ошибка загрузки изображения!" << endl;
        return -1;
    }

    // Создаем матрицу для хранения результата
    result = Mat::zeros(image.rows, image.cols, image.type());

    // Количество потоков для тестирования
    vector<int> threadCounts = {1, 2, 4, 8, 16, 32};

    for (int threadCount : threadCounts) {
        // Начало измерения времени
        auto start = high_resolution_clock::now();

        pthread_t threads[threadCount];
        ThreadData threadData[threadCount];

        int rowsPerThread = image.rows / threadCount;

        // Создаем потоки
        for (int i = 0; i < threadCount; ++i) {
            threadData[i].startRow = i * rowsPerThread;
            threadData[i].endRow = (i == threadCount - 1) ? image.rows : (i + 1) * rowsPerThread;
            pthread_create(&threads[i], nullptr, applySobelFilter, &threadData[i]);
        }

        // Ждем завершения всех потоков
        for (int i = 0; i < threadCount; ++i) {
            pthread_join(threads[i], nullptr);
        }

        // Конец измерения времени
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start).count();

        cout << "Threads: " << threadCount << ", Time: " << duration << " ms" << endl;
    }

    // Сохранение результата
    imwrite("sobel_output.jpg", result);

    return 0;
}
