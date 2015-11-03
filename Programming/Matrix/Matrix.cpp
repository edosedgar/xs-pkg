#include <string>
#include <fstream>
#include <iostream>
#include <limits>
#include <cmath>
#include <typeinfo>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-  TMatrix  -#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename DataT>
TMatrix<DataT>::TMatrix(char* MatrixStore) : Storage(MatrixStore){
	Size.x = 0;
	Size.y = 0;
	Data = NULL;
	if (!MatrixStore || MatrixStore[0] == '\0') 
		throw TMException(errBADF);		
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename DataT>
TMatrix<DataT>::TMatrix(char* MatrixStore,unsigned y,unsigned x): \
														Storage(MatrixStore){
	Size.x = x;
	Size.y = y;
	try {
		Data = new DataT*[Size.y];
		for ( unsigned i=0; i<Size.y; i++ )
			Data[i] = new DataT[Size.x];
	}
	catch (std::bad_alloc &None) {
		throw TMException(errNOMEM);	
	}	
	for ( unsigned i=0; i<Size.y; i++ )
		for ( unsigned j=0; j<Size.x; j++ ) 
			Data[i][j]=0;
	if (!MatrixStore || MatrixStore[0] == '\0') 
		throw TMException(errBADF);		
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename DataT>
TMatrix<DataT>::TMatrix(const TMatrix &TM) {
	TM.IsOK();
	try {
		Data = new DataT*[TM.Size.y];
		for ( unsigned i=0; i<TM.Size.y; i++ )
			Data[i] = new DataT[TM.Size.x];
	}
	catch (std::bad_alloc &None) {
		throw TMException(errNOMEM);	
	}	
	this->Size.x=TM.GetRowsNumber();
	this->Size.y=TM.GetLinesNumber();
	for ( unsigned i=0; i<Size.y; i++ )
		for ( unsigned j=0; j<Size.x; j++ ) 
			this->Data[i][j]=TM.Data[i][j];
	this->Storage = TM.Storage;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename DataT> 
template <typename NewT> 
TMatrix<DataT>::operator TMatrix<NewT>() const {
	TMatrix<NewT> TempMatrix(Storage, Size.y, Size.x);
	TempMatrix.IsOK();
	for ( unsigned i=0; i<Size.y; i++ )
		for ( unsigned j=0; j<Size.x; j++ ) 
			TempMatrix(i,j) = this->Data[i][j];
	return TempMatrix;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename DataT>
TMatrix<DataT>::~TMatrix() {
	for ( unsigned i=0; i<Size.y; i++ )
		delete[](Data[i]);
	delete[](Data);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename DataT>
unsigned TMatrix<DataT>::GetRowsNumber() const{
	return Size.x;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename DataT>
unsigned TMatrix<DataT>::GetLinesNumber() const{
	return Size.y;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename DataT>
int TMatrix<DataT>::operator<<=(unsigned id) {
	std::ifstream DStorage(Storage, std::ios_base::in);
	//@@@@@@@@@@@@@@@@@@@@@@ Free old memory (if necessary ) @@@@@@@@@@@@@@@@@@@
	if ( Data != NULL ) {
		for ( unsigned i=0; i<Size.y; i++ )
			delete[](Data[i]);
		delete[](Data);
	}
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//@@@@@@@@@@@@@@@@@@@@@@ Found matrix @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	unsigned CurrentID = std::numeric_limits<int>::max();	
	DStorage.exceptions ( std::ios_base::eofbit);
	try {
	while ( CurrentID != id )
		{
			DStorage>>CurrentID;
			if (CurrentID == id && DStorage.peek() != ')')
				CurrentID=std::numeric_limits<int>::max();
			DStorage.get();
		}
	} 
	catch (std::ios_base::failure &None){
		DStorage.close();
		throw TMException(errMNFOUND);
	} 	
	DStorage.exceptions ( std::ios_base::goodbit );
	DStorage.ignore(255,'\n');
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//@@@@@@@@@@@@@@@@@@@@@@ Calculate size @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	unsigned BeforeMatrix=DStorage.tellg();
	Size.x=0;
	Size.y=1;
	DataT Buffer=0;
	//Calculate Size.x
	while (true) {
		int ExpectChar = DStorage.peek();
		if (isdigit(ExpectChar) || ExpectChar=='-') {
			Size.x++;
			DStorage>>Buffer;
		}
		ExpectChar = DStorage.peek();
		if (ExpectChar=='\n') 
			break;
		DStorage.get();
	}
	//Calculate Size.y
	while (true) {
		DStorage>>Buffer;
		if ((DStorage.peek())==')' || DStorage.eof() )
			break;
		else
			Size.y++;
		DStorage.ignore(255,'\n');
	}
	DStorage.clear();
	DStorage.seekg(BeforeMatrix);
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//@@@@@@@@@@@@@@@@@@@@@@ Allocate memory @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	try {
		Data = new DataT*[Size.y];
		for ( unsigned i=0; i<Size.y; i++ )
			Data[i] = new DataT[Size.x];
	}
	catch (std::bad_alloc &None) {
		DStorage.close();
		throw TMException(errNOMEM);	
	}		
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//@@@@@@@@@@@@@@@@@@@@@@@@ Fill memory @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	DStorage.exceptions ( std::ios_base::failbit);
	try {
		for ( unsigned i=0; i<Size.y; i++ )
			for ( unsigned j=0; j<Size.x; j++ )
				DStorage>>Data[i][j];
	}
	catch (std::ios_base::failure &None){
		DStorage.close();
		throw TMException(errDATA);
	}
	DStorage.exceptions ( std::ios_base::goodbit );
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	DStorage.close();	
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename DataT>
std::ostream& operator<<(std::ostream &OSTR,const TMatrix<DataT> &TM) {
	TM.IsOK();
	//@@@@@@@@@@@@@@@@@@ Found the biggest element @@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	DataT BiggestElem=0;
	bool FlagAddWidth=0;
	for ( unsigned i=0; i<TM.Size.y; i++ )
		for ( unsigned j=0; j<TM.Size.x; j++ ) {
			if ( abs(TM.Data[i][j]) > BiggestElem ) 
				BiggestElem=abs(TM.Data[i][j]);
			if (TM.Data[i][j] < 0) 
				FlagAddWidth=1;
			}
	BiggestElem=(DataT)((unsigned)BiggestElem);
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//@@@@@@@@@@@@@@@@@@@ Calc the biggest length @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	unsigned BiggestLength=(unsigned)(floor(log10(BiggestElem)))+1;
	//If found numbers below zero increase width for print
	if (FlagAddWidth) 
		BiggestLength+=1;
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//@@@@@@@@@@@@@@@@@@@ Print with matrix lines @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	OSTR.setf(std::ios_base::right,std::ios_base::adjustfield);
	OSTR.setf(std::ios_base::fixed,std::ios_base::floatfield);
	OSTR.precision(1);
	//Determinate type data of class
	unsigned TypeNum = 0;
	if ( typeid(TMatrix<int>) == typeid(TM) )
		TypeNum = 0;
	if ( typeid(TMatrix<double>) == typeid(TM) )
		TypeNum = 1;
	//-------------------------------
	for ( unsigned i=0; i<TM.Size.y; i++ ) {
		OSTR<<(unsigned char)161; //KOI8-R║
		for ( unsigned j=0; j<TM.Size.x; j++ ) {
			if ( TypeNum == 0 )
				OSTR.width(BiggestLength+1);
			if ( TypeNum == 1 )
				OSTR.width(BiggestLength+3);
			OSTR<<TM.Data[i][j];
		}
		OSTR<<' '<<(unsigned char)161; //KOI8-R║
		OSTR<<std::endl;
	}
	OSTR.unsetf(std::ios_base::right);
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	return OSTR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename DataT>
std::string size( const TMatrix<DataT> &TM ) {
	TM.IsOK();
	std::string NewString="Matrix size: ";
	NewString+=std::to_string(TM.Size.y);
	NewString+='*';
	NewString+=std::to_string(TM.Size.x);
	NewString+='.';
	return NewString;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename DataT>
DataT det( const TMatrix<DataT> &TM ) {
	TM.IsOK();
	if ( TM.GetRowsNumber() != TM.GetLinesNumber() )
		throw TMException(TM.errDET);
	TMatrix<double> TempMatrix = TM;
	double Det=1;
    for (int i=0; i<TM.Size.x; i++)
    {
        int k = i;
        for (int j=i+1; j<TM.Size.x; j++)
            if (fabs(TempMatrix(j,i)) > fabs(TempMatrix(k,i)))
                k=j;
        if (fabs (TempMatrix(k,i)) < TM.Accuracy )
        {
            Det=0;
            break;
        }
        double* TempPtr = TempMatrix[i];
        TempMatrix[i] = TempMatrix[k];
        TempMatrix[k] = TempPtr;
        if (i != k)
            Det = -Det;
        Det *= TempMatrix(i,i);
        for (int j=i+1; j<TM.Size.x; j++)
            TempMatrix(i,j) /= TempMatrix(i,i);
        for (int j=0; j<TM.Size.x; j++)
            if (j != i && fabs (TempMatrix(j,i)) > TM.Accuracy)
                for (int k=i+1; k<TM.Size.x; k++)
                    TempMatrix(j,k) -= TempMatrix(i,k) * TempMatrix(j,i);
    }
    return (DataT)Det;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename DataT>
DataT trail( const TMatrix<DataT> &TM ) {
	TM.IsOK();
	if ( TM.Size.x != TM.Size.y )
		throw TMException(TM.errTR);
	DataT Trail=0;
    for (int i=0; i<TM.Size.x; ++i)
    {
        Trail+=TM.Data[i][i];
    }
    return Trail;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename DataT>
TMatrix<DataT> TMatrix<DataT>::operator*(double Mul1) const{
	this->IsOK();
	TMatrix<DataT> NewMatrix(Storage, GetLinesNumber(), GetRowsNumber());
	for ( unsigned i=0; i<Size.y; i++ )
		for ( unsigned j=0; j<Size.x; j++ )
			NewMatrix.Data[i][j]=(this->Data[i][j])*Mul1;		
	return NewMatrix;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename DataT>
TMatrix<DataT> operator* (double Mul1,const TMatrix<DataT> &TM){	
	TM.IsOK();	
	return TM*Mul1;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename DataT>
TMatrix<DataT> TMatrix<DataT>::operator*(const TMatrix &TM){
	TM.IsOK();
	if ( Size.x != TM.Size.y )
		throw TMException(errMUL);
	TMatrix<DataT> TempMatrix = ~TM;
	TMatrix<DataT> Answer(Storage, GetLinesNumber(), TM.GetRowsNumber());
	for ( unsigned i=0; i<Size.y; i++ )
		for ( unsigned j=0; j<TempMatrix.Size.y; j++ )
			for ( unsigned k=0; k<Size.x; k++ ) {
				Answer.Data[i][j]+= Data[i][k]*TempMatrix.Data[j][k];
			}
	return Answer;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename DataT>
TMatrix<DataT> TMatrix<DataT>::operator+(const TMatrix &TM) {
	TM.IsOK();
	if ( Size.x != TM.GetRowsNumber() || Size.y != TM.GetLinesNumber() )
		throw TMException(errADD);
	TMatrix TempMatrix(Storage, Size.y, Size.x);
	for ( unsigned i=0; i<TM.Size.y; i++ )
		for ( unsigned j=0; j<TM.Size.x; j++ )
			TempMatrix.Data[i][j] = Data[i][j]+TM.Data[i][j];
	return TempMatrix;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename DataT>
TMatrix<DataT> TMatrix<DataT>::operator-(const TMatrix &TM) {
	TM.IsOK();
	if ( Size.x != TM.GetRowsNumber() || Size.y != TM.GetLinesNumber() )
		throw TMException(errSUB);
	TMatrix TempMatrix(Storage, Size.y, Size.x);
	for ( unsigned i=0; i<TM.Size.y; i++ )
		for ( unsigned j=0; j<TM.Size.x; j++ )
			TempMatrix.Data[i][j] = Data[i][j]-TM.Data[i][j];
	return TempMatrix;	
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename DataT>
TMatrix<DataT> operator~(const TMatrix<DataT> &TM) {
	TM.IsOK();
	TMatrix<DataT> TempMatrix(TM.Storage, TM.Size.x, TM.Size.y);
	for ( unsigned i=0; i<TM.Size.y; i++ )
		for ( unsigned j=0; j<TM.Size.x; j++ )
			TempMatrix(j,i)=TM.Data[i][j];
	return TempMatrix;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 
template <typename DataT>
DataT*& TMatrix<DataT>::operator[](unsigned Shift) {
	this->IsOK();
	if (Shift>=this->Size.y)
		throw TMException(errOUTR);
	return this->Data[Shift];
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename DataT>
DataT& TMatrix<DataT>::operator()(const unsigned Y, const unsigned X) {
	this->IsOK();
	if ( X >= Size.x || Y >= Size.y )
		throw TMException(errOUTR);
	return Data[Y][X];
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename DataT>
TMatrix<DataT>& TMatrix<DataT>::operator= (const TMatrix &TM) {
	TM.IsOK();
	if ( Size.x != TM.GetRowsNumber() || Size.y != TM.GetLinesNumber() )
		throw TMException(errASSIGN);
	for ( unsigned i=0; i<Size.y; i++ )
		for ( unsigned j=0; j<Size.x; j++ ) 
			this->Data[i][j]=TM.Data[i][j];
	return *this;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename DataT>
void TMatrix<DataT>::IsOK() const{
	if (!Data) throw TMException(errPTR);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-# TMException #-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename DataT>
TMatrix<DataT>::TMException::TMException ( unsigned Error ) : ExType(Error) {
	//Create class
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename DataT>
const char* TMatrix<DataT>::TMException::what() {
	return Errors[ExType]; 
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
