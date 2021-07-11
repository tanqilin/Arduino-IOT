import cv2
im=cv2.imread('/home/pi/Pictures/凌.jpeg')
cv2.imshow('凌',im)
cv2.waitKey(0)
cv2.destroyAllWindows()