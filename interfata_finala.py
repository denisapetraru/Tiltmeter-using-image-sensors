# -*- coding: utf-8 -*-
"""
Created on Tue Jun 11 16:04:39 2024

@author: Denisa
"""

import customtkinter as ctk
from tkinter import messagebox
from tkinter import ttk
import serial
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from scipy.signal import savgol_filter
import numpy as np
import time

# Initialize serial connection
ser = serial.Serial('COM9', baudrate=9600, timeout=2)
time.sleep(0.2)

# Global variable to store the data
y_final = None
result_sav = None  # Initialize result_sav globally

# Function to send 'm' to Arduino
def start_reading_x():
    start_char = 'm'
    ser.write(start_char.encode())  # Send the start character
    read_data()
    messagebox.showinfo("Info", "Started reading x axis")
    
def start_reading_y():
    start_char = 'n'
    ser.write(start_char.encode())  # Send the start character
    read_data()
    messagebox.showinfo("Info", "Started reading y axis")

def read_data():
    global y_final
    global result_sav #filtered result
    y = ser.readline()
    print("number of characters received", len(y))
    print("Output type: ", type(y))
    print(f"Received non-numeric data: {y}")
    
    # Decode the bytes object to string and strip any leading/trailing whitespace
    y_str = y.decode('utf-8').strip()
    print("Output type: ", type(y_str))
    
    # Convert the string into a list of characters
    y_list = list(y_str)
    print("Output type: ", type(y_list))
    
    # Convert the list of characters to a list of integers (0s and 1s)
    y_final = np.array([int(char) for char in y_list])
    print("Output type: ", type(y_final))
    print("Final result", y_final)
    
    # Calculate filtered data
    result_sav = savgol_filter(y_final, 40, 2)
    print("Filtered data:", result_sav)
    
    #calculate_position()

# Function to plot original data
def plot_original():
    
    if y_final is None:
        messagebox.showerror("Error", "No data to plot. Press 'Start' first.")
        return

    fig, ax = plt.subplots()
    ax.plot(y_final)
    ax.set_title("Original")
    ax.set_ylim(-0.5, 1.5)  # Ensure 0 is below 1 on the y-axis
    
    # Embed the plot in the tkinter window
    canvas = FigureCanvasTkAgg(fig, master=plot_frame)
    canvas.draw()
    canvas.get_tk_widget().pack(side=ctk.TOP, fill=ctk.BOTH, expand=1)

# Function to plot filtered data
def plot_filtered():
    if y_final is None:
        messagebox.showerror("Error", "No data to plot. Press 'Start' first.")
        return

    fig, ax = plt.subplots()
    ax.plot(result_sav)
    ax.set_title("Filtered")
    ax.set_ylim(-0.5, 1.5)  # Ensure 0 is below 1 on the y-axis
    
    # Embed the plot in the tkinter window
    canvas = FigureCanvasTkAgg(fig, master=plot_frame)
    canvas.draw()
    canvas.get_tk_widget().pack(side=ctk.TOP, fill=ctk.BOTH, expand=1)
    

def calculate_position():
    if result_sav is not None:  # Check if result_sav has been initialized
        interval_start = 0
        interval_stop = 0
        interval_length = 0
        result = 0
        i = 0

        while i < 1520: 
            if abs(result_sav[i] - 1) > 0.05:
                interval_start = i
                while abs(result_sav[i] - 1) > 0.1 and i < 1535:
                    i = i + 1
                interval_stop = i
                if interval_stop - interval_start > interval_length:
                    interval_length = interval_stop - interval_start + 1
                    result = (interval_length / 2 + interval_start) * 0.0635
            i = i + 1
        messagebox.showinfo("Info", "The position is: " + str(result) + " mm")  # Convert result to string
            
        

# Function to close the serial connection and exit
def on_closing():
    ser.close()
    root.destroy()

# Create the main window
root = ctk.CTk()
root.title("Data Reader")

# Add buttons
start_button_x = ctk.CTkButton(root, text="Start x", command=start_reading_x)
start_button_x.pack(pady=10)
start_button_y = ctk.CTkButton(root, text="Start y", command=start_reading_y)
start_button_y.pack(pady=10)

original_button = ctk.CTkButton(root, text="Original", command=plot_original)
original_button.pack(pady=10)

filtered_button = ctk.CTkButton(root, text="Filtered", command=plot_filtered)
filtered_button.pack(pady=10)

position_button = ctk.CTkButton(root, text="Position", command=calculate_position)
position_button.pack(pady=10)

# Create a frame to hold the plot
plot_frame = ctk.CTkFrame(root)
plot_frame.pack(fill=ctk.BOTH, expand=1)


# Bind the close event
root.protocol("WM_DELETE_WINDOW", on_closing)

# Start the GUI event loop
root.mainloop()
