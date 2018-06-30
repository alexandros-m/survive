from tkinter import *
import subprocess


def quitApp():
    root.destroy()

class Window(Frame):
    def __init__(self, master=None):
        Frame.__init__(self, master)
        self.master = master
        self.init_window()

    def init_window(self):
        self.master.title("SURVIVE")
        self.pack(fill=BOTH, expand=1)
        t = Label(root,font = ("Ubuntu", 20), text="SURVIVE")
        t.pack()
        t.place(x=10,y=10)        
        self.mainButton = Button(self, text="Start main", font = ("Ubuntu"),command=self.startMain)
        self.mainButton.place(x=10,y=55)
        self.checkButton = Button(self, text="Start checking for plates", font = ("Ubuntu"),command=self.startChecking)
        self.checkButton.place(x=120,y=55)
        self.uploadButton = Button(self, text="Start uploading", font= ("Ubuntu"),command=self.startUploading)
        self.uploadButton.place(x=340, y=55)
    def startChecking(self):
        self.checkingApp = subprocess.Popen('gnome-terminal -x bash -c "cd backend; ./check_for_plates"', shell=True)
        self.checkButton.config(state=DISABLED)
    def startMain(self):
        self.mainApp = subprocess.Popen('gnome-terminal -x bash -c "cd backend; sudo ./main_pressure"', shell=True)
        self.mainButton.config(state=DISABLED)
    def startUploading(self):
        self.uploadApp = subprocess.Popen('gnome-terminal -x bash -c "cd backend; python3 syncPhotos.py"', shell=True)
        self.uploadButton.config(state=DISABLED)

root = Tk()
root.geometry("500x150")
app = Window(root)
root.title('SURVIVE')
root.resizable(width=False, height=False)
#icon = PhotoImage(file='data_and_docs/icon.ico')
#root.tk.call('wm', 'iconphoto', root._w, icon)
root.protocol('WM_DELETE_WINDOW', quitApp)
root.mainloop()
