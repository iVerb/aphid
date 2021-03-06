/*
 *  sparse linear math test
 */
 
#include <iostream>
#include <math/ConjugateGradient.h>
using namespace aphid;

void testSpVec()
{
	std::cout<<"\n test sparse vec";
	
	static const int elmInd[] = {40, 11, 19, 43, 3, 18, 199, 39, 300, 31, 42, 97};
	static const int elmV[] = {0,1,2,3,4,5,6,7,8,9, 10, 11};
	static const int nElm = 12;
	
	SparseVector<int> vecx;
	
	std::cout<<"\n add elms\n";
	
	for(int i=0;i<nElm;++i) {
		std::cout<<" "<<elmInd[i]<<":"<<elmV[i];
		std::cout.flush();
		vecx.set(elmInd[i], elmV[i]);
	}
	
	std::cout<<"\n index:value\n";
	
	SparseIterator<int> iter = vecx.begin();
	SparseIterator<int> itEnd = vecx.end();
	for(;iter != itEnd;++iter) {
		std::cout<<" "<<iter.index()<<":"<<iter.value();
	}
	
	std::cout<<"\n remove 43\n";
	vecx.remove(43);
	
	iter = vecx.begin();
	itEnd = vecx.end();
	for(;iter != itEnd;++iter) {
		std::cout<<" "<<iter.index()<<":"<<iter.value();
	}

}

void printSpMat(const SparseMatrix<int>& matx)
{
	const int& nrow = matx.numRows();
	const int& ncol = matx.numCols();
	std::cout<<"\n "<<nrow<<"-by-"<<ncol<<" mat";
	if(matx.isColumnMajor() ) {
		std::cout<<" column-major\n";
	} else {
		std::cout<<" row-major\n";
	}
	for(int i=0;i<nrow;++i) {
		for(int j=0;j<ncol;++j) {
			int e = matx.get(i,j);
			std::cout<<" "<<e;
		}
		std::cout<<"\n";
	}
	
}

void testSpMat()
{
	std::cout<<"\n test sparse mat";
	
	SparseMatrix<int> matx;
	static const int nrow = 30;
	static const int ncol = 40;
	matx.create(nrow, ncol);
	
	std::cout<<"\n add elms\n";
	
	for(int i=0;i<nrow;++i) {
		matx.set(i,i,1);
		for(int j=0; j< 3;++j) {
			matx.set(i,rand() % ncol ,1);
		}
	}
	
	printSpMat(matx);
	
	SparseMatrix<int> matt = matx.transposed();
	
	printSpMat(matt);
	
	std::cout<<"\n c = ab";
	SparseMatrix<int> matc = matx * matt;
	
	printSpMat(matc);
}

/// attachment S 3-by-3n n is num of particle
/// spring S 6-by-3n
/// attachment A 3-by-3
/// |1    |
/// |  1  |
/// |    1|

void testAttachmentStAtAS()
{
    std::cout<<"\n test attachment StAtAS";
    SparseMatrix<float> S;
    S.create(3, 30);
    S.set(0, 12, 1.f);
    S.set(1, 13, 1.f);
    S.set(2, 14, 1.f);
    std::cout<<"\n S ";
    S.printMatrix();
    
    SparseMatrix<float> St = S.transposed();
    
    SparseMatrix<float> A;
    A.create(3, 3);
    A.set(0, 0, 1.f);
    A.set(1, 1, 1.f);
    A.set(2, 2, 1.f);
     
    SparseMatrix<float> StAt = St * A; // A^T is A
    SparseMatrix<float> StAtA = StAt * A;
    SparseMatrix<float> StAtAS = StAtA * S;
    std::cout<<"\n StAtAS ";
    StAtAS.printMatrix();
    
}

/// spring A 6-by-6
/// | 0.5        -0.5          |
/// |     0.5         -0.5     |
/// |         0.5          -0.5|
/// |-0.5         0.5          |
/// |    -0.5          0.5     |
/// |        -0.5           0.5|

void testSpringStAtAS()
{
    std::cout<<"\n test spring StAtAS";
    SparseMatrix<float> S;
    S.create(6, 30);
    S.set(0, 12, 1.f);
    S.set(1, 13, 1.f);
    S.set(2, 14, 1.f);
    S.set(3, 3, 1.f);
    S.set(4, 4, 1.f);
    S.set(5, 5, 1.f);
    std::cout<<"\n S ";
    S.printMatrix();
    
    SparseMatrix<float> St = S.transposed();
    
    SparseMatrix<float> A;
    A.create(6, 6);
    A.set(0, 0, .5f);
    A.set(1, 1, .5f);
    A.set(2, 2, .5f);
    A.set(3, 3, .5f);
    A.set(4, 4, .5f);
    A.set(5, 5, .5f);
    
    A.set(0, 3, -.5f);
    A.set(1, 4, -.5f);
    A.set(2, 5, -.5f);
    A.set(3, 0, -.5f);
    A.set(4, 1, -.5f);
    A.set(5, 2, -.5f);
	
	A *= 10.f;
    
    std::cout<<"\n A ";
    A.printMatrix();
    
    SparseMatrix<float> StAt = St * A; // A^T is A
    SparseMatrix<float> StAtB = StAt * A; // B is A
    std::cout<<"\n StAtB ";
    StAtB.printMatrix();
    
    DenseVector<float> p;
    p.create(6);
    p[0] = -99.f;
    p[1] = -30.f;
    p[2] = -10.f;
    p[3] = 39.f;
    p[4] = 10.f;
    p[5] = 12.f;
    std::cout<<"\n p "<<p;
    
    DenseVector<float> StAtBp = StAtB * p;
    std::cout<<"\n StAtBp "<<StAtBp;
	
	SparseMatrix<float> StAtAS = StAtB * S;
    std::cout<<"\n StAtAS ";
    StAtAS.printMatrix();
}

void testCg()
{
	std::cout<<"\n test conjugate gradient";
    SparseMatrix<float> A;
	A.create(4,4,true);
	A.set(0,0,4.f);
	A.set(0,2,-1.f);
	A.set(0,3,-1.f);
	A.set(1,1,4.f);
	A.set(1,2,-1.f);
	A.set(1,3,-1.f);
	A.set(2,0,-1.f);
	A.set(2,1,-1.f);
	A.set(2,2,3.f);
	A.set(3,0,-1.f);
	A.set(3,1,-1.f);
	A.set(3,3,3.f);
	std::cout<<"\n A ";
    A.printMatrix();
	
	DenseVector<float> b;
	b.create(4);
	b[0] = 3.f;
	b[1] = 0.f;
	b[2] = -2.f;
	b[3] = 1.f;
	std::cout<<"\n b"<<b;
	
	DenseVector<float> x;
	x.create(4);
	x[0] = 0.f;
	x[1] = 1.f;
	x[2] = -1.f;
	x[3] = 0.f;
	
	ConjugateGradient<float> cg(&A);
	cg.solve(x, b);
	
	std::cout<<"\n x"<<x;
	
	DenseVector<float> ax = A * x;
	std::cout<<"\n proof "<<ax;
	
}

int main(int argc, char **argv)
{        
    std::cout<<"\n test sparse linear math";
	//testSpVec();
	//testSpMat();
	//testAttachmentStAtAS();
	testSpringStAtAS();
	//testCg();
	std::cout<<"\ndone.\n";
    exit(0);
}
