-- =============================================================================
-- The following directives assign pins to the locations specific for the
-- CY8CKIT-044 kit.
-- =============================================================================

-- === UART ===
attribute port_location of \UART:tx(0)\ : label is "PORT(7,1)";

-- === RGB LED ===
attribute port_location of Pin_LED(0) : label is "PORT(2,6)"; -- GREEN LED

attribute port_location of Pin_Vin(0) : label is "PORT(3,0)";