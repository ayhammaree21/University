# Part 7
import cv2


import numpy as np



#convoluation function with Sobel filter , using the graidents Gx and Gy,
# and convolve the image with  the gradients and get the magnitude values
# using the square root of the squared value, and normalzie the magnitude and convert it to 8-bit datatype
def ConvolveToSobelFilter(tigerImage):
    #get the image size Horizontal and Vertical using shape parameter
    Vertical, Horizontal = tigerImage.shape
    # create an array for magnitude result from sobel filter with zero valuesand the data type 64-bit float datatype
    MagnitudeG = np.zeros_like(tigerImage, dtype=np.float64)
    #Matrix for Vertical Gy
    GradientY = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]])
    #Matrix for Horizontal Gx
    GradientX = np.array([[-1, -2, -1], [0, 0, 0], [1, 2, 1]])
    #First Loop for Vertical pixels
    for i in range(1, Vertical - 1):
        #Second Loop for Horizontal pixel
        for j in range(1, Horizontal - 1):
            # get a 3x3 neghiborhood aroud the pixels i and j in the original image
            #and apply the sobel filter in the horizontal direction to the neghiborhood
            GxFiltered = np.sum(tigerImage[i - 1:i + 2, j - 1:j + 2] * GradientX)
            # get a 3x3 neghiborhood aroud the pixels i and j in the original image
            # and apply the sobel filter in the vertical direction to the neghiborhood
            GyFiltered = np.sum(tigerImage[i - 1:i + 2, j - 1:j + 2] * GradientY)
            #calculate the magnitude of Gx and Gy at each pixels i and j
            MagnitudeG[i, j] = np.sqrt(GxFiltered ** 2 + GyFiltered ** 2)

    #Normalize the magnitude to be in range from 0 to 255
    MagnitudeG = (MagnitudeG / MagnitudeG.max()) * 255
    #converts the magnitude to 8-bit datatype
    MagnitudeG = np.uint8(MagnitudeG)

    return MagnitudeG




# read the original image
tigerImg = cv2.imread('tiger.jpg', cv2.IMREAD_GRAYSCALE)





#apply convoluation function with sobel filter
FinalResultForSobel = ConvolveToSobelFilter(tigerImg)



#save the result of the sobel filter
cv2.imwrite('tigerImageWithSobelFilter.jpg', FinalResultForSobel)


#show the original image and resulted image from sobel fiter
cv2.imshow('tiger Image', tigerImg)


cv2.imshow('tiger Sobel Filtered Image', FinalResultForSobel)


cv2.waitKey(0)


cv2.destroyAllWindows()