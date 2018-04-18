-- =============================================================================
-- The following directives assign pins to the locations specific for the
-- CY8CKIT-042 kit.
-- =============================================================================

-- === UART ===
attribute port_location of \UART:tx(0)\ : label is "PORT(0,5)";

-- === RGB LED ===
attribute port_location of Pin_LED(0) : label is "PORT(0,2)"; -- GREEN LED

attribute port_location of Pin_Vin(0) : label is "PORT(0,4)";


