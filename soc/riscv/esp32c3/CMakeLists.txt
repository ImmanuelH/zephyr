# SPDX-License-Identifier: Apache-2.0

zephyr_sources(
  idle.c
  vectors.S
  soc_irq.S
  soc_irq.c
  soc.c
  loader.c
  )

# get code-partition slot0 address
dt_nodelabel(dts_partition_path NODELABEL "slot0_partition")
dt_reg_addr(img_0_off PATH ${dts_partition_path})

# get code-partition boot address
dt_nodelabel(dts_partition_path NODELABEL "boot_partition")
dt_reg_addr(boot_off PATH ${dts_partition_path})

if(CONFIG_BOOTLOADER_ESP_IDF)
  include(ExternalProject)

  ## we use hello-world project, but I think any can be used.
  set(espidf_components_dir   ${ESP_IDF_PATH}/components)
  set(espidf_prefix    ${CMAKE_BINARY_DIR}/esp-idf)
  set(espidf_build_dir ${espidf_prefix}/build)

  ExternalProject_Add(
    EspIdfBootloader
    PREFIX ${espidf_prefix}
    SOURCE_DIR ${espidf_components_dir}/bootloader/subproject
    BINARY_DIR ${espidf_build_dir}/bootloader
    CONFIGURE_COMMAND
    ${CMAKE_COMMAND} -G${CMAKE_GENERATOR}
    -S ${espidf_components_dir}/bootloader/subproject
    -B ${espidf_build_dir}/bootloader -DSDKCONFIG=${espidf_build_dir}/sdkconfig
    -DIDF_PATH=${ESP_IDF_PATH} -DIDF_TARGET=${CONFIG_SOC}
    -DPYTHON_DEPS_CHECKED=1
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    -DCMAKE_ASM_COMPILER=${CMAKE_ASM_COMPILER}
    -DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}
    -DPYTHON=${PYTHON_EXECUTABLE}
    BUILD_COMMAND
    ${CMAKE_COMMAND} --build .
    INSTALL_COMMAND ""      # This particular build system has no install command
    )

  ExternalProject_Add(
    EspPartitionTable
    SOURCE_DIR ${espidf_components_dir}/partition_table
    BINARY_DIR ${espidf_build_dir}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND
    ${PYTHON_EXECUTABLE} ${ESP_IDF_PATH}/components/partition_table/gen_esp32part.py -q
    --offset 0x8000 --flash-size ${CONFIG_ESPTOOLPY_FLASHSIZE} ${ESP_IDF_PATH}/components/partition_table/partitions_singleapp.csv ${espidf_build_dir}/partitions_singleapp.bin
    INSTALL_COMMAND ""
    )

  if(CONFIG_BUILD_OUTPUT_BIN)
    set_property(GLOBAL APPEND PROPERTY extra_post_build_commands
      COMMAND ${PYTHON_EXECUTABLE} ${ESP_IDF_PATH}/components/esptool_py/esptool/esptool.py
      ARGS --chip ${CONFIG_SOC} elf2image --flash_mode dio --flash_freq 40m --flash_size ${CONFIG_ESPTOOLPY_FLASHSIZE}
      -o ${CMAKE_BINARY_DIR}/zephyr/${CONFIG_KERNEL_BIN_NAME}.bin
      ${CMAKE_BINARY_DIR}/zephyr/${CONFIG_KERNEL_BIN_NAME}.elf)
  endif()

  set_property(TARGET bintools PROPERTY disassembly_flag_inline_source)

  add_dependencies(app EspIdfBootloader EspPartitionTable)

  board_finalize_runner_args(esp32 "--esp-flash-bootloader=${espidf_build_dir}/bootloader/bootloader.bin")

  board_finalize_runner_args(esp32 "--esp-flash-partition_table=${espidf_build_dir}/partitions_singleapp.bin")

  board_finalize_runner_args(esp32 "--esp-partition-table-address=0x8000")

endif()

board_finalize_runner_args(esp32 "--esp-boot-address=${boot_off}")

board_finalize_runner_args(esp32 "--esp-app-address=${img_0_off}")
