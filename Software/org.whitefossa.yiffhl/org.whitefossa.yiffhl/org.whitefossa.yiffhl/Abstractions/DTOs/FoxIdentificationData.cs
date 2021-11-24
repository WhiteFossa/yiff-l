using System;

namespace org.whitefossa.yiffhl.Abstractions.DTOs
{
    /// <summary>
    /// Fox identification data
    /// </summary>
    public class FoxIdentificationData
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
        /// Serial number
        /// </summary>
        public uint SerialNumber { get; set; }
    }
}
