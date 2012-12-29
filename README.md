logoDetect
==========

Logo detection utility. Tells does supplied logo is on image. Can be used to find image on image also.

Logo detection C++ utility. Produces quite stable results. Suplied argument have to be transperent logo PNG.
Usage example:
./logo_detect testcases/logo.png testcases/1.jpg
Output:
Success case
 LOGO_DETECTED|0.041689
 On failure:
 LOGO_NOT_DETECTED|0.698299
Threshold value
 Utility has 0.45 as treshold value. 0.00000 - best match, 1.00000 - worst match.
Author: Remigijus Kiminas (output to file and debug option)
 remdex@gmail.com 