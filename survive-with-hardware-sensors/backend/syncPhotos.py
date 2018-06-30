import glob
import time
from pydrive.auth import GoogleAuth
from pydrive.drive import GoogleDrive
import os

def delPhoto(filename):
    os.system('sudo rm -rf '+filename)

def loginToDrive():
    '''
    logins to google drive
    '''
    global gauth, drive
    gauth = GoogleAuth()
    gauth.LoadCredentialsFile("mycreds.txt")
    if gauth.credentials is None:
        gauth.LocalWebserverAuth()
    elif gauth.access_token_expired:
        gauth.Refresh()
    else:
        gauth.Authorize()
    gauth.SaveCredentialsFile("mycreds.txt")

    drive = GoogleDrive(gauth)

def uploadFile(fname):
    '''
    uploads a file to
    google drive
    '''
    nfile = drive.CreateFile({'title':os.path.basename(fname)})
    nfile.SetContentFile(fname)
    nfile.Upload() 

loginToDrive()
while True:
    photoList = glob.glob("checkedPhotos/*.jpg")
    if photoList != []:
        for photo in photoList:
            #try:
                #uploadFile(photo)
                #delPhoto(photo)
            #except:
                #print('Cannot upload/delete '+photo)
            uploadFile(photo)
            print('Uploaded photo from checkedPhotos/' + photo)
            delPhoto(photo)
            print('Deleted photo from checkedPhotos/' + photo)
