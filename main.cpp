/*
 * Author: Remigijus Kiminas
 * remdex@gmail.com
 * Logo detection C++ utility. Produces quite stable results. Suplied argument have to be transperent logo PNG.
 * Usage example:
 * Single logo and single image
 * ./logo_detect testcases/logo.png testcases/1.jpg
 * Multiple logo and multiple images
 * ./logo_detect testcases/logo.png|testcases/logo1.png testcases/1.jpg|testcases/2.jpg
 * Output:
 * Success case
 *  LOGO_DETECTED|<logo_path>|<image_tested>|0.193502
 * On failure:
 *  LOGO_NOT_DETECTED|<logo_path>|<image_tested>|0.698299
 * Threshold value for gradient mode
 *  Utility has 0.20 as treshold value. 0.00000 - best match, 1.00000 - worst match.
 * Threshold value for color mode
 *  Utility has 0.10 as treshold value. 0.00000 - best match, 1.00000 - worst match.
 */

#include <cv.h>
#include <highgui.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

// #define DEBUG_SAVE

const double tresholdLimit = 0.20;
const double tresholdLimitColor = 0.10;

int main(int argc, char** argv)
{
    const char* object_filename = argc >= 3 ? argv[1] : "box.png";   // Logo itself
    const char* scene_filename = argc >= 3 ? argv[2] : "box_in_scene.png"; // In scene
    
    // Match modes
    // gradien - default one, converts to greyscale and searches for matches
    // color - image not converted to and default search executed instantly without greyconversion
    const char* match_mode_execute = argc >= 4 ? argv[3] : "gradient"; // Match method

    Mat img, imggrey; Mat templ, templgrey; Mat result;
    
    // Sobel filter
    int ddepth = CV_16S;
    int scale = 1;
    int delta = 0;
    Mat grad, grad_template, grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    
    // Create an array of templates images
    std::vector<char*> v;
    std::vector<Mat> logo_items; // Holds logo as array avoids disk reads.
    char* chars_array = strtok(  const_cast<char *>(object_filename), "|");       
    while(chars_array)
    {        
        Mat templ = imread( chars_array, 1 );
    
        if (strcmp(match_mode_execute,"gradient") == 0) { 
        
            cvtColor( templ, templgrey,   CV_RGB2GRAY );
            
            /// Gradient X
            Sobel( templgrey, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
            convertScaleAbs( grad_x, abs_grad_x );
            
            /// Gradient Y
            Sobel( templgrey, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
            convertScaleAbs( grad_y, abs_grad_y );
          
            /// Total Gradient (approximate)
            addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad_template );
            
            logo_items.push_back( grad_template );             
        } else {        
            logo_items.push_back( templ );            
        }
       
        v.push_back(chars_array);
        chars_array = strtok(NULL, "|");
    }   

    // Create an array of images  
    std::vector<char*> vscene;  
    chars_array = strtok(  const_cast<char *>(scene_filename), "|");
       
    while(chars_array)
    {
        vscene.push_back(chars_array);
        chars_array = strtok(NULL, "|");     
    }

    for(size_t nscene = 0; nscene < vscene.size(); ++nscene)
    {
        bool imageProcessed = false;
           
        img = imread( vscene[nscene], 1 );
                
        if (strcmp(match_mode_execute,"gradient") == 0) {
            cvtColor( img, imggrey,  CV_RGB2GRAY );
            
            /// Gradient X
            Sobel( imggrey, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
            convertScaleAbs( grad_x, abs_grad_x );
            
            /// Gradient Y
            Sobel( imggrey, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
            convertScaleAbs( grad_y, abs_grad_y );
          
            /// Total Gradient (approximate)
            addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
            
            img = grad;
        }

        for(size_t n = 0; n < v.size(); ++n)
        {                    
            templ = logo_items[n];            
          
            #ifdef DEBUG_SAVE
            Mat img_display;
            img.copyTo( img_display );
            #endif
                
           /// Create the result matrix
           int result_cols =  img.cols - templ.cols + 1;
           int result_rows = img.rows - templ.rows + 1;
           result.create( result_cols, result_rows, CV_32FC1 );
            
           int match_method = CV_TM_SQDIFF_NORMED ;
              
           /// Do the Matching
           matchTemplate( img, templ, result, match_method );
           
           /// Localizing the best match with minMaxLoc
           double minVal; double maxVal; Point minLoc; Point maxLoc;      
           minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
        
           #ifdef DEBUG_SAVE

               Point matchLoc;

               /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
               if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
                 { matchLoc = minLoc; }
               else
                  { matchLoc = maxLoc; }
       
               rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), cvScalar(255, 255, 255, 255), 2, 8, 0 );
               rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), cvScalar(255, 255, 255, 255), 2, 8, 0 );
                    
               IplImage iplimg = img_display;  
               cvSaveImage("img_display1.jpg" , & iplimg);

           #endif
            
           if ( ( strcmp(match_mode_execute,"gradient") == 0 && minVal < tresholdLimit ) || ( strcmp(match_mode_execute,"color") == 0 && minVal < tresholdLimitColor ) ) {
                printf("LOGO_DETECTED|%s|%s|%f\n",v[n],vscene[nscene],minVal); 
           } else {
                printf("LOGO_NOT_DETECTED|%s|%s|%f\n",v[n],vscene[nscene],minVal); 
           }
       }
   }

   return 0;
}