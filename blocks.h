/* time interval in seconds to sleep before looking for updates in the main loop */
#define SLEEPINTERVAL			1

#define PATH(name)			"/home/ashish/.local/projects/dwmblocks/blocks/"name

/* If multiple realtime signals are pending, then the lowest numbered signal is delivered first.
 * If interval of a block is set to 0, the block will only be updated once at startup.
 * If interval is set to a negative value, the block will never be updated in the main loop.
 * Set signal to 0 if signalling is not required for the block.
 * Keep signal for clickable blocks less than 10. */

/* pathu - path of the program whose output is to be used for status text
 * patch - path of the program to be executed on clicks */
static Block blocks[] = {
/*	pathu				pathc					interval	signal */
	{ PATH("hotspot.sh"),		PATH("hotspot_button.sh"),		-1,		3},

	{ PATH("time.sh"),		NULL,					30,		0},

	{ PATH("calendar.sh"),		PATH("calendar_button.sh"),		60,		6},

//	{ PATH("mail.sh"),		PATH("mail_button.sh"),			30,		2},

	{ PATH("volume.sh"),		PATH("volume_button.sh"),		0,		1},

	{ PATH("cpu_temp.sh"),		PATH("cpu_temp_button.sh"),		1,		5},

	{ PATH("battery.sh"),		PATH("battery_button.sh"),		30,		4},

        { NULL } /* just to mark the end of the array */
};

static const char *delim =  "   ";
