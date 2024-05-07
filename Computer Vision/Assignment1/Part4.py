# Part4


import cv2


import numpy as np

#read noisy image from the previous images with adding gaussian noise
tigerImageWithNoise = cv2.imread('tigerImageWithNoise.jpg', cv2.IMREAD_GRAYSCALE)



#use mean filter or avargeing filter, to noise reduction, with kernel size 5x5 using function blur, with conversion to 8-bit datatype
tigerMeanFilteredImage = (cv2.blur(tigerImageWithNoise, (5, 5))).astype(np.uint8)



#save the image resulted
cv2.imwrite('tigerMeanFilteredImage.jpg', tigerMeanFilteredImage)



#show the results and original image
cv2.imshow('Noisy tiger Image', tigerImageWithNoise)



cv2.imshow('Mean filtered tiger Image', tigerMeanFilteredImage)


cv2.waitKey(0)


cv2.destroyAllWindows()