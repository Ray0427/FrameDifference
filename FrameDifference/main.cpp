//
//  main.cpp
//  FrameDifference
//
//  Created by RAY on 2015/12/16.
//  Copyright (c) 2015年 RAY. All rights reserved.
//

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;
#define NUM 128
#define LEVEL 80
int WIDTH=640,HEIGHT=480;

//the element chosen here is a 3px by 3px rectangle
//Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
//dilate with larger element so make sure object is nicely visible
//Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));
Mat BackgroundSubtraction(Mat background, Mat current){
    
    Mat newObject(background.rows,background.cols,CV_8UC1);
    absdiff(background, current, newObject);
    //morphOps
    //    erode(newObject, newObject, erodeElement);
    //    dilate(newObject, newObject, dilateElement);
    return newObject;
}
int main(int, char**)
{
    VideoCapture cap(0);
//    IplImage *pic=cvLoadImage("DSC_2029.jpg");
    Mat pic =imread("/Users/ray/Google 雲端硬碟/人機介面/FrameDifference/FrameDifference/DSC_2029.jpg");
    bool picBool=false;
    cvNamedWindow("pic",CV_WINDOW_AUTOSIZE);
    //    help();
    if( !cap.isOpened() )
        return -1;
    cap.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);
    int col=HEIGHT,row=WIDTH;
    int a=0,b=0,c=0,d=0;
    Mat frame,back,gray, prevgray;
    for(;;)
    {
        cap >> frame;
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        a=0,b=0,c=0,d=0;
        if( !prevgray.empty() )
        {
            back=BackgroundSubtraction(prevgray, gray);
            
            for(int i=0; i<row; i++){
                for(int j=0; j<col; j++){
                    if((i<row/3)&&(j<col/2)){
                        a+=(int)(back.at<uchar>(Point(i,j))/NUM);
                    }
                    else if((i>row/3)&&(i<row/3*2)&&(j>col/2)){
                        b+=(int)(back.at<uchar>(Point(i,j))/NUM);
                    }
                    else if((i>row/3*2)&&(j<col/2)){
                        c+=(int)(back.at<uchar>(Point(i,j))/NUM) ;
                    }
//                    else if((i>row/2)&&(j>col/2)){
//                        d+=(int)(back.at<uchar>(Point(i,j))/NUM) ;
//                    }
                    
                }
            }
            if (a>LEVEL&&(!picBool)) {
                picBool=true;
//                cvShowImage("pic", pic);
                imshow("pic", pic);

            }else if(c>LEVEL&&picBool){
                picBool=false;
                cvDestroyWindow("pic");
            }else if(b>LEVEL){
                destroyAllWindows();
                return 0;
            }
//            printf("%d %d %d %d\n",a,b,c,d);
//            imshow("flow", back);
            imshow("frame", frame);
//                            imshow("show", show);

        }
        if(waitKey(30)>=0)
            break;
        std::swap(prevgray, gray);
    }
    
    destroyAllWindows();
    return 0;
}

