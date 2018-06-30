#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <vector>
#include <string>

using namespace std;
using namespace cv;

Mat detect(Mat frame);
int getdir (string dir, vector<string> &files);
Mat rotate(double angle, Mat unrotated);
String euPlatesPath = "cascades/eu_plates.xml";
CascadeClassifier cascade;


int main(void) {
    //sleep(3);
    while(1){

        cascade.load(euPlatesPath);
        Mat image;
        Mat rotated;
        Mat detectedAndRotated;
        Mat detected;
        string destination;
        string photoDir;
        string dir = string("photos/");
        vector<string> files = vector<string>();

        getdir(dir,files);
        for (unsigned int i = 0;i < files.size();i++) {
            if (files.size() != 0){
                sleep(1);
                if (files[i].find(".jpg") != string::npos){
                    photoDir = string("photos/" + files[i]);
                    image = imread(photoDir, cv::IMREAD_COLOR);
                    rotated = rotate(-15, image);
                    detectedAndRotated = detect(rotated);
                    detected = rotate(15, detectedAndRotated);
                    destination = "checkedPhotos/" + files[i];
//                 imwrite("image.jpg", image);
//                 imwrite("rotated.jpg", rotated);
//                 imwrite("detectedAndRotated.jpg", detectedAndRotated);
//                 imwrite("detected.jpg", detected);
                    imwrite(destination, detected);
                    cout << "Checked " << destination << endl;
                    const char * c = photoDir.c_str();
                    remove(c);
                    cout << "Deleted " << destination << endl;

                }
            }
        }
    }
    return 0;
}
Mat detect(Mat frame) {
    std::vector<Rect> plates;
    Mat frame_gray;
    //cvtColor(frame, frame_gray, COLOR_BGR2GRAY );
//     if(frame.empty())
//         cout << "";
    if(frame.channels()>1){
        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    }
    else {
        cout << "Image already grey";
        frame_gray = frame;
    }
    equalizeHist( frame_gray, frame_gray );

    cascade.detectMultiScale(frame_gray, plates, 1.1, 2,
       0|CASCADE_SCALE_IMAGE, Size(30, 30) );
    for( size_t i = 0; i < plates.size(); i++ ) {
        Point center( plates[i].x + plates[i].width/2, 
          plates[i].y + plates[i].height/2 );
        ellipse( frame, center, Size(plates[i].width/2, plates[i].height/2),
           0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0);
        Mat faceROI = frame_gray( plates[i] );    }
        return frame;
    }

    Mat rotate(double angle, Mat unrotated){
        Mat src = unrotated;
        Point2f center((src.cols-1)/2.0, (src.rows-1)/2.0);
        Mat rot = getRotationMatrix2D(center, angle, 1.0);
        Rect2f bbox = RotatedRect(Point2f(), src.size(), angle).boundingRect2f();
        rot.at<double>(0,2) += bbox.width/2.0 - src.cols/2.0;
        rot.at<double>(1,2) += bbox.height/2.0 - src.rows/2.0;
        Mat dst;
        warpAffine(src, dst, rot, bbox.size());
        return dst;
    }

    int getdir (string dir, vector<string> &files)
    {
        DIR *dp;
        struct dirent *dirp;
        if((dp  = opendir(dir.c_str())) == NULL) {
            cout << "Error(" << errno << ") opening " << dir << endl;
            return errno;
        }

        while ((dirp = readdir(dp)) != NULL) {
            files.push_back(string(dirp->d_name));
        }
        closedir(dp);
        return 0;
    }
