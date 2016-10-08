hdmi_routine:
	gcc -o hdmi_switcher ./hdmi_switcher.c 

clear: 
	rm -rf ./*.o

install:
	rm /usr/bin/hdmi_switcher
	ln -s $(shell pwd)/hdmi_switcher /usr/bin/hdmi_switcher
