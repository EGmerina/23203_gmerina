#include <iostream>            // for standard I/O
#include <string>              // for strings
#include <opencv2/imgproc.hpp> //  threshold
#include <opencv2/highgui.hpp> // OpenCV window I/O

using namespace std;
using namespace cv;

// unset GTK_PATH
// cmake ../
// cmake --build .

const int max_value = 300;
int low = 70;
const String window_name = "Filter";

/*static void on_low_thresh_trackbar(int, void *)
{
    low = min(max_value - 1, low);
    setTrackbarPos("Low", window_name, low);
}*/

int main(int argc, char *argv[])
{
    VideoCapture cap(0);
    Mat img, cvt_img;
    int64 time1, time2, dtime = INT64_MAX, frame_cnt = 0, fps;

    fps = cap.get(CAP_PROP_FPS);

    //namedWindow(window_name);
    //createTrackbar("Low", window_name, &low, max_value, on_low_thresh_trackbar);
    int64 time_beg = getTickCount();
    while (true)
    {
        time1 = getTickCount();
        cap.read(img);
        threshold(img, cvt_img, low, max_value, THRESH_BINARY);
        imshow("Image", img);
        imshow("Converted image", cvt_img);
        time2 = getTickCount();
        dtime = min(dtime, time2 - time1);
        char c = waitKey(1);
        frame_cnt += 1;
        if (c == 'z')
        {
            break;
        }
    }
    int64 time_end = getTickCount();

    printf("Количество кадров в секунду (с помощью функции get(CAP_PROP_FPS)) : %ld\n", fps);
    printf("Количество кадров в секунду (посчитано) : %d\n", (int)(frame_cnt * getTickFrequency() / (time_end - time_beg)));
    printf("Время на обработку : %lf sec\n", dtime / getTickFrequency());
    printf("Доля времени на обработку : %lf %\n", dtime / (double)(time_end - time_beg) * 100);
    return 0;
}
