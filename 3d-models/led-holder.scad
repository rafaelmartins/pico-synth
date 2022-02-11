/*
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: CERN-OHL-S-2.0
 */

// print two of this

margin = 1;
width = 3;
height = 3;
led_width = 1.8;
led_length = 1;
length = 2 * margin + led_length;

difference() {
    cube([length, width, height]);
    translate([margin, 0, 0]) {
        cube([led_length, (width - led_width) / 2, height]);
        translate([0, width - ((width - led_width) / 2), 0])
            cube([led_length, (width - led_width) / 2, height]);
    }
}