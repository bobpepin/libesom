#
# Regular cron jobs for the libesom package
#
0 4	* * *	root	[ -x /usr/bin/libesom_maintenance ] && /usr/bin/libesom_maintenance
