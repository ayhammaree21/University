# Part3

import cv2


import numpy as np



#read the image using cv2
tigerImage = cv2.imread('tiger.jpg', cv2.IMREAD_GRAYSCALE)



# calculate the standard deviation using the square root of the variance value which equals 40
Standard_dev = np.sqrt(40)



#get the size of the image using shape parameter
height, width = tigerImage.shape



#generate and gaussian noise randomly based on the standrd deviation value and with zero mean-value and using the height and width if the image
Gaussian_Noise = np.random.normal(0,Standard_dev , (height, width))



#adding the nose on the array of original image by using + sign
tigerImageWithNoise = tigerImage + Gaussian_Noise



#clip the image to be within range between 0 to 255 and then convert the data type of the array to unsigned 8-bit.
tigerImageWithNoise = np.clip(tigerImageWithNoise, 0, 255).astype(np.uint8)



#save the image
cv2.imwrite('tigerImageWithNoise.jpg', tigerImageWithNoise)



#show results and original image
cv2.imshow('Original tiger Image', tigerImage)



cv2.imshow('Noisy tiger Image', tigerImageWithNoise)


cv2.waitKey(0)


cv2.destroyAllWindows()