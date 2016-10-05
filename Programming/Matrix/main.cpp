#include <iostream>
#include "Matrix.h"

int main ( int argc, char* argv[] )
{
	std::cout.setf(std::ios_base::fixed,std::ios_base::floatfield);
	//try {
		TMatrix<double> Matrix(argv[1]);
		//TMatrix<double> Matrix2 = Matrix;
		Matrix<<=3;
		std::cout<<Matrix<<std::endl<<Matrix.GetLinesNumber()<<' '<<Matrix.GetRowsNumber()<<std::endl;
		std::cout<<"Determinate: "<<det(Matrix)<<std::endl;
		std::cout<<"Trail: "<<trail(Matrix)<<std::endl;
		//std::cout<<NewMatrix<<std::endl<<size(NewMatrix)<<std::endl;;
		//TMatrix Matrix2(argv[1]);
		//Matrix2<<=4;
		//std::cout<<Matrix*Matrix2<<std::endl;
		//std::cout<<-1*(Matrix*Matrix2)-Matrix2;
		//std::cout<<Matrix*Matrix2;
		/*std::cout<<Matrix<<std::endl;
		TMatrix Matrix2 = Matrix;
		std::cout<<~Matrix2;*/
	//} 
	//template<typename DataT> 
	/*catch (TMatrix<int>::TMException & TME) {
		std::cout<<TME.what()<<std::endl;
		exit ( EXIT_FAILURE );*/
	//}    
    return 0;
}
