''' ----- Serial EEPROM Programmer -----
	This program takes a binary file and writes it to an EEPROM using an Arduino 
	and serial protocol

	The program first reades in a binary file and unpacks the binary data
	Then, the program writes the data in bytes to the Arduino, which in turn
	programs the EEPROM

	Author: Brandon Michelsen
	Date: 2/22/2019
'''

# Import necessary modules
import time
import re
import sys
import struct
import serial
import serial.tools.list_ports

# Create a serial object that times out after 1 second of waiting
# (The rest of the settings are set up in the init_serial function)
ser = serial.Serial(timeout=5)

# Initiates the serial connection on the proper COM port
# with the given baud rate
def init_serial(baud_rate):
	ser.baudrate = baud_rate # Set the baud rate

	# View all the active ports
	ports = list(serial.tools.list_ports.comports())

	# Check each port and only connect to one with the "Arduino name"
	for p in ports:
		if "Arduino" in p[1]:
			print("Arduino found on port", p[0])
			ser.port = p[0]
			break
		else:
			print("No Arduino found")

# Sends a byte over the serial connection
def send_command(addr, data, rw):
	# If there is no connected COM port, do not send data
	if ser.port == None:
		print("Serial port is not connected.")
	# Send data once the serial port is open
	if ser.is_open == True and ser.port != None:
		if rw == 'w':
			cmd = "w {:04X} {:02X}".format(addr, data) # Format the command to write to the EEPROM
			ser.write(cmd.encode()) # Send the write command
			time.sleep(2) # Wait
		if rw == 'r':
			cmd = "r {:04X}".format(addr) # Format the command to read from EEPROM
			ser.write(cmd.encode()) # Send the command
			time.sleep(2) # Wait
			c = (ser.read_until('\n', 32)).decode() # Read in the data
			n = [int(x) for x in re.findall('\d+', c)] # Convert the returned string values to numbers
			print("{:04X}: {:02X}".format(n[0], n[1])) # Print the data

# Reads binary data as hexadecimal from a binary file
def read_binary():
	file_name = sys.argv[1] # Name of the binary file given as cmd line argument

	with open(file_name, 'rb') as file:
		data = file.read() # Read the file
		file.close() # Close the file
		length = len(data) # Get the length of the file
		out_form = "<"+str(length)+"B"
		binary = struct.unpack(out_form, data) # Unpack the binary data
		return binary

# Function to print out usage data
def print_help():
	print("A tool for loading 6502 binary file data onto an EEPROM.")
	print("\nUsage:\npython Binary_to_EEPROM.py <binary-file> <starting-address>\n")
	print("Example:\npython Binary_to_EEPROM.py binary-file.bin E000\n")
	print("Options:\n--help\t\t\tShows this screen")
	print("<binary-file>\t\tThe binary file to load onto the EEPROM")
	print("<starting-address>\tThe address where the program is to be loaded in memory")
	print("\nPlease note, this program works best with the AT28C64A series of EEPROM chips.")

def main():
	# If the user enters the help option, show the help menu
	if (sys.argv[1] == "--help"):
		print_help()
	# Ensure the user entered the proper number of arguments
	elif (len(sys.argv) < 3):
		print("You do not have enough arguments. Please look at the documentation.")
	else:
		# Initiate the serial port
		init_serial(9600)
		# If the serial port is not open, open it
		if ser.is_open == False and ser.port != None:
			ser.open() # Open the serial port
			time.sleep(1) # Wait 1 second before going further
			print("\nReading binary file...")
			data = read_binary() # Read the binary file

			# Get the reset vector address from the command line
			reset_high = int(sys.argv[2][0:2], 16)
			reset_low = int(sys.argv[2][2:], 16)
			
			# Set the reset vector in memory
			print("\nWriting to the reset vector...")
			send_command(0x1FFC, reset_low, 'w')
			send_command(0x1FFD, reset_high, 'w')

			# Get the starting address of memory
			start_addr = int(sys.argv[2], 16)
			addr = start_addr & 0x1FFF

			# Begin sending the binary file data
			print("\nSending binary file data...")
			for d in data:
				send_command(addr, d, 'w')
				addr += 1

			# Read back binary file data
			addr = start_addr & 0x1FFF
			print("\nData in EEPROM:")
			for d in data:
				send_command(addr, 0x00, 'r')
				addr += 1

			print("\nReset Vector:")
			send_command(0x1FFC, 0x00, 'r')
			send_command(0x1FFD, 0x00, 'r')

			print("\nFinished.")

			ser.close() # Close the serial port
		else:
			print("Serial port could not be found")

if __name__ == "__main__":
	main()