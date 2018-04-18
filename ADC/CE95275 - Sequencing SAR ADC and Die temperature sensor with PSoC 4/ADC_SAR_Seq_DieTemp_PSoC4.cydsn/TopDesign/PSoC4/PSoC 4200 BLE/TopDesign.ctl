-- =============================================================================
-- The following directives assign pins to the locations specific for the
-- CY8CKIT-042-BLE kit.
-- =============================================================================

-- === UART ===
attribute port_location of \UART:tx(0)\ : label is "PORT(1,5)";

-- === RGB LED ===
attribute port_location of Pin_LED(0) : label is "PORT(3,6)"; -- GREEN LED

attribute port_location of Pin_Vin(0) : label is "PORT(3,0)";