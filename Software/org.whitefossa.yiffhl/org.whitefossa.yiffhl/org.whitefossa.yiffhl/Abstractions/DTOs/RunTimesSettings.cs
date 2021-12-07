using System;

namespace org.whitefossa.yiffhl.Abstractions.DTOs
{
    /// <summary>
    /// Settings, related to fox run times
    /// </summary>
    public class RunTimesSettings
    {
        /// <summary>
        /// Start time (days are discarded)
        /// </summary>
        public DateTime StartTime { get; set; }

        /// <summary>
        /// Finish time (days are discarder)
        /// </summary>
        public DateTime FinishTime { get; set; }
    }
}
