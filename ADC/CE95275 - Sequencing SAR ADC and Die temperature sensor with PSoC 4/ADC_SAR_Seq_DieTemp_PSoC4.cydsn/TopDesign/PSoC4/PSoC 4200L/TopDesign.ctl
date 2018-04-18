-- =============================================================================
-- The following directives assign pins to the locations specific for the
-- CY8CKIT-046 kit.
-- =============================================================================

-- === UART ===
attribute port_location of \UART:tx(0)\ : label is "PORT(3,1)";

-- === RGB LED ===
attribute port_location of REPLACE_WITH_ACTUAL_PIN_NAME(0) : label is "PORT(5,2)"; -- RED LED
attribute port_location of Pin_LED(0) : label is "PORT(5,3)"; -- GREEN LED
attribute port_location of REPLACE_WITH_ACTUAL_PIN_NAME(0) : label is "PORT(5,4)"; -- BLUE LED

attribute port_location of Pin_Vin(0) : label is "PORT(3,0)";