Logo Detect
==========
Author: Remigijus Kiminas


Logo detection utility. Tells does supplied logo is on image. Can be used to find image on image also. Produces quite stable results. Suplied argument have to be transperent logo PNG.

Usage example:
Default search
> ./logo-detect testcases/logo.png testcases/1.jpg

Color search:
> ./logo-detect testcases/logo.png testcases/1.jpg color

 * Output:
  * Success case
  * LOGO_DETECTED|0.041689
  * On failure:
  * LOGO_NOT_DETECTED|0.698299


 * Threshold value for gradient mode
  * Utility has 0.28 as treshold value. 0.00000 - best match, 1.00000 - worst match.
 * Threshold value for color mode
  * Utility has 0.10 as treshold value. 0.00000 - best match, 1.00000 - worst match.


Best practice for best result.
 * If logo you want to find is semi transparent, small, consisted of text and it color influences background color you should use gradient mode, the default one.
 * If your logo is solid color without transparent body I suggest to use color mode search. By my testing in these cases it produces better results.
 * Mainly result quality depends on how well you prepare logo, remove unnecesary background.
