-- =============================================================================
-- The following directives assign pins to the locations specific for the
-- CY8CKIT-046 kit.
-- =============================================================================

-- === RGB LED ===
attribute port_location of LED(0) : label is "PORT(5,3)"; -- GREEN LED

attribute port_location of Vminus(0) : label is "PORT(0,1)";
attribute port_location of Vplus(0) : label is "PORT(0,0)";