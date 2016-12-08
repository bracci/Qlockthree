# Qlockthree

## Short installation guide:

In order to get the firmware compile you have to install following libraries:

from Arduino Library Manager (guide: https://www.arduino.cc/en/Guide/Libraries):
* [NeoPixel library](https://github.com/adafruit/Adafruit_NeoPixel)
* [DotStar library](https://github.com/adafruit/Adafruit_DotStar)
* [LPD8806 library](https://github.com/adafruit/LPD8806)
* [LedControl (MAX7219) library](https://github.com/wayoda/LedControl)

others you have to install/copy manually to your library directory (C:\Users\MyUserName\Documents\Arduino\libraries\):
* [LPD8806DBL library](https://github.com/bracci/LPD8806DBL) (find download button on the right "Download ZIP")(only used for XXL-matrix using 2 LED each letter)
* [LPD8806RGBW library](https://github.com/bracci/LPD8806RGBW) (find download button on the right "Download ZIP")(only used for LPD8806 RGBW LED's)

P.S.: Unfortunately the LPD8806 library from Adafruit includes a bug which prevents from successfully compile the sketch.
You have to open the file "LPD8806.h" from "C:\Users\MyUserName\Documents\Arduino\libraries\LPD8806" and paste the following lines after file header:

```
#ifndef __LPD8806__H__
#define __LPD8806__H__
```

At the end of the file close the hash-if:
```
#endif
```

After this adaption the class should look somehow like that:
```
[... header...]
#ifndef __LPD8806__H__
#define __LPD8806__H__

class LPD8806 {
[... class content...]
};

#endif
```
