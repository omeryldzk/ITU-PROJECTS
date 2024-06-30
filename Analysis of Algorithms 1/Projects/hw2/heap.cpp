#include <iostream>    // Input/output operations
#include <fstream>     // File input/output operations
#include <string>      // String manipulation operations
#include <stdexcept>   // Standard exception classes for error handling
#include <climits>     // Constants related to integer limits (e.g., MAX_SIZE)
#include <cmath>       // Mathematical operations (e.g., log, ceil)
#include <string.h>    // C-style string manipulation functions (e.g., strcmp)
#include <ctime>       // Time-related functions
#include <chrono>      // C++11 time utilities for high precision time measurement


const int MAX_SIZE = 20000;

class CityData {
public:
    std::string cityName;
    int population;
    CityData() : cityName(""), population(0) {}
    CityData(const std::string& name, int pop) : cityName(name), population(pop) {}
};
struct ComparisonCounter {
    int value;
};
// Function to maintain the max-heap property after inserting an element
// We need heapify up operation beacuse we need to check parents on the heapify_down operation 
// we can only check childs 
void dary_heapify_Up(CityData* heap, int index, int d) {
    while (index > 0) {
        int parent = (index - 1) / d;
        if (heap[index].population > heap[parent].population) {
            std::swap(heap[index], heap[parent]);
            index = parent;
        } else {
            break; // Max-heap property is restored
        }
    }
}

// INSERT operation for a d-ary max-heap
void dary_insert_element(CityData* heap, int& heapSize, const CityData& element, int d) {
    heap[heapSize] = element;
    dary_heapify_Up(heap, heapSize, d);
    heapSize++;
}

// Function to maintain the max-heap property after extracting the maximum element
void dary_heapify_down(CityData* heap, int index, int heapSize, int d) {
    while (true) {
        int largest = index;
        for (int i = 1; i <= d; ++i) {
            int child = d * index + i;
            if (child < heapSize && heap[child].population > heap[largest].population) {
                largest = child;
            }
        }

        if (largest != index) {
            std::swap(heap[index], heap[largest]);
            index = largest;
        } else {
            break; // Max-heap property is restored
        }
    }
}

// EXTRACT-MAX operation for a d-ary max-heap
CityData dary_extract_max(CityData* heap, int& heapSize, int d) {
    if (heapSize == 0) {
        std::cerr << "Heap is empty. Cannot extract max." << std::endl;
        return CityData(); // Assuming an empty CityData as an indicator for an error or empty heap
    }

    CityData maxElement = heap[0];
    heap[0] = heap[--heapSize];
    dary_heapify_down(heap, 0, heapSize, d);

    return maxElement;
}

// INCREASE-KEY operation for a d-ary max-heap
void dary_increase_key(CityData* heap, int index, const CityData& newKey, int d) {
    if (newKey.population < heap[index].population) {
        std::cerr << "New key is smaller than the current key. Cannot increase key." << std::endl;
    } else {
        heap[index] = newKey;
        dary_heapify_Up(heap, index, d);
    }
}

// Calculate height of a d-ary heap
int dary_calculate_height(int n, int d) {
    // Height of a d-ary heap with n elements is given by:
    // height = ceil(log_d(n*d + 1) - 1)
    return std::ceil(log(n * d + 1) / log(d)) - 1;
}

// Function to build a d-ary max heap from an arbitrary array
void build_dary_Heap(CityData* arr, int size, int d) {
    for (int i = size / d - 1; i >= 0; --i) {
        dary_heapify_down(arr, i, size, d);
    }
}

// Function to print the elements of an array
void printArray(CityData* arr, int size) {
    for (int i = 0; i < size; ++i) {
        std::cout << arr[i].population << " ";
    }
    std::cout << std::endl;
}

// Function to perform the MAX-HEAPIFY operation on a max-heap represented by an array
void max_heapify(CityData* arr, int size, int index, ComparisonCounter& counter) {
    int leftChildIndex = 2 * index + 1;
    int rightChildIndex = 2 * index + 2;
    int largestIndex = index;

    // Check if the child is larger than the parent
    if (leftChildIndex < size) {
        counter.value++; // Increment the counter for each comparison
        if (arr[leftChildIndex].population > arr[largestIndex].population) {
            largestIndex = leftChildIndex;
        }
    }

    if (rightChildIndex < size) {
        counter.value++; // Increment the counter for each comparison
        if (arr[rightChildIndex].population > arr[largestIndex].population) {
            largestIndex = rightChildIndex;
        }
    }

    // If child is larger than parent, swap them and heapify the old parent
    if (largestIndex != index) {
        std::swap(arr[index], arr[largestIndex]);
        max_heapify(arr, size, largestIndex, counter);
    }
}

// Function to increase the key of a specified element in the max heap
void heap_increase_key(CityData* arr, int size, int index, const CityData& key) {
    if (key.population < arr[index].population) {
        std::cerr << "New key is smaller than the current key.\n";
        return;
    }

    arr[index] = key;
    while (index > 0 && arr[(index - 1) / 2].population < arr[index].population) {
        std::swap(arr[index], arr[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}

// Function to build a max heap from an array
void build_max_heap(CityData* arr, int size,ComparisonCounter& counter) {
    for (int i = size / 2 - 1; i >= 0; i--) {
        max_heapify(arr, size, i,counter);
    }
}

// Function to insert a new element into the max heap
void max_heap_insert(CityData* arr, int& size, const CityData& key) {
    // Increase the size of the heap
    size++;
    // Place the new element at the end of the array
    arr[size - 1] = key;
    // Increase the key of the new element to the desired value
    heap_increase_key(arr, size, size - 1, key);
}

// Function to remove and return the maximum element from the max heap
CityData heap_extract_max(CityData* arr, int& size,ComparisonCounter& counter) {
    if (size < 1) {
        std::cerr << "Heap underflow: The heap is empty.\n";
        return CityData(); // Assuming an empty CityData as an indicator for an error or empty heap
    }

    CityData max = arr[0];
    arr[0] = arr[size - 1];
    size--;
    max_heapify(arr, size, 0, counter);

    return max;
}

// Function to return the maximum element from the max heap without removing it
CityData heap_maximum(CityData* arr) {
    return arr[0];
}

// Function to perform heap sort on an array
void heapsort(CityData* arr, int size,ComparisonCounter& counter) {
    // Build max heap
    build_max_heap(arr, size,counter);

    // Extract elements one by one
    for (int i = size - 1; i > 0; i--) {
        // Move current root to the end
        std::swap(arr[0], arr[i]);

        // Call maxHeapify on the reduced heap
        counter.value++;
        max_heapify(arr, i, 0, counter);
    }
}

void performOperation(const char* operation, int d, int i, int k, CityData* heap, int& heapSize, std::string city, ComparisonCounter& counter) {
    if (strcmp(operation, "heapsort") == 0) {
        // Call heapsort operation

        heapsort(heap, heapSize,counter);
        std::cout << "Heapsort completed." << std::endl;
        std::cout << "Total number of comparison :" << counter.value <<std::endl;
    } else if (strcmp(operation, "dary_calculate_height") == 0) {
        // Call calculate height operation
        int height = dary_calculate_height(heapSize, d);
        std::cout << "Calculated height of D-ary max heap with degree " << d << ": " << height << std::endl;

    } else if (strcmp(operation, "dary_extract_max") == 0) {
        // Call D-ary extract max operation
        CityData maxElement = dary_extract_max(heap, heapSize, d);
        if (maxElement.population != 0) {
            std::cout << "Extracted Max Element from D-ary max heap: " << maxElement.population << std::endl;
        }

    } else if (strcmp(operation, "dary_insert_element") == 0) {
        // Call D-ary insert element operation
        CityData element(city, i);
        dary_insert_element(heap, heapSize, element, d);
        std::cout << "Inserted element " << i << " into D-ary max heap." << std::endl;

    } else if (strcmp(operation, "dary_increase_key") == 0) {
        // Call D-ary increase key operation
        CityData newKey(city, k);
        dary_increase_key(heap, i - 1, newKey, d); // Adjust index to 0-based
        std::cout << "Increased key at index " << i << " to " << k << " in D-ary max heap." << std::endl;

    } else if (strcmp(operation, "build_dary_Heap") == 0) {
        // Call build D-ary max heap operation
        build_dary_Heap(heap, heapSize, d);
        std::cout << "Built D-ary max heap with degree " << d << "." << std::endl;

    } else if (strcmp(operation, "max_heapify") == 0) {
        // Call max heapify operation
        max_heapify(heap, heapSize, i,counter);
        std::cout << "Performed max heapify operation at index " << i << "." << std::endl;

    } else if (strcmp(operation, "heap_increase_key") == 0) {
        // Call heap increase key operation
        CityData newKey(city, k);
        heap_increase_key(heap, heapSize, i, newKey);
        std::cout << "Increased key at index " << i << " to " << k << " in max heap." << std::endl;

    } else if (strcmp(operation, "build_max_heap") == 0) {
        // Call build max heap operation
        build_max_heap(heap, heapSize,counter);
        std::cout << "Built max heap." << std::endl;

    } else if (strcmp(operation, "max_heap_insert") == 0) {
        // Call max heap insert operation
        CityData newKey(city, k);
        max_heap_insert(heap, heapSize, newKey);
        std::cout << "Inserted key " << k << " into max heap." << std::endl;
        
    } else if (strcmp(operation, "heap_extract_max") == 0) {
        // Call heap extract max operation
        CityData maxElement = heap_extract_max(heap, heapSize, counter);
        if (maxElement.population != 0) {
            std::cout << "Extracted Max Element from max heap: " << maxElement.population << std::endl;
        }

    } else if (strcmp(operation, "heap_maximum") == 0) {
        // Call heap maximum operation
        CityData maxElement = heap_maximum(heap);
        std::cout << "Maximum Element in max heap: " << maxElement.population << std::endl;
    } 
    
    else {
        std::cerr << "Invalid operation: " << operation << std::endl;
    }
}
std::pair<std::string, std::string> separateString(const std::string& str, char delimiter) {
    size_t pos = str.find(delimiter);
    if (pos != std::string::npos) {
        return {str.substr(0, pos), str.substr(pos + 1)};
    }
    return {str, ""}; // Return the whole string and an empty string if delimiter is not found
}
void printCSV(const CityData arr[], size_t size, const std::string& outputFileName) {
    std::ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening the output file." << std::endl;
        return;
    }

    for (size_t i = 0; i < size; ++i) {
        outputFile << arr[i].cityName << "," << arr[i].population << "\n";
    }

    outputFile.close();
}
// Function to print the elapsed time
void printElapsedTime(const std::chrono::high_resolution_clock::time_point& start_time) {
    // Get the current time
    auto end_time = std::chrono::high_resolution_clock::now();

    // Calculate the duration between start and end times
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);

    // Print the elapsed time in milliseconds
    std::cout << "Elapsed time: " << duration.count() << " nanoseconds" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <filename> <operation> <outputFilename> [d<value>] [i<value>] [k<value>]" << std::endl;
        return 1;
    }

    const char* filename = argv[1];
    const char* operation = argv[2];
    const char* outputFilename = argv[3];

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
                data.cityName = result.first;

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

    
    int d = 2;
    int i = 0;
    int k = 0;

    std::string city; // 'city' is used to store city name for the key value

    for (int argIndex = 4; argIndex < argc; ++argIndex) {
        const char* arg = argv[argIndex];

        if (arg[0] == 'd') {
            d = atoi(arg + 1);
        } else if (arg[0] == 'i') {
            i = atoi(arg + 1);
        } else if (arg[1] == 'k') {
            // Parse the argument in the specified format k_[cityname]_[population]
            std::string kArg = arg + 1;
            size_t underscore1 = kArg.find('_');
            size_t underscore2 = kArg.rfind('_');

            if (underscore1 != std::string::npos && underscore2 != std::string::npos) {
                city = kArg.substr(underscore1 + 1, underscore2 - underscore1 - 1);
                k = atoi(kArg.substr(underscore2 + 1).c_str());
            } else {
                std::cerr << "Invalid 'k' argument format: " << arg << std::endl;
                return 1;
            }
        } else {
            std::cerr << "Invalid optional argument: " << arg << std::endl;
            return 1;
        }
    }
    ComparisonCounter comparisonCounter{0}; // Initialize the comparison counter
    auto start_time = std::chrono::high_resolution_clock::now();
    performOperation(operation,d,i,k,cityDataList,cityDataCount,city,comparisonCounter);
     // Print the sorted data to the output file
    
   
    // Call the function to print the elapsed time
    printElapsedTime(start_time);

    printCSV(cityDataList,cityDataCount, outputFilename);

    file.close();
   return 0;
}
