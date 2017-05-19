#include <iostream>
#include <math.h>
#include <vld.h>
#define PI 3.14159265358979323846


// �������ͽṹ��
struct ComplexNumber
{
	float imag; //�鲿
	float real; //ʵ��
};

using namespace std;

// һάFFT
void FFT1D(ComplexNumber *arrayBuf, int n);
// һάIFFT
void IFFT1D(ComplexNumber *arrayBuf, int n);

// ��άFFT
void ImgFFT2D(double* imgBuf, int width, int height,double*imgBufOut,ComplexNumber *m_pFFTBuf);
// ��άIFFT
void ImgIFFT2D(double* imgBufOut, int width, int height,ComplexNumber *m_pFFTBuf);

// Ƶ��ƽ��
void FFTShift(int width,int height,ComplexNumber *m_pFFTBuf);

// ͼ���С����
void ImgSizeHandle(double *imgBuf, double *&imgBufout,int &width, int &height);

// ͼ���С��ԭ
void ResetImgSize(double *imgBuf, double *&imgBufout,int width, int height,int pWidth,int pHeight);

// Ƶ���ھ������
void FourierTrans(double **imgInput,double **mtfInput ,double **imgBufout,int pWidth,int pHeight);
