# Part2
import cv2

import numpy as np

#read the image using cv2
tigerImage = cv2.imread('tiger.jpg', cv2.IMREAD_GRAYSCALE)


#use power law tranformation with gamma value= 0.4, by created an array with changing in the values
tigerTransImage = np.array(255.0 * (tigerImage / 255.0) ** 0.4, dtype='uint8')


#save the trnasformed image
cv2.imwrite("tigerTransImage.jpg", tigerTransImage)


#showing the results and the original for the comparison
cv2.imshow("Original tiger Image", tigerImage)


cv2.imshow("Transformed tiger Image", tigerTransImage)


cv2.waitKey(0)

cv2.destroyAllWindows()
