#include<iostream>
#include<cmath>
#include<string>
#include<vector>
#include "../include/stb_image.h"
#include "../include/stb_image_resize.h"


using std::cout;   using std::endl;
using std::vector; using std::string;


/*----------General Parameters-----------------*/
const int scaling = 10;
const string brightness_method = "lightness";
const int times_to_print = 3;
const string colour = "\033[1;35m"; //change last number for other colours
/*---------------------------------------------*/

struct Pixel {

    int r;
    int g;
    int b;
    double brightness;

    double average(){return (r+g+b)/3.;}
    double luminosity(){return 0.21*r + 0.72*g + 0.07*b;}
    double lightness()
    {
        int max = r > g ? r : g;
        max = max > b ? max : b;

        int min = r < g ? r : g;
        min = min < b ? min : b;

        return (min + max)/2.;
    }

};




void create_pixel_matrix(vector< vector<Pixel> > &pixel,
                         const unsigned char *data,
                         const int &w, const int &h, const int &nch);

void print_image(const vector< vector<Pixel> > &pixel_matrix,
                 const string &ascii, const size_t &width, const size_t &height);




int main(int argc, char *argv[])
{

    string ascii = "`^\",:;Il!i~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*"
                    "#MW&8%B@$";


    char *file;
    if(argc != 2)
    {
        cout << "Enter a valid Image file name!" << endl;
        cout << "Try again!" << endl;
        return EXIT_FAILURE;
    }
    else
    {
        file = argv[1];
    }

    int width, height, nchannels;
    unsigned char *data = stbi_load(file, &width, &height, &nchannels, 3);
    cout << "---" << endl;
    cout << "Raw Image has been read!" << endl;
    cout << "Raw Image width:" << width << endl;
    cout << "Raw Image height:" << height << endl;
    cout << "Raw Image channels:" << nchannels << endl;
    cout << "---" << endl;


    int new_width = width/scaling - 1;
    int new_height = height/scaling - 1;
    unsigned char *new_data = new unsigned char[new_width*new_height*nchannels];
    //in pixels, in_w, in_h, in_stride, out pixels, out_w, out_h, out_stride,
                                                                 // channels
    stbir_resize_uint8(data, width, height, 0, new_data, new_width, new_height,
                                                                         0, 3);  
    cout << "Raw Image has been resized!" << endl;


    vector< vector<Pixel> > pixel_matrix(height, vector<Pixel>(width)); 
    create_pixel_matrix(pixel_matrix, new_data, new_width, new_height,
                                                             nchannels);
    cout << "Successfully constructed pixel matrix!" << endl;
    cout << "Pixel matrix size: " << new_width << " x " << new_height << endl;
    cout << "---" << endl;


    cout << "Printing ascii Image:" << endl;
    cout << endl;
    print_image(pixel_matrix, ascii, new_width, new_height);


    return 0;
}


void create_pixel_matrix(vector< vector<Pixel> > &pixel,
                         const unsigned char *data,
                         const int &w, const int &h, const int &nch)
{
    size_t data_size = w * h * nch;
    size_t color_size = w * h;
    int *red   = new int[color_size];
    int *green = new int[color_size];
    int *blue  = new int[color_size];

    size_t count = 0;              
    for(size_t i = 0 ; i <= (data_size-3) ; i += 3)
    {           
        red[count] = (int)data[i];
        count++;
    }
    count = 0;              
    for(size_t i = 1 ; i <= (data_size-2) ; i += 3)
    {           
        red[count] = (int)data[i];
        count++;
    }
    count = 0;              
    for(size_t i = 2 ; i < data_size ; i += 3)
    {           
        red[count] = (int)data[i];
        count++;
    }

    for(size_t i = 0 ; i < color_size ; ++i)
    {
        size_t row = i / w;
        size_t col = i % w;

        pixel[row][col].r = red[i];
        pixel[row][col].g = green[i];
        pixel[row][col].b = blue[i];

        if(brightness_method == "average")
            pixel[row][col].brightness = pixel[row][col].average();
        else if(brightness_method == "lightness")
            pixel[row][col].brightness = pixel[row][col].lightness();
        else if(brightness_method == "luminosity")
            pixel[row][col].brightness = pixel[row][col].luminosity();
    }
    
}


void print_image(const vector< vector<Pixel> > &pixel_matrix,
                 const string &ascii, const size_t &width, const size_t &height)
{
    for(size_t i = 0 ; i < height ; ++i)
    {
        for(size_t j = 0 ; j < width ; ++j)
        {

            //string_character = string_size/ (max_bright/bright_i)
            double ratio = 255./pixel_matrix[i][j].brightness;
            int indx = (int)round(ascii.size()/ratio);

            for(size_t k = 0 ; k < times_to_print ; ++k)
            {
                cout << colour << ascii[indx];
            }
        }
        cout << endl;
    }

}