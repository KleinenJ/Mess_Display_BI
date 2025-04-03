	AREA    Bootloader_Section, DATA, READONLY
; Includes the binary file ../Drivers/vendor/Board/SE21159/Loader/H750SerialPort_EDTBoot21159_v10001_231220.bin from the current source folder
        INCBIN  ../Drivers/vendor/Board/SE21159/Loader/H750SerialPort_EDTBoot21159_v10001_231220.bin
        END