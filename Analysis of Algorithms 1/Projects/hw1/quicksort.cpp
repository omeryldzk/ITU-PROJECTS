#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <chrono>

const int MAX_SIZE = 20000; // Set a maximum size for the array

struct CityData {
    std::string city;
    int population;
};

struct ComparisonCounter {
    int value;
};

int partitionLast(CityData arr[], int low, int high, ComparisonCounter& counter) {
    int pivot = arr[high].population;
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        ++counter.value; // Increment the counter for each comparison
        if (arr[j].population <= pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }

    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

int partitionRandom(CityData arr[], int low, int high, ComparisonCounter& counter) {
    std::srand(std::time(0));
    int randomIndex = low + rand() % (high - low + 1);
    std::swap(arr[randomIndex], arr[high]);
    return partitionLast(arr, low, high, counter);
}

int partitionMedianOfThree(CityData arr[], int low, int high, ComparisonCounter& counter) {
    std::srand(std::time(0));

    int index1 = low + rand() % (high - low + 1);
    int index2 = low + rand() % (high - low + 1);
    int index3 = low + rand() % (high - low + 1);

    int value1 = arr[index1].population;
    int value2 = arr[index2].population;
    int value3 = arr[index3].population;

    int medianValue;
    if ((value1 <= value2 && value2 <= value3) || (value3 <= value2 && value2 <= value1))
        medianValue = value2;
    else if ((value1 <= value3 && value3 <= value2) || (value2 <= value3 && value3 <= value1))
        medianValue = value3;
    else
        medianValue = value1;

    if (medianValue == value1)
        std::swap(arr[index1], arr[high]);
    else if (medianValue == value2)
        std::swap(arr[index2], arr[high]);
    else
        std::swap(arr[index3], arr[high]);

    return partitionLast(arr, low, high, counter);
}

void quickSort(CityData arr[], int low, int high, char pivotStrategy, std::ofstream& logFile, ComparisonCounter& counter) {
    if (low < high) {
        int partitionIndex;

        switch (pivotStrategy) {
            case 'l':
                partitionIndex = partitionLast(arr, low, high, counter);
                break;
            case 'r':
                partitionIndex = partitionRandom(arr, low, high, counter);
                break;
            case 'm':
                partitionIndex = partitionMedianOfThree(arr, low, high, counter);
                break;
            default:
                return;
        }
        if (logFile.is_open()) {
            logFile << "Pivot: " << arr[partitionIndex].population << " Array: [";
            for (int i = low; i <= high; ++i) {
                logFile << arr[i].population << (i < high ? ", " : "");
            }
            logFile << "] ..." << std::endl;
        }

        quickSort(arr, low, partitionIndex - 1, pivotStrategy, logFile, counter);
        quickSort(arr, partitionIndex + 1, high, pivotStrategy, logFile, counter);
    }
}

void insertionSort(CityData arr[], int low, int high, ComparisonCounter& counter) {
    for (int i = low + 1; i <= high; ++i) {
        CityData key = arr[i];
        int j = i - 1;

        while (j >= low && arr[j].population > key.population) {
            arr[j + 1] = arr[j];
            --j;
            ++counter.value; // Increment the counter for each comparison in insertion sort
        }

        arr[j + 1] = key;
    }
}

void hybridQuickSort(CityData arr[], int low, int high, char pivotStrategy, int k, std::ofstream& logFile, ComparisonCounter& counter) {
    if (low < high) {
        if (high - low + 1 <= k) {
            insertionSort(arr, low, high, counter);
        } else {
            int partitionIndex;

            switch (pivotStrategy) {
                case 'l':
                    partitionIndex = partitionLast(arr, low, high, counter);
                    break;
                case 'r':
                    partitionIndex = partitionRandom(arr, low, high, counter);
                    break;
                case 'm':
                    partitionIndex = partitionMedianOfThree(arr, low, high, counter);
                    break;
                default:
                    return;
            }
            if (logFile.is_open()) {
                logFile << "Pivot: " << arr[partitionIndex].population << " Array: [";
                for (int i = low; i <= high; ++i) {
                    logFile << arr[i].population << (i < high ? ", " : "");
                }
                logFile << "] ..." << std::endl;
            }

            hybridQuickSort(arr, low, partitionIndex - 1, pivotStrategy, k, logFile, counter);
            hybridQuickSort(arr, partitionIndex + 1, high, pivotStrategy, k, logFile, counter);
        }
    }
}

void printCSV(const CityData arr[], int size, const std::string& outputFileName) {
    std::ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening the output file." << std::endl;
        return;
    }

    for (int i = 0; i < size; ++i) {
        outputFile << arr[i].city << "," << arr[i].population << "\n";
    }

    outputFile.close();
}

std::pair<std::string, std::string> separateString(const std::string& input, char delimiter) {
    std::size_t pos = input.find(delimiter);
    if (pos != std::string::npos) {
        std::string city = input.substr(0, pos);
        std::string data = input.substr(pos + 1);
        return std::make_pair(city, data);
    } else {
        return std::make_pair(input, "");
    }
}

void printElapsedTime(std::chrono::nanoseconds duration, char pivotStrategy, int threshold, bool verbose) {
    if (verbose) {
        std::cout << "Time taken by QuickSort with pivot strategy '" << pivotStrategy
                  << "' and threshold " << threshold << ": " << duration.count() << " ns." << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 6) {
        std::cerr << "Usage: " << argv[0] << " <DataSetFile> <PivotStrategy> <k> <OutputFile> <Verbose(v)>" << std::endl;
        return 1;
    }

    const char* filename = argv[1];
    char pivotStrategy = argv[2][0];
    int k = std::stoi(argv[3]);
    const char* outputFilename = argv[4];
    bool verbose = (argv[5][0] == 'v');

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
        return 1;
    }

    CityData cityDataList[MAX_SIZE];
    std::string line;
    int lineNumber = 0;
    int cityDataCount = 0;

    while (std::getline(file, line)) {
        ++lineNumber;

        auto result = separateString(line, ';');
        if (!result.first.empty()) {
            if (cityDataCount < MAX_SIZE) {
                CityData data;
                data.city = result.first;

                // Check if result.second is not empty before converting to integer
                if (!result.second.empty()) {
                    try {
                        data.population = std::stoi(result.second);
                        cityDataList[cityDataCount++] = data;
                    } catch (const std::invalid_argument& e) {
                        std::cerr << "Error parsing line " << lineNumber << ": Invalid population data - " << e.what() << std::endl;
                    }
                } else {
                    std::cerr << "Error parsing line " << lineNumber << ": Population data is empty." << std::endl;
                }
            } else {
                std::cerr << "Array size exceeded. Increase MAX_SIZE.\n";
                break;
            }
        } else {
            std::cerr << "Error parsing line " << lineNumber << ": " << line << std::endl;
        }
    }

    file.close();

    ComparisonCounter comparisonCounter{0}; // Initialize the comparison counter

    auto start = std::chrono::high_resolution_clock::now();

    if (verbose) {
        std::ofstream logFile("log.txt");
        if (!logFile.is_open()) {
            std::cerr << "Error opening the log file." << std::endl;
            return 1;
        }

        if (k == 1) {
            quickSort(cityDataList, 0, cityDataCount - 1, pivotStrategy, logFile, comparisonCounter);
        } else {
            hybridQuickSort(cityDataList, 0, cityDataCount - 1, pivotStrategy, k, logFile, comparisonCounter);
        }

        logFile.close();
    } 

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

    printElapsedTime(duration, pivotStrategy, k, verbose);

    printCSV(cityDataList, cityDataCount, outputFilename);

    // Print the total number of comparisons
    std::cout << "Total number of comparisons: " << comparisonCounter.value << std::endl;

    return 0;
}
