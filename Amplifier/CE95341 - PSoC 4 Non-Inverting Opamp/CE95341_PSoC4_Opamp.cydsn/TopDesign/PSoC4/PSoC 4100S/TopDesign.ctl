-- =============================================================================
-- The following directives assign pins to the locations specific for the
-- CY8CKIT-041 kit.
-- =============================================================================

-- === UART ===
attribute port_location of \UART:rx(0)\ : label is "PORT(0,4)";
attribute port_location of \UART:tx(0)\ : label is "PORT(0,5)";

-- === I2C ===
attribute port_location of \I2C:scl(0)\ : label is "PORT(3,0)";
attribute port_location of \I2C:sda(0)\ : label is "PORT(3,1)";

-- === RGB LED ===
attribute port_location of REPLACE_WITH_ACTUAL_PIN_NAME(0) : label is "PORT(3,4)"; -- RED LED
attribute port_location of REPLACE_WITH_ACTUAL_PIN_NAME(0) : label is "PORT(2,6)"; -- GREEN LED
attribute port_location of REPLACE_WITH_ACTUAL_PIN_NAME(0) : label is "PORT(3,6)"; -- BLUE LED

-- === USER SWITCH ===
attribute port_location of REPLACE_WITH_ACTUAL_PIN_NAME(0) : label is "PORT(0,7)";

attribute port_location of Vminus(0) : label is "PORT(1,1)";
attribute port_location of Vplus(0) : label is "PORT(1,0)";
attribute port_location of Vout(0) : label is "PORT(1,2)";
 