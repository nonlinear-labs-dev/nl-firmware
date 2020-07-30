/*
 * GPIO user space helpers
 *
 * Copyright 2009 Analog Devices Inc.
 * Michael Hennerich (hennerich@blackfin.uclinux.org)
 *
 * Licensed under the GPL-2 or later
 */

#pragma once

int linuxgpio_export(unsigned int gpio);
int linuxgpio_unexport(unsigned int gpio);
int linuxgpio_dir_out(unsigned int gpio);
int linuxgpio_dir_in(unsigned int gpio);
int linuxgpio_openfd(unsigned int gpio);
