# Part6
import numpy as np


import cv2


#read the image with salt and pepper noise
tigerImageWithSP = cv2.imread('tigerImageWithSalt&PepperNoise.jpg', cv2.IMREAD_GRAYSCALE)



#filter the image read above using mean or averaging fiter with kernel size 7x7, using blur functiom
tigerImagewithMeanFilterForSP = cv2.blur(tigerImageWithSP, (7, 7))



#save the filtered image
cv2.imwrite('tigerImageSPwithMeanFilter.jpg',tigerImagewithMeanFilterForSP)



#show the result of the code
cv2.imshow('tiger image with salt and pepper with mean filter', tigerImagewithMeanFilterForSP)


cv2.waitKey(0)


cv2.destroyAllWindows()
