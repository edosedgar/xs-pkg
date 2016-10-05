#ifndef Matrix_H
#define Matrix_H

#include <exception>
#include <iostream>
#include <string>

//************************ Prototype of class **********************************
template<typename DataT> class TMatrix;
//******************************************************************************

//******************* Prototype of friend functions ****************************
template <typename DataT> std::ostream& operator<<(std::ostream &OSTR,\
						   const TMatrix<DataT> &TM);
template <typename DataT> std::string size(const TMatrix<DataT> &TM);	
template <typename DataT> DataT det( const TMatrix<DataT> &TM );
template <typename DataT> DataT trail( const TMatrix<DataT> &TM );
template <typename DataT> TMatrix<DataT> operator*(double Mul1, 
                                                   const TMatrix<DataT> &TM);
template <typename DataT> TMatrix<DataT> operator~(const TMatrix<DataT> &TM);
//******************************************************************************

//******************* Description of TMatrix class *****************************
template <typename DataT>
class TMatrix {
public:	
	explicit TMatrix(char* MatrixStore);	
	TMatrix(char* MatrixStore, unsigned y, unsigned x);
	TMatrix(const TMatrix &TM);
	template <typename NewT> operator TMatrix<NewT>() const;
	virtual ~TMatrix();
	unsigned GetRowsNumber() const;
	unsigned GetLinesNumber() const;
	int operator<<=(unsigned id);
	friend std::ostream& operator<< <>(std::ostream &OSTR,
                                           const TMatrix &TM);
	friend std::string size <>( const TMatrix &TM );
	friend DataT det <>( const TMatrix &TM );
	friend DataT trail<>( const TMatrix &TM );
	TMatrix<DataT> operator*(double Mul1) const;
	friend TMatrix<DataT> (::operator* <>)(double Mul1, const TMatrix &TM);
	TMatrix<DataT> operator*(const TMatrix &TM);
	TMatrix<DataT> operator+(const TMatrix &TM);
	TMatrix<DataT> operator-(const TMatrix &TM);
	friend TMatrix<DataT> operator~ <>(const TMatrix &TM);//Transpose
	DataT*& operator[](unsigned Shift);
	DataT& operator()(const unsigned Y, const unsigned X);
	TMatrix<DataT>& operator= (const TMatrix &TM);
	void IsOK() const; 
	//**************** Exception class ***********************
	class TMException : public std::exception {
	public:
		explicit TMException (unsigned Error);
		const char* what();
	private:
		const char* Errors[]={
			"Unknown error.",
			"Not found matrix with current id!",
			"Invalide address on data matrix.", 
			"Bad input file.",
			"Matrix not found.",
			"No memory.",
			"Unsupported symbol in matrix storage.",
			"Cannot assigned. The dimensions of the"
                        " matrices are not equal.",
			"Cannot multiply two matrices.",
			"Out of range.",
			"Cannot add. The dimensions of the matrices"
                        " are not equal.",
			"Cannot substraction. The dimensions of the matrices"
                        " are not equal.",
			"Matrix isn't square. Cannot calculate determinate.",
			"Matrix isn't square. Cannot calculate trail."
		};
		unsigned ExType;
	};
	//********************************************************
private:
	enum {
		errEMPTY,
		errID,
		errPTR,
		errBADF,
		errMNFOUND,
		errNOMEM,
		errDATA,
		errASSIGN,
		errMUL,
		errOUTR,
		errADD,
		errSUB,
		errDET,
		errTR
	};
	struct {
		unsigned x;
		unsigned y;
	} Size;	
	double Accuracy=0.001;
	DataT** Data;
	char* Storage;
};

//******************************************************************************

#include "Matrix.cpp"

//******************************************************************************
#endif
