# Q6

import cv2


#read the image using cv
OriginalImage = cv2.imread('Q_4.jpg', cv2.IMREAD_GRAYSCALE)




#apply Canny edge detector using ranges of thresholds to show the strenghts of edges in each range:

#1- Low Threshold: 50-100, the result discussed in the report
EdgeDetctorLow = cv2.Canny(OriginalImage, threshold1=50, threshold2=100)


#2-Medium Threshold: 100-150, the result discussed in the report
EdgeDetctorMedium = cv2.Canny(OriginalImage, threshold1=100, threshold2=150)


#High Threshold: 150-200, the result discussed in the report
EdgeDetctorHigh = cv2.Canny(OriginalImage, threshold1=150, threshold2=200)


#show the original image to compare
cv2.imshow('Original Image', OriginalImage)


cv2.waitKey(0)


cv2.destroyAllWindows()


#show the result of Canny Edge detctor in each case of threshold ranges
cv2.imshow('Edges (Low Threshold)', EdgeDetctorLow)


cv2.imshow('Edges (Medium Threshold)', EdgeDetctorMedium)


cv2.imshow('Edges (High Threshold)', EdgeDetctorHigh)


#save the result of Canny Edge detctor in each case of threshold ranges
cv2.imwrite('EdgesLowThreshold.jpg', EdgeDetctorLow)


cv2.imwrite('EdgesMediumThreshold.jpg', EdgeDetctorMedium)


cv2.imwrite('EdgesHighThreshold.jpg', EdgeDetctorHigh)


cv2.waitKey(0)

cv2.destroyAllWindows()