import board
import busio

# docs.circuitpython.org/en/latest/shared-bindings/busio/#busio.I2C
# class busio.I2C(scl: microcontroller.Pin, sda: microcontroller.Pin, *, frequency: int = 100000, timeout: int = 255)¶
#           scl (Pin) – The clock pin
#           sda (Pin) – The data pin
#           frequency (int) – The clock frequency in Hertz
#           timeout (int) – The maximum clock stretching timeut - (used only for bitbangio.I2C; ignored for busio.I2C)

#   i2c = busio.I2C(SCL, SDA)
i2c_bus = busio.I2C(board.GP7, board.GP6) # RP2040
i2c_bus.try_lock()
print("i2c_bus.scan():", i2c_bus.scan())
i2c_bus.unlock()
