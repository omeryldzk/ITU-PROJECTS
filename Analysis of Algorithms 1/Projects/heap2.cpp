#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <climits>
#include <sstream>

using namespace std;

// Function to parse CSV file and populate the dataset
vector<int> parseCSV(const string& filename) {
    vector<int> data;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int value;
        while (ss >> value) {
            data.push_back(value);
            if (ss.peek() == ',') {
                ss.ignore();
            }
        }
    }

    file.close();
    return data;
}

// Function to write sorted dataset to CSV file
void writeCSV(const vector<int>& dataset, const string& filename) {
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < dataset.size(); ++i) {
        file << dataset[i];
        if (i < dataset.size() - 1) {
            file << ",";
        }
    }

    file.close();
}

// Function to maintain the max-heap property by fixing a violation at the given index
void maxHeapify(vector<int>& heap, int index, int heapSize) {
    int largest = index; // Assume the current index is the largest

    // Calculate the indices of the left and right children
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;

    // Check if the left child is larger than the current largest
    if (leftChild < heapSize && heap[leftChild] > heap[largest]) {
        largest = leftChild;
    }

    // Check if the right child is larger than the current largest
    if (rightChild < heapSize && heap[rightChild] > heap[largest]) {
        largest = rightChild;
    }

    // If the largest element is not the current index, swap them and recursively call maxHeapify
    if (largest != index) {
        swap(heap[index], heap[largest]);
        maxHeapify(heap, largest, heapSize);
    }
}

// Function to build a max-heap from an unordered input array
void buildMaxHeap(vector<int>& heap) {
    int heapSize = heap.size();

    // Start from the last non-leaf node and perform MAX-HEAPIFY in reverse order
    for (int i = (heapSize / 2) - 1; i >= 0; --i) {
        maxHeapify(heap, i, heapSize);
    }
}

// HEAP-INCREASE-KEY: Increases the key of a specified element in the max-heap
void heapIncreaseKey(vector<int>& heap, int index, int newKey) {
    if (newKey < heap[index]) {
        cerr << "New key is smaller than the current key." << endl;
        return;
    }

    heap[index] = newKey;

    // Fix the max-heap property by swapping with parents if necessary
    while (index > 0 && heap[index] > heap[(index - 1) / 2]) {
        swap(heap[index], heap[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}
// HEAP-EXTRACT-MAX: Removes and returns the maximum element from the max-heap
int heapExtractMax(vector<int>& heap) {
    if (heap.size() < 1) {
        cerr << "Heap underflow: Heap is empty." << endl;
        return INT_MIN; // Assuming INT_MIN as a sentinel value for an empty heap
    }

    int maxElement = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    maxHeapify(heap, 0, heap.size());

    return maxElement;
}

// HEAP-MAXIMUM: Returns the maximum element from the max-heap without removing it
int heapMaximum(const vector<int>& heap) {
    if (heap.size() < 1) {
        cerr << "Heap is empty." << endl;
        return INT_MIN; // Assuming INT_MIN as a sentinel value for an empty heap
    }

    return heap[0];
}

// MAX-HEAP-INSERT: Inserts a new element into the max-heap
void maxHeapInsert(vector<int>& heap, int key) {
    heap.push_back(INT_MIN); // Add a placeholder at the end
    int index = heap.size() - 1;

    // Increase the key to the desired value
    heapIncreaseKey(heap, index, key);
}

// Function to maintain the max-heap property by fixing a violation at the given index (d-ary version)
void daryMaxHeapify(vector<int>& heap, int index, int heapSize, int d) {
    int largest = index;

    // Calculate the indices of the children
    for (int i = 1; i <= d; ++i) {
        int childIndex = d * index + i;
        if (childIndex < heapSize && heap[childIndex] > heap[largest]) {
            largest = childIndex;
        }
    }

    // If the largest element is not the current index, swap them and recursively call daryMaxHeapify
    if (largest != index) {
        swap(heap[index], heap[largest]);
        daryMaxHeapify(heap, largest, heapSize, d);
    }
}

// Function to build a d-ary max-heap from an unordered input array
void buildDaryMaxHeap(vector<int>& heap, int d) {
    int heapSize = heap.size();

    // Start from the last non-leaf node and perform d-ary MAX-HEAPIFY in reverse order
    for (int i = (heapSize / d) - 1; i >= 0; --i) {
        daryMaxHeapify(heap, i, heapSize, d);
    }
}

// (c) INSERT Implementation for d-ary heap
void daryHeapInsert(vector<int>& heap, int key, int d) {
    heap.push_back(key);

    int i = heap.size() - 1;
    while (i > 0) {
        int parent = (i - 1) / d;
        if (heap[i] > heap[parent]) {
            swap(heap[i], heap[parent]);
            i = parent;
        } else {
            break;
        }
    }
}

// Function to maintain the max-heap property by fixing a violation at the given index (d-ary version)
void daryMaxHeapIncreaseKey(vector<int>& heap, int index, int newKey, int d) {
    if (newKey < heap[index]) {
        cerr << "New key is smaller than the current key." << endl;
        return;
    }

    heap[index] = newKey;

    // Fix the d-ary max-heap property by swapping with parents if necessary
    while (index > 0) {
        int parent = (index - 1) / d;
        if (heap[index] > heap[parent]) {
            swap(heap[index], heap[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

// (a) Height Calculation for d-ary heap
int calculateDaryHeight(int n, int d) {
    return static_cast<int>(ceil(log(n + 1) / log(d)) - 1);
}

// (d) INCREASE-KEY Implementation for d-ary heap
void daryHeapIncreaseKey(vector<int>& heap, int i, int k, int d) {
    if (i < 0 || i >= heap.size()) {
        cerr << "Index out of bounds." << endl;
        return;
    }

    daryMaxHeapIncreaseKey(heap, i, k, d);
}

// Function to perform heapsort on the input array
void heapSort(vector<int>& array) {
    int arraySize = array.size();

    // Step 1: Build Max-Heap
    buildMaxHeap(array);

    // Step 2: Sort the heap
    for (int i = arraySize - 1; i > 0; --i) {
        // Swap the root (maximum element) with the last element
        swap(array[0], array[i]);

        // Reduce the heap size and heapify the root
        maxHeapify(array, 0, i);
    }
}

// Main function to execute heap operations based on command-line arguments
int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <DataSet file> <Function name> <Output file> [Optional arguments]" << endl;
        return 1;
    }

    // Parse command-line arguments
    string datasetFile = argv[1];
    string functionName = argv[2];
    string outputFile = argv[3];

    // Load dataset from CSV file
    vector<int> dataset = parseCSV(datasetFile);

    // Execute the specified function based on the given name
    if (functionName == "max_heapify") {
        int index = stoi(argv[4]);
        int d = stoi(argv[5]);
        daryMaxHeapify(dataset, index, dataset.size(), d);
    } else if (functionName == "build_max_heap") {
        int d = stoi(argv[4]);
        buildDaryMaxHeap(dataset, d);
    } else if (functionName == "heapsort") {
        heapSort(dataset);
    } else if (functionName == "max_heap_insert") {
        int key = stoi(argv[4]);
        maxHeapInsert(dataset, key);
    } else if (functionName == "heap_extract_max") {
        heapExtractMax(dataset);
   } else if (functionName == "heap_increase_key") {
    int index = stoi(argv[4]) - 1;  // Adjust index to start from 0
    int newKey = stoi(argv[5]);
    heapIncreaseKey(dataset, index, newKey);
}
else if (functionName == "heap_maximum") {
        heapMaximum(dataset);
    } 
    else if (functionName.substr(0, 1) == "d") {
        // d-ary heap operations
        int d = stoi(functionName.substr(1));
        if (argc >= 5) {
            string optionalArg = argv[4];
            if (optionalArg.substr(0, 1) == "i") {
                int i = stoi(optionalArg.substr(1));
                int newKey = stoi(argv[5]);
                daryHeapIncreaseKey(dataset, i, newKey, d);
            } else if (optionalArg.substr(0, 1) == "k") {
                int key = stoi(optionalArg.substr(1));
                daryHeapInsert(dataset, key, d);
            }
        } else {
            calculateDaryHeight(dataset.size(), d);
        }
    } else {
        cerr << "Invalid function name. Supported functions: max_heapify, build_max_heap, heapsort, max_heap_insert, heap_extract_max, heap_increase_key, heap_maximum, dary_calculate_height, dary_extract_max, dary_insert_element, dary_increase_key" << endl;
        return 1;
    }

    // Write the sorted dataset to the output file
    writeCSV(dataset, outputFile);

    return 0;
}