#include <iostream>
#include <cassert>
#include <matio.h>
#pragma comment(lib,"libmatio.lib")


int main()
{
	mat_t *matfp;
	const char* matFilePath = "E:\\Program Files\\Opencv\\test_OpenMat\\test_OpenMat\\x64\\Debug\\model\\model.mat";
	matfp = Mat_Open(matFilePath, MAT_ACC_RDONLY);
	if (NULL == matfp) {
		fprintf(stderr, "Error opening MAT file %s\n", matFilePath);
		return EXIT_FAILURE;
	}
	else
	{
		printf("打开成功");
	}
	matvar_t* matVar = NULL;
	// read data 'x'
	matVar = Mat_VarRead(matfp, "x");
	if (matVar) {
		assert(matVar->data_type == MAT_T_DOUBLE);
		size_t xSize = matVar->nbytes / matVar->data_size;
		const double* xData = static_cast<const double*>(matVar->data);
		for (int i = 0; i < xSize; ++i) {
			std::cout << xData[i] << " ";
		}
		std::cout << std::endl;
		for (int i = 0; i < matVar->rank; ++i) {
			std::cout << matVar->dims[i] << " ";
		}
		std::cout << std::endl;
	}
	// read data 'y'
	matVar = Mat_VarRead(matfp, "y");
	if (matVar) {
		assert(matVar->data_type == MAT_T_DOUBLE);
		size_t ySize = matVar->nbytes / matVar->data_size;
		const double* yData = static_cast<const double*>(matVar->data);
		for (int i = 0; i < ySize; ++i) {
			std::cout << yData[i] << " ";
		}
		std::cout << std::endl;
		for (int i = 0; i < matVar->rank; ++i) {
			std::cout << matVar->dims[i] << " ";
		}
		std::cout << std::endl;
	}
	// read 'minX' and 'step' is similar
	Mat_VarFree(matVar);
	Mat_Close(matfp);
	system("pause");
	return EXIT_SUCCESS;
}