from tkinter import *
import tkinter as tk
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg, NavigationToolbar2Tk)
from matplotlib.backend_bases import key_press_handler
import numpy as np
import os
import sys
from time import strftime
from time import gmtime

# VERSION : 03/03/2023

SIMULATION_DELAY=500 # Delay between two simulation ticks (1000=1s)
REAL_DELAY_VALUE=1250 # Real delay between 2 ticks (seconds)
FIGURE_SIZE=10 # nb points

def is_number(s):
	try:
		float(s)
		return True
	except ValueError:
		return False

class myapp:
		def __init__(self, rootw):
			self.init_variables()
			self.rootw = rootw
			self.init_lpanel()
			self.init_rpanel()

		def init_variables(self):
			self.flg_start_stop = False; # false:stop  true:start
			self.ctime        = 0        # Time steps counter
			self.arrTime      = []
			self.arrTint      = []
			self.arrText      = []
			self.cbvar        = IntVar()
			self.datafilename         = "data.txt"
			self.consignefilename     = "consigne.txt"
			self.lockfilenamedata     = ".verrouData"
			self.lockfilenameconsigne = ".verrouConsigne"
			#Read data.txt and init variables with read values
			try:
				fp = open(self.datafilename,"r")
			except IOError:
				print("Can not open ", self.datafilename, " file for initialization! Program stopped");
				sys.exit()
			arrtxt = fp.read().split('\n')
			self.oText = 0.0
			if is_number(arrtxt[0]):
				self.oText = float(arrtxt[0])
			self.oTint = 0.0
			if is_number(arrtxt[1]):
				self.oTint = float(arrtxt[1])
			fp.close()		
			
		def init_lpanel(self):
			self.lpanel = tk.Frame(master=self.rootw, width=200, height=500 )
			self.lpanel.pack(fill=tk.BOTH, side=tk.LEFT, expand=False)
			self.lpanel.columnconfigure(0, minsize=100)
			self.lpanel.columnconfigure(1, minsize=50)
			self.lpanel.rowconfigure([0, 1], minsize=100)
			crow=0
			# ISEN text
			tk.Label(self.lpanel, text="ISEN Yncrea (c)").grid(column=0, row=crow)
			crow += 1
			# ---
			tk.Label(self.lpanel, text="---").grid(column=0, row=crow)
			crow += 1
			# Check button FREEZE
			tk.Checkbutton(self.lpanel, text='FREEZE', variable=self.cbvar, onvalue=1, offvalue=0, command=self.cb_selection).grid(column=0, row=crow)
			crow += 1
			# ---
			tk.Label(self.lpanel, text="---").grid(column=0, row=crow)
			crow += 1
			# Text HEAT/STOP
			self.str_Theat = tk.StringVar(self.lpanel, "OFF")
			tk.Label(self.lpanel, text="HEATER :").grid(column=0, row=crow)
			tk.Label(self.lpanel, textvariable=self.str_Theat).grid(column=1, row=crow)
			crow += 1
			# ---
			tk.Label(self.lpanel, text="---").grid(column=0, row=crow)
			crow += 1
			# Text 1
			self.str_Text = StringVar(self.lpanel,self.oText)
			tk.Label(self.lpanel, text="Text(°C) :").grid(column=0, row=crow)
			tk.Label(self.lpanel, textvariable=self.str_Text).grid(column=1, row=crow)
			crow += 1
			# Text 2
			self.str_Tint = tk.StringVar(self.lpanel, self.oTint)
			tk.Label(self.lpanel, text="Tint(°C) :").grid(column=0, row=crow)
			tk.Label(self.lpanel, textvariable=self.str_Tint).grid(column=1, row=crow)
			crow += 1
			# Text 3
			self.str_Thermostat = StringVar(self.lpanel,"--")
			tk.Label(self.lpanel, text="Thermostat(°C) :").grid(column=0, row=crow)
			tk.Label(self.lpanel, textvariable=self.str_Thermostat).grid(column=1, row=crow)
			crow += 1
			# ---
			tk.Label(self.lpanel, text="---").grid(column=0, row=crow)
			crow += 1
			# Button Start
			tk.Label(self.lpanel, text="Press to:").grid(column=0, row=crow)
			self.btn_StartStop_txt = StringVar(self.lpanel,"START")
			self.btn_StartStop = Button(self.lpanel, textvariable=self.btn_StartStop_txt, command=self.startstopit, bg="black", fg="white").grid(column=1, row=crow)
			crow += 1
			# ---
			tk.Label(self.lpanel, text="---").grid(column=0, row=crow)
			crow += 1
			# slidebar
			self.str_Thermostat = tk.StringVar( self.lpanel, self.oText );
			self.slidebar = tk.Scale( self.lpanel, variable=self.str_Thermostat, from_=30, to=0, resolution=0.5, length=200,tickinterval=5, orient=VERTICAL, command=self.cbk_thermostat).grid(column=0, row=crow)
			crow += 1
			# exit button
			tk.Label(self.lpanel, text="Press to:").grid(column=0, row=crow)
			Button(self.lpanel, text="Exit", command=sys.exit).grid(column=1, row=crow)
			
		def cb_selection(self):
			if self.cbvar.get() == 1:
				fp = open( self.lockfilenamedata, "x" )
				fp.close()
				fp = open( self.lockfilenameconsigne, "x" )
				fp.close()
				print( "Freeze on (set lock file ", self.lockfilenamedata, " and ", self.lockfilenameconsigne, ")" );
			else:
				if os.path.exists(self.lockfilenamedata):
					os.remove( self.lockfilenamedata )
				if os.path.exists(self.lockfilenameconsigne):
					os.remove( self.lockfilenameconsigne )
					print( "Freeze off (remove lock files)" );
		
		def init_rpanel(self):
			self.rpanel = tk.Frame(master=self.rootw, width=600, height=400, bg="white", )
			self.rpanel.pack(fill=tk.BOTH, side=tk.LEFT, expand=True)
			# here we are creating sub plots
			self.fig, self.ax = plt.subplots()
			plt.xlabel("Time")
			plt.ylabel("Temperature (°C)")
			self.ax.set_ylim([0,30])
			self.canvas = FigureCanvasTkAgg(self.fig, master=self.rpanel)
			self.canvas.get_tk_widget().pack(side=TOP, fill=BOTH, expand=True)
			
		def startstopit(self):
			if self.flg_start_stop == False:
				self.flg_start_stop = True
				self.btn_StartStop_txt.set( "STOP" )
				self.rootw.after(SIMULATION_DELAY, self.cbk_refresh)
			else:
				self.flg_start_stop = False
				self.btn_StartStop_txt.set( "START" )

		def cbk_refresh(self): # Called each time
			# Check lock file and add previous value for the plot if it exists
			try:
				fp = open( self.lockfilenamedata, "x" )
			except IOError:
				Tint = self.oTint
				Text = self.oText
				print("Lock file ", self.lockfilenamedata, " exists. Use previous values for plotting Text and Tint")
			else:
				# Create lock file for data
				fp.close()
				# Read data.txt
				try:
					fp = open(self.datafilename,"r")
				except IOError:
					print("Can not open ", self.datafilename, " file. Use previous values for plotting Text and Tint.")
					Tint = self.oTint
					Text = self.oText
				else:
					arrtxt = fp.read().split('\n')
					arrtxt = arrtxt[:3]
					arrtxt.reverse()
					Text = 0.0
					if is_number(arrtxt[0]):
						Text = float(arrtxt[0])
					Tint = 0.0
					if is_number(arrtxt[1]):
						Tint = float(arrtxt[1])
					if( arrtxt[2]=='false' ):
						self.str_Theat.set( "OFF" )
					elif ( arrtxt[2]=='true' ):
						self.str_Theat.set( "ON" )
					else :
						print("Erreur lecture temoin dans data.txt")
					fp.close()
				# Close lock file
				if os.path.exists(self.lockfilenamedata):
					os.remove( self.lockfilenamedata )
			self.ctime = self.ctime+1
			print( "Read ", self.datafilename, " with Tint:",Tint,"°C and Text:", Text, "°C at time ", strftime("%H:%M:%S", gmtime(self.ctime*REAL_DELAY_VALUE)))
			self.str_Tint.set(Tint)
			self.str_Text.set(Text)
			# Refresh figure
			#self.arrTime.append( self.ctime*SIMULATION_DELAY/1000 )
			self.arrTime.append( strftime("%H:%M:%S", gmtime(self.ctime*REAL_DELAY_VALUE)) )
			self.arrTint.append( Tint )
			self.oTint = Tint
			self.arrText.append( Text )
			self.oText = Text
			if len(self.arrTime)>FIGURE_SIZE:
				self.arrTime.pop(0)
				self.arrTint.pop(0)
				self.arrText.pop(0)
			self.ax.cla()
			self.ax.plot(np.array(self.arrTime), np.array(self.arrTint), label="Tint")
			self.ax.plot(np.array(self.arrTime), np.array(self.arrText), label="Text")
			self.ax.set_ylim([0,30])
			plt.xlabel("Time")
			plt.ylabel("Temperature (°C)")
			plt.legend()
			self.fig.canvas.draw()
			self.fig.canvas.flush_events()
			# Start a new tick
			if self.flg_start_stop:
				self.rootw.after(SIMULATION_DELAY, self.cbk_refresh)
			
		def cbk_thermostat(self, value): # Called when slider has been modified
			print( "Write ", self.consignefilename, " with ",self.str_Thermostat.get(),"°C at time ", strftime("%H:%M:%S", gmtime(self.ctime*REAL_DELAY_VALUE)))
			# Check lock file and add previous value for the plot if it exists
			try:
				fp = open( self.lockfilenameconsigne, "x" )
			except IOError:
				print(self.consignefilename, " is locked (by ", self.lockfilenameconsigne, "). ", self.consignefilename, " is not written.")
			else:
				# Lock consigne
				fp.close()
				# Write consigne
				try:
					fp = open(self.consignefilename,"w")
				except IOError:
					print("Can not open ", self.consignefilename, " file. It is not written.")
				else:
					fp.write(self.str_Thermostat.get())
					fp.close()
				# Unlock consigne
				os.remove( self.lockfilenameconsigne )
			
if __name__ == "__main__":
	rootw = tk.Tk()
	rootw.title("Temperature control")
	app = myapp( rootw )
	rootw.mainloop()
	
