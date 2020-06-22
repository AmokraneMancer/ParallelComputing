
#ifndef _IMAGE_HPP_
#define _IMAGE_HPP_

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

typedef std::vector<unsigned char> image_data_t;

// write image
void writePnm(const std::string &filename, int width, int height, const image_data_t & data) 
{
    std::ofstream os(filename);
    // write magic number
    os << "P2" << std::endl;
    // write size
    os << width << ' ' << height << std::endl;
    // write max value
    os << "255" << std::endl;
    // write data
    for (unsigned char pixel : data) 
        os << (unsigned)pixel << '\n';
}

// read image
void readPnm(const std::string &filename, int & width, int & height, image_data_t & data) 
{
    std::ifstream is(filename);
    std::string tmp;
    // read magic number
    std::getline(is, tmp);
    if (tmp != "P2")
    {
        width = height = 0;
        return;
    }
    // read comments
    std::getline(is, tmp);
    while (tmp[0] == '#')
        std::getline(is, tmp);
    // read size
    std::stringstream ss(tmp);
    ss >> width >> height;
    // read max value
    std::getline(is, tmp);
    // read data
    data = image_data_t(width*height);
    for (unsigned k=0; k<data.size(); k++)
    {
        int pixel;
        is >> pixel;
        data[k] = pixel;
    }
}

// Returns a blurred subimage of data1 (using the specified radius for the convolution kernel).
// The size of data1 is width*height.
// The blurred region is, in data1, (x0, y0, x1, y1).
// Therefore the size of the subimage is (x1-x0)*(y1-y0)
image_data_t blur(const image_data_t & data1, int width, int height, int radius, int x0, int y0, int x1, int y1)
{
    int width2 = x1-x0;
    int height2 = y1-y0;
    assert(data1.size() == unsigned(width*height));
    assert(width2 > 0);
    assert(height2 > 0);
    image_data_t data2(width2*height2);
    for (int x=x0; x<x1; x++)
    {
        for (int y=y0; y<y1; y++)
        {
            unsigned s = 0;
            unsigned n = 0;
            for (int lx=-radius; lx<radius; lx++)
            {
                for (int ly=-radius; ly<radius; ly++)
                {
                    int xx = x+lx;
                    int yy = y+ly;
                    if (xx >= 0 and xx < width and yy >= 0 and yy < height)
                    {
                        s += data1[(y+ly)*width + (x+lx)];
                        n++;
                    }
                }
            }
            int pixel = n>0 ? s/n : data1[y*width + x];
            if (x==x0 or y==y0 or x==x1-1 or y==y1-1) pixel = 0;    // draw border
            data2[(y-y0)*width2 + (x-x0)] = pixel;
        }
    }
    return data2;
}

#endif

