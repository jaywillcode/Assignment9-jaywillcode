#include <iostream>
#include <stdexcept>

// A simple templated dynamic array that mimics part of std::vector.
// Works with any basic numerical type (int, float, double, ...).
template <typename T>
class GenericArray
{
private:
    T *data;     // pointer to dynamically allocated array
    int length;  // number of elements currently stored

public:
    // Default constructor: start with an empty array
    GenericArray() : data(nullptr), length(0) {}

    // Copy constructor (deep copy)
    GenericArray(const GenericArray &other) : data(nullptr), length(other.length)
    {
        if (length > 0)
        {
            data = new T[length];
            for (int i = 0; i < length; i++)
                data[i] = other.data[i];
        }
    }

    // Assignment operator (deep copy)
    GenericArray &operator=(const GenericArray &other)
    {
        if (this != &other)
        {
            delete[] data;
            length = other.length;
            data = nullptr;
            if (length > 0)
            {
                data = new T[length];
                for (int i = 0; i < length; i++)
                    data[i] = other.data[i];
            }
        }
        return *this;
    }

    // Destructor: free the memory
    ~GenericArray()
    {
        delete[] data;
    }

    // Add an element to the end of the array.
    // Allocates a new, larger array, copies the old data over,
    // adds the new element, then deletes the old array.
    void addElement(T value)
    {
        T *newData = new T[length + 1];
        for (int i = 0; i < length; i++)
            newData[i] = data[i];
        newData[length] = value;

        delete[] data;
        data = newData;
        length++;
    }

    // Return the value at index (bounds checked)
    T at(int index) const
    {
        if (index < 0 || index >= length)
            throw std::out_of_range("GenericArray::at(): index out of range");
        return data[index];
    }

    // Return the number of elements
    int size() const
    {
        return length;
    }

    // Return the sum of all elements
    T sum() const
    {
        T total = T();
        for (int i = 0; i < length; i++)
            total += data[i];
        return total;
    }

    // Return the maximum element
    T max() const
    {
        if (length == 0)
            throw std::out_of_range("GenericArray::max(): array is empty");
        T result = data[0];
        for (int i = 1; i < length; i++)
            if (data[i] > result)
                result = data[i];
        return result;
    }

    // Return the minimum element
    T min() const
    {
        if (length == 0)
            throw std::out_of_range("GenericArray::min(): array is empty");
        T result = data[0];
        for (int i = 1; i < length; i++)
            if (data[i] < result)
                result = data[i];
        return result;
    }

    // Return a NEW array containing elements begin..end (inclusive).
    // The new array has (end - begin + 1) elements.
    // Returns nullptr if the bounds are invalid.
    // The caller is responsible for calling delete[] on the result.
    T *slice(int begin, int end) const
    {
        if (begin < 0 || end >= length || begin > end)
        {
            std::cerr << "slice(): invalid bounds (" << begin << ", " << end
                      << ") for array of size " << length << std::endl;
            return nullptr;
        }

        int sliceLength = end - begin + 1;
        T *result = new T[sliceLength];
        for (int i = 0; i < sliceLength; i++)
            result[i] = data[begin + i];
        return result;
    }
};

int main()
{
    GenericArray<int> int_array;
    for (int i = 0; i <= 12; i++)
        int_array.addElement(i * 10);   // 0, 10, 20, ... 120

    std::cout << "size of array is " << int_array.size() << std::endl;
    std::cout << "sum of the array is " << int_array.sum() << std::endl;
    std::cout << "maximum and minimum of array is " << int_array.max()
              << "\t" << int_array.min() << std::endl;
    std::cout << "element at index 3 is " << int_array.at(3) << std::endl;

    // Valid slice (inclusive of both ends)
    int begin = 5, end = 10;
    int *sliced_array = int_array.slice(begin, end);
    if (sliced_array != nullptr)
    {
        std::cout << "slice(" << begin << "," << end << ") = [";
        for (int i = 0; i < end - begin + 1; i++)
        {
            std::cout << sliced_array[i];
            if (i < end - begin)
                std::cout << ", ";
        }
        std::cout << "]" << std::endl;
        delete[] sliced_array;   // clean up the slice
    }

    // Invalid slice test (bounds checking)
    int *bad_slice = int_array.slice(5, 100);
    if (bad_slice == nullptr)
        std::cout << "bad slice correctly rejected" << std::endl;

    // Test with a different type
    GenericArray<double> double_array;
    double_array.addElement(1.5);
    double_array.addElement(2.5);
    double_array.addElement(-3.0);
    std::cout << "double array sum: " << double_array.sum()
              << ", max: " << double_array.max()
              << ", min: " << double_array.min() << std::endl;

    return 0;
}
