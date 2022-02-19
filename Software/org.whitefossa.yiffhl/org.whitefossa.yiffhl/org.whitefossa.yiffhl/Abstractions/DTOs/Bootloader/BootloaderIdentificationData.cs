namespace org.whitefossa.yiffhl.Abstractions.DTOs.Bootloader
{
    public class BootloaderIdentificationData
    {
        /// <summary>
        /// Fox hardware revision
        /// </summary>
        public uint HardwareRevision { get; set; }

        /// <summary>
        /// Firmware version
        /// </summary>
        public uint FirmwareVersion { get; set; }

        /// <summary>
        /// FLASH start address
        /// </summary>
        public uint FlashStartAddress { get; set; }

        /// <summary>
        /// Main firmware start address
        /// </summary>
        public uint MainFirmwareStartAddress { get; set; }

        /// <summary>
        /// FLASH end address
        /// </summary>
        public uint FlashEndAddress { get; set; }
    }
}
