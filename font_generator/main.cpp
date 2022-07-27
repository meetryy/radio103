#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include  <iostream>
#include  <iomanip>

namespace std {
  template <typename _CharT, typename _Traits>
  inline basic_ostream<_CharT, _Traits> &
  tab(basic_ostream<_CharT, _Traits> &__os) {
    return __os.put(__os.widen('\t'));
  }
}

using namespace cv;

int main(int argc, char *argv[])
{
    int bytesTotal = 0;
    int charsTotal = 0;

    // =========== SMALL FONT ===============
    Mat img = imread("font.bmp", IMREAD_COLOR);
    if(img.empty()){
        std::cout << "font.bmp not found!" << std::endl;
        return -1;
    }

    int fontH = 8;
    int fontW = 5;
    int charNum = 59;

    namedWindow("font file", cv::WindowFlags::WINDOW_AUTOSIZE);

    std::cout << "uint8_t font[] = {" << std::endl;
    for (int cha = 0; cha < charNum; cha++){
        for (int x = 0; x < fontW; x++){
            int colHex = 0;
            for (int y = 0; y < fontH; y++){
                Vec3b pix = img.at<Vec3b>(y, cha * fontW + x + cha);
                if (pix == (Vec3b)(0,0,0)){
                    colHex |= (1 << y);
                }
            }
            std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << colHex << ", " << std::tab;
            bytesTotal++;

        }
        std::cout << "// " << std::dec <<  (int)(cha+32) << " = '" << ((char)(cha + 32)) << "'" << std::endl;
        charsTotal++;
    }

    std::cout << "};" << std::endl;

    imshow("font file", img);
    //waitKey(0);

    // ============ BIG FONT
    img = imread("font_big.bmp", IMREAD_COLOR);
    if(img.empty()){
        std::cout << "font_big.bmp not found!" << std::endl;
        return -1;
    }

    fontH = 16;
    fontW = 9;
    charNum = 10;

    namedWindow("font_big file", cv::WindowFlags::WINDOW_AUTOSIZE);

    std::cout << "uint16_t fontBig[] = {" << std::endl;
    for (int cha = 0; cha < charNum; cha++){
        for (int x = 0; x < fontW; x++){
            int colHex = 0;
            for (int y = 0; y < fontH; y++){
                Vec3b pix = img.at<Vec3b>(y, cha * fontW + x + cha);
                if (pix == (Vec3b)(0,0,0)){
                    colHex |= (1 << y);
                }
            }
             std::cout << "0x" << std::setfill('0') << std::setw(4) << std::hex << colHex << ", " << std::tab;
             bytesTotal += 2;

        }
        charsTotal++;
        std::cout << "// " << std::dec <<  (int)(cha+48) << " = '" << ((char)(cha + 48)) << "'" << std::endl;
    }

    std::cout << "};"<< std::endl;

    imshow("font_big file", img);


     // ============ MID FONT
    img = imread("font_mid.bmp", IMREAD_COLOR);
    if(img.empty()){
        std::cout << "font_mid.bmp not found!" << std::endl;
        return -1;
    }

    fontH = 11;
    fontW = 6;
    charNum = 10;

    namedWindow("font_mid file", cv::WindowFlags::WINDOW_AUTOSIZE);

    std::cout << "uint16_t fontMid[] = {" << std::endl;
    for (int cha = 0; cha < charNum; cha++){
        for (int x = 0; x < fontW; x++){
            int colHex = 0;
            for (int y = 0; y < fontH; y++){
                Vec3b pix = img.at<Vec3b>(y, cha * fontW + x + cha);
                if (pix == (Vec3b)(0,0,0)){
                    colHex |= (1 << y);
                }
            }
             std::cout << "0x" << std::setfill('0') << std::setw(4) << std::hex << colHex << ", " << std::tab;
             bytesTotal += 2;

        }
        charsTotal++;
        std::cout << "// " << std::dec <<  (int)(cha+48) << " = '" << ((char)(cha + 48)) << "'" << std::endl;
    }

    std::cout << "};"<< std::endl;
    std::cout << std::dec;

    std::cout << "//" << charsTotal << " chars, " << (int)bytesTotal << " bytes" << std::endl;

    imshow("font_mid file", img);
    waitKey(0);

    return 0;
}



