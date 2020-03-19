// This program is the implementation of the OTSU'S ALGORITHM/


# include <iostream>
# include <math.h>
# include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;



void image_histogram(Mat image, int histogram[])
{

    	//Initialize all intensity values to 0
    	for(int i = 0; i < 256; i++)
            	histogram[i] = 0;

    	//Calculate the no of pixels for each intensity values
    	for(int y = 0; y < image.rows; y++)
        	for(int x = 0; x < image.cols; x++)
            		histogram[(int)image.at<uchar>(y,x)]++;

}



void histDisplay(int histogram[], const char* name)
{

    	int hist[256];
    
	for(int i = 0; i < 256; i++)
        	hist[i]=histogram[i];

    	//Draw the histograms
    	int hist_w = 512; int hist_h = 400;
    	int bin_w = cvRound((double) hist_w/256);

    	Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255));

    	//Find the maximum intensity element from the histogram
    	int max = hist[0];
    	for(int i = 1; i < 256; i++)
        	if(max < hist[i])
	            	max = hist[i];

    	//Normalize the histogram between 0 and histImage.rows
    	for(int i = 0; i < 256; i++)
        	hist[i] = ((double)hist[i]/max) * histImage.rows;

    	//Draw the intensity line for histogram
	//void line(Mat &img, Point pt1, Point pt2, const Scalar &color, int thickness=1, int lineType=8, int shift=0)
    	for(int i = 0; i < 256; i++)
            	line(histImage, Point(bin_w*(i), hist_h), Point(bin_w*(i), hist_h - hist[i]), Scalar(0), 1, 8, 0);

    	//Display histogram
    	namedWindow(name, CV_WINDOW_AUTOSIZE);
    	imshow(name, histImage);

}





int main(int argc, char *argv[])
{


	Mat in, out;
	float P1[255] = {0.0}, m[255] = {0.0}, mG = 0.0, k_star = 0.0, sig_B[255], avg, sig_G = 0.0, n;
	int kk, sum = 0;

	in = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

	if(in.empty())
	{
		cout << "Sorry !!! Image cannot be read.\n";
		exit(0);
	}

	//Generate the histogram
    	int histogram[256];
    	image_histogram(in, histogram);

	//Caluculate the size of image
    	int size = in.rows * in.cols;

	//Calculate the probability of each intensity
    	float prob[256], nk, A_nk[256];

    	for(int i = 0; i < 256; i++)
	{
        	prob[i] = (double)histogram[i] / size;		//pr(rk) = nk/size,  k = 0, ---, 255
		//cout << "prob[" << i << "] : " << prob[i] << endl;
	}

	//Calculating the global intensity mean, mG, and cumulative sums, P1's and cumulative means, m's
	for(int i = 0; i < 256; i++)
	{
		mG = mG + (i*prob[i]);

		for(int j = 0; j < i; j++)
		{
			P1[i] = P1[i] + prob[j];
			m[i] = m[i] + (j * prob[j]);
		}
		//cout << "P1[" << i << "] : " << P1[i] << endl;
		//cout << "m[" << i << "] : " << m[i] << endl;
	}

	//cout << "mG : " << mG << endl;

	//Calculating the between class variance, sig_B
	for(int k = 0; k < 256; k++)
	{
		sig_B[k] = ( ((mG*P1[k]) - m[k])*((mG*P1[k]) - m[k]) ) / ( P1[k]*(1-P1[k]) );
		cout << "sig_B[" << k << "] : " <<m[k] << endl;
		//cout<<	((mG*P1[k]) - m[k]) <<endl;
	}	

	for(int k = 0; k < 256; k++)
	{
		if(k_star < sig_B[k] && isfinite(sig_B[k]))
		{
			k_star = sig_B[k];
			kk = k;
		}
		//cout << "max[" << k << "] : " << k_star << endl;
	}

	for(int k = 0; k < 256; k++)
		sig_G += ((k-mG)*(k-mG)*prob[k]);

	//for(int k = 0; k < 256; k++)
	//{
	n = sig_B[kk] / sig_G;
	cout << n << endl;
	//}

	for(int i = 0; i < 256; i++)
	{
		for(int j = 0; j < 256; j++)
		{
			if(sig_B[i] >= kk)
				out.at<uchar>(i, j) = 255;

			else
				out.at<uchar>(i, j) = 0;
		}
	}

	//cout << "kk : " << kk << ",   k_star : " << k_star << endl;	

	histDisplay(histogram, "Original Histogram");

	namedWindow("in", WINDOW_AUTOSIZE);
	namedWindow("out", WINDOW_AUTOSIZE);

	imshow("in", in);
	imshow("out", out);

	waitKey(0);

	return 0;


}
