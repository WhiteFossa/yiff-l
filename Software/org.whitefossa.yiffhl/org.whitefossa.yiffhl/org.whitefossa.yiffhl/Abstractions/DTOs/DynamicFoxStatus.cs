namespace org.whitefossa.yiffhl.Abstractions.DTOs
{
    /// <summary>
    /// Dynamic fox status (have to be polled)
    /// </summary>
    public class DynamicFoxStatus
    {
        /// <summary>
        /// Battery level in percent
        /// </summary>
        public float BatteryLevel { get; set; }

        /// <summary>
        /// Antenna matching status data
        /// </summary>
        public AntennaMatchingStatusData AntennaMatchingStatus { get; set; } = new AntennaMatchingStatusData();

        /// <summary>
        /// If true then fox is on countdown/on the run
        /// </summary>
        public bool IsFoxArmed { get; set; }

        /// <summary>
        /// Current profile ID (we use it to detect profile change)
        /// </summary>
        public int CurrentProfileId { get; set; }

        /// <summary>
        /// True if some changes made in fox profile by user via the fox controls
        /// </summary>
        public bool IsManualProfileChangesExist { get; set; }
    }
}
