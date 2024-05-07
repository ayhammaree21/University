# Part5
import numpy as np


import cv2


#read the image
tigerImage = cv2.imread('tiger.jpg', cv2.IMREAD_GRAYSCALE)



#get 10% of over all pixel in the image by multiplying the size of the original image with 10%
TenPercentOfPixels = int(0.1 * tigerImage.size)



#get a copy from the original image, to apply salt and pepper noise on 10% from it
tigerImageWithSaltAndPepperNoise = tigerImage.copy()


# generate random indices based on 10% of all pixels in the original image
TenPercentIndices = np.random.choice(np.arange(tigerImage.size), size=TenPercentOfPixels)



# compute number of pixels to set as salt (white) and pepper (black) noise
NumOfSaltPixels = TenPercentOfPixels // 2


# get the first half of the random indices for set salt noise
NumOfSaltIndices = TenPercentIndices[:NumOfSaltPixels]



# get the second half of the random indices for set pepper noise
NumOfPepperIndices = TenPercentIndices[NumOfSaltPixels:]



# put pixels at salt indices with 255 (white)
tigerImageWithSaltAndPepperNoise.flat[NumOfSaltIndices] = 255



# put pixels at pepper indices with 0 (black)
tigerImageWithSaltAndPepperNoise.flat[NumOfPepperIndices] = 0



#filter the image using median filter with kernel size 7x7, using MedianBlur function for the image with added noise as shown above
tigerMedianFilteredImage = cv2.medianBlur(tigerImageWithSaltAndPepperNoise, 7)



#save the images , noisy image , and filtered noisy image
cv2.imwrite('tigerImageWithSalt&PepperNoise.jpg', tigerImageWithSaltAndPepperNoise)


cv2.imwrite('tigerMedianFilteredImage.jpg', tigerMedianFilteredImage)


#show result to show the images with noise and filtered image
cv2.imshow('tiger Noisy Image with Salt and Pepper Noise', tigerImageWithSaltAndPepperNoise)


cv2.imshow('tiger Median Filtered Image', tigerMedianFilteredImage)


cv2.waitKey(0)


cv2.destroyAllWindows()
