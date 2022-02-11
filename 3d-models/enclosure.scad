/*
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: CERN-OHL-S-2.0
 */

thickness = 1.6;

width = 100;
length = 1 + (29.71 + thickness) * 2;

pcb_width = 82.804;
pcb_length = 55.753;
pcb_thickness = 1.6;
pcb_screw_padding = 3.56;
pcb_base_height = 6.5;
pcb_base_d = 5;
pcb_base_screw_d = 1.8;

pcb0_x = thickness + 0.5;
pcb0_y = width - pcb_width - 3;

usb_conn_width = 9.1;
usb_conn_height = 3.9;
usb_conn_distance_y = 51.85;

height = thickness + pcb_base_height + pcb_thickness + usb_conn_height + 10;

oled_hole_z = 3;
oled_hole_base_z = 1.65;
oled_hole_base_d = 4;
oled_hole_d = 1.8;
oled_hole_spacing_x = 30.5;
oled_hole_spacing_y = 28.5;
oled0_x = (length - oled_hole_spacing_x) / 2;
oled0_y = pcb0_y - 7;
oled_screen_width = 33;
oled_screen_height = 18;
oled_screen_distance_x = (oled_hole_spacing_x - oled_screen_width) / 2;
oled_screen_distance_y = 3.6;

encoder_distance_x = 29.71;
encoder_distance_y = 34.33;
encoder_d = 7.3;

led_d = 3.2;
led_distance_x = 13.467;
led_distance_y = 73.66;
led_spacing = 32.502;

jack_d = 5.8 + 0.7;
jack_out_d = 8.5;
jack_spacing = 25;
jack_distance_x = (length - jack_spacing) / 2;
jack_distance_z = height - 9;

module pcb_base(x, y) {
    translate([x, y, 0]) {
        difference() {
            cylinder(pcb_base_height, d=pcb_base_d, $fn=20);
            cylinder(pcb_base_height, d=pcb_base_screw_d, $fn=20);
        }
    }
}

difference() {
    union() {
        cube([length, width, thickness]);
        translate([0, 0, thickness]) {
            cube([thickness, width, pcb_base_height + pcb_thickness + usb_conn_height]);
            cube([length, thickness, height - thickness]);
            translate([length - thickness, 0, 0])
                cube([thickness, width, pcb_base_height + pcb_thickness + usb_conn_height]);
            translate([0, width - thickness, 0])
                cube([length, thickness, height - thickness]);
        }

        translate([pcb0_x, pcb0_y, thickness]) {
            pcb_base(pcb_screw_padding, pcb_screw_padding);
            pcb_base(pcb_screw_padding, pcb_width - pcb_screw_padding);
            pcb_base(pcb_length - pcb_screw_padding, pcb_screw_padding);
            pcb_base(pcb_length - pcb_screw_padding, pcb_width - pcb_screw_padding);
        }

        translate([oled0_x, oled0_y, thickness]) {
            cylinder(h=oled_hole_base_z, d=oled_hole_base_d, $fn=20);
            translate([oled_hole_spacing_x, 0, 0])
                cylinder(h=oled_hole_base_z, d=oled_hole_base_d, $fn=20);
            translate([0, oled_hole_spacing_y, 0]) {
                cylinder(h=oled_hole_base_z, d=oled_hole_base_d, $fn=20);
                translate([oled_hole_spacing_x, 0, 0])
                    cylinder(h=oled_hole_base_z, d=oled_hole_base_d, $fn=20);
            }
        }

    }

    translate([0, pcb0_y, thickness]) {
        translate([0, usb_conn_distance_y, pcb_base_height + pcb_thickness + 0.5])
            cube([thickness, usb_conn_width, usb_conn_height - 0.5 + 0.1]);
    }

    translate([pcb0_x, pcb0_y, 0]) {
        translate([encoder_distance_x, encoder_distance_y, 0])
            cylinder(thickness, d=encoder_d, $fn=20);
        translate([led_distance_x, led_distance_y, 0]) {
            cylinder(thickness, d=led_d, $fn=20);
            translate([led_spacing, 0, 0])
                cylinder(thickness, d=led_d, $fn=20);
        }
    }

    translate([oled0_x, oled0_y, 0]) {
        translate([oled_screen_distance_x, oled_screen_distance_y, 0])
            cube([oled_screen_width, oled_screen_height, thickness]);

        translate([0, 0, thickness + oled_hole_base_z - oled_hole_z + 0.1]) {
            cylinder(h=oled_hole_z, d=oled_hole_d, $fn=20);
            translate([oled_hole_spacing_x, 0, 0])
                cylinder(h=oled_hole_z, d=oled_hole_d, $fn=20);
            translate([0, oled_hole_spacing_y, 0]) {
                cylinder(h=oled_hole_z, d=oled_hole_d, $fn=20);
                translate([oled_hole_spacing_x, 0, 0])
                    cylinder(h=oled_hole_z, d=oled_hole_d, $fn=20);
            }
        }
    }

    rotate([-90, 0, 0]) {
        translate([jack_distance_x, -jack_distance_z, 0]) {
            translate([0, 0, thickness - 0.6])
                cylinder(0.6, d=jack_out_d, $fn=20);
            cylinder(thickness, d=jack_d, $fn=20);

            translate([jack_spacing, 0, 0]) {
                translate([0, 0, thickness - 0.6])
                    cylinder(0.6, d=jack_out_d, $fn=20);
                cylinder(thickness, d=jack_d, $fn=20);
            }
        }

        translate([3, -height + 3, 0])
            cylinder(width, d=2.2, $fn=20);
        translate([length - 3, -height + 3, 0])
            cylinder(width, d=2.2, $fn=20);
    }
}

rotate([-90, 0, 0]) {
    translate([jack_distance_x, -jack_distance_z, 0]) {
        translate([0.5 + jack_d / 2, 0, thickness / 2])
            cube([1, jack_d, thickness], center=true);
        translate([-0.5 - jack_d / 2, 0, thickness / 2])
            cube([1, jack_d, thickness], center=true);

        translate([jack_spacing, 0, 0]) {
            translate([0.5 + jack_d / 2, 0, thickness / 2])
                cube([1, jack_d, thickness], center=true);
            translate([-0.5 - jack_d / 2, 0, thickness / 2])
                cube([1, jack_d, thickness], center=true);
        }
    }
}
