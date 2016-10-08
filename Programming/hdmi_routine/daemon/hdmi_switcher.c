#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <fcntl.h>
#include <limits.h>
#include <syslog.h>
#include <stdint.h>
#include <sys/wait.h>

#define RX 1
#define TX 2

/*
 * Change constants and enjoy yourself
 */
const char* pCommonFIFO        = "/tmp/hdmi.pipe";
const int   BufferSize         = 1;
char        HDMI_STATUS_PATH[] = "/sys/class/drm/card0-HDMI-A-1/status"; 
char        HDMI_CONF_PATH[]   = "/home/medos/.config/hdmi_switcher/mode"; 
char        LID_STATUS_PATH[]  = "/proc/acpi/button/lid/LID0/state"; 
char        INTERNAL_SCRIPT[]  = "/home/medos/.config/hdmi_switcher/internal.sh";
char        SYSTEMD_CTL[]      = "/usr/bin/systemctl"; 
char        EXTERNAL_SCRIPT[]  = "/home/medos/.config/hdmi_switcher/external.sh";
char        EXPAND_SCRIPT[]    = "/home/medos/.config/hdmi_switcher/expand.sh";
char        BACKLIGHT_SCRIPT[] = "/home/medos/.config/hdmi_switcher/backlight.sh";
char        FEH_PATH[]         = "/usr/bin/feh";
char        BG_PICTURE[]       = "/home/medos/Pictures/linux-tux-luv-love.jpg";



int OptHandler (int argc, char* argv[], char** FileName);
int ReceiverModule(void);
int TransmitterModule(char* FileName);

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

int main (int argc, char* argv[]) {

	char* FileName = NULL;
	int WorkMode = 0;

	if ( (WorkMode = OptHandler(argc, argv, &FileName)) == -1 ) 
		exit(EXIT_FAILURE);

	if ( WorkMode == RX )
		if (ReceiverModule() == -1 )
			exit(EXIT_FAILURE);

	if ( WorkMode == TX ) 
		if (TransmitterModule(FileName) == -1 ) 
			exit(EXIT_FAILURE);
	
	exit(EXIT_SUCCESS);
}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

/*
 * Print help and extended help
 */
static void usage(unsigned status) 
{
        if (status != EXIT_SUCCESS) 
                fprintf(stderr, "Try 'hdmi_switcher -help' for more"
                                " information.\n");
        else {
                printf("Usage: hdmi_switcher OPTIONS \n");
                printf("\n"
                       "-r    Receiver mode                       \n"
                       "-t    Tramsmitter mode                    \n"
                       "                                          \n"
                       "The string in ~/.config/hdmi_switcher/mode\n"
                       "characterizes the work mode of program:   \n"
                       "\"expand\" - two desktops                 \n"
                       "\"switch\" - external desktop           \n\n"
                       "Author: Edgar Kaziahmedov edos@linux.com  \n");
        }
        exit(status);
}

int OptHandler (int argc, char* argv[], char** FileName) {
	int opt = 0;
	opterr = 0;
	int WorkMode = 0;
	while ( (opt = getopt(argc, argv, "rth")) != -1 ) 
	{
		switch ( opt ) 
		{
		case 'h': {
                        usage(EXIT_SUCCESS);
			break;
		}
		case 'r': { 
			if ( WorkMode == 0 ) 
				WorkMode = RX;
			else {
                                usage(EXIT_FAILURE);
			   	return -1;	
			}
			break;
		}
		case 't': { 
			if ( WorkMode == 0 ) 
				WorkMode = TX;
			else {
                                usage(EXIT_FAILURE);
			   	return -1;	
			}
			break;
		}
		default: {
			fprintf(stderr, "Unrecognized command line option.\n");
			return -1;
		}			
		}
	}
	if ( WorkMode == 0 ) {
                usage(EXIT_FAILURE);
		return -1;
	}
	return WorkMode;
}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
int getHDMIstatus(void) {
        int hdmi_st = open(HDMI_STATUS_PATH, O_RDONLY);
        char status = '\0';
        lseek(hdmi_st, 0, SEEK_SET);
        read(hdmi_st, &status, 1);
        close(hdmi_st);
        return (status == 'c') ? 1 : 0;
}

int getExpandMode(void) {
        char mode = '\0';
        int config = open(HDMI_CONF_PATH, O_RDONLY);
        read(config, &mode, 1);
        close(config);
        if (mode == 's') 
                return 0;
        if (mode == 'e') 
                return 1;
        return -1;
}



int setInternalMonitor(void) {
        int lid_d = open(LID_STATUS_PATH, O_RDONLY);
        char lid_c = '\0';
        lseek(lid_d, 12, SEEK_CUR);
        read(lid_d, &lid_c, 1);
        
        int child = 0;
        int code = 1;
        while (code != 0) {
                child = fork();
                if (child == 0) {
                        execl(INTERNAL_SCRIPT, INTERNAL_SCRIPT, NULL);
                        exit(EXIT_FAILURE);
                } else {
                        waitpid(child, &code, 0);
                }
        }               
        if (lid_c != 'o') {
                child = fork();
                if (child == 0) {
                        execl(SYSTEMD_CTL, SYSTEMD_CTL, "suspend", NULL);
                        exit(EXIT_FAILURE);
                } else {
                        waitpid(child, &code, 0);
                }
        }
        return 0;
}

int setExternalMonitor(void) {
        int child = 0;
        int code = 1;
        while (code != 0) {
                child = fork();
                if (child == 0) {
                        execl(EXTERNAL_SCRIPT, EXTERNAL_SCRIPT, NULL);
                        exit(EXIT_FAILURE);
                } else {
                        waitpid(child, &code, 0);
                }
        }                
        return 0;
}
       
int setExpandMonitor(void) {
        int child = 0;
        int code = 1;
        while (code != 0) {
                child = fork();
                if (child == 0) {
                        execl(EXPAND_SCRIPT, EXPAND_SCRIPT, NULL);
                        exit(EXIT_FAILURE);
                } else {
                        waitpid(child, &code, 0);
                }
        }
        return 0;
}

int update_desktop() {
        int child = 0;
        int code = 1;
        while (code != 0) {
                child = fork();
                if (child == 0) {
                        execl(FEH_PATH, FEH_PATH, "--bg-scale", BG_PICTURE, NULL);
                        exit(EXIT_FAILURE);
                } else {
                        waitpid(child, &code, 0);
                }
        }
        return 0;
}

int set_backlight() {
        int child = 0;
        int code = 1;
        while (code != 0) {
                child = fork();
                if (child == 0) {
                        execl(BACKLIGHT_SCRIPT, BACKLIGHT_SCRIPT, NULL);
                        exit(EXIT_FAILURE);
                } else {
                        waitpid(child, &code, 0);
                }
        }
        return 0;
}

int ReceiverModule(void){
	//Create common fifo or use exist
	mkfifo( pCommonFIFO, 0666 );
	//Generate DataPipe path, create buffers and other
	char Buffer = '\0';
        int hdmi_status = 0;
        int expand_mode = 0;
	int CommonFIFO = open(pCommonFIFO, O_RDWR);

	//Start receive
	while (read(CommonFIFO, &Buffer, BufferSize) == BufferSize) {
                usleep(500);
		hdmi_status = getHDMIstatus();
	        if (hdmi_status == 1)  
                        syslog(LOG_NOTICE, "HDMI is connected");
                else
                        syslog(LOG_NOTICE, "HDMI is disconnected");
                expand_mode = getExpandMode();                
                if (hdmi_status == 0) {
                        syslog(LOG_NOTICE, "Internal monitor");
                        setInternalMonitor();
                }
                if (hdmi_status == 1 && expand_mode == 0) {
                        syslog(LOG_NOTICE, "External monitor");
                        setExternalMonitor();
                }
                if (hdmi_status == 1 && expand_mode == 1) {
                        syslog(LOG_NOTICE, "Expand desktop");
                        setExpandMonitor();
                }
                update_desktop();
	}
	close(CommonFIFO);
	return 0;
}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

int TransmitterModule(char* FileName){
	char Buffer = '+';
	mkfifo(pCommonFIFO, 0666);
	int CommonFIFO = open ( pCommonFIFO, O_WRONLY );
        write(CommonFIFO, &Buffer, BufferSize);
	close( CommonFIFO );
        set_backlight();
	return 0;
}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
