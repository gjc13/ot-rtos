/* Linker script to configure memory regions. */

SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

MEMORY
{
  FLASH (rx) : ORIGIN = 0x0, LENGTH = 0x100000
  RAM (rwx) :  ORIGIN = 0x20000000, LENGTH = 0x40000
}

FLASH_PAGE_SIZE       = 4096;
FLASH_DATA_PAGES_USED = 4;

SECTIONS
{
}

SECTIONS
{
  . = ALIGN(4);
  .mem_section_dummy_ram :
  {
  }
  .log_dynamic_data :
  {
    PROVIDE(__start_log_dynamic_data = .);
    KEEP(*(SORT(.log_dynamic_data*)))
    PROVIDE(__stop_log_dynamic_data = .);
  } > RAM
  .log_filter_data :
  {
    PROVIDE(__start_log_filter_data = .);
    KEEP(*(SORT(.log_filter_data*)))
    PROVIDE(__stop_log_filter_data = .);
  } > RAM

} INSERT AFTER .data;

SECTIONS
{
  .mem_section_dummy_rom :
  {
  }
  .log_const_data :
  {
    PROVIDE(__start_log_const_data = .);
    KEEP(*(SORT(.log_const_data*)))
    PROVIDE(__stop_log_const_data = .);
  } > FLASH
  .log_backends :
  {
    PROVIDE(__start_log_backends = .);
    KEEP(*(SORT(.log_backends*)))
    PROVIDE(__stop_log_backends = .);
  } > FLASH
    .nrf_balloc :
  {
    PROVIDE(__start_nrf_balloc = .);
    KEEP(*(.nrf_balloc))
    PROVIDE(__stop_nrf_balloc = .);
  } > FLASH

  __stop_ot_flash_data = (ORIGIN(FLASH) + LENGTH(FLASH));
  __start_ot_flash_data = (__stop_ot_flash_data - (FLASH_PAGE_SIZE * FLASH_DATA_PAGES_USED));

  /* Assure that code does not overlap flash data area.*/
  ASSERT((__start_ot_flash_data >= __etext + SIZEOF(.data)), "Error: Code overlaps flash data area.")

} INSERT AFTER .text

INCLUDE "nrf_common.ld"
