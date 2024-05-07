#Q5
import cv2


import numpy as np

#read images using cv2 as
FirstImageWalk_1 = cv2.imread('walk_1.jpg', cv2.IMREAD_GRAYSCALE)

SecondImageWalk_2 = cv2.imread('walk_2.jpg', cv2.IMREAD_GRAYSCALE)


#check if the images exists or not, if not print Error
if FirstImageWalk_1 is None or SecondImageWalk_2 is None:

    print("Error loading images.")

else:
    #if images exists perform and subtraction and reverse subtraction
    # to show the diffrence between two results based on the images and the contnet of the images
    #chech if the images has the same size height and width using shape parameter,
    # if not print error diffrence in dimensions
    if FirstImageWalk_1.shape == SecondImageWalk_2.shape:


        #subtraction between two images using cv2
        result = cv2.subtract(FirstImageWalk_1, SecondImageWalk_2)


        result2 = cv2.subtract(SecondImageWalk_2, FirstImageWalk_1)


        #save and show result of subtraction and reverse subtraction
        cv2.imwrite('SubtractionResult.jpg', result)


        cv2.imwrite('ReversedSubtractionResult.jpg', result2)


        cv2.imshow('Subtraction Result', result)


        cv2.imshow('Inverse Subtraction Result', result2)

        cv2.waitKey(0)


        cv2.destroyAllWindows()

    else:
        #print for not equal sizes of images
        print("Images have different dimensions.")