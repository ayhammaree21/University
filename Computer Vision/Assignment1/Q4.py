#Q4
import cv2


import numpy as np


import matplotlib.pyplot as plt


# read the image
OriginalImage = cv2.imread('Q_4.jpg', cv2.IMREAD_GRAYSCALE)



# apply Sobel gradients for horizontal Gx and vertical Gy with kernel size 3x3
Gx = cv2.Sobel(OriginalImage, cv2.CV_64F, 1, 0, ksize=3)



Gy = cv2.Sobel(OriginalImage, cv2.CV_64F, 0, 1, ksize=3)



# compute the gradient magnitude using square root
NormalMagnitude = np.sqrt(Gx ** 2 + Gy ** 2)


# stretch the magnitude for better visualization
StretchedMagnitude = cv2.normalize(NormalMagnitude, None, 0, 255, cv2.NORM_MINMAX)


# convert magnitude to 8-bit data type
StretchedMagnitude = np.uint8(StretchedMagnitude)



# compute the histogram of the stretched gradient magnitude
HistogramOfStretchedMagnitude = cv2.calcHist([StretchedMagnitude], [0], None, [256], [0, 256])


# compute the histogram of the normal gradient magnitude
HistogramOfNormalMagnitude = cv2.calcHist([np.uint8(NormalMagnitude)], [0], None, [256], [0, 256])


# compute the gradient orientation 
GradientOrientation = np.arctan2(Gy, Gx)


# normalize the orientation to the range [0, 360]
OrientationNormalization = (GradientOrientation + 2 * np.pi) % (2 * np.pi)


# flat the array before computing the histogram
FlatToHistogramForOrientation = OrientationNormalization.flatten()


# ompute the histogram of the gradient orientation
HistogramOfGradientOrientation = cv2.calcHist([np.float32(FlatToHistogramForOrientation)], [0], None, [256],
                                              [0, 2 * np.pi])

# save images resulted using cv2
cv2.imshow('QuestionNormalizedGradientOrientation.jpg', OrientationNormalization)

cv2.imwrite('Question4GradientMagnitude.jpg', NormalMagnitude)

cv2.imwrite('Question4StretchedGradientMagnitude.jpg', StretchedMagnitude)

cv2.waitKey(0)

cv2.destroyAllWindows()

# create subplots to show original image, gradient magnitude, and stretched gradient magnitude
plt.figure(figsize=(15, 5))

# Original Image
plt.subplot(1, 3, 1)

plt.imshow(OriginalImage, cmap='gray')

plt.title('Original Image')

# Gradient Magnitude image
plt.subplot(1, 3, 2)

plt.imshow(NormalMagnitude, cmap='gray')

plt.title('Gradient Magnitude')

# Stretched Gradient Magnitude image
plt.subplot(1, 3, 3)

plt.imshow(StretchedMagnitude, cmap='gray')

plt.title('Stretched Gradient Magnitude')



plt.show()


# create subplots for histograms
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 7))

# plot the histogram of the stretched gradient magnitude
ax1.plot(HistogramOfStretchedMagnitude, color='red', label='Stretched Magnitude')

ax1.set_title('Histogram of Stretched Gradient Magnitude')

ax1.set_xlabel('Pixel Value')

ax1.set_ylabel('Frequency')

ax1.legend()

# plot the histogram of the normal gradient magnitude
ax2.plot(HistogramOfNormalMagnitude, color='blue', label='Normal Magnitude')

ax2.set_title('Histogram of Normal Gradient Magnitude')

ax2.set_xlabel('Pixel Value')

ax2.set_ylabel('Frequency')

ax2.legend()

plt.show()

# show the histogram of gradient orientation in radians
plt.figure(figsize=(8, 4))

bin_edges = np.linspace(0, 2 * np.pi, 257)

plt.plot(bin_edges[:-1], HistogramOfGradientOrientation.flatten(), color='green', label='Orientation')

plt.title('Histogram of Gradient Orientation (Radians)')

plt.xlabel('Orientation (radians)')

plt.ylabel('Frequency')

plt.legend()

plt.show()