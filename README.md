Logo Detect
==========
Author: Remigijus Kiminas



Logo detection utility. Tells does supplied logo is on image. Can be used to find image on image also. Produces quite stable results. Suplied argument have to be transperent logo PNG.

Usage example:
> ./logo-detect testcases/logo.png testcases/1.jpg

 * Output:
  * Success case
  * LOGO_DETECTED|0.041689
  * On failure:
  * LOGO_NOT_DETECTED|0.698299



 * Threshold value
   * Utility has 0.45 as treshold value. 0.00000 - best match, 1.00000 - worst match.
