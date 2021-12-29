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
    }
}
