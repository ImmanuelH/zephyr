#
# Copyright (c) 2022 Immanuel Huebner  
#
# SPDX-License-Identifier: Apache-2.0
#

board_runner_args(jlink "--device=EFM32PG22CxxxF512")
include(${ZEPHYR_BASE}/boards/common/jlink.board.cmake)
