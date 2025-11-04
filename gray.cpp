#include <opencv2/opencv.hpp>
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm> // For std::min/std::max

const int MAX_DISPLAY_DIM = 800;
// Define the actual maximum pixel value for 10-bit data
const double ACTUAL_MAX_RAW_VALUE = 1023.0; 
const double DISPLAY_GAMMA = 2.2;

void resizeMat(cv::Mat* SMat);

int main() {
    // --- 1. Define Image Parameters ---
    int width = 4624;
    int height = 3468;
    std::string filename = "test.raw";

    // --- 2. Load Raw Data from File ---
    std::ifstream file(filename, std::ios::binary | std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file " << filename << std::endl;
        return -1;
    }

    std::vector<unsigned short> raw_data(width * height);
    file.read(reinterpret_cast<char*>(raw_data.data()), width * height * sizeof(unsigned short));
    file.close();

    // --- 3. Create OpenCV Mat (16-bit single channel) ---
    cv::Mat bayer_image(height, width, CV_16UC1, raw_data.data());

    // --- 4. Demosaic (Convert to BGR color image) ---
    // cv::Mat debayeredImage;
    // // Use the RGGB pattern to BGR output format, using the Edge-Aware (EA) algorithm for quality
    // cv::cvtColor(bayer_image, debayeredImage, cv::COLOR_BayerRG2BGR_EA); 

    // // --- 5. Prepare for Display (Gamma Correction and Scaling) ---

    // // 5a. Convert 16-bit image to 32-bit float image (scaling 0-1023 to 0.0-1.0)
    // cv::Mat floatImage;
    // debayeredImage.convertTo(floatImage, CV_32F, 1.0 / ACTUAL_MAX_RAW_VALUE); 

    // // 5b. Apply Gamma Correction on the floating-point data
    // cv::Mat gammaCorrectedFloat;
    // cv::pow(floatImage, 1.0 / DISPLAY_GAMMA, gammaCorrectedFloat); 

    // // 5c. Convert back to 8-bit unsigned integer (scaling 0.0-1.0 range to 0-255 range)
    // cv::Mat final_display_image_8bit;
    // gammaCorrectedFloat.convertTo(final_display_image_8bit, CV_8U, 255.0); 

    // cv::Mat floatImg;
    // bayer_image.convertTo(floatImg,CV_32F,1.0/ACTUAL_MAX_RAW_VALUE);

    


    cv::Mat final_display_image_8bit;
    bayer_image.convertTo(final_display_image_8bit,CV_8U,255.0/ACTUAL_MAX_RAW_VALUE);

    cv::Mat edges;
    cv::Canny(final_display_image_8bit,edges,0,25);



    // // --- 6. Resize for Display ---
    // if(final_display_image_8bit.cols > MAX_DISPLAY_DIM || final_display_image_8bit.rows > MAX_DISPLAY_DIM){
    //     float display_scale = std::min(static_cast<float>(MAX_DISPLAY_DIM) / final_display_image_8bit.cols,
    //                                    static_cast<float>(MAX_DISPLAY_DIM) / final_display_image_8bit.rows);
    //     cv::Mat resizedImg;
    //     cv::resize(final_display_image_8bit, resizedImg, cv::Size(), display_scale, display_scale, cv::INTER_AREA);
    //     final_display_image_8bit = resizedImg;
    // }

    resizeMat(&edges);
    resizeMat(&final_display_image_8bit);
    

    

    cv::imshow("EDGES",edges);

    // --- 7. Display and Save ---
    cv::imshow("Demosaiced and Gamma Corrected Image", final_display_image_8bit);
    // Saves the full-resolution, gamma-corrected 8-bit image to disk
    //cv::imwrite("output_final_image.png", final_display_image_8bit); 
    cv::waitKey(0);

    return 0;
}


void resizeMat(cv::Mat* SMat){
    cv::Mat mat = *SMat;
        if(mat.cols > MAX_DISPLAY_DIM || mat.rows > MAX_DISPLAY_DIM){
        float display_scale = std::min(static_cast<float>(MAX_DISPLAY_DIM) / mat.cols,
                                       static_cast<float>(MAX_DISPLAY_DIM) / mat.rows);
        cv::Mat resizedImg;
        cv::resize(mat, resizedImg, cv::Size(), display_scale, display_scale, cv::INTER_AREA);
        *SMat = resizedImg;
    }
}
