import gc

print("264 KB SRAM in six independent banks (four 64 KB, two 4 KB)") # RP2040
gc.collect()
print("Alloc memory: {} kB".format(gc.mem_alloc() / 1024) )
print("Free  memory: {} kB".format(gc.mem_free() / 1024) )
print(f'Free  memory: {(gc.mem_free()/1024):.1f} kB')
