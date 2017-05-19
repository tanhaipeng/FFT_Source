#include "FFT.h"

//һάFFT
void FFT1D(ComplexNumber *arrayBuf, int n)
{
	//ѭ������
	int i, k, r;

	//������ʱ����������buf1������Ϊn
	ComplexNumber *buf1=new ComplexNumber[n];

	//��arrayBuf������buf1
	memcpy(buf1,arrayBuf,sizeof(ComplexNumber)*n);

	//������ʱ����������buf2������Ϊn
	ComplexNumber *buf2=new ComplexNumber[n];

	//��arrayBuf����Ԫ�ػ�2��ȡ����������
	//��0��1��2��3��4��5��6��7�˵����жԵ������0��4��2��6��1��5��3��7
	int t1,t2;
	for(r=1;pow(2.0,r)<n;r++){
		t1=pow(2.0,r);
		t2=pow(2.0,r-1);
		for(k=0;k<t1;k++){
			for(i=0;i<n/t1;i++){
				buf2[k*n/t1+i].real=buf1[k/2*n/t2+i*2+k%2].real;
				buf2[k*n/t1+i].imag=buf1[k/2*n/t2+i*2+k%2].imag;
			}
		}
		memcpy(buf1,buf2,sizeof(ComplexNumber)*n);
	}


	//���õ����㷨���п��ٸ���Ҷ�任
	//buf1�ǵ�r�������룬buf2��ŵ�r�������
	float c,s;
	for(r=1;pow(2.0,r)<=n;r++){
		t1=pow(2.0,r);
		for(k=0;k<n/t1;k++){
			for(i=t1/2;i<t1;i++){
				//��Ȩ����
				c=cos(-2*PI*(i-t1/2)/t1);
				s=sin(-2*PI*(i-t1/2)/t1);
				buf1[k*t1+i].real= buf2[k*t1+i].real*c - buf2[k*t1+i].imag*s;
				buf1[k*t1+i].imag=buf2[k*t1+i].imag*c+buf2[k*t1+i].real*s;
			}
		}
		for(k=0; k<n/t1; k++){
			for(i=0;i<t1/2;i++){
				buf2[k*t1+i].real= buf1[k*t1+i].real+buf1[k*t1+i+t1/2].real;
				buf2[k*t1+i].imag= buf1[k*t1+i].imag+buf1[k*t1+i+t1/2].imag;
			}
			for(i=t1/2;i<t1;i++){
				buf2[k*t1+i].real= buf1[k*t1+i-t1/2].real-buf1[k*t1+i].real;
				buf2[k*t1+i].imag= buf1[k*t1+i-t1/2].imag-buf1[k*t1+i].imag;
			}
		}

		//��r�����������buf1,��Ϊ��һ������������
		memcpy(buf1,buf2,sizeof(ComplexNumber)*n);
	}


	//����Ҷ�任�Ľ������arrayBuf
	memcpy(arrayBuf,buf2,sizeof(ComplexNumber)*n);

	//�ͷŻ�����
	delete[]buf2;
	delete[]buf1;

}
//��άFFT
void ImgFFT2D(double* imgBuf, int width, int height,double *imgBufOut,ComplexNumber *m_pFFTBuf)
{
	//ѭ������
	int i, j, u, v;

	//ͼ�����ݱ�ɸ������ʹ���m_pFFTBuf
	for(i=0;i<width*height;i++){
		m_pFFTBuf[i].real=imgBuf[i];
		m_pFFTBuf[i].imag=0;
	}

	//����ComplexNumber�ṹ������,����Ϊheight
	ComplexNumber *array=new ComplexNumber[height];

	//������һά���ٸ���Ҷ�任
	for(u=0;u<width;u++){
		for(v=0;v<height;v++){
			array[v].real=m_pFFTBuf[v*width+u].real;
			array[v].imag=m_pFFTBuf[v*width+u].imag;
		}
		FFT1D(array, height);
		for(v=0;v<height;v++){
			m_pFFTBuf[v*width+u].real=array[v].real;
			m_pFFTBuf[v*width+u].imag=array[v].imag;
		}
	}
	delete []array;

	//�ٺ���һά���ٸ���Ҷ�任
	for(v=0;v<height;v++){
		FFT1D(m_pFFTBuf+v*width, width);
	}

	//��Ƶ��ͼ��ͼ����ʽ����imgBufOut
	float t;
	int i0,j0;
	for(i=0;i<height;i++){
		//i0 = i;
		//j0 = j;
		for(j=0;j<width;j++){
			if(i<height/2)
				i0=i+height/2;
			else
				i0=i-height/2;
			if(j<width/2)
				j0=j+width/2;
			else
				j0=j-width/2;

			t=sqrt(m_pFFTBuf[i0*width+j0].real*m_pFFTBuf[i0*width+j0].real
				+m_pFFTBuf[i0*width+j0].imag*m_pFFTBuf[i0*width+j0].imag);
			t=t/500;
			if(t>255)
				imgBufOut[i*width+j]=255;
			else 
				imgBufOut[i*width+j]=t;
		}
	}
}

//һάIFFT
void IFFT1D(ComplexNumber *arrayBuf, int n)
{
	//����
	int i;

	//������arrayBuf����
	for(i=0;i<n;i++)
		arrayBuf[i].imag=-arrayBuf[i].imag;

	//һά���ٸ���Ҷ�任
	FFT1D(arrayBuf, n);

	//ʱ���������������N
	for(i=0;i<n;i++){
		arrayBuf[i].real=arrayBuf[i].real/n;
		arrayBuf[i].imag=-arrayBuf[i].imag/n;
	}

}

// ��άIFFT
void ImgIFFT2D(double* imgBufOut, int width, int height,ComplexNumber *m_pFFTBuf)
{
	//������Ҷ�任������Ϊ��,��ʧ�ܷ���
	if(!m_pFFTBuf)
		return;

	//����
	int i, u, v;

	//��������Ҷ���任
	ComplexNumber *array=new ComplexNumber[height];
	for(u=0;u<width;u++){
		for(v=0;v<height;v++){
			array[v].real=m_pFFTBuf[v*width+u].real;
			array[v].imag=m_pFFTBuf[v*width+u].imag;
		}
		IFFT1D(array, height);
		for(v=0;v<height;v++){
			m_pFFTBuf[v*width+u].real=array[v].real;
			m_pFFTBuf[v*width+u].imag=array[v].imag;
		}
	}
	delete []array;

	//�ٺ�����Ҷ���任
	for(v=0;v<height;v++){
		IFFT1D(m_pFFTBuf+v*width, width);
	}

	//��m_pFFTBuf�ķ�����,�ָ�ͼ�����imgBufOut
	float t;
	for(i=0;i<width*height;i++){
		t=sqrt(m_pFFTBuf[i].real*m_pFFTBuf[i].real
			+m_pFFTBuf[i].imag*m_pFFTBuf[i].imag);
		if(t>255)
			imgBufOut[i]=255;
		else imgBufOut[i]=t;
	}
}

// ͼ��ߴ紦��(����Ϊ2���ݴη�)
void ImgSizeHandle(double *imgBuf, double *&imgBufout, int &width, int &height)
{
	//�б�ߡ�����2���ݴη�
	int flag1=1,flag2=1;
	int isValidateFlag=1;

	while(flag1<height)
		flag1=flag1*2;
	if(flag1!=height)			//ͼ��߲���2���ݴη�
		isValidateFlag=0;

	while(flag2<width)
		flag2=flag2*2;
	if(flag2!=width)			//ͼ�����2���ݴη�
		isValidateFlag=0;

	if(isValidateFlag==0)
	{

		imgBufout=new double[flag1*flag2];
		memset(imgBufout, 0, sizeof(double)*flag1*flag2);
		int x=(flag1-height)/2;
		int y=(flag2-width)/2;
		int k=0;
		for(int i=x;i<x+height;++i)
			for(int j=y;j<y+width;++j)
			{
				imgBufout[i*flag2+j]=imgBuf[k++];
			}
			height=flag1;
			width=flag2;
	}
	else
		imgBufout=imgBuf;
}


// Ƶ�������ƶ�
void FFTShift(int width,int height,ComplexNumber *m_pFFTBuf)
{

	// ��Զ�ά������
	int mid=height/2;
	if(height%2==1)
		mid=mid+1;

	ComplexNumber *tmpBuf=new ComplexNumber[mid];
	for(int i=0;i<width;++i)
	{
		for(int k=0;k<mid;++k)
			tmpBuf[k]=m_pFFTBuf[k*width+i];
		int t=0;
		for(int k=mid;k<height;++k)
		{
			m_pFFTBuf[t*width+i]=m_pFFTBuf[k*width+i];
			++t;
		}
		int p=0;
		for(int k=t;k<height;++k)
		{
			m_pFFTBuf[k*width+i]=tmpBuf[p];
			++p;
		}
	}
	delete []tmpBuf;

	mid=width/2;
	if(width%2==1)
		mid=mid+1;
	tmpBuf=new ComplexNumber[mid];
	for(int i=0;i<height;++i)
	{
		for(int k=0;k<mid;++k)
			tmpBuf[k]=m_pFFTBuf[i*width+k];
		int t=0;
		for(int k=mid;k<width;++k)
		{
			m_pFFTBuf[i*width+t]=m_pFFTBuf[i*width+k];
			++t;
		}
		int p=0;
		for(int k=t;k<width;++k)
		{
			m_pFFTBuf[i*width+k]=tmpBuf[p];
			++p;
		}
	}
	delete []tmpBuf;
}


void ResetImgSize(double *imgBuf, double *&imgBufout,int width, int height,int pWidth,int pHeight)
{
	imgBufout=new double[pWidth*pHeight];
	memset(imgBufout,0,sizeof(double)*pWidth*pHeight);

	int x=(height-pHeight)/2;
	int y=(width-pWidth)/2;

	int k=0;
	for(int i=x;i<x+pHeight;++i)
	{
		for(int j=y;j<y+pWidth;++j)
		{
			imgBufout[k++]=imgBuf[i*width+j];
		}
	}
}


void FourierTrans(double **imgInput,double **mtfInput ,double **imgBufout,int pWidth,int pHeight)
{

	// �任������ͼ���С
	int width,height;
	// ����ԭ��С
	width=pWidth;
	height=pHeight;

	double *m_pImgData,*m_pImgResize,*m_pImgDataOut,*finalRes; 

	// ��ά����->>һά����
	m_pImgData=new double[pWidth*pHeight];
	memset(m_pImgData, 0, sizeof(double)*pWidth*pHeight);
	int k=0;
	for(int i=0;i<pHeight;++i)
		for(int j=0;j<pWidth;++j)
			m_pImgData[k++]=imgInput[i][j];



	// ͼ���С��������ָ���¿ռ�ָ��m_pImgResize(ImgSizeHandle�ڿ��ٿռ�)
	m_pImgResize=NULL;
	ImgSizeHandle(m_pImgData,m_pImgResize,width,height);

	// ���븵��Ҷ������
	ComplexNumber *m_pFFTBuf=new ComplexNumber[width*height];
	memset(m_pFFTBuf, 0, sizeof(ComplexNumber)*width*height);

	m_pImgDataOut=new double[width*height];
	memset(m_pImgDataOut, 0, sizeof(double)*width*height);


	// FFT
	ImgFFT2D(m_pImgResize, width, height, m_pImgDataOut, m_pFFTBuf);


	// FFTShift
	FFTShift(width,height,m_pFFTBuf);

	//Ƶ��������
	int x=(height-pHeight)/2;
	int y=(width-pWidth)/2;
	for(int i=x;i<x+pHeight;++i)
	{
		for(int j=y;j<y+pWidth;++j)
		{
			m_pFFTBuf[i*width+j].real=m_pFFTBuf[i*width+j].real*mtfInput[i-x][j-y];
			m_pFFTBuf[i*width+j].imag=m_pFFTBuf[i*width+j].imag*mtfInput[i-x][j-y];
		}
	}

	// IFFT
	ImgIFFT2D(m_pImgDataOut, width, height,m_pFFTBuf);

	// ͼ���С�ָ�
	ResetImgSize(m_pImgDataOut,finalRes,width,height,pWidth,pHeight);

	int m,n;
	// ���ض�ά����
	for(int i=0;i<pWidth*pHeight;++i)
	{
		m=i/pWidth;
		n=i%pWidth;
		imgBufout[m][n]=finalRes[i];
	}


	//�ڴ��ͷ�
	if(m_pImgData==m_pImgResize)
		delete []m_pImgData;
	else
	{
		delete []m_pImgData;
		delete []m_pImgResize;
	}
	delete []m_pImgDataOut;
	delete []finalRes;
	delete []m_pFFTBuf;

}