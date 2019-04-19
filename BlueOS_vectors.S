/***
 * BlueOS vector tables
 * Make sure the correct processor series is #defined before this
***/


    .section ".vector_table"
#ifndef BOS_DEBUG
    .word   __StackTop              //Top of stack as defined in the linker file
    .word   os_entry                //Reset handler
    .word   hold_hdlr               //Non mask handler
    .word   hold_hdlr               //Hard fault handler
    .word   hold_hdlr               //Memory management handler
    .word   hold_hdlr               //Bus fault handler
    .word   hold_hdlr               //Usage fault handler
    .word   0x00000000
    .word   0x00000000
    .word   0x00000000
    .word   0x00000000
    .word   hold_hdlr               //System call handler
    .word   hold_hdlr               //Debug monitor handler
    .word   0x00000000
    .word   switcher_Handler        //Task switch handler
    .word   sysTick_Handler         //Timer tick handler

#ifdef STM32F10X_MD
    .word   hold_hdlr               //  window_watchdog_hdlr
    .word   hold_hdlr               //  power_valid_hdlr
    .word   hold_hdlr               //  tamper_hdlr
    .word   hold_hdlr               //  real_time_clock_hdlr
    .word   hold_hdlr               //  flash_mem_hdlr
    .word   hold_hdlr               //  reset_clock_hdlr
    .word   hold_hdlr               //  external_0_hdlr
    .word   hold_hdlr               //  external_1_hdlr
    .word   hold_hdlr               //  external_2_hdlr
    .word   hold_hdlr               //  external_3_hdlr
    .word   hold_hdlr               //  external_4_hdlr
    .word   hold_hdlr               //  direct_mem1_ch_1_hdlr
    .word   hold_hdlr               //  direct_mem1_ch_2_hdlr
    .word   hold_hdlr               //  direct_mem1_ch_3_hdlr
    .word   hold_hdlr               //  direct_mem1_ch_4_hdlr
    .word   hold_hdlr               //  direct_mem1_ch_5_hdlr
    .word   hold_hdlr               //  direct_mem1_ch_6_hdlr
    .word   hold_hdlr               //  direct_mem1_ch_7_hdlr
    .word   hold_hdlr               //  analog2digital_1and2_hdlr
    .word   hold_hdlr               //  usb_hp_can_tx_hdlr
    .word   hold_hdlr               //  usb_lp_can_rx0_hdlr
    .word   hold_hdlr               //  can_rx1_hdlr
    .word   hold_hdlr               //  can_sce_hdlr
    .word   hold_hdlr               //  external_5to9_hdlr
    .word   hold_hdlr               //  timer1_break_hdlr
    .word   hold_hdlr               //  timer1_update_hdlr
    .word   hold_hdlr               //  timer1_trig_commutation_hdlr
    .word   hold_hdlr               //  timer1_capture_compare_hdlr
    .word   hold_hdlr               //  timer2_hdlr
    .word   hold_hdlr               //  timer3_hdlr
    .word   hold_hdlr               //  timer4_hdlr
    .word   hold_hdlr               //  i2c1_event_hdlr
    .word   hold_hdlr               //  i2c1_error_hdlr
    .word   hold_hdlr               //  i2c2_event_hdlr
    .word   hold_hdlr               //  i2c2_error_hdlr
    .word   hold_hdlr               //  spi1_hdlr
    .word   hold_hdlr               //  spi2_hdlr
    .word   hold_hdlr               //  uart1_hdlr
    .word   hold_hdlr               //  uart2_hdlr
    .word   hold_hdlr               //  uart3_hdlr
    .word   hold_hdlr               //  external_10to15_hdlr
    .word   hold_hdlr               //  real_time_alarm_hdlr
    .word   hold_hdlr               //  usb_wakeup_hdlr
    .word   hold_hdlr               //  timer8_wakeup_hdlr
    .word   hold_hdlr               //  timer8_update_hdlr
    .word   hold_hdlr               //  timer8_trig_commutation_hdlr
    .word   hold_hdlr               //  timer8_capture_compare_hdlr
    .word   hold_hdlr               //  analog2digital_3_hdlr
    .word   hold_hdlr               //  external_mem_hdlr
    .word   hold_hdlr               //  sdio_hdlr
    .word   hold_hdlr               //  timer5_hdlr
    .word   hold_hdlr               //  spi3_hdlr
    .word   hold_hdlr               //  uart4_hdlr
    .word   hold_hdlr               //  uart5_hdlr
    .word   hold_hdlr               //  timer6_hdlr
    .word   hold_hdlr               //  timer7_hdlr
    .word   hold_hdlr               //  direct_mem2_ch_1_hdlr
    .word   hold_hdlr               //  direct_mem2_ch_2_hdlr
    .word   hold_hdlr               //  direct_mem2_ch_3_hdlr
    .word   hold_hdlr               //  direct_mem2_ch_4and5_hdlr
#endif
#else
    .word   __StackTop              //Top of stack as defined in the linker file
    .word   os_entry                //Reset handler
    .word   hold_hdlr               //Non mask handler
    .word   hf_hdlr                 //Hard fault handler
    .word   mm_hdlr                 //Memory management handler
    .word   bf_hdlr                 //Bus fault handler
    .word   uf_hdlr                 //Usage fault handler
    .word   0x00000000
    .word   0x00000000
    .word   0x00000000
    .word   0x00000000
    .word   sc_hdlr                 //System call handler
    .word   hold_hdlr               //Debug monitor handler
    .word   0x00000000
    .word   switcher_Handler        //Task switch handler
    .word   sysTick_Handler         //Timer tick handler

#ifdef STM32F10X_MD
    .word   hold_hdlr               //  window_watchdog_hdlr
    .word   hold_hdlr               //  power_valid_hdlr
    .word   hold_hdlr               //  tamper_hdlr
    .word   hold_hdlr               //  real_time_clock_hdlr
    .word   hold_hdlr               //  flash_mem_hdlr
    .word   hold_hdlr               //  reset_clock_hdlr
    .word   hold_hdlr               //  external_0_hdlr
    .word   hold_hdlr               //  external_1_hdlr
    .word   hold_hdlr               //  external_2_hdlr
    .word   hold_hdlr               //  external_3_hdlr
    .word   hold_hdlr               //  external_4_hdlr
    .word   hold_hdlr               //  direct_mem1_ch_1_hdlr
    .word   hold_hdlr               //  direct_mem1_ch_2_hdlr
    .word   hold_hdlr               //  direct_mem1_ch_3_hdlr
    .word   hold_hdlr               //  direct_mem1_ch_4_hdlr
    .word   hold_hdlr               //  direct_mem1_ch_5_hdlr
    .word   hold_hdlr               //  direct_mem1_ch_6_hdlr
    .word   hold_hdlr               //  direct_mem1_ch_7_hdlr
    .word   hold_hdlr               //  analog2digital_1and2_hdlr
    .word   hold_hdlr               //  usb_hp_can_tx_hdlr
    .word   hold_hdlr               //  usb_lp_can_rx0_hdlr
    .word   hold_hdlr               //  can_rx1_hdlr
    .word   hold_hdlr               //  can_sce_hdlr
    .word   hold_hdlr               //  external_5to9_hdlr
    .word   hold_hdlr               //  timer1_break_hdlr
    .word   hold_hdlr               //  timer1_update_hdlr
    .word   hold_hdlr               //  timer1_trig_commutation_hdlr
    .word   hold_hdlr               //  timer1_capture_compare_hdlr
    .word   hold_hdlr               //  timer2_hdlr
    .word   hold_hdlr               //  timer3_hdlr
    .word   hold_hdlr               //  timer4_hdlr
    .word   hold_hdlr               //  i2c1_event_hdlr
    .word   hold_hdlr               //  i2c1_error_hdlr
    .word   hold_hdlr               //  i2c2_event_hdlr
    .word   hold_hdlr               //  i2c2_error_hdlr
    .word   hold_hdlr               //  spi1_hdlr
    .word   hold_hdlr               //  spi2_hdlr
    .word   hold_hdlr               //  uart1_hdlr
    .word   hold_hdlr               //  uart2_hdlr
    .word   hold_hdlr               //  uart3_hdlr
    .word   hold_hdlr               //  external_10to15_hdlr
    .word   hold_hdlr               //  real_time_alarm_hdlr
    .word   hold_hdlr               //  usb_wakeup_hdlr
    .word   hold_hdlr               //  timer8_wakeup_hdlr
    .word   hold_hdlr               //  timer8_update_hdlr
    .word   hold_hdlr               //  timer8_trig_commutation_hdlr
    .word   hold_hdlr               //  timer8_capture_compare_hdlr
    .word   hold_hdlr               //  analog2digital_3_hdlr
    .word   hold_hdlr               //  external_mem_hdlr
    .word   hold_hdlr               //  sdio_hdlr
    .word   hold_hdlr               //  timer5_hdlr
    .word   hold_hdlr               //  spi3_hdlr
    .word   hold_hdlr               //  uart4_hdlr
    .word   hold_hdlr               //  uart5_hdlr
    .word   hold_hdlr               //  timer6_hdlr
    .word   hold_hdlr               //  timer7_hdlr
    .word   hold_hdlr               //  direct_mem2_ch_1_hdlr
    .word   hold_hdlr               //  direct_mem2_ch_2_hdlr
    .word   hold_hdlr               //  direct_mem2_ch_3_hdlr
    .word   hold_hdlr               //  direct_mem2_ch_4and5_hdlr
#endif
#endif
