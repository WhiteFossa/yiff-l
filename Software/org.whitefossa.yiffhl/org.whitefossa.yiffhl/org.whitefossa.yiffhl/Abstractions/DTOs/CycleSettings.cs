using System;

namespace org.whitefossa.yiffhl.Abstractions.DTOs
{
    /// <summary>
    /// Fox cycle settings
    /// </summary>
    public class CycleSettings
    {
        /// <summary>
        /// True if continuous cycle
        /// </summary>
        public bool IsContinuous { get; set; }

        /// <summary>
        /// Transmission duration
        /// </summary>
        public TimeSpan TxDuration { get; set; }

        /// <summary>
        /// Pause duration
        /// </summary>
        public TimeSpan PauseDuration { get; set; }
    }
}
