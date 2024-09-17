import time
import random
import board
import busio
import digitalio
import adafruit_pcd8544 # imports adafruit_framebuf
from adafruit_ina219 import ADCResolution, BusVoltageRange, INA219
import neopixel

pixel_pin = board.NEOPIXEL
# The order of the pixel colors - RGB or GRB. Some NeoPixels have red and green reversed!
# For RGBW NeoPixels, simply change the ORDER to RGBW or GRBW.
ORDER = neopixel.GRB
pixels = neopixel.NeoPixel(pixel_pin, 1, brightness=0.2, auto_write=False, pixel_order=ORDER)
pixels[0] = (24,3,46)
pixels.show()

print("INA219-PCD8544") # on serial

i2c_bus = busio.I2C(board.GP7, board.GP6)  # i2c = busio.I2C(SCL, SDA)
i2c_bus.try_lock()
print("i2c-scan (INA219 = 64):", i2c_bus.scan())
i2c_bus.unlock()

ina219 = INA219(i2c_bus)

# optional : change configuration to use 32 samples averaging for both bus voltage and shunt voltage
ina219.bus_adc_resolution = ADCResolution.ADCRES_12BIT_32S
ina219.shunt_adc_resolution = ADCResolution.ADCRES_12BIT_32S
# optional : change voltage range to 16V
ina219.bus_voltage_range = BusVoltageRange.RANGE_16V

# read actual values
bus_voltage = ina219.bus_voltage  # voltage on V- (load side)
shunt_voltage = ina219.shunt_voltage  # voltage between V+ and V- across the shunt
current = ina219.current  # current in mA
power = ina219.power  # power in watts

# INA219 measure bus voltage on the load side. So PSU voltage = bus_voltage + shunt_voltage
print("Voltage (VIN+) : {:6.3f}   V".format(bus_voltage + shunt_voltage))
print("Voltage (VIN-) : {:6.3f}   V".format(bus_voltage))
print("Shunt Voltage  : {:8.5f} V".format(shunt_voltage))
print("Shunt Current  : {:7.4f}  A".format(current / 1000))
print("Power Calc.    : {:8.5f} W".format(bus_voltage * (current / 1000)))
print("Power Register : {:6.3f}   W".format(power))
print("")


print("init PCD8544")
# Initialize SPI bus and control pins
spi = busio.SPI(clock=board.GP2, MOSI=board.GP3)
dc = digitalio.DigitalInOut(board.GP4)  # data/command
cs = digitalio.DigitalInOut(board.GP1)  # Chip select
reset = digitalio.DigitalInOut(board.GP0)  # reset
backlight = digitalio.DigitalInOut(board.GP5)  # backlight - active low

display = adafruit_pcd8544.PCD8544(spi, dc, cs, reset)

display.bias = 4
display.contrast = 60

# Turn on the Backlight LED
backlight.switch_to_output()
backlight.value = False

#display.fill(0) # Clear the display. (adafruit_framebuf)
#display.show() # Always call show() after changing pixels to make the display update visible!

'''print("Text test")
display.text("**************", 0, 0, 1) # text(string, x, y, color)
display.text("12345678901234", 0, 8, 1)
display.text("14x6 character", 0, 16, 1)
display.text("display with", 0, 24, 1)
display.text("PCD8544", 0, 32, 1)
display.text("**************", 0, 40, 1)
display.show()
'''
'''time.sleep(1)
display.invert = True
time.sleep(1)
display.invert = False
'''
display.fill(0)
display.text("Voltage (VIN+)", 0, 0, 1)
display.text("Shunt Current", 0, 24, 1)

while True:
  pixels[0] = (10,15,25)
  pixels.show()

  bus_voltage = ina219.bus_voltage  # voltage on V- (load side)
  current = ina219.current
  #print("Voltage (VIN+)  {:4.2f}  V".format(bus_voltage + shunt_voltage))
  #print("Shunt Current   {:4.0f} mA".format(current))
  #print(" ",random.randint(10,99)) # alife indicator
  pixels[0] = (0,0,0)
  pixels.show()

  display.text(" {:4.2f} V".format(bus_voltage + shunt_voltage), 0, 8, 1, size=2)
  display.text("{:4.0f} mA".format(current), 0, 32, 1, size=2)
  display.show()

  # clear the values
  display.text(" {:4.2f} V".format(bus_voltage + shunt_voltage), 0, 8, 0, size=2)
  display.text("{:4.0f} mA".format(current), 0, 32, 0, size=2)

  #time.sleep(0.5)
