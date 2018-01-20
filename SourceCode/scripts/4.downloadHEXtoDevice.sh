cd ../build/Exe/
sudo avrdude -c usbasp -p atmega16 -P /dev/ttyACM0 -U flash:w:cube555.hex
