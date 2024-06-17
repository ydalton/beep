#ifndef __BEEP_H_
#define __BEEP_H_

#ifndef __linux__
#error Non-Linux operating systems are not supported!
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <linux/input.h>

#define BEEP_PCSPKR_DEV "/dev/input/by-path/platform-pcspkr-event-spkr"

/**
 * Called at the start of note
 */
#define BEEP_INIT \
	int main(void)								\
	{									\
	struct input_event e = {						\
		.type = EV_SND,							\
		.code = SND_TONE,						\
		.value = 0,							\
	};									\
	int fd = open(BEEP_PCSPKR_DEV, O_WRONLY);				\
	if(fd == -1) {								\
		fprintf(stderr, "Can't open PC speaker, do you have one?\n");	\
		return EXIT_FAILURE;						\
	}									\

#define BEEP_WRITE \
	do {									\
		if (sizeof(e) != write(fd, &e, sizeof(e))) {			\
			perror("write:");					\
			return -1;						\
		}								\
	} while(0);

/**
 * Play a note.
 *
 * @param pitch The pitch of the played note, in hertz.
 * @param duration The duration of the played note, in milliseconds.
 */
#define BEEP_NOTE(pitch, duration) \
	do {									\
		e.value = pitch;						\
		BEEP_WRITE							\
		usleep(duration * 1000);					\
	} while(0);

#define BEEP_OFF \
	do {									\
		e.value = 0;							\
		BEEP_WRITE							\
	} while(0);
/**
 * Rest for a duration.
 *
 * @param duration The duration of the rest.
 */
#define BEEP_REST(duration) \
	do {									\
		BEEP_OFF							\
		usleep(duration * 1000);					\
	} while(0);

/**
 * Called at the end of the program.
 */
#define BEEP_END \
		BEEP_OFF							\
		return EXIT_SUCCESS; 						\
	}

#endif
