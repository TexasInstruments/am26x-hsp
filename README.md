Design models in Simulink, generate code using Embedded Coder, and run the executables on AM263x, AM263Px, AM261x hardware platforms designed for real-time control applications without using manual programming. Apply industry-proven techniques for Model-Based Design to verify that the algorithms work during simulation and then implement the algorithms as standalone applications on the AM26X platforms using automatic code generation. By default, the generated code is ANSI/ISO C/C++.

Embedded Coder Support Package for Texas Instruments AM26X Processors supports:

- Library of Simulink blocks that connect to AM263x, AM263Px, AM261x I/O such as ADC, DAC, CMPSS, EPWM, ECAP, EQEP, SDFM, GPIO, UART, MCAN, SPI, HWI
- Rapid prototyping of algorithms on-chip using automated build, deploy and execution
- Built-in support for specific hardware (ControlCards and LaunchPads), scheduler (baremetal scheduler), FreeRTOS and compiler
- Processor-in-the-loop PIL support
- Real-time parameter tuning and logging using External mode (XCP over Serial, XCP over CAN)

Supported hardware:
- controlCard: AM263x, AM263Px
- LaunchPad: AM263x, AM263Px, AM261x
- Custom boards: AM263x based, AM263Px based

## Getting Started

### Requirements:

1. MATLAB, Simulink, MATLAB Coder, Simulink Coder, Embedded Coder
R2025a
2. Code Composer Studio (select AM2x support)
12.8.1
3. MCU PLUS SDK AM263x
10_02_00_13
4. MCU PLUS SDK AM263Px
10_02_00_15
5. MCU PLUS SDK AM2631x
10_02_00_15

Please note: It is not mandatory to install both AM263x MCUSDK, AM263Px MCUSDK and AM261x MCUSDK. You may install the MCUSDK for atleast one device (AM263x or AM263Px or AM261x).


### Installation:
#### Download and install the pre-requisites:

MATLAB/Simulink - Download and install R2025a with the following products: MATLAB Coder, Simulink Coder, Embedded Coder

Code Composer Studio - Download and install CCS 12.8.1

- Obtain CCS12.8.1.00005_win64.zip from https://www.ti.com/tool/download/CCSTUDIO
- Extract the zip and run the executable ccs_setup_12.8.1.00005.exe
- Use Installation Directory as C:\ti\ccs1281
- Use 'Custom Installation' -> 'Select Sitara AM2x MCUs'
- Wait for completion of the installation (approx. 10mins) and click 'Finish'

MCU PLUS SDK AM263x / MCU PLUS SDK AM263Px / MCU PLUS SDK AM261x – Download and install the version mentioned above

- Please note: It is not mandatory to install both MCU PLUS SDK AM263x, MCU PLUS SDK AM263Px and MCU PLUS SDK AM261x. You may install the MCU PLUS SDK for atleast one device (AM263x or AM263Px or AM261x or both).
- Obtain mcu_plus_sdk_am263x_10_02_00_13-windows-x64-installer.exe from https://www.ti.com/tool/download/MCU-PLUS-SDK-AM263X
- Obtain mcu_plus_sdk_am263px_10_02_00_15-windows-x64-installer.exe from https://www.ti.com/tool/download/MCU-PLUS-SDK-AM263PX
- Obtain mcu_plus_sdk_am261x_10_02_00_15-windows-x64-installer.exe from https://www.ti.com/tool/download/MCU-PLUS-SDK-AM261X
- Run the executable
- Use Installation Directory as C:\ti
- Wait for completion of the installation (approx. 5 mins) and click 'Finish'

#### Download the HSP package:

Method 1:

- Step 1. Open MATLAB
- Step 2. Click Add-Ons -> Get Hardware Support Packages
- Step 3. In the Add-On Explorer window, use 'Search for add-ons' field to search for 'AM26x'
- Step 4. From the dropdown, choose 'Embedded Coder Support Hardware Support Package for TI AM26x'
- Step 5. Expect the page for the add-on to open
- Step 6. Click Add -> Download Only. Do not use Add -> Add to MATLAB (Download and add to path)
- Step 7. Sign In  using MathWorks Account when prompted
- Step 8. Download the zip file

Method 2:

- Step 1. In your browser, browse to link: https://in.mathworks.com/matlabcentral/fileexchange/174295-embedded-coder-hardware-support-package-for-ti-am26x
- Step 2. Click Download
- Step 3. Sign In using MathWorks Account
- Step 4. Download the zip file

#### Install the HSP package:

- Step 1. Open MATLAB
- Step 2. Ensure to set 'Preferences'->'MATLAB'->'Add-Ons'->'Installation Folder' to a path with no spaces. For example: C:\workarea\AddOns
- Step 3. Extract the zip file to the installation folder. For example: C:\workarea\AddOns\am26x-hsp-xxxxxxx
- Step 4. In the MATLAB environment, browse to the installed folder. For example: C:\workarea\AddOns\am26x-hsp-xxxxxxx folder
- Step 5. Run command hsp_am26x_setup in the Command Window
- Step 6. Expect a dialog box which prompts for the paths of CCS, SDK and compiler
- Step 7. Enter the paths. Use the defaults if the CCS, compiler and SDK are installed in the default paths. Click OK
- Step 8. Expect following logs in the Command Window
    - HSP AM26x setup...
    - Path validation: Success!
    - Installed AM26x HSP and device driver blocks!
    - Installed TI ARM CLANG toolchain!
    - Complete!

### Use:

Steps for AM263x:

- Step 1: Open Simulink, select Blank Model -> Create Model
- Step 2: In the model, select 'Modeling' tab and select 'Model settings'. Select 'Hardware Implementation' and select 'AM263x LP' or 'AM263X ControlCard' or 'AM263X' as Hardware board. Click OK
- Step 3: Use blocks in 'Library browser' -> 'Embedded Coder Support Package for AM263x' and develop your model.
- Step 4: Save the model. Go to 'Hardware' tab and click 'Build, Deploy & Start'. This builds and runs the model on hardware.

Uninstall:

- Step 1: Run command hsp_am26x_uninstall from MATLAB command window.
    - Expect following messages in MATLAB command window: 
        - Uninstalled AM26x HSP and device driver blocks!
        - Uninstalled TI ARM CLANG toolchain!
- Step 2: Delete the contents of the installation folder. For example: C:\workarea\AddOns\am26x-hsp-xxxxxxx
