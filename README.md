# Systick Driver
A general wrapper around systick and systick-like features to allow for general timekeeping
 and timeout functionality to other elements of the HAL.
 
## Note:
The systick and sysclock elements of the HAL are some of the most difficult to use and test, and are generally wrappers around 
MCU vendor created functions, or reimplementations thereof with more freedom (but **STRONG** recommendations).
If you have ANY doubts about my implementations or experience issues, redirect the interface targets to HALs created by the vendors.
