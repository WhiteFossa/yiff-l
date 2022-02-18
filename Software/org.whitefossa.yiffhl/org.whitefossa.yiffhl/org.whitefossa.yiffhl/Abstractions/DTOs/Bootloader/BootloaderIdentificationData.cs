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
    }
}
