# Q3

import cv2


import numpy as np


#read the images by using cv
NoisyImage1 = cv2.imread('Noisyimage1.jpg', 0)

NoisyImage2 = cv2.imread('Noisyimage2.jpg', 0)

#filtering the images with mean filter and median filter with kernel size 5x5 
# using blur() for mean filter and medianblur() for median filter 
MeanFilterFirstImg = cv2.blur(NoisyImage1, (5, 5))


MedianFilterFirstImg = cv2.medianBlur(NoisyImage1, 5)


MeanFilterSecondImg = cv2.blur(NoisyImage2, (5, 5))


MedianFilterSecondImg = cv2.medianBlur(NoisyImage2, 5)



#save the images resulted from the filters with jpg extension
cv2.imwrite('NoisyImg1WithMeanFilter.jpg', MeanFilterFirstImg)


cv2.imwrite('NoisyImg1WithMedianFilter.jpg', MedianFilterFirstImg)



cv2.imwrite('NoisyImg2WithMeanFilter.jpg', MeanFilterSecondImg)


cv2.imwrite('NoisyImg2WithMedianFilter.jpg', MedianFilterSecondImg)


#show the resulted images from the filters and the original images
cv2.imshow('Noisy Image 1', NoisyImage1)

cv2.imshow('Noisy Image 1 With Mean Filter', MeanFilterFirstImg)


cv2.imshow('Noisy Image 1 With Median Filter', MedianFilterFirstImg)


cv2.imshow('Noisy Image 2', NoisyImage2)


cv2.imshow('Noisy Image 2 With Mean Filter', MeanFilterSecondImg)


cv2.imshow('Noisy Image 2 With Median Filter', MedianFilterSecondImg)

cv2.waitKey(0)

cv2.destroyAllWindows()