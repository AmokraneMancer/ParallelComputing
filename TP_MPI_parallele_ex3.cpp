
#include "image.hpp"

#include <chrono>

int main(int argc, char ** argv)
{
    // get command line arguments
    if (argc!=2)
    {
        std::cout << "usage: " << argv[0] << " <radius> " << std::endl;
        return -1;
    }
    int radius = std::stoi(argv[1]);

    std::chrono::time_point<std::chrono::system_clock> t0 = std::chrono::system_clock::now();

    // read image
    image_data_t data1;
    int width, height;
    readPnm("backloop.pnm", width, height, data1);
    if (width <= 0 or height <= 0)
    {
        std::cout << "error: failed to read pnm file" << std::endl;
        exit(-1);
    }
    std::chrono::time_point<std::chrono::system_clock> t1 = std::chrono::system_clock::now();

    // compute blur
    //int x0 = 0, y0 = 0, x1 = width, y1 = height;    // the whole image
    int x0 = 400, y0 = 100, x1 = 1000, y1 = 400;    // only the guy doing backloop
    image_data_t data2 = blur(data1, width, height, radius, x0, y0, x1, y1);
    std::chrono::time_point<std::chrono::system_clock> t2 = std::chrono::system_clock::now();

    // write blurred image
    writePnm("output.pnm", x1-x0, y1-y0, data2);
    std::chrono::time_point<std::chrono::system_clock> t3 = std::chrono::system_clock::now();

    // display computation times
    std::chrono::duration<double> readTime = t1 - t0;
    std::cout << "readTime = " << readTime.count() << std::endl;
    std::chrono::duration<double> blurTime = t2 - t1;
    std::cout << "blurTime = " << blurTime.count() << std::endl;
    std::chrono::duration<double> writeTime = t3 - t2;
    std::cout << "writeTime = " << writeTime.count() << std::endl;
    std::chrono::duration<double> totalTime = t3 - t0;
    std::cout << "totalTime = " << totalTime.count() << std::endl;

    return 0;
}

