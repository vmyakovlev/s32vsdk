#include "timer.h"

int cnt = 0;

void tsp_generic_timer_start(void)
{
	uint64_t cval;
	uint32_t ctl = 0;

	cval = read_cntpct_el0() + (read_cntfrq_el0() >> 1);
	write_cntps_cval_el1(cval);

	/* Enable the secure physical timer */
	set_cntp_ctl_enable(ctl);
	write_cntps_ctl_el1(ctl);
}

void tsp_generic_timer_handler(void)
{
	/*
	 * Disable the timer and reprogram it. The barriers ensure that there is
	 * no reordering of instructions around the reprogramming code.
	 */
	isb();
	write_cntps_ctl_el1(0);
	if (cnt < 999) tsp_generic_timer_start();
	isb();
	cnt++;
}

void tsp_generic_timer_stop(void)
{
	/* Disable the timer */
	write_cntps_ctl_el1(0);
} 