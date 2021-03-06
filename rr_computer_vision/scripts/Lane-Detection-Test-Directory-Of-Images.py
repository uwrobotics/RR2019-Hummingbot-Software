import cv2
import numpy as np

import glob, os
import argparse
import sys
from pathlib import Path

def perspective_transform(img):
    img_size = (img.shape[1], img.shape[0])

    # Perspective transform source and destiantion co-ordinates
    # Drag race: (1.5 m wide lanes)
    src = np.float32([[400,350], [760,350], [1280,500], [0,500]])

    # Urban road: (1m wide lanes)
    # src = np.float32([[420,380], [930,380], [1280,720], [50,720]])

    # Circuit race: (2m wide lanes)
    # src = np.float32([[500,350], [750,350], [1280,550], [0,450]])

    dst = np.float32([[300, 0], [900,0], [900,720], [300,720]])

    # Apply transform
    M = cv2.getPerspectiveTransform(src, dst)
    warped = cv2.warpPerspective(img, M, img_size)

    # Contour filter to take out noise and blobs that aren't the lanes
    blob_size = 1000
    filtered = np.zeros(warped.shape, np.uint8)
    contours, _ = cv2.findContours(warped, cv2.RETR_LIST, cv2.CHAIN_APPROX_NONE)
    for cnt in contours:
        if cv2.contourArea(cnt) >= blob_size: 
            cv2.drawContours(filtered, [cnt], -1, 255, -1)
    return filtered

def threshold(img):
    img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    adaptive_thres = cv2.adaptiveThreshold(channel_v, 255, 
                               cv2.ADAPTIVE_THRESH_MEAN_C, 
                               cv2.THRESH_BINARY, 25, -20)
    return adaptive_thres

def main(image_dir):
    # Find all images in the directory
    os.chdir(image_dir)
    types = ("*.jpg", "*.png")
    files_found = []
    for files in types:
        files_found.extend(glob.glob(files))

    # Create output directories if they don't exist
    warp_save_dir = "Warped/"
    thres_save_dir = "Thresholded/"
    if not os.path.exists(warp_save_dir):
        os.makedirs(warp_save_dir)
    if not os.path.exists(thres_save_dir):
        os.makedirs(thres_save_dir)
    
    # Apply lane detection algorithm to each image found
    for file in files_found:
        img = cv2.imread(file)

        # Threshold thege ima and save it 
        thres_save_path = thres_save_dir + os.path.splitext(file)[0] + '_thres.jpg'
        img_thres = threshold(img)
        cv2.imwrite(thres_save_path, img_thres)

        # Warp image and save it 
        warp_save_path = warp_save_dir + os.path.splitext(file)[0] + '_warped.jpg'
        img_warped = perspective_transform(img_thres)
        cv2.imwrite(warp_save_path, img_warped)

if __name__ == "__main__":
    # Get command line arguments
    PARSER = argparse.ArgumentParser()
    PARSER.add_argument('image_dir', type = str, default = None,
                        help = 'path to directory containing images of lanes')
    ARGS = PARSER.parse_args()
    image_dir = Path(ARGS.image_dir)

    # Check to make sure image path exists
    if not image_dir.exists():
        print("Directory %s does not exist!" % image_dir)
        sys.exit(1)

    main(str(image_dir))