/*
 * Copyright (c) 2016-2017, CESAR.
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license. See the LICENSE file for details.
 *
 */

#include <stdio.h>
#include <ell/ell.h>

static void main_loop_quit(struct l_timeout *timeout, void *user_data)
{
	l_main_quit();
	l_timeout_remove(timeout);
}

static void l_terminate(void)
{
	static bool terminating = false;

	if (terminating)
		return;

	terminating = true;

	l_timeout_create(1, main_loop_quit, NULL, NULL);
}

static bool l_main_loop_init()
{
	return l_main_init();
}

static void signal_handler(uint32_t signo, void *user_data)
{
	switch (signo) {
	case SIGINT:
	case SIGTERM:
		l_terminate();
		break;
	}
}

static void l_main_loop_run()
{
	l_main_run_with_signal(signal_handler, NULL);

	l_main_exit();
}

int main(int argc, char *argv[])
{
	printf("LoRa daemon\n");


	if (!l_main_loop_init())
		goto fail_main_loop;

	l_main_loop_run();

fail_main_loop:
	l_main_exit();
	return 0;
}
