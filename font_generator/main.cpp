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


    // ======== SPLASH =============
    int imgW = 128;
    int imgH = 64;

    img = imread("splash.bmp", IMREAD_COLOR);
    if(img.empty()){
        std::cout << "splash.bmp not found!" << std::endl;
        return -1;
    }

    if((img.rows != 64) || (img.cols != 128)){
        std::cout << "splash is not 128*64!" << std::endl;
        return -1;
    }

    namedWindow("splash file", cv::WindowFlags::WINDOW_AUTOSIZE);

    int lastByteHex = 0;
    int byteHex = 0;
    int byteCounter = 1;
    int blockCounter = 0;
    int RLEdata[128*64*2];

    cv::Scalar colors[] = {Scalar(255,248,240),
      Scalar(35,142,107),  Scalar(0,165,255),  Scalar(0,69,255),
  Scalar(214,112,218),  Scalar(170,232,238),  Scalar(152,251,152),  Scalar(238,238,175),
  Scalar(147,112,219), Scalar(215,235,250),  Scalar(255,255,0),  Scalar(212,255,127),
  Scalar(255,255,240),  Scalar(220,245,245),  Scalar(196,228,255),
  Scalar(0,0,0),  Scalar(205,235,255),  Scalar(255,0,0),  Scalar(226,43,138),
  Scalar(42,42,165),  Scalar(135,184,222),  Scalar(160,158,95), Scalar(0,255,127),  Scalar(30,105,210),  Scalar(80,127,255),  Scalar(237,149,100),  Scalar(220,248,255),  Scalar(60,20,220),  Scalar(255,255,0),  Scalar(139,0,0),  Scalar(139,139,0),  Scalar(11,134,184),
  Scalar(169,169,169),  Scalar(0,100,0),  Scalar(169,169,169),  Scalar(107,183,189),  Scalar(139,0,139),  Scalar(47,107,85),  Scalar(0,140,255),  Scalar(204,50,153),  Scalar(0,0,139),  Scalar(122,150,233),  Scalar(143,188,143),  Scalar(139,61,72),  Scalar(79,79,47),  Scalar(79,79,47),
  Scalar(209,206,0),  Scalar(211,0,148),  Scalar(147,20,255),  Scalar(255,191,0),  Scalar(105,105,105),  Scalar(105,105,105),  Scalar(255,144,30),  Scalar(34,34,178),  Scalar(240,250,255),  Scalar(34,139,34),  Scalar(255,0,255),
  Scalar(220,220,220),  Scalar(255,248,248),  Scalar(0,215,255),  Scalar(32,165,218), Scalar(128,128,128), Scalar(0,128,0),  Scalar(47,255,173), Scalar(128,128,128), Scalar(240,255,240), Scalar(180,105,255), Scalar(92,92,205),
 Scalar(130,0,75), Scalar(240,255,255), Scalar(140,230,240), Scalar(250,230,230), Scalar(245,240,255), Scalar(0,252,124), Scalar(205,250,255), Scalar(230,216,173), Scalar(128,128,240), Scalar(255,255,224), Scalar(210,250,250),
 Scalar(211,211,211), Scalar(144,238,144),  Scalar(211,211,211),  Scalar(193,182,255), Scalar(122,160,255),  Scalar(170,178,32),  Scalar(250,206,135),  Scalar(153,136,119),  Scalar(153,136,119),  Scalar(222,196,176),
  Scalar(224,255,255),  Scalar(0,255,0),  Scalar(50,205,50),  Scalar(230,240,250),  Scalar(255,0,255),  Scalar(0,0,128),  Scalar(170,205,102),  Scalar(205,0,0),  Scalar(211,85,186),  Scalar(219,112,147),  Scalar(113,179,60),
  Scalar(238,104,123),  Scalar(154,250,0),  Scalar(204,209,72),  Scalar(133,21,199),  Scalar(112,25,25),  Scalar(250,255,245),  Scalar(225,228,255),  Scalar(181,228,255),  Scalar(173,222,255),  Scalar(128,0,0),
  Scalar(230,245,253),  Scalar(0,128,128),  Scalar(213,239,255),  Scalar(185,218,255),  Scalar(63,133,205),  Scalar(203,192,255),  Scalar(221,160,221),  Scalar(230,224,176),  Scalar(128,0,128),  Scalar(0,0,255), Scalar(143,143,188),  Scalar(225,105,65),  Scalar(19,69,139),  Scalar(114,128,250),  Scalar(96,164,244),  Scalar(87,139,46),  Scalar(238,245,255),  Scalar(45,82,160),  Scalar(192,192,192),  Scalar(235,206,135),  Scalar(205,90,106),  Scalar(144,128,112),  Scalar(144,128,112),  Scalar(250,250,255),Scalar(127,255,0),Scalar(180,130,70),Scalar(140,180,210),Scalar(128,128,0),Scalar(216,191,216),Scalar(71,99,255),Scalar(208,224,64),Scalar(238,130,238),Scalar(179,222,245),Scalar(255,255,255),Scalar(245,245,245),Scalar(0,255,255),Scalar(50,205,154)
};

    std::cout << "const RLEdata splashRLEdata[] = {" << std::endl;
    for (int page = 0; page < 8; page++){
       for (int byte = 0; byte < 128; byte++){
            byteHex = 0;
            for (int pix=0; pix<8; pix++){

                int y = page * 8 + pix;
                int x = byte;

                Vec3b &pixel = img.at<Vec3b>(y, x);
                if (pixel == (Vec3b)(0,0,0))
                    byteHex |= (1 << pix);

                pixel[0] = colors[blockCounter][0];
                pixel[1] = colors[blockCounter][1];
                pixel[2] = colors[blockCounter][2];
            }
            //std::cout << page << " " << byte << " = " << byteHex << std::endl;
            if (byteHex == lastByteHex){
                byteCounter++;
            }

            else {
                RLEdata[blockCounter * 2] = byteCounter;
                RLEdata[blockCounter * 2 + 1] = lastByteHex;

                lastByteHex = 0xFF55;

                byteCounter = 0;
                blockCounter++;
            }

           // std::cout << "block " << blockCounter << std::endl;
           // std::cout << "byte " << byteCounter << std::endl;

            //Vec3b &color = image.at<Vec3b>(y,x);

             lastByteHex = byteHex;
        }
    }

    RLEdata[blockCounter * 2] = byteCounter;
    RLEdata[blockCounter * 2 + 1] = lastByteHex;

    std::cout << "max block " << blockCounter << std::endl;
    for (int i=0; i<blockCounter; i++){
        //std::cout << "0x" << std::setfill('0') << /*std::setw(4) << */std::hex << RLEdata[blockCounter*2] << ", " << std::tab;
        std::cout << RLEdata[i*2] << "," <<std::tab;
        std::cout << "0x" << std::setfill('0') << /*std::setw(4) << */std::hex << RLEdata[i*2 + 1] << ", // block " <<  std::dec << i  << std::endl;
    }

    std::cout << std::dec << "// total bytes: " <<  blockCounter*2 << " (compression ratio is " << 1.0f/((float)(blockCounter*2) / (float)1024) << ")" <<std::endl;
    std::cout << "}" << std::endl;

    Mat img2;
    resize(img, img2, Size(), 4, 4, INTER_CUBIC); // upscale 2x
    imshow("splash file", img2);

    /*
        for (int x = 0; x < fontW; x++){
            int colHex = 0;
            for (int y = 0; y < fontH; y++){
                Vec3b pix = img.at<Vec3b>(y, cha * fontW + x + cha);
                if (pix == (Vec3b)(0,0,0)){
                    colHex |= (1 << y);
                }
            }
             std::cout << "0x" << std::setfill('0') << std::setw(4) << std::hex << colHex << ", " << std::tab;

        }
        std::cout << "// " << std::dec <<  (int)(cha+48) << " = '" << ((char)(cha + 48)) << "'" << std::endl;
        */

    waitKey(0);

    return 0;
}



