/* time interval in seconds to sleep before looking for updates in the main loop */
#define SLEEPINTERVAL			1

#define PATH(name)			"/home/ashish/.local/projects/dwmblocks/blocks/"name

/* If interval of a block is set to 0, the block will only be updated once at startup.
 * If interval is set to a negative value, the block will never be updated in the main loop.
 * Set signal to 0 if signalling is not required for the block.
 * Signal must be less than 10 for clickable blocks.
 * If multiple realtime signals are pending, then the lowest numbered signal is delivered first. */

/* pathu - path of the program whose output is to be used for status text
 * pathc - path of the program to be executed on clicks */
static Block blocks[] = {
/*	pathu				pathc					interval	signal */
	{ PATH("time.sh"),		NULL,					30,		10},

	{ PATH("calendar.sh"),		NULL,					30,		3},

	{ PATH("volume.sh"),		PATH("volume_button.sh"),		0,		1},

	{ PATH("cpu_temp.sh"),		PATH("cpu_temp_button.sh"),		1,		4},

	{ PATH("battery.sh"),		PATH("battery_button.sh"),		30,		2},

        { NULL } /* just to mark the end of the array */
};

static const char *delim =  "   ";
