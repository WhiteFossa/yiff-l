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
        public UInt16 HardwareRevision { get; set; }

        /// <summary>
        /// Firmware version
        /// </summary>
        public UInt16 FirmwareVersion { get; set; }

        /// <summary>
        /// Serial number
        /// </summary>
        public UInt32 SerialNumber { get; set; }
    }
}
