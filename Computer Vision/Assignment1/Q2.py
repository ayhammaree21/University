# Q2
import cv2



import numpy as np


def GaussianLambada(Parameter, Volume):
    # creat an array showing a Gaussian kernel by  numpy  fromfunction()
    kernel = np.fromfunction(
        # create the function to generate each element of the kernel based on it x and y
        lambda x, y: (1 / (2 * np.pi * Parameter ** 2)) * np.exp(
            -((x - (Volume - 1) / 2) ** 2 + (y - (Volume - 1) / 2) ** 2) / (2 * Parameter ** 2)),
        # Specify the shape of the 2D array (Volume x Volume) to create the kernel
        (Volume, Volume)
    )

    # normailze the kernel by dividing each element on the sum of all elemnts on the kernel
    return kernel / np.sum(kernel)


def FunctionForPaddingAndConvolve(Image, Kernels):
    # get the size of the convolution kernel using width and height or row and column using shape parameter
    RowK, ColK = Kernels.shape

    # get the size of the image using width and height or row and column using shape parameter
    RowI, ColI = Image.shape

    # paad the image with zeros to ease the  convolution on the edges that have not enough size as kernel size
    PaddingResult = np.pad(Image, ((RowK // 2, RowK // 2), (ColK // 2, ColK // 2)), mode='constant')

    # create an array for the convolved image with zero values and data type 64-bit float
    EditedImage = np.zeros_like(Image, dtype=np.float64)

    # first loop for horizontal pixels
    for i in range(RowI):
        #second loop for vertical pixels
        for j in range(ColI):
            # calculate the convolution at each pixel using the kernel specified
            EditedImage[i, j] = np.sum(PaddingResult[i:i + RowK, j:j + ColK] * Kernels)

    return EditedImage

# read the House_1 image
ImageOfHouse1 = cv2.imread('House1.jpg', cv2.IMREAD_GRAYSCALE)

# read the House_2 image
ImageOfHouse2 = cv2.imread('House2.jpg', cv2.IMREAD_GRAYSCALE)

#show the original image of House_1
cv2.imshow('House1 Original Image', ImageOfHouse1)

#show the original image of House_2
cv2.imshow('House2 Original Image', ImageOfHouse2)



#mean filter for images using mean filter or averaginf filter with kernel size 3x3
MeanFilter3BY3 = np.ones((3, 3)) / 9.0


#mean filter for images using mean filter or averaginf filter with kernel size 5x5
MeanFilter5BY5 = np.ones((5, 5)) / 25.0


#result of mean filtering on House_1 and House_2 images by 3x3 and 5x5 kernel size
ResultOfMeanFilter3BY3House_1 = FunctionForPaddingAndConvolve(ImageOfHouse1, MeanFilter3BY3)



ResultOfMeanFilter5BY5House_1 = FunctionForPaddingAndConvolve(ImageOfHouse1, MeanFilter5BY5)

ResultOfMeanFilter3BY3House_2 = FunctionForPaddingAndConvolve(ImageOfHouse2, MeanFilter3BY3)


ResultOfMeanFilter5BY5House_2 = FunctionForPaddingAndConvolve(ImageOfHouse2, MeanFilter5BY5)


#show the results for mean filtering for the images
cv2.imshow('House1 Mean Filter 3x3 Image', ResultOfMeanFilter3BY3House_1)


cv2.imshow('House1 Mean Filter 5x5 Image', ResultOfMeanFilter5BY5House_1)


cv2.imshow('House2 Mean Filter 3x3 Image', ResultOfMeanFilter3BY3House_2)


cv2.imshow('House2 Mean Filter 5x5 Image', ResultOfMeanFilter5BY5House_2)

#save the results for mean filtering for the images
cv2.imwrite('ResultOfMeanFilter3BY3House_1.jpg', ResultOfMeanFilter3BY3House_1)


cv2.imwrite('ResultOfMeanFilter5BY5House_1.jpg', ResultOfMeanFilter5BY5House_1)


cv2.imwrite('ResultOfMeanFilter3BY3House_2.jpg', ResultOfMeanFilter3BY3House_1)


cv2.imwrite('ResultOfMeanFilter5BY5House_2.jpg', ResultOfMeanFilter5BY5House_1)

cv2.waitKey(0)

cv2.destroyAllWindows()



# create an array of parameter values (sigma)
ParameterValues = [1, 2, 3]


# loop on the parameter values and perform convolution with equations give in the question
for Pixel, Parameter in enumerate(ParameterValues):


    # compute Gaussian kernel with equation 2*sigma + 1 using Lambda function  with sigma 1 and 2 and 3
    GaussianK = GaussianLambada(Parameter, 2 * Parameter + 1)


    # apply convolution with padding to House_1 and House_@ using the Gaussian kernel with sigma 1 and 2 and 3
    ResultOfGaussianOfHouse1 = FunctionForPaddingAndConvolve(ImageOfHouse1, GaussianK)



    ResultOfGaussianOfHouse2 = FunctionForPaddingAndConvolve(ImageOfHouse2, GaussianK)

    # show images with applied Gaussian kernel
    cv2.imshow(f'House 1 with Gaussian Kernel with sigma{Parameter}', ResultOfGaussianOfHouse1)



    cv2.imshow(f'House 2 with Gaussian Kernel with sigma{Parameter}', ResultOfGaussianOfHouse2)


    # save images with applied Gaussian kernel
    cv2.imwrite(f'House1WithGaussianKernelWithSigma{Parameter}.jpg', ResultOfGaussianOfHouse1)



    cv2.imwrite(f'House2WithGaussianKernelWithSigma{Parameter}.jpg', ResultOfGaussianOfHouse2)

cv2.waitKey(0)


cv2.destroyAllWindows()



#Matrix for Horizontal Gx
Gx = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]])

#Matrix for Vertical Gy
Gy = np.array([[-1, -2, -1], [0, 0, 0], [1, 2, 1]])


#Apply Convolution with Gx and Gy for each image
ConvolveGxWithImageHouse_1 = FunctionForPaddingAndConvolve(ImageOfHouse1, Gx)

ConvolveGyWithImageHouse_1 = FunctionForPaddingAndConvolve(ImageOfHouse1, Gy)


ConvolveGxWithImageHouse_2 = FunctionForPaddingAndConvolve(ImageOfHouse2, Gx)


ConvolveGyWithImageHouse_2 = FunctionForPaddingAndConvolve(ImageOfHouse2, Gy)

#Matrix for Horizontal Px
Px = np.array([[-1, 0, 1], [-1, 0, 1], [-1, 0, 1]])


#Matrix for Vertical Py
Py = np.array([[-1, -1, -1], [0, 0, 0], [1, 1, 1]])

#Apply Convolution with Px and Py for each image
ConvolvePxWithImageHouse_1 = FunctionForPaddingAndConvolve(ImageOfHouse1, Px)


ConvolvePyWithImageHouse_1 = FunctionForPaddingAndConvolve(ImageOfHouse1, Py)


ConvolvePxWithImageHouse_2 = FunctionForPaddingAndConvolve(ImageOfHouse2, Px)


ConvolvePyWithImageHouse_2 = FunctionForPaddingAndConvolve(ImageOfHouse2, Py)


#calculate magnitude for Sobel filter and Prewitt filter using the square root of the gradients
MagnitudeG_House_1 = np.sqrt(np.power(ConvolveGxWithImageHouse_1 ,2) + np.power(ConvolveGyWithImageHouse_1,2))


MagnitudeG_House_2 = np.sqrt(np.power(ConvolveGxWithImageHouse_2,2) + np.power(ConvolveGyWithImageHouse_2,2))


MagnitudeP_House_1 = np.sqrt(np.power(ConvolvePxWithImageHouse_1,2) + np.power(ConvolvePyWithImageHouse_1,2))


MagnitudeP_House_2 = np.sqrt(np.power(ConvolvePxWithImageHouse_2,2) + np.power(ConvolvePyWithImageHouse_2,2))

#showing the result of the sobel filter and prewitt filters
cv2.imshow('House 1 with Sobel Filter', MagnitudeG_House_1)

cv2.imshow('House 2 with Sobel Filter', MagnitudeG_House_2)

cv2.imshow('House 1 with Prewitt Filter', MagnitudeP_House_1)

cv2.imshow('House 2 with Prewitt Filter', MagnitudeP_House_2)

#saving the result of the sobel filter and prewitt filters
cv2.imwrite('House1WithSobelFilter.jpg', MagnitudeG_House_1)

cv2.imwrite('House2WithSobelFilter.jpg', MagnitudeG_House_2)

cv2.imwrite('House1WithPrewittFilter.jpg', MagnitudeP_House_1)

cv2.imwrite('House2WithPrewittFilter.jpg', MagnitudeP_House_2)

cv2.waitKey(0)


cv2.destroyAllWindows()