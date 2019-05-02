# AudioVisual
# Copyright (C) 2019  Luca Cireddu
# sardylan@gmail.com
# https://www.lucacireddu.it
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

OUTPUT_FILE = "../waterfall_color_table.hpp"

if __name__ == "__main__":
    fd = open(OUTPUT_FILE, "w")

    fd.write("""/*
 * AudioVisual
 * Copyright (C) 2019  Luca Cireddu
 * sardylan@gmail.com
 * https://www.lucacireddu.it
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __AUDIOVISUAL__CUSTOMWIDGETS_WATERFALL_COLOR_TABLE__H
#define __AUDIOVISUAL__CUSTOMWIDGETS_WATERFALL_COLOR_TABLE__H

const int waterfall_color_table[1025][3] = {
""")

    for v in range(0, 1025):
        if v < 256:
            fd.write("    {%d, %d, %d},\n" % (0, 0, v))
        elif v < 512:
            k = v - 256
            fd.write("    {%d, %d, %d},\n" % (k, k, 255 - k))
        elif v < 768:
            k = v - 512
            fd.write("    {%d, %d, %d},\n" % (255, 255 - k, 0))
        elif v < 1024:
            k = v - 768
            fd.write("    {%d, %d, %d},\n" % (255, k, k))

    fd.write("""    {255, 0, 255}
};

#endif

""")

    fd.close()
