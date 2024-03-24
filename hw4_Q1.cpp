#include <iostream>
#include <iomanip> // Include <iomanip> for setw
#include <stdexcept>

class DoubleSubscriptedArray {
    friend std::ostream &operator<<(std::ostream &, const DoubleSubscriptedArray &);
    friend std::istream &operator>>(std::istream &, DoubleSubscriptedArray &);
public:
    explicit DoubleSubscriptedArray(int = 3, int = 5); // default constructor
    DoubleSubscriptedArray(const DoubleSubscriptedArray &); // copy constructor
    ~DoubleSubscriptedArray(); // destructor
    DoubleSubscriptedArray &operator=(const DoubleSubscriptedArray &); // assignment operator
    bool operator==(const DoubleSubscriptedArray &) const; // equality operator
    bool operator!=(const DoubleSubscriptedArray &right) const {
        return !(*this == right);
    }
    // function operator() overloads
    int &operator()(int, int); // return int& for lvalue
    int operator()(int, int) const; // return int for rvalue
private:
    size_t rows; // number of rows
    size_t columns; // number of columns
    int *ptr; // pointer to first element of the array
};

// constructor
DoubleSubscriptedArray::DoubleSubscriptedArray(int r, int c)
    : rows(r > 0 ? r : throw std::invalid_argument("Number of rows must be greater than 0")),
      columns(c > 0 ? c : throw std::invalid_argument("Number of columns must be greater than 0")),
      ptr(new int[rows * columns]) {
    for (size_t i = 0; i < rows * columns; ++i)
        ptr[i] = 0; // initialize elements to 0
}

// copy constructor
DoubleSubscriptedArray::DoubleSubscriptedArray(const DoubleSubscriptedArray &arrayToCopy)
    : rows(arrayToCopy.rows), columns(arrayToCopy.columns), ptr(new int[arrayToCopy.rows * arrayToCopy.columns]) {
    for (size_t i = 0; i < rows * columns; ++i)
        ptr[i] = arrayToCopy.ptr[i]; // copy array elements
}

// destructor
DoubleSubscriptedArray::~DoubleSubscriptedArray() {
    delete[] ptr;
}

// assignment operator
DoubleSubscriptedArray &DoubleSubscriptedArray::operator=(const DoubleSubscriptedArray &right) {
    if (&right != this) {
        // check for self-assignment
        if (rows != right.rows || columns != right.columns) {
            // if sizes are different, reallocate memory
            delete[] ptr;
            rows = right.rows;
            columns = right.columns;
            ptr = new int[rows * columns];
        }
        for (size_t i = 0; i < rows * columns; ++i)
            ptr[i] = right.ptr[i]; // copy elements
    }
    return *this;
}

// equality operator
bool DoubleSubscriptedArray::operator==(const DoubleSubscriptedArray &right) const {
    if (rows != right.rows || columns != right.columns)
        return false; // different sizes
    for (size_t i = 0; i < rows * columns; ++i) {
        if (ptr[i] != right.ptr[i])
            return false; // different elements
    }
    return true; // arrays are equal
}

// function operator() overloads
int &DoubleSubscriptedArray::operator()(int row, int column) {
    if (row < 0 || row >= rows || column < 0 || column >= columns)
        throw std::out_of_range("Subscript out of range");
    return ptr[row * columns + column];
}

int DoubleSubscriptedArray::operator()(int row, int column) const {
    if (row < 0 || row >= rows || column < 0 || column >= columns)
        throw std::out_of_range("Subscript out of range");
    return ptr[row * columns + column];
}

// overloaded output operator
std::ostream &operator<<(std::ostream &output, const DoubleSubscriptedArray &array) {
    for (size_t i = 0; i < array.rows; ++i) {
        for (size_t j = 0; j < array.columns; ++j) {
            output << std::setw(5) << array(i, j); // print element with width 5
        }
        output << std::endl; // new line after each row
    }
    return output;
}

// overloaded input operator
std::istream &operator>>(std::istream &input, DoubleSubscriptedArray &array) {
    for (size_t i = 0; i < array.rows; ++i) {
        for (size_t j = 0; j < array.columns; ++j) {
            input >> array(i, j); // input element
        }
    }
    return input;
}

int main() {
    DoubleSubscriptedArray chessBoard(3, 5); // 3-by-5 DoubleSubscriptedArray
    std::cout << "Enter values for the chessBoard (3x5):\n";
    std::cin >> chessBoard;

    std::cout << "\nOutput chessBoard (3x5):\n";
    std::cout << chessBoard;

    std::cout << "\nAccessing element at row 1 and column 3: " << chessBoard(1, 3) << std::endl;

    return 0;
}
