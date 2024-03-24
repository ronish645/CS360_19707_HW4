#include <iostream>
#include <vector>
#include <algorithm>

class Term {
public:
    Term(double coef = 0.0, int exp = 0) : coefficient(coef), exponent(exp) {}

    double getCoefficient() const { return coefficient; }
    int getExponent() const { return exponent; }

    void setCoefficient(double coef) { coefficient = coef; }
    void setExponent(int exp) { exponent = exp; }

    bool operator==(const Term& other) const {
        return exponent == other.exponent;
    }

private:
    double coefficient;
    int exponent;
};

class Polynomial {
public:
    // Constructors
    Polynomial() {}
    Polynomial(const std::vector<Term>& terms) { setTerms(terms); }

    // Destructor
    ~Polynomial() {}

    // Set and get functions
    void setTerms(const std::vector<Term>& t) {
        terms = t;
        simplify();
    }

    std::vector<Term> getTerms() const { return terms; }

    // Printing function
    void print() const {
        if (terms.empty()) {
            std::cout << "0";
        } else {
            bool isFirst = true;
            for (const auto& term : terms) {
                if (!isFirst && term.getCoefficient() > 0) std::cout << " + ";
                else if (term.getCoefficient() < 0) std::cout << " - ";
                if (std::abs(term.getCoefficient()) != 1 || term.getExponent() == 0)
                    std::cout << std::abs(term.getCoefficient());
                if (term.getExponent() != 0)
                    std::cout << "x^" << term.getExponent();
                isFirst = false;
            }
        }
        std::cout << std::endl;
    }

    // Operator overloads
    Polynomial operator+(const Polynomial& other) const;
    Polynomial operator-(const Polynomial& other) const;
    Polynomial operator*(const Polynomial& other) const;
    Polynomial& operator=(const Polynomial& other);
    Polynomial& operator+=(const Polynomial& other);
    Polynomial& operator-=(const Polynomial& other);
    Polynomial& operator*=(const Polynomial& other);

private:
    std::vector<Term> terms;

    // Helper function to simplify a polynomial
    void simplify();
};

// Helper function definitions
void Polynomial::simplify() {
    std::vector<Term> newTerms;
    std::sort(terms.begin(), terms.end(), [](const Term& a, const Term& b) {
        return a.getExponent() > b.getExponent();
    });

    for (auto& term : terms) {
        if (!newTerms.empty() && newTerms.back().getExponent() == term.getExponent()) {
            newTerms.back().setCoefficient(newTerms.back().getCoefficient() + term.getCoefficient());
        } else {
            if (term.getCoefficient() != 0) {
                newTerms.push_back(term);
            }
        }
    }

    terms.swap(newTerms);
}

Polynomial Polynomial::operator+(const Polynomial& other) const {
    Polynomial result = *this;
    for (const auto& term : other.terms) {
        auto it = std::find_if(result.terms.begin(), result.terms.end(), [&term](const Term& t) {
            return t.getExponent() == term.getExponent();
        });
        if (it != result.terms.end()) {
            it->setCoefficient(it->getCoefficient() + term.getCoefficient());
        } else {
            result.terms.push_back(term);
        }
    }
    result.simplify();
    return result;
}

Polynomial Polynomial::operator-(const Polynomial& other) const {
    Polynomial result = *this;
    for (const auto& term : other.terms) {
        auto it = std::find_if(result.terms.begin(), result.terms.end(), [&term](const Term& t) {
            return t.getExponent() == term.getExponent();
        });
        if (it != result.terms.end()) {
            it->setCoefficient(it->getCoefficient() - term.getCoefficient());
        } else {
            result.terms.push_back(Term(-term.getCoefficient(), term.getExponent()));
        }
    }
    result.simplify();
    return result;
}

Polynomial Polynomial::operator*(const Polynomial& other) const {
    Polynomial result;
    // Temporary storage for the product before simplification
    std::vector<Term> productTerms;

    for (const auto& term1 : terms) {
        for (const auto& term2 : other.terms) {
            double coef = term1.getCoefficient() * term2.getCoefficient();
            int exp = term1.getExponent() + term2.getExponent();
            // Check if this exponent already exists in the productTerms
            auto it = std::find_if(productTerms.begin(), productTerms.end(), [exp](const Term& t) {
                return t.getExponent() == exp;
            });
            if (it != productTerms.end()) {
                // If the exponent exists, add the coefficients together
                it->setCoefficient(it->getCoefficient() + coef);
            } else {
                // If the exponent doesn't exist, add a new Term to productTerms
                productTerms.push_back(Term(coef, exp));
            }
        }
    }

    // Assign the combined terms to the result and simplify
    result.setTerms(productTerms);
    return result;
}


          Polynomial& Polynomial::operator=(const Polynomial& other) {
              if (this != &other) {
                  terms = other.terms;
              }
              return *this;
          }

          Polynomial& Polynomial::operator+=(const Polynomial& other) {
              *this = *this + other;
              return *this;
          }

          Polynomial& Polynomial::operator-=(const Polynomial& other) {
              *this = *this - other;
              return *this;
          }

          Polynomial& Polynomial::operator*=(const Polynomial& other) {
              *this = *this * other;
              return *this;
          }

int main() {
Polynomial p1({Term(2, 4), Term(-3, 1), Term(6, 0)});
Polynomial p2({Term(4, 2), Term(1, 2), Term(2, 0)});

std::cout << "Polynomial 1: ";
p1.print();

std::cout << "Polynomial 2: ";
p2.print();

Polynomial sum = p1 + p2;
std::cout << "Sum: ";
sum.print();

Polynomial diff = p1 - p2;
std::cout << "Difference: ";
diff.print();

Polynomial prod = p1 * p2;
std::cout << "Product: ";
prod.print();

return 0;
}

