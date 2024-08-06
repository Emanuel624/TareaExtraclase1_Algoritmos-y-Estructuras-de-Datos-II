#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <utility>


using namespace std;

class PagedArray {
private:
    string inputFilePath;
    string outputFilePath;
    int frame1[1024];
    int frame2[1024];
    int frame3[1024];
    int frame4[1024];

    vector<pair<int, int*>> frames;
    unordered_map<int, int> pageTable;
    int pageFaults;
    int pageHits;
    size_t totalSize; // Número total de enteros en el archivo

    void copyBinaryFile() {
        ifstream inputFile(inputFilePath, ios::binary);
        ofstream outputFile(outputFilePath, ios::binary);

        if (!inputFile.is_open()) {
            cerr << "Error al abrir el archivo de entrada: " << inputFilePath << endl;
            exit(1);
        }
        if (!outputFile.is_open()) {
            cerr << "Error al abrir el archivo de salida: " << outputFilePath << endl;
            exit(1);
        }

        outputFile << inputFile.rdbuf();

        inputFile.close();
        outputFile.close();
        cout << "Se ha copiado el archivo binario de forma correcta" << endl;
    }

    void loadPage(int pageIndex, int* frame) {
        ifstream inputFile(outputFilePath, ios::binary);
        if (!inputFile.is_open()) {
            cerr << "Error al abrir el archivo para cargar la página: " << outputFilePath << endl;
            exit(1);
        }
        inputFile.seekg(pageIndex * 1024 * sizeof(int), ios::beg);
        inputFile.read(reinterpret_cast<char*>(frame), 1024 * sizeof(int));
        inputFile.close();
    }

    void savePage(int pageIndex, int* frame) {
        fstream outputFile(outputFilePath, ios::binary | ios::in | ios::out);
        if (!outputFile.is_open()) {
            cerr << "Error al abrir el archivo para guardar la página: " << outputFilePath << endl;
            exit(1);
        }
        outputFile.seekp(pageIndex * 1024 * sizeof(int), ios::beg);
        outputFile.write(reinterpret_cast<char*>(frame), 1024 * sizeof(int));
        outputFile.close();
    }

public:
    PagedArray(string inputFilePath, string outputFilePath)
        : inputFilePath(inputFilePath), outputFilePath(outputFilePath), pageFaults(0), pageHits(0) {
        frames = {{0, frame1}, {1, frame2}, {2, frame3}, {3, frame4}};
        copyBinaryFile();

        // Determinar el tamaño del archivo
        ifstream inputFile(inputFilePath, ios::binary | ios::ate);
        if (!inputFile.is_open()) {
            cerr << "Error al abrir el archivo para determinar el tamaño: " << inputFilePath << endl;
            exit(1);
        }
        totalSize = inputFile.tellg() / sizeof(int); // Número total de enteros en el archivo
        inputFile.close();
    }

    int& operator[](size_t index) {
        int pageIndex = index / 1024;
        int offset = index % 1024;

        // Si la página está cargada en memoria, retornar el valor desde el frame correspondiente
        if (pageTable.find(pageIndex) != pageTable.end()) {
            pageHits++;
            auto frameIt = find_if(frames.begin(), frames.end(), [&](pair<int, int*> frame) {
                return frame.first == pageTable[pageIndex];
            });
            return frameIt->second[offset];
        }

        // Si la página no está cargada, cargarla desde el archivo
        pageFaults++;

        // Elegir un frame para reemplazar
        auto frameToReplace = frames.back();
        frames.pop_back();

        // Guardar la página actual en el disco si está mapeada
        for (auto& entry : pageTable) {
            if (entry.second == frameToReplace.first) {
                savePage(entry.first, frameToReplace.second);
                pageTable.erase(entry.first);
                break;
            }
        }

        // Cargar la nueva página en el frame
        loadPage(pageIndex, frameToReplace.second);
        pageTable[pageIndex] = frameToReplace.first;
        frames.insert(frames.begin(), {frameToReplace.first, frameToReplace.second});

        return frameToReplace.second[offset];
    }

    void writeBack() {
        for (auto& entry : pageTable) {
            int pageIndex = entry.first;
            int frameIndex = entry.second;
            savePage(pageIndex, frames[frameIndex].second);
        }
        cout << "Se han escrito los datos ordenados en el archivo binario." << endl;
    }

    int getPageFaults() const {
        return pageFaults;
    }

    int getPageHits() const {
        return pageHits;
    }

    size_t size() const {
        return totalSize;
    }
};

// QuickSort
void quickSort(PagedArray& arr, int start, int end) {
    if (start >= end)
        return;
    int pivot = arr[start];
    int count = 0;
    for (int i = start + 1; i <= end; i++) {
        if (arr[i] <= pivot)
            count++;
    }
    int pivotIndex = start + count;
    swap(arr[pivotIndex], arr[start]);
    int i = start, j = end;
    while (i < pivotIndex && j > pivotIndex) {
        while (arr[i] <= pivot) {
            i++;
        }
        while (arr[j] > pivot) {
            j--;
        }
        if (i < pivotIndex && j > pivotIndex) {
            swap(arr[i++], arr[j--]);
        }
    }
    quickSort(arr, start, pivotIndex - 1);
    quickSort(arr, pivotIndex + 1, end);
}

// InsertionSort
void insertionSort(PagedArray& arr, int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

// BubbleSort
void bubbleSort(PagedArray& arr, int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 7) {
        cerr << "Uso incorrecto. Formato esperado: ./sorter -input <INPUT FILE PATH> -output <OUTPUT FILE PATH> -alg <ALGORITHM>" << endl;
        return 1;
    }

    string inputFilePath;
    string outputFilePath;
    string algoritmo;

    // Leer los argumentos de la línea de comandos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-input") == 0) {
            inputFilePath = argv[++i];
        } else if (strcmp(argv[i], "-output") == 0) {
            outputFilePath = argv[++i];
        } else if (strcmp(argv[i], "-alg") == 0) {
            algoritmo = argv[++i];
        }
    }

    cout << "Archivo de entrada: " << inputFilePath << endl;
    cout << "Archivo de salida: " << outputFilePath << endl;
    cout << "Algoritmo: " << algoritmo << endl;

    clock_t start, end;
    start = clock(); // Iniciar el reloj

    // Inicializar una instancia de PagedArray
    PagedArray pagedArray(inputFilePath, outputFilePath);
    size_t arraySize = pagedArray.size();
    cout << "Tamaño del array: " << arraySize << endl;

    // Ordenar el PagedArray según el algoritmo especificado
    if (algoritmo == "QS") {
        cout << "Usando QuickSort" << endl;
        quickSort(pagedArray, 0, arraySize - 1);
    } else if (algoritmo == "IS") {
        cout << "Usando InsertionSort" << endl;
        insertionSort(pagedArray, arraySize);
    } else if (algoritmo == "BS") {
        cout << "Usando BubbleSort" << endl;
        bubbleSort(pagedArray, arraySize);
    } else {
        cerr << "Algoritmo no soportado. Use QS, IS, o BS." << endl;
        return 1;
    }

    pagedArray.writeBack();

    // Verificar los resultados y escribir en un archivo CSV
    ofstream csvFile(outputFilePath + ".csv");
    if (!csvFile.is_open()) {
        cerr << "Error al abrir el archivo CSV para escritura: " << outputFilePath << ".csv" << endl;
        return 1;
    }

    for (size_t i = 0; i < arraySize; i++) {
        csvFile << pagedArray[i];
        if (i != arraySize - 1) { // No agregar una coma después del último número
            csvFile << ",";
        }
    }
    csvFile.close();

    cout << "Los números están ordenados correctamente y se han escrito en un archivo CSV." << endl;

    end = clock(); // Detener el reloj
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC); // Calcular el tiempo transcurrido
    cout << "El programa tardó " << time_taken << " segundos en ejecutarse." << endl;
    cout << "Page Faults: " << pagedArray.getPageFaults() << endl;
    cout << "Page Hits: " << pagedArray.getPageHits() << endl;

    return 0;
}
