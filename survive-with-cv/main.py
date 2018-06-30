import cv2
from timeit import default_timer
#Image 1 aoe
#24,227,295,345

car_cascade = cv2.CascadeClassifier('cascades/cars.xml')
counter = 0
cap = cv2.VideoCapture(0)
font = cv2.FONT_HERSHEY_SIMPLEX

def takePhoto(name, times=1):
    '''
    takes a photo and saves it to jpeg
    takes a name as a parameter and the times var
    states how many photos you would like taken
    '''
    cam = cv2.VideoCapture(0)
    for i in range(times):
        dimg = cam.read()[1]
        cv2.imwrite(name + '.jpg', dimg)
    cam.release()

def checkForExit():
    '''
    if an opencv window is open, it waits till the user
    preses ESC, then it closes.
    It is essential to use this if you open an opencv window
    (e.g. after imshow())
    '''
    k = cv2.waitKey(30) & 0xff
    if k == 27:
        exit()

class Image(object):
    '''
    class for more beatiful code 
    concerning images.
    Doesn't have to do something with the ocv
    image class, since this is just for better 
    understanding of the code.
    I just put my methods here, to make the code more compact
    '''
    def __init__(self, path):
        self.path = path
        self.img = cv2.imread(path) 
        self.dimensions = self.img.shape[:2]
        self.height = self.dimensions[0]
        self.width = self.dimensions[1]
    
    def paste(self,img1,img2, x, y):
        '''
        pastes an image to another image at x,y
        '''
        self.img[y:y+img2.shape[0], x:x+img2.shape[1]] = img2
        
    def checkForCar(self, x, y, w, h):
        '''
        checks image for cars, inside the
        dimensions that are passed as parameters
        '''
        img = self.img
        croppedImg = self.crop(x,y,w,h)
        grayImg = cv2.cvtColor(croppedImg, cv2.COLOR_BGR2GRAY)
        objects = car_cascade.detectMultiScale(grayImg, 1.01, 5)
        #objects = car_cascade.detectMultiScale(grayImg, 1.05, 7)
        for (ox,oy,ow,oh) in objects:
            cv2.rectangle(croppedImg,(ox,oy),(ox+ow,oy+oh),(255,0,0),2)
            #cv2.putText(croppedImg,'Car',(ox,oy+25), font, 1,(0,0,255),2,cv2.LINE_AA)
            self.paste(img,croppedImg,x,y)
        self.img = img
        return img
        #return img
    
    def crop(self, x, y, w, h):
        croppedImg = self.img[y:h, x:w]
        return croppedImg

def checkPerformace():
    '''
    helper function to check the performance
    0.02 secs average is good enough
    '''
    durationList = []
    for i in range(600):
        start = default_timer()

        img = Image('test_photos/1photo1.jpg')
        cv2.imwrite('test_photos/3photo1_detected.jpg', img.checkForCar(24,227,295,345))

        duration = default_timer() - start
        durationList.append(duration)
    print(float(sum(durationList)) / len(durationList))

#checkPerformace()
img = Image('test_photos/1photo1.jpg')
cv2.imwrite('test_photos/3photo1_detected.jpg', img.checkForCar(24,227,295,345))