################################################################################
#
# File: PiSatZ_disp.py
#
#   Purpose: drives the PiSat Zero's onboard OLED display to provide user 
#               with info reguarding PiSat status 
#
#   OWNER: Daniel McGann    Project: ACCESS
#
################################################################################

#IMPORTS
import time


import Adafruit_GPIO.SPI as SPI
import Adafruit_SSD1306

from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont

import sys
import socket
import fcntl
import struct

#Raspberry pi GPIO configurations
#   Numbers are RPi GPIO numbers (not physical numbers or Wiring Pi Numbers)
RST = 17
DC = 27
SCLK = 11
DATA = 10
CS = 8

SPI_PORT = 0
SPI_DEVICE = 0

# Initialize the display
disp = Adafruit_SSD1306.SSD1306_128_64(
            rst=RST, 
            dc=DC,
            sclk=SCLK, 
            din=DATA, 
            cs=CS)

# Initialize the Library 
disp.begin()

# Clear the display 
disp.clear()
disp.display()


# Create a new Image to display
width = disp.width
height = disp.height
image = Image.new('1', (width, height))

# Get drawing object to draw on image
draw = ImageDraw.Draw(image)

top = 2
bot = height - top

font1 = ImageFont.load_default()

################################################################
# CUSTOMIZE THIS LINE FOR SAT SPECIFIC INFORMATION

draw.text((2, top), "PiSatZ: 0000   ~ name ~", font=font1, fill=255)

################################################################

# Grabbing IP Address
iface = 'eth0'
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
IP = socket.inet_ntoa(fcntl.ioctl(
        s.fileno(),
        0x8915,
        struct.pack('256s', iface[:15]))[20:24])

draw.text((2, top+50), IP, font=font1, fill=255)


# Draw Sat Pic

rx = 90
ry = 35
rh = 20
rw = 20

# body
draw.rectangle((rx, ry, rx+rw , ry+rh), outline=255, fill=0)

# Solar Panels
draw.line((rx-5, ry+(rh/2), rx, ry+(rh/2)), fill=255)
draw.line((rx+rw+5, ry+(rh/2), rx+rw, ry+(rh/2)), fill=255)

draw.rectangle( (rx-15, ry, rx-5, ry+rh), outline=255, fill=255)
draw.rectangle( (rx+rw+15, ry, rx+rw+5, ry+rh), outline=255, fill=255)

# Dish
draw.polygon( (rx,ry-10, rx+2,ry-5, rx+7,ry, rx+13,ry, rx+18,ry-5, rx+20,ry-10), outline=255, fill=0)


# Display the image
disp.image(image)
disp.display()
